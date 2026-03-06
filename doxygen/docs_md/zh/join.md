@page join 参与 DaoLib 开发

#  开发流程

欢迎参与 DaoLib 的开发。本项目采用受保护的主分支，所有变更必须通过 Pull Request 完成。请遵循以下流程：

---

## 1. Fork 仓库

点击 "Fork"将仓库复刻到你的个人仓库 

---

## 2. 克隆仓库

将代码下载到本地

```bash
git clone https://github.com/<your-username>/DaoLib.git
cd DaoLib
```

---

## 3. 创建工作分支

请勿在主分支上直接开发，可以用你的 Github 用户名作为分支名

```bash
git checkout -b feature/your-feature
```

---

## 4. 编写代码并提交

```bash
git add .
git commit -m "简要说明本次修改"
git push origin feature/your-feature
```

---

## 5. 提交 Pull Request

前往 GitHub，创建 PR：

* 选择你的分支 → 本仓库的 `main`
* 填写标题和说明

PR 会自动触发审核流程。

---

## 6. 审核与修改

维护者会对 PR 进行审核，如需调整，请在同一分支继续提交，PR 会自动更新。

---

## 7. 合并

通过审核后，维护者会将 PR 合并进主分支。

---

## 8. 行为准则

请保持讨论礼貌、清晰、专业。

---

如有任何问题，可在 Issue 区和提交反馈或在 Discussions 区讨论。 

# 项目依赖

以下是经过测试可构建的版本，其他版本可自行尝试

- C++ 23
- Cmake 3.31
- vcpkg :sdl3 3.2.26 sdl3-image 3.2.4[png] <!--第三方库具体内容可参考 vcpkg.json 文件--> 