# 基于简单优先法的微型程序语言编译器 — 阶段性总结与 TODO

## 一、项目总览

**目标：** 基于简单优先文法，实现 Micro-C 语言的词法分析、语法分析、语义分析和目标代码生成。

**当前进度：** 词法分析 ✅ → 语法分析 ✅ → 语义分析 ✅ → 代码生成 ✅

---

## 二、已完成的功能

### 2.1 词法分析（Lexer）✅

| 功能 | 状态 | 说明 |
|------|------|------|
| 关键字识别 | ✅ | `int char double struct if else while return read write main` |
| 标识符识别 | ✅ | `[a-zA-Z_][a-zA-Z0-9_]*` |
| 整型常数 | ✅ | 连续数字 |
| 字符常数 | ✅ | `'A'` `'\n'` 等转义 |
| 运算符 | ✅ | `+ - * / > < == = .` |
| 界符 | ✅ | `( ) { } ; [ ] ,` |
| 注释跳过 | ✅ | `//` 单行、`/* */` 多行 |
| 错误检测 | ✅ | 非法字符、未闭合注释、标识符以数字开头 |
| Token 输出 | ✅ | `output/Lexer/tokens.txt`（Line/Col/Type/Value 二元式） |

### 2.2 语法分析（Parser）✅

| 功能 | 状态 | 说明 |
|------|------|------|
| 文法定义 | ✅ | 29 条产生式，覆盖声明/赋值/表达式/控制流/函数调用 |
| FIRSTVT 计算 | ✅ | 不动点迭代算法 |
| LASTVT 计算 | ✅ | 不动点迭代算法 |
| 优先关系表 | ✅ | 标准算法 + 括号匹配特殊关系 |
| 移进-归约算法 | ✅ | 标准简单优先文法算法 |
| 语法错误检测 | ✅ | 错误定位（行号+列号） |
| 容错机制 | ✅ | 跳过错误继续分析 |
| 语法分析树 | ✅ | `output/Parser/parse_tree.txt` |
| 移进-归约日志 | ✅ | `output/Parser/trace.txt` |

### 2.3 语义分析（SemanticAnalyzer）✅

| 功能 | 状态 | 说明 |
|------|------|------|
| 符号表 | ✅ | 记录变量名、类型、偏移量 |
| 结构体类型表 | ✅ | 记录结构体名称和成员 |
| 未声明变量检测 | ✅ | `variable 'x' is not declared` |
| 类型不匹配警告 | ✅ | `assigning char to int` 等 |
| 未定义结构体检测 | ✅ | `struct type 'Point' is not defined` |
| 非结构体成员访问 | ✅ | `variable 'x' is not a struct type` |
| 结构体成员不存在 | ✅ | `struct 'Point' has no member 'z'` |
| 临时变量管理 | ✅ | `t0, t1, t2, ...` |
| 标签管理 | ✅ | `L0, L1, L2, ...` |
| 回填机制 | ✅ | 控制流跳转地址回填 |

### 2.4 代码生成（CodeGenerator）✅

| 功能 | 状态 | 说明 |
|------|------|------|
| 四元式生成 | ✅ | `(op, arg1, arg2, result)` |
| Jack VM 指令生成 | ✅ | `push/pop/add/sub/call/label/goto/if-goto/return` |
| 变量映射 | ✅ | 用户变量 → `local N`，临时变量 → `temp N` |
| 常量映射 | ✅ | 字面量 → `constant N`，字符 → ASCII |
| 赋值代码 | ✅ | `push src; pop dst` |
| 算术代码 | ✅ | `add/sub/call Math.multiply/call Math.divide` |
| 关系代码 | ✅ | `lt/gt/eq` |
| 控制流代码 | ✅ | `label/goto/if-goto` |
| 函数调用代码 | ✅ | `call funcName nArgs` |
| read/write 映射 | ✅ | `Keyboard.readInt` / `Output.printInt` |
| VM 验证 | ✅ | VMEmulator 运行输出正确（`50`） |

### 2.5 测试覆盖 ✅

| 测试项 | 状态 |
|--------|------|
| `int x ;` | ✅ |
| `char c ;` | ✅ |
| `double d ;` | ✅ |
| `struct Point { int a ; } ;` | ✅ |
| `struct Point p ;` | ✅ |
| `x = 10 ;` | ✅ |
| `c = 'A' ;` | ✅ |
| `z = x + y * 2 ;`（优先级） | ✅ |
| `z = x < y ;` | ✅ |
| `z = x > b ;` | ✅ |
| `read(x) ;` | ✅ |
| `write(z) ;` | ✅ |
| `foo(x) ;`（自定义函数） | ✅ |
| `bar() ;`（无参函数） | ✅ |
| `set(p, x, 10) ;`（双参函数） | ✅ |
| `z = get(a, i) ;`（双参表达式） | ✅ |
| `while(x < 10) { x = x + 1 ; } ;` | ✅ |
| `if(x > 5) { x = 1 ; } else { x = 2 ; } ;` | ✅ |
| `return 0 ;` | ✅ |
| 类型不匹配警告 | ✅ |
| 未声明变量检测 | ✅ |
| 未定义结构体检测 | ✅ |

