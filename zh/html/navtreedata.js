/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "DaoUI", "index.html", [
    [ "DaoLib", "index.html", null ],
    [ "参与 DaoLib 开发", "join.html", [
      [ "开发流程", "join.html#autotoc_md0", [
        [ "1. Fork 仓库", "join.html#autotoc_md2", null ],
        [ "2. 克隆仓库", "join.html#autotoc_md4", null ],
        [ "3. 创建工作分支", "join.html#autotoc_md6", null ],
        [ "4. 编写代码并提交", "join.html#autotoc_md8", null ],
        [ "5. 提交 Pull Request", "join.html#autotoc_md10", null ],
        [ "6. 审核与修改", "join.html#autotoc_md12", null ],
        [ "7. 合并", "join.html#autotoc_md14", null ],
        [ "8. 行为准则", "join.html#autotoc_md16", null ]
      ] ],
      [ "项目依赖", "join.html#autotoc_md18", null ]
    ] ],
    [ "DaoLib 从零基础到精通教程", "tutorial.html", [
      [ "DaoLib 从零基础到精通教程", "tutorial.html#autotoc_md22", [
        [ "简介", "tutorial.html#autotoc_md23", [
          [ "主要特性", "tutorial.html#autotoc_md24", null ]
        ] ],
        [ "安装与构建", "tutorial.html#autotoc_md26", [
          [ "系统要求", "tutorial.html#autotoc_md27", null ],
          [ "使用 vcpkg 管理依赖", "tutorial.html#autotoc_md28", null ],
          [ "集成到 CMake 项目", "tutorial.html#autotoc_md29", null ],
          [ "独立构建 DaoLib", "tutorial.html#autotoc_md30", null ]
        ] ],
        [ "快速开始", "tutorial.html#autotoc_md32", [
          [ "最小应用示例", "tutorial.html#autotoc_md33", null ],
          [ "第一个完整应用", "tutorial.html#autotoc_md34", null ]
        ] ],
        [ "核心概念", "tutorial.html#autotoc_md36", [
          [ "1. 应用 (App)", "tutorial.html#autotoc_md37", null ],
          [ "2. 窗口 (Window)", "tutorial.html#autotoc_md38", null ],
          [ "3. 页面 (Page)", "tutorial.html#autotoc_md39", null ],
          [ "4. 组件 (Components)", "tutorial.html#autotoc_md40", [
            [ "按钮 (SimpleButton)", "tutorial.html#autotoc_md41", null ],
            [ "输入框 (InputBox)", "tutorial.html#autotoc_md42", null ],
            [ "文本 (Text)", "tutorial.html#autotoc_md43", null ],
            [ "图片 (Image)", "tutorial.html#autotoc_md44", null ],
            [ "矩形 (Rectangle)", "tutorial.html#autotoc_md45", null ],
            [ "纹理 (AtlasTexture)", "tutorial.html#autotoc_md46", null ]
          ] ],
          [ "5. 渲染批处理 (VertexBatchBuilder)", "tutorial.html#autotoc_md47", null ],
          [ "6. 上下文管理器 (Context)", "tutorial.html#autotoc_md48", null ]
        ] ],
        [ "完整示例解析", "tutorial.html#autotoc_md50", [
          [ "示例 1：计数器应用", "tutorial.html#autotoc_md51", null ],
          [ "示例 2：用户登录界面", "tutorial.html#autotoc_md52", null ]
        ] ],
        [ "高级功能", "tutorial.html#autotoc_md54", [
          [ "托盘系统", "tutorial.html#autotoc_md55", null ],
          [ "多窗口管理", "tutorial.html#autotoc_md56", null ],
          [ "纹理管理", "tutorial.html#autotoc_md57", null ],
          [ "自定义 Drawable 组件", "tutorial.html#autotoc_md58", null ]
        ] ],
        [ "最佳实践", "tutorial.html#autotoc_md60", [
          [ "1. 资源管理", "tutorial.html#autotoc_md61", null ],
          [ "2. 性能优化", "tutorial.html#autotoc_md62", null ],
          [ "3. 代码组织", "tutorial.html#autotoc_md63", null ],
          [ "4. 错误处理", "tutorial.html#autotoc_md64", null ]
        ] ],
        [ "常见问题", "tutorial.html#autotoc_md66", [
          [ "Q1: 如何调试 UI 布局？", "tutorial.html#autotoc_md67", null ],
          [ "Q2: 如何实现页面切换？", "tutorial.html#autotoc_md68", null ],
          [ "Q3: 如何处理中文输入？", "tutorial.html#autotoc_md69", null ],
          [ "Q4: 如何自定义组件样式？", "tutorial.html#autotoc_md70", null ]
        ] ],
        [ "故障排除", "tutorial.html#autotoc_md72", [
          [ "构建问题", "tutorial.html#autotoc_md73", null ],
          [ "运行时问题", "tutorial.html#autotoc_md74", null ]
        ] ],
        [ "下一步", "tutorial.html#autotoc_md76", null ]
      ] ]
    ] ],
    [ "使用 DaoLib 开发", "use.html", null ],
    [ "Bug 列表", "bug.html", null ],
    [ "命名空间", "namespaces.html", [
      [ "命名空间列表", "namespaces.html", "namespaces_dup" ],
      [ "命名空间成员", "namespacemembers.html", [
        [ "全部", "namespacemembers.html", null ],
        [ "函数", "namespacemembers_func.html", null ],
        [ "变量", "namespacemembers_vars.html", null ],
        [ "类型定义", "namespacemembers_type.html", null ],
        [ "枚举", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "概念", "concepts.html", "concepts" ],
    [ "类", "annotated.html", [
      [ "类列表", "annotated.html", "annotated_dup" ],
      [ "类索引", "classes.html", null ],
      [ "类继承关系", "hierarchy.html", "hierarchy" ],
      [ "类成员", "functions.html", [
        [ "全部", "functions.html", "functions_dup" ],
        [ "函数", "functions_func.html", "functions_func" ],
        [ "变量", "functions_vars.html", null ],
        [ "类型定义", "functions_type.html", null ],
        [ "枚举", "functions_enum.html", null ],
        [ "枚举值", "functions_eval.html", null ]
      ] ]
    ] ],
    [ "文件", "files.html", [
      [ "文件列表", "files.html", "files_dup" ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"AnimatedSprite_8hpp.html",
"classdao_1_1BoundingBox.html#ae6d7ef9eadc2c39d706136376acd0903",
"classdao_1_1SimpleTextureButtonStyle.html#a836705b180a043707ec8a6d9472a7ef5",
"classdao_1_1ifc_1_1IGeneralPage.html#ab240866f12ef7bced2a369c871293f80",
"structdao_1_1WindowCommand.html"
];

var SYNCONMSG = '点击 关闭 面板同步';
var SYNCOFFMSG = '点击 开启 面板同步';
var LISTOFALLMEMBERS = '所有成员列表';