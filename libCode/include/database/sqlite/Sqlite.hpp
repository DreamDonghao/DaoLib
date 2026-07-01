#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <core/tool/Log.hpp>
#include <core/tool/type.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dao::db {
    class DataVal {
    public:
        DataVal() = default;

        explicit DataVal(std::string valString) : m_cell(std::move(valString)) {
        }

        const std::string &asString() const {
            return m_cell;
        }

        i32 asInt32() const {
            return std::stoi(m_cell);
        }

        i64 asInt64() const {
            return std::stoll(m_cell);
        }

        f32 asFloat32() const {
            return std::stof(m_cell);
        }

        f64 asFloat64() const {
            return std::stod(m_cell);
        }

    private:
        std::string m_cell;
    };

    /// @brief SQLite 查询结果行
    class Row {
    public:
        explicit Row(std::map<std::string, DataVal> data) : m_data(std::move(data)) {
        }

        /// @brief 获取字符串值
        [[nodiscard]] const std::string &asString(const std::string &column) const {
            static const std::string empty;
            const auto it = m_data.find(column);
            return it != m_data.end() ? it->second.asString() : empty;
        }

        /// @brief 获取整数值
        [[nodiscard]] i64 asInt64(const std::string &column) const {
            const auto it = m_data.find(column);
            return it != m_data.end() ? it->second.asInt64() : 0;
        }

        /// @brief 获取整数值
        [[nodiscard]] i32 asInt32(const std::string &column) const {
            return static_cast<i32>(asInt64(column));
        }

        /// @brief 获取浮点值
        [[nodiscard]] f64 asFloat64(const std::string &column) const {
            const auto it = m_data.find(column);
            return it != m_data.end() ? it->second.asFloat64() : 0.0;
        }

        /// @brief 获取布尔值
        [[nodiscard]] bool asBool(const std::string &column) const {
            return asInt64(column) != 0;
        }

        /// @brief 检查列是否存在
        [[nodiscard]] bool has(const std::string &column) const {
            return m_data.contains(column);
        }

        /// @brief 通过列名访问（返回字符串）
        [[nodiscard]] const std::string &operator[](const std::string &column) const {
            return asString(column);
        }

    private:
        std::map<std::string, DataVal> m_data;
    };

    /// @brief SQLite 查询结果集
    class Result {
    public:
        using iterator = std::vector<Row>::iterator;
        using const_iterator = std::vector<Row>::const_iterator;

        void addRow(Row row) { m_rows.push_back(std::move(row)); }

        [[nodiscard]] bool empty() const { return m_rows.empty(); }
        [[nodiscard]] size_t size() const { return m_rows.size(); }

        [[nodiscard]] iterator begin() { return m_rows.begin(); }
        [[nodiscard]] iterator end() { return m_rows.end(); }
        [[nodiscard]] const_iterator begin() const { return m_rows.begin(); }
        [[nodiscard]] const_iterator end() const { return m_rows.end(); }

        [[nodiscard]] const Row &operator[](const size_t index) const { return m_rows[index]; }
        [[nodiscard]] Row &operator[](const size_t index) { return m_rows[index]; }

    private:
        std::vector<Row> m_rows;
    };

    /// @brief SQLite 数据库封装类
    class Sqlite {
    public:
        /// @brief 构造函数
        /// @param filePath 数据库文件路径
        /// @param createIfNotExist 文件不存在时是否创建
        /// @param readOnly 是否只读模式
        explicit Sqlite(const std::string &filePath,
                        const bool createIfNotExist = true,
                        const bool readOnly = false)
            : m_database(std::make_unique<SQLite::Database>(filePath, getDbMode(createIfNotExist, readOnly))) {
        }

        /// @brief 检查数据库是否已打开
        [[nodiscard]] bool isOpen() const noexcept { return m_database != nullptr; }

        /// @brief 隐式转换为 bool，用于检查数据库状态
        [[nodiscard]] explicit operator bool() const noexcept { return isOpen(); }

        /// @brief 执行 SQL 语句（INSERT, UPDATE, DELETE, CREATE 等）
        /// @tparam Args 参数类型
        /// @param sql SQL 语句，使用 {} 作为占位符
        /// @param args 参数值
        /// @return 影响的行数
        template<typename... Args>
        int exec(std::string_view sql, Args &&... args) {
            if (!m_database) {
                Log{LogLevel::ERROR}("数据库未打开");
                return 0;
            }
            try {
                const std::string formattedSql = formatSql(sql, std::forward<Args>(args)...);
                return m_database->exec(formattedSql);
            } catch (const SQLite::Exception &e) {
                Log{LogLevel::WARN}.fmt("SQL 执行失败({}):{} ", e.what(), sql);
                return 0;
            }
        }

        /// @brief 执行查询语句（SELECT）
        /// @tparam Args 参数类型
        /// @param sql SQL 查询语句，使用 {} 作为占位符
        /// @param args 参数值
        /// @return 查询结果集
        template<typename... Args>
        Result select(std::string_view sql, Args &&... args) {
            Result result;
            if (!m_database) {
                Log{LogLevel::DEBUG}("数据库未打开");
                return result;
            }
            try {
                const std::string formattedSql = formatSql(sql, std::forward<Args>(args)...);
                SQLite::Statement query(*m_database, formattedSql);
                // 获取列名
                std::vector<std::string> columnNames;
                const int columnCount = query.getColumnCount();
                columnNames.reserve(columnCount);
                for (int i = 0; i < columnCount; ++i) {
                    columnNames.push_back(query.getColumnName(i));
                }

                // 遍历结果
                while (query.executeStep()) {
                    std::map<std::string, DataVal> rowData;
                    for (int i = 0; i < columnCount; ++i) {
                        if (const auto col = query.getColumn(i); col.isNull()) {
                            rowData[columnNames[i]] = DataVal("");
                        } else {
                            rowData[columnNames[i]] = DataVal(col.getText());
                        }
                    }
                    result.addRow(Row(std::move(rowData)));
                }
            } catch (const SQLite::Exception &e) {
                ErrorLog(std::string("查询失败: ") + e.what());
            }
            return result;
        }

        /// @brief 开始事务
        void beginTransaction() const {
            if (m_database) {
                m_database->exec("BEGIN TRANSACTION");
            }
        }

        /// @brief 提交事务
        void commit() const {
            if (m_database) {
                m_database->exec("COMMIT");
            }
        }

        /// @brief 回滚事务
        void rollback() const {
            if (m_database) {
                m_database->exec("ROLLBACK");
            }
        }

        /// @brief 获取最后插入的 rowid
        [[nodiscard]] i64 lastInsertRowId() const {
            return m_database ? m_database->getLastInsertRowid() : 0;
        }

        /// @brief 获取最近语句影响的行数
        [[nodiscard]] int changes() const {
            return m_database ? m_database->getChanges() : 0;
        }

        /// @brief 获取底层数据库对象（高级用法）
        [[nodiscard]] SQLite::Database &raw() { return *m_database; }
        [[nodiscard]] const SQLite::Database &raw() const { return *m_database; }

    private:
        std::unique_ptr<SQLite::Database> m_database;

        static int getDbMode(const bool createIfNotExist, const bool readOnly) {
            int mode = readOnly ? SQLite::OPEN_READONLY : SQLite::OPEN_READWRITE;
            if (createIfNotExist) {
                mode |= SQLite::OPEN_CREATE;
            }
            return mode;
        }

        /// @brief 格式化 SQL 语句，将 {} 替换为参数
        template<typename... Args>
        static std::string formatSql(const std::string_view sql, Args &&... args) {
            std::string result(sql);
            size_t pos = 0;
            ((pos = replaceNext(result, pos, std::forward<Args>(args))), ...);
            return result;
        }

        template<typename T>
        static size_t replaceNext(std::string &sql, const size_t pos, T &&value) {
            const size_t found = sql.find("{}", pos);
            if (found == std::string::npos) {
                return std::string::npos;
            }
            const auto escaped = escapeValue(std::forward<T>(value));
            sql.replace(found, 2, escaped);
            return found + escaped.size();
        }

        // 字符串转义
        static std::string escapeValue(const std::string &value) {
            return "'" + value + "'"; // SQLiteCpp 内部会处理转义
        }

        static std::string escapeValue(const char *value) {
            return "'" + std::string(value) + "'";
        }

        static std::string escapeValue(const std::string_view value) {
            return "'" + std::string(value) + "'";
        }

        // 数值类型直接转换
        template<typename T>
            requires std::integral<T> || std::floating_point<T>
        static std::string escapeValue(T value) {
            return std::to_string(value);
        }

        // null 处理
        static std::string escapeValue(std::nullptr_t) {
            return "NULL";
        }
    };
}
