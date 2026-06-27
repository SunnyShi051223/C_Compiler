# Agent使用说明

## 项目概述

这是一个**基于简单优先法的微型程序语言编译器**课程设计项目，位于 `D:\c_cpp\ShiXvn_Compiler`。

### 项目信息
- **项目名称**：基于简单优先法的微型程序语言编译器设计与实现
- **编程语言**：C++17
- **构建工具**：CMake
- **目标平台**：Hack虚拟机（nand2tetris）
- **学生信息**：需要在报告封面填写（姓名、班级、指导教师）

---

## 已完成的工作

### 1. 课程报告（Word文档）

| 文件 | 位置 | 大小 | 内容 |
|------|------|------|------|
| 第一章 | `report/课程报告_基于简单优先法的微型程序语言编译器设计与实现.docx` | 41.7KB | 封面+目录+实训任务概述 |
| 第二章 | `report/第二章_系统分析.docx` | 45.6KB | 源语言定义、目标语言定义、功能需求 |
| 第三章 | `report/第三章_系统开发.docx` | 47.1KB | 总体设计、详细设计、编码实现、测试 |
| 第四章 | `report/第四章_实训小结.docx` | 43.6KB | 设计特点、实训体会、改进建议、总结 |
| 参考文献 | `report/参考文献.docx` | 38.4KB | 32篇参考文献，含格式说明 |

### 2. 辅助文档

| 文件 | 位置 | 大小 | 内容 |
|------|------|------|------|
| PPT大纲 | `report/PPT大纲.md` | 45.9KB | 22页PPT的详细内容和讲解要点 |
| 代码讲解说明 | `report/代码讲解说明.md` | 36.7KB | 所有源代码文件的函数设计和实现详解 |
| 本说明 | `report/AGENT使用说明.md` | - | Agent使用说明 |

### 3. 生成脚本

| 文件 | 位置 | 功能 |
|------|------|------|
| `generate_report.py` | `report/` | 生成第一章Word文档 |
| `generate_chapter2.py` | `report/` | 生成第二章Word文档 |
| `generate_chapter3.py` | `report/` | 生成第三章Word文档 |
| `generate_chapter4.py` | `report/` | 生成第四章Word文档 |
| `generate_references.py` | `report/` | 生成参考文献Word文档 |

---

## 待完成的工作

### 1. 用户需要手动完成

| 任务 | 说明 | 优先级 |
|------|------|--------|
| 填写封面信息 | 学生姓名、专业班级、指导教师 | ⭐⭐⭐ |
| 绘制图表 | 约13个需要手动绘制的图表 | ⭐⭐⭐ |
| 合并文档 | 将5个Word文档合并成一个完整报告 | ⭐⭐⭐ |
| 统一格式 | 字体、字号、行间距、页码等 | ⭐⭐ |
| 制作PPT | 根据PPT大纲制作演示文稿 | ⭐⭐⭐ |
| 准备答辩 | 演练讲解、准备回答问题 | ⭐⭐ |

### 2. 需要绘制的图表清单

#### 第一章（4个）
- 图1-1 编译器工具链示意图
- 图1-2 编译器编译流程图
- 图1-3 工具链组成图
- 图1-4 项目目录结构图

#### 第二章（2个）
- 图2-1 Hack虚拟机内存布局图
- 图2-2 函数调用栈帧结构图

#### 第三章（7个）
- 图3-1 编译器整体架构图
- 图3-2 编译流程图
- 图3-3 词法分析器状态转换图
- 图3-4 简单优先法分析过程示例
- 图3-5 函数调用栈帧结构图
- 图3-6 测试用例1运行结果截图
- 图3-7 测试用例2运行结果截图

---

## 文件结构

```
D:\c_cpp\ShiXvn_Compiler\
├── main.cpp                    # 主程序入口
├── include/                    # 头文件目录
│   ├── Token.h
│   ├── Lexer.h
│   ├── Grammar.h
│   ├── PrecedenceParser.h
│   ├── SemanticAnalyzer.h
│   ├── CodegenVisitor.h
│   ├── AST.h
│   ├── ErrorHandler.h
│   └── SymbolTable.h
├── src/                        # 源代码目录
│   ├── Lexer.cpp
│   ├── Grammar.cpp
│   ├── PrecedenceParser.cpp
│   ├── SemanticAnalyzer.cpp
│   ├── CodegenVisitor.cpp
│   ├── AST.cpp
│   ├── ErrorHandler.cpp
│   └── SymbolTable.cpp
├── tools/
│   └── VMTranslator.cpp       # VM翻译器
├── test/                       # 测试用例
│   ├── test_flow.txt          # 简单算术运算测试
│   └── test_bubble.txt        # 冒泡排序测试
├── output/                     # 输出目录
│   ├── Lexer/tokens.txt       # Token序列
│   ├── Parser/                # 语法分析输出
│   └── Codegen/               # 代码生成输出
└── report/                     # 报告目录（所有文档在这里）
    ├── 课程报告_*.docx         # 第一章
    ├── 第二章_*.docx
    ├── 第三章_*.docx
    ├── 第四章_*.docx
    ├── 参考文献.docx
    ├── PPT大纲.md
    ├── 代码讲解说明.md
    ├── AGENT使用说明.md        # 本文件
    └── generate_*.py          # 生成脚本
```