---

## 三、已知限制（简单优先文法根本限制）

| 限制 | 原因 | 替代方案 |
|------|------|---------|
| 多语句序列 | `P → S ; P` 右递归，`S ;` 被过早归约为 `P` | 单语句测试，或改用 LR 分析 |
| `z = x == b` | `==` 的 FIRSTVT 传播污染 `=` 的关系 | 用函数 `z = eq(x, b)` |
| 成员赋值 `p.x = 10` | `id . id` 被过早归约为 `E` | 用函数 `set(p, x, 10)` |
| 数组赋值 `a[0] = 5` | `id [ E ]` 被过早归约为 `E` | 用函数 `set(a, 0, 5)` |
| 数组声明 `int a[10]` | `id = [` 与 `E [ E ]` 冲突 | 用变量 `int a ;` |
| 函数体多语句 | `{ }` 内多语句触发归约时机问题 | 单语句块 |

---

## 四、待实现/改进的功能

### 4.1 语义分析增强

- [ ] **函数参数检查**：检查 `read(x)` / `write(x)` 的参数数量和类型
- [ ] **结构体成员类型检查**：访问 `p.x` 时检查 `x` 的类型
- [ ] **数组类型支持**：区分 `int` 和 `int[]`
- [ ] **隐式类型转换规则**：`int ← char` 的具体转换逻辑

### 4.2 代码生成增强

- [ ] **while/if 的代码优化**：解决条件代码在标签之前生成的问题
- [ ] **结构体成员访问代码**：`p.x` 映射到正确的内存偏移
- [ ] **数组访问代码**：`a[i]` 映射到 `base + i` 的内存地址
- [ ] **函数定义代码**：生成 `function name nLocals` 头部
- [ ] **多函数支持**：`Main.main` + 其他函数

### 4.3 文法扩展（需换用更强分析方法）

- [ ] **多语句序列**：解决 `P → S ; P` 的归约时机问题
- [ ] **`==` 运算符**：解决 FIRSTVT 传播污染
- [ ] **成员赋值**：`p.x = 10` 直接支持
- [ ] **数组赋值**：`a[0] = 5` 直接支持
- [ ] **函数体**：`int main() { ... }` 多语句块
- [ ] **for 循环**：`for(init; cond; step) { ... }`

### 4.4 文档完善

- [ ] 更新 `编译器前端说明.md`：加入代码生成阶段说明
- [ ] 更新 `实验总结.md`：加入最终测试结果
- [ ] 更新 `Lexer.md`：同步所有已实现/未实现的功能

### 4.5 虚拟机验证

- [ ] 完整 VMEmulator 测试：用 `verify_test/Main.vm` 运行验证
- [ ] VMTranslator 链路：`.vm → .asm → .hack → CPUEmulator`（如需）
- [ ] 多测试用例验证：不同输入下的输出正确性

---

## 五、文件结构

```
ShiXvn_Compiler/
├── CMakeLists.txt
├── main.cpp                    ← 驱动程序
├── test.txt                    ← 源程序输入
├── Lexer.md                    ← 词法规范与语法定义
├── 编译器前端说明.md             ← Pipeline 完整说明
├── 实验总结.md                  ← 实验报告
├── todolist.md                 ← 本文档
│
├── include/                    ← 头文件（仅声明）
│   ├── Token.h
│   ├── ErrorHandler.h
│   ├── SymbolTable.h
│   ├── Lexer.h
│   ├── Grammar.h
│   ├── SemanticAnalyzer.h
│   └── PrecedenceParser.h
│
├── src/                        ← 实现文件
│   ├── Lexer.cpp
│   ├── ErrorHandler.cpp
│   ├── SymbolTable.cpp
│   ├── Grammar.cpp
│   ├── SemanticAnalyzer.cpp
│   └── PrecedenceParser.cpp
│
├── output/                     ← 中间产物
│   ├── Lexer/
│   │   └── tokens.txt
│   ├── Parser/
│   │   ├── productions.txt
│   │   ├── firstvt.txt
│   │   ├── lastvt.txt
│   │   ├── precedence_table.txt
│   │   ├── trace.txt
│   │   └── parse_tree.txt
│   └── Codegen/
│       ├── quads.txt
│       └── target.vm
│
└── verify_test/
    └── Main.vm                 ← VMEmulator 验证用
```
