@page database 数据库模块参考 {#database}

[TOC]

# 数据库模块参考 — Sqlite / Row / Result

---

## Sqlite — SQLite 数据库 {#db-sqlite}

`dao::db::Sqlite` 封装 SQLiteCpp，提供类型安全的 SQL 操作和自动列名映射。

@ref database "→ 返回教程：SQLite 数据库"

### 头文件

```cpp
#include <database/sqlite/Sqlite.hpp>
```

### 构造函数

```cpp
Sqlite(const std::string &filePath,
       bool createIfNotExist = true,
       bool readOnly = false);
```

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `filePath` | — | 数据库文件路径 |
| `createIfNotExist` | `true` | 文件不存在时自动创建 |
| `readOnly` | `false` | 只读模式 |

### 状态检查

```cpp
bool isOpen() const noexcept;
explicit operator bool() const noexcept;  // 等效于 isOpen()
```

### exec — 执行 SQL

```cpp
template<typename... Args>
int exec(std::string_view sql, Args &&... args);
```

使用 `{}` 作为参数占位符，返回影响的行数。

```cpp
db.exec("INSERT INTO users (name, score) VALUES ({}, {})", "Alice", 100);
db.exec("UPDATE users SET score = {} WHERE id = {}", 200, 1);
db.exec("DELETE FROM users WHERE id = {}", 3);
```

### select — 查询

```cpp
template<typename... Args>
Result select(std::string_view sql, Args &&... args);
```

返回 `Result` 对象，可遍历访问每一行。

```cpp
Result result = db.select("SELECT * FROM users WHERE score > {}", 50);
for (const auto &row : result) {
    std::string name = row.asString("name");
    i32 score = row.asInt32("score");
}
```

### 事务

```cpp
void beginTransaction() const;
void commit() const;
void rollback() const;
```

```cpp
db.beginTransaction();
db.exec("UPDATE accounts SET balance = balance - 100 WHERE id = {}", 1);
db.exec("UPDATE accounts SET balance = balance + 100 WHERE id = {}", 2);
db.commit();  // 或 rollback()
```

### 其他方法

| 方法 | 说明 |
|------|------|
| `lastInsertRowId()` | 获取最后插入的 rowid |
| `changes()` | 获取最近语句影响的行数 |
| `raw()` | 获取底层 `SQLite::Database` 引用（高级用法） |

---

## Row — 查询结果行 {#db-row}

`Row` 表示查询结果中的一行数据，通过列名字符串访问。

### 方法

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `asString(col)` | `const string &` | 获取字符串值 |
| `asInt32(col)` | `i32` | 获取 32 位整数 |
| `asInt64(col)` | `i64` | 获取 64 位整数 |
| `asFloat64(col)` | `f64` | 获取浮点数 |
| `asBool(col)` | `bool` | 获取布尔值（非 0 即 true） |
| `has(col)` | `bool` | 检查列是否存在 |
| `operator[](col)` | `const string &` | 通过列名访问（返回字符串） |

所有取值方法对不存在的列返回默认值（0、空字符串等）。

---

## Result — 查询结果集 {#db-result}

`Result` 是 `Row` 的集合，支持范围 for 和索引访问。

### 方法

| 方法 | 说明 |
|------|------|
| `empty()` | 结果集是否为空 |
| `size()` | 行数 |
| `operator[](i)` | 按索引访问行 |
| `begin() / end()` | 迭代器（支持范围 for） |

### 示例

```cpp
Result result = db.select("SELECT id, name, score FROM users");

if (result.empty()) {
    dao::Log{dao::LogLevel::INFO}("没有数据");
    return;
}

dao::Log{}.fmt("共 {} 条记录", result.size());

for (const auto &row : result) {
    dao::Log{}.fmt("ID: {}, 姓名: {}, 分数: {}",
                    row.asInt64("id"),
                    row.asString("name"),
                    row.asInt32("score"));
}

// 索引访问
auto &firstRow = result[0];
```

---

## DataVal — 单元格值 {#db-dataval}

`DataVal` 是单元格值的内部表示，通常不直接使用。

| 方法 | 说明 |
|------|------|
| `asString()` | 字符串 |
| `asInt32()` | 32 位整数 |
| `asInt64()` | 64 位整数 |
| `asFloat32()` | 32 位浮点 |
| `asFloat64()` | 64 位浮点 |