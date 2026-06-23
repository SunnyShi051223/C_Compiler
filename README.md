# ShiXvn Compiler — 基于简单优先文法的 Micro-C 编译器

将 C 语言子集（Micro-C）编译为 Jack VM 字节码的编译器，采用**简单优先文法（Simple Precedence Grammar, SPG）**进行语法分析。

---

## 编译流程

```
源代码 ──→ [词法分析器] ──→ Token 流
                              │
                              ▼
                     [优先语法分析器] ──→ 语法分析树 + AST
                              │
                              ▼
                     [语义分析器] ──→ 四元式（中间代码）
                              │
                              ▼
                     [代码生成器] ──→ Jack VM 字节码
```

---

## 支持的数据类型

| 类型 | 说明 | 示例 |
|------|------|------|
| `int` | 整型（32 位） | `int x ;` |
| `char` | 字符型（8 位，存储 ASCII 值） | `char c ;` |
| `double` | 浮点型（编译时截断为整数输出） | `double d ;` |
| `struct` | 结构体（自定义复合类型） | `struct Point { int x ; int y ; } ;` |

---

## 支持的运算

### 算术运算

| 运算符 | 说明 | 示例 |
|--------|------|------|
| `+` | 加法 | `z = x + y ;` |
| `-` | 减法 | `z = x - y ;` |
| `*` | 乘法 | `z = x * y ;` |
| `/` | 除法 | `z = x / y ;` |

运算符优先级：`*` `/` > `+` `-`（与 C 语言一致）

### 关系运算

| 运算符 | 说明 | 示例 |
|--------|------|------|
| `<` | 小于 | `z = x < y ;` |
| `>` | 大于 | `z = x > y ;` |

关系运算结果为 `1`（真）或 `0`（假）。

### 赋值运算

| 语法 | 说明 | 示例 |
|------|------|------|
| `id = expr` | 简单赋值 | `x = 10 ;` |

---

## 支持的语句

### 声明语句

```c
int x ;                          // 整型变量声明
char c ;                         // 字符型变量声明
double d ;                       // 浮点型变量声明
struct Point { int x ; int y ; } ;  // 结构体类型定义
struct Point p ;                 // 结构体变量声明
```

### 赋值语句

```c
x = 10 ;                        // 整数赋值
c = 'A' ;                       // 字符赋值（ASCII 值）
d = 3.14 ;                      // 浮点赋值（截断为整数存储）
z = x + y * 2 ;                 // 表达式赋值
```

### 输入输出

```c
read(x) ;                       // 从键盘读取整数到变量 x
write(x) ;                      // 输出变量 x 的值
write(c) ;                      // 输出字符变量 c 的 ASCII 值
write(42) ;                     // 输出常量 42
```

### 控制流

#### if-else 语句

```c
if ( z > x ) {
    write(x) ;
} else {
    write(y) ;
} ;
```

- 条件为真（非零）时执行 `then` 分支
- 条件为假（零）时执行 `else` 分支
- `else` 分支可省略（需保持语法结构）

#### while 循环

```c
while ( x < 15 ) {
    x = x + 1 ;
} ;
```

- 条件为真时重复执行循环体
- 条件为假时退出循环

### return 语句

```c
return 0 ;                      // 返回值
return x ;                      // 返回变量值
```

### 语句块

```c
{                               // 进入新作用域
    int local_var ;             // 局部变量
    local_var = 42 ;
    write(local_var) ;
}                               // 退出作用域
```

---

## 支持的表达式

### 算术表达式

```c
z = x + y * 2 ;                 // 乘法优先于加法
z = (x + y) * 2 ;               // 括号改变优先级
z = x - y / 3 ;                 // 除法优先于减法
```

### 关系表达式

```c
z = x < y ;                     // 小于比较
z = x > y ;                     // 大于比较
```

### 函数调用表达式

```c
y = read(x) ;                   // 带返回值的函数调用
z = foo(x) ;                    // 单参数函数调用
z = bar() ;                     // 无参函数调用
z = add(x, y) ;                 // 双参数函数调用
z = set(a, b, c) ;              // 三参数函数调用
```

### 括号表达式

```c
z = (x + y) * 2 ;               // 括号改变运算优先级
```

### 字面量

| 类型 | 语法 | 示例 |
|------|------|------|
| 整数 | 数字序列 | `42`, `0`, `1024` |
| 浮点数 | 数字.数字 | `3.14`, `0.5`（截断为整数） |
| 字符 | `'字符'` | `'A'`, `'0'`, `'\n'`, `'\t'` |

---

## 作用域

编译器支持**嵌套作用域**：

```c
int x = 10 ;                    // 全局作用域

{
    int x = 20 ;                // 内层作用域，隐藏外层 x
    write(x) ;                  // 输出 20
}

write(x) ;                      // 输出 10（外层 x 恢复）
```

- 内层变量可隐藏外层同名变量
- 退出作用域时内层变量自动销毁

---

## 注释

```c
// 单行注释

/*
   多行注释
   可以跨越多行
*/
```

---

## 内置函数

| 函数 | 参数 | 说明 |
|------|------|------|
| `read()` | 无 | 从键盘读取整数，返回读取的值 |
| `write(expr)` | 1 个 | 输出表达式的值（整数或字符 ASCII） |

---

## 目标代码：Jack VM

编译器生成 [Jack VM](https://www.nand2tetris.org/) 字节码，可在 VMEmulator 中运行。

### 生成的指令类型

| 指令 | 说明 |
|------|------|
| `push segment index` | 将值压入栈 |
| `pop segment index` | 将栈顶值弹出到指定位置 |
| `add`, `sub` | 算术运算 |
| `lt`, `gt`, `eq` | 关系运算 |
| `not` | 逻辑非 |
| `label L` | 标签定义 |
| `goto L` | 无条件跳转 |
| `if-goto L` | 条件跳转（栈顶为假时跳转） |
| `call func nArgs` | 函数调用 |
| `return` | 函数返回 |
| `function name nLocals` | 函数定义 |

### 内置函数映射

| 源代码 | Jack VM 调用 |
|--------|-------------|
| `read()` | `call Keyboard.readInt 0` |
| `write(x)` | `call Output.printInt 1` |
| `x * y` | `call Math.multiply 2` |
| `x / y` | `call Math.divide 2` |

### 运行方式

将生成的 `Main.vm` 连同 Jack OS 库文件（`Array.vm`, `Keyboard.vm`, `Math.vm`, `Memory.vm`, `Output.vm`, `Screen.vm`, `String.vm`, `Sys.vm`）一起加载到 VMEmulator 中运行。

---

## 示例程序

```c
// 声明变量
int x ;
int y ;
int z ;
char c ;

// 赋值
x = 10 ;
y = 20 ;
c = 'A' ;

// 算术表达式
z = x + y * 2 ;         // z = 10 + 20*2 = 50

// 条件判断
if ( z > x ) {
    write(x) ;          // 输出 10
} else {
    write(y) ;
} ;

// 循环
while ( x < 15 ) {
    x = x + 1 ;         // x 从 10 递增到 15
} ;

write(z) ;              // 输出 50

return 0 ;
```

预期输出：`65 3 20 10 50`

---

## 构建与运行

```bash
# 编译器
g++ -std=c++17 -Iinclude -o ShiXvn_Compiler.exe \
    main.cpp src/Lexer.cpp src/ErrorHandler.cpp src/SymbolTable.cpp \
    src/Grammar.cpp src/PrecedenceParser.cpp src/SemanticAnalyzer.cpp \
    src/AST.cpp src/CodegenVisitor.cpp

# 运行编译
./ShiXvn_Compiler.exe test.txt

# 输出文件
# output/Lexer/tokens.txt        — 词法分析结果
# output/Parser/productions.txt  — 产生式列表
# output/Parser/firstvt.txt      — FIRSTVT 集合
# output/Parser/lastvt.txt       — LASTVT 集合
# output/Parser/precedence_table.txt — 优先关系表
# output/Parser/trace.txt        — 移进-归约过程
# output/Parser/parse_tree.txt   — 语法分析树
# output/Parser/ast.txt          — 抽象语法树
# output/Codegen/quads.txt       — 四元式序列
# output/Codegen/Main.vm         — Jack VM 字节码
```

---

## 文法定义（简单优先文法）

```
P → S ; P | S ;
S → int id | char id | double id
S → struct id { P } | struct id id
S → id = E
S → E
S → while ( E ) { P }
S → if ( E ) { P } else { P }
S → return E
E → E + E | E - E | E * E | E / E
E → E < E | E > E
E → id ( ) | id ( E ) | id ( E , E ) | id ( E , E , E )
E → ( E ) | id | num | char
```

**非终结符**：`P`（程序）、`S`（语句）、`E`（表达式）

**终结符**：`id`, `num`, `char`, `int`, `double`, `struct`, `if`, `else`, `while`, `return`, `=`, `+`, `-`, `*`, `/`, `<`, `>`, `(`, `)`, `{`, `}`, `;`, `,`, `$`

---

## 项目结构

```
ShiXvn_Compiler/
├── CMakeLists.txt              — CMake 构建配置
├── main.cpp                    — 编译器入口
├── test.txt                    — 测试输入文件
├── include/                    — 头文件
│   ├── Token.h                 — Token 定义
│   ├── ErrorHandler.h          — 错误处理
│   ├── SymbolTable.h           — 词法符号表
│   ├── Lexer.h                 — 词法分析器
│   ├── Grammar.h               — 文法定义
│   ├── PrecedenceParser.h      — 优先语法分析器
│   ├── SemanticAnalyzer.h      — 语义分析器
│   ├── AST.h                   — 抽象语法树
│   └── CodegenVisitor.h        — 代码生成访问器
├── src/                        — 源文件
│   ├── Lexer.cpp               — 词法分析实现
│   ├── ErrorHandler.cpp        — 错误处理实现
│   ├── SymbolTable.cpp         — 符号表实现
│   ├── Grammar.cpp             — 文法实现（产生式、FIRSTVT/LASTVT、优先表）
│   ├── PrecedenceParser.cpp    — 语法分析实现（移进-归约、AST 构建）
│   ├── SemanticAnalyzer.cpp    — 语义分析实现（作用域栈、四元式、VM 生成）
│   ├── AST.cpp                 — AST 实现
│   └── CodegenVisitor.cpp      — 代码生成访问器实现
├── output/                     — 编译输出
│   ├── Lexer/tokens.txt
│   ├── Parser/                 — 语法分析中间产物
│   └── Codegen/                — 目标代码
│       ├── quads.txt           — 四元式
│       └── Main.vm             — Jack VM 字节码
└── verify_test/                — 参考 VM 文件
```