---

## 技术要点

### 1. 编译器架构

```
源程序 → 词法分析(Lexer) → 语法分析(PrecedenceParser) → 语义分析(SemanticAnalyzer) → 代码生成 → VM指令
```

### 2. 核心算法

- **词法分析**：有限自动机（DFA）
- **语法分析**：简单优先法（FIRSTVT/LASTVT、优先关系表、移进-归约）
- **语义分析**：属性文法、语法制导翻译
- **代码生成**：访问者模式

### 3. 文法定义

- **产生式数量**：27条
- **非终结符**：P（程序）、S（语句）、E（表达式）
- **终结符**：关键字、标识符、常量、运算符、分隔符

### 4. 支持的语言特性

- 数据类型：int、char、double、数组、结构体
- 控制结构：if-else、while
- 函数：定义、调用、参数传递、返回值
- 输入输出：read、write

---

## 使用Python生成文档

### 环境要求
- Python环境：`D:\Anaconda3\envs\ReadPaper\python.exe`
- 依赖库：python-docx

### 生成命令

```powershell
# 生成第一章
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_report.py"

# 生成第二章
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_chapter2.py"

# 生成第三章
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_chapter3.py"

# 生成第四章
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_chapter4.py"

# 生成参考文献
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_references.py"
```

---

## 编译和测试

### 编译项目

```powershell
cd D:\c_cpp\ShiXvn_Compiler
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```

### 运行测试

```powershell
# 测试1：简单算术运算
.\cmake-build-debug\ShiXvn_Compiler.exe test\test_flow.txt

# 测试2：冒泡排序
.\cmake-build-debug\ShiXvn_Compiler.exe test\test_bubble.txt
```

### 完整工具链测试

```powershell
# 1. 编译
.\ShiXvn_Compiler.exe test\test_flow.txt

# 2. VMTranslator
.\VMTranslator.exe output\Codegen\Main.vm output\Codegen\Main.asm

# 3. Assembler
D:\nand2tetris\nand2tetris\tools\Assembler.bat output\Codegen\Main.asm

# 4. CPUEmulator
D:\nand2tetris\nand2tetris\tools\CPUEmulator.bat output\Codegen\Main.hack
```

---

## 注意事项

### 1. 文档格式
- 正文字体：宋体，12号
- 标题字体：黑体
- 行间距：1.5倍
- 页边距：默认

### 2. 图表绘制
- 文中用**红色斜体**标注了需要绘制的图表位置
- 图表需要用户手动绘制，Agent无法自动生成

### 3. 个人信息
- 封面的姓名、班级、指导教师需要用户填写
- 使用"XXXX"作为占位符

### 4. 测试截图
- 第三章需要插入测试结果截图
- 需要用户运行测试并截图

### 5. PPT制作
- PPT大纲已提供详细内容
- 需要用户使用PowerPoint制作正式PPT

---

## 常见问题

### Q1: 如何重新生成某个章节的Word文档？
A: 运行对应的Python脚本即可，例如：
```powershell
& "D:\Anaconda3\envs\ReadPaper\python.exe" "D:\c_cpp\ShiXvn_Compiler\report\generate_chapter2.py"
```

### Q2: 如何修改报告内容？
A: 直接编辑对应的Python脚本，然后重新运行生成。

### Q3: 图表在哪里绘制？
A: 文中用红色斜体标注了图表位置，用户需要在Word文档中手动绘制或插入图片。

### Q4: 如何合并多个Word文档？
A: 可以使用Word的"插入→对象→文件中的文字"功能，或者手动复制粘贴。

---

## Agent工作建议

### 如果用户要求修改报告
1. 找到对应的Python脚本
2. 修改脚本中的内容
3. 重新运行脚本生成Word文档

### 如果用户要求添加新内容
1. 确定内容属于哪个章节
2. 在对应的Python脚本中添加内容
3. 重新运行脚本生成Word文档

### 如果用户要求生成新的文档
1. 参考现有的Python脚本结构
2. 创建新的Python脚本
3. 运行脚本生成文档

### 如果用户要求查看代码
1. 查看 `report/代码讲解说明.md`
2. 或直接查看 `src/` 目录下的源代码文件

---

## 联系和反馈

如有问题，请查看：
- 项目README：`D:\c_cpp\ShiXvn_Compiler\README.md`
- 测试说明：`D:\c_cpp\ShiXvn_Compiler\test\README.md`
- 代码讲解：`D:\c_cpp\ShiXvn_Compiler\report\代码讲解说明.md`

---

**最后更新**：2026年6月27日
**Agent版本**：Claude Code
