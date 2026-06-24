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
| `int` | 整型 | `int x ;` |
| `char` | 字符型（存储 ASCII 值） | `char c ;` |
| `double` | 浮点型（截断为整数存储） | `double d ;` |
| `struct` | 结构体（自定义复合类型） | `struct Point { int a ; char b ; } ;` |
| `int[]` | 整型数组 | `int arr[10] ;` |
| `char[]` | 字符型数组 | `char buf[64] ;` |
| `double[]` | 浮点型数组 | `double vals[5] ;` |

---

## 内置函数

| 函数 | 语法 | 说明 | 返回值 |
|------|------|------|--------|
| `read()` | `x = read() ;` | 从键盘读取一个整数 | 读取到的整数 |
| `write()` | `write(expr) ;` | 输出表达式的值 | 无 |
| `set()` | `set(base, member, value) ;` | 写入结构体成员或数组元素 | 0 |
| `get()` | `x = get(base, member) ;` | 读取结构体成员或数组元素 | 读取到的值 |

### `read()` — 键盘输入

从键盘读取一个整数，返回读取的值。

```c
int x ;
x = read() ;       // 等待用户输入整数，存入 x
write(x) ;         // 输出 x
```

**生成的 VM 代码：**
```asm
push constant 0          ; 创建空字符串（提示信息）
call String.new 1
call Keyboard.readInt 1  ; 读取整数
pop local 0              ; 存入变量
```

### `write()` — 输出

输出表达式的值（整数或字符的 ASCII 值）。

```c
write(42) ;        // 输出常量 42
write(x) ;         // 输出变量 x
write(c) ;         // 输出字符 c 的 ASCII 值
write(x + y * 2) ; // 输出表达式结果
```

**生成的 VM 代码：**
```asm
push local 0
call Output.printInt 1
pop temp 0
```

### `set()` — 结构体/数组写入

将值写入结构体成员或数组元素。**成员名自动转换为偏移量**。

```c
// 结构体成员写入
struct Point { int a ; char b ; } ;
struct Point p ;
set(p, a, 100) ;    // p.a = 100（自动计算 a 的偏移量为 0）
set(p, b, 65) ;     // p.b = 65（自动计算 b 的偏移量为 1）

// 数组元素写入
int arr[5] ;
set(arr, 0, 10) ;   // arr[0] = 10
set(arr, 2, 30) ;   // arr[2] = 30
int i ;
i = 3 ;
set(arr, i, 99) ;   // arr[3] = 99（支持变量作为索引）
```

**生成的 VM 代码（set(p, a, 100)）：**
```asm
push local 7         ; p（堆内存指针）
push constant 0      ; a 的偏移量（自动计算）
push constant 100    ; 要写入的值
pop temp 1           ; 暂存值
add                  ; p + offset = 绝对地址
pop pointer 1        ; THAT = 绝对地址
push temp 1          ; 恢复值
pop that 0           ; 写入内存
```

### `get()` — 结构体/数组读取

读取结构体成员或数组元素的值。**成员名自动转换为偏移量**。

```c
// 结构体成员读取
int val ;
val = get(p, a) ;       // val = p.a
write(get(p, a)) ;      // 直接输出 p.a

// 数组元素读取
write(get(arr, 0)) ;    // 输出 arr[0]
int i ;
i = 2 ;
write(get(arr, i)) ;    // 输出 arr[2]（支持变量作为索引）
```

**生成的 VM 代码（get(p, a)）：**
```asm
push local 7         ; p（堆内存指针）
push constant 0      ; a 的偏移量（自动计算）
add                  ; p + offset = 绝对地址
pop pointer 1        ; THAT = 绝对地址
push that 0          ; 读取内存
```

---

## 支持的运算

### 算术运算

| 运算符 | 说明 | 优先级 | 示例 |
|--------|------|--------|------|
| `*` | 乘法 | 高 | `z = x * y ;` |
| `/` | 除法 | 高 | `z = x / y ;` |
| `+` | 加法 | 低 | `z = x + y ;` |
| `-` | 减法 | 低 | `z = x - y ;` |

> `*` 和 `/` 映射到 Jack OS 的 `Math.multiply` 和 `Math.divide`

### 关系运算

| 运算符 | 说明 | 示例 |
|--------|------|------|
| `<` | 小于 | `if ( x < 10 ) { ... }` |
| `>` | 大于 | `if ( x > 5 ) { ... }` |

关系运算结果为 `1`（真）或 `0`（假）。

### 赋值

```c
x = 10 ;            // 简单赋值
z = x + y * 2 ;     // 表达式赋值
c = 'A' ;           // 字符赋值（ASCII 值）
d = 3.14 ;          // 浮点赋值（截断为 3）
```

---

## 支持的语句

### 声明语句

```c
int x ;                             // 整型变量
char c ;                            // 字符型变量
double d ;                          // 浮点型变量
int arr[10] ;                       // 整型数组（分配 10 个字的堆内存）
struct Point { int a ; char b ; } ; // 结构体类型定义
struct Point p ;                    // 结构体变量（自动分配堆内存）
```

### 赋值语句

```c
x = 10 ;
z = x + y * 2 ;
set(p, a, 66) ;         // 结构体成员赋值
set(arr, i, 99) ;       // 数组元素赋值
```

### 输入输出

```c
x = read() ;            // 从键盘读取整数
write(x) ;              // 输出整数
write(get(p, a)) ;      // 输出结构体成员
write(get(arr, i)) ;    // 输出数组元素
```

### if-else 语句

```c
if ( z > x ) {
    write(x) ;
} else {
    write(y) ;
} ;
```

- 条件为真（非零）时执行 `then` 分支
- 条件为假（零）时执行 `else` 分支
- **注意：`{ }` 内的语句必须以 `;` 结尾**

### while 循环

```c
while ( x < 15 ) {
    x = x + 1 ;
} ;
```

- 条件为真时重复执行循环体
- 条件为假时退出循环
- **注意：`{ }` 内的语句必须以 `;` 结尾**

### return 语句

```c
return 0 ;
return x ;
```

---

## 表达式

### 算术表达式

```c
z = x + y * 2 ;         // 乘法优先于加法
z = (x + y) * 2 ;       // 括号改变优先级
z = x - y / 3 ;         // 除法优先于减法
```

### 关系表达式

```c
z = x < y ;             // 小于比较
z = x > y ;             // 大于比较
```

### 函数调用表达式

```c
x = read() ;            // 无参函数，带返回值
write(x) ;              // 单参数函数，无返回值
z = foo(x) ;            // 单参数函数，带返回值
z = add(x, y) ;         // 双参数函数
set(p, a, 66) ;         // 三参数函数
```

### 字面量

| 类型 | 语法 | 示例 |
|------|------|------|
| 整数 | 数字序列 | `42`, `0`, `1024` |
| 浮点数 | 数字.数字 | `3.14`（截断为 `3`） |
| 字符 | `'字符'` | `'A'`（ASCII 65）, `'\n'`, `'\t'` |

---

## 注释

```c
// 单行注释

/*
   多行注释
*/
```

---

## 完整示例

### 示例 1：基本输入输出

```c
int x ;
x = read() ;
write(x) ;
return 0 ;
```

运行时等待用户输入一个整数，然后输出该整数。

### 示例 2：结构体操作

```c
struct Point {
    int x ;
    int y ;
} ;
struct Point p ;

set(p, x, 10) ;         // p.x = 10
set(p, y, 20) ;         // p.y = 20

write(get(p, x)) ;      // 输出 10
write(get(p, y)) ;      // 输出 20
write(get(p, x) + get(p, y)) ;  // 输出 30

return 0 ;
```

### 示例 3：数组操作

```c
int arr[5] ;
int i ;

// 写入数组
set(arr, 0, 100) ;
set(arr, 1, 200) ;
set(arr, 2, 300) ;

// 读取数组
write(get(arr, 0)) ;    // 输出 100
write(get(arr, 1)) ;    // 输出 200

// 变量作为索引
i = 2 ;
write(get(arr, i)) ;    // 输出 300

// 修改数组元素
set(arr, i, 999) ;
write(get(arr, i)) ;    // 输出 999

return 0 ;
```

### 示例 4：综合测试

```c
int x ;
int y ;
int z ;
char c ;
int arr[3] ;
struct Point { int a ; char b ; } ;
struct Point p ;

// 赋值
x = read() ;
y = 20 ;
c = 'A' ;

// 结构体操作
set(p, a, 66) ;
write(get(p, a)) ;      // 输出 66

// 数组操作
set(arr, 0, 10) ;
set(arr, 1, 20) ;
set(arr, 2, 30) ;
write(get(arr, 0)) ;    // 输出 10

// 算术表达式
z = x + y * 2 ;

// 条件判断
if ( z > x ) {
    write(x) ;
} else {
    write(y) ;
} ;

// 循环
while ( x < 15 ) {
    x = x + 1 ;
} ;

write(z) ;

return 0 ;
```

---

## 文法定义（简单优先文法）

```
P → S ; P | S ;
S → int id | char id | double id
S → int id [ num ] | char id [ num ] | double id [ num ]
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

**终结符**：`id`, `num`, `char`, `int`, `double`, `struct`, `if`, `else`, `while`, `return`, `=`, `+`, `-`, `*`, `/`, `<`, `>`, `(`, `)`, `[`, `]`, `{`, `}`, `;`, `,`, `$`

---

## 目标代码：Jack VM

编译器生成 [Jack VM](https://www.nand2tetris.org/) 字节码，可在 VMEmulator 中运行。

### 内置函数映射

| 源代码 | Jack VM 代码 |
|--------|-------------|
| `read()` | `push constant 0` + `call String.new 1` + `call Keyboard.readInt 1` |
| `write(x)` | `push x` + `call Output.printInt 1` |
| `x * y` | `push x` + `push y` + `call Math.multiply 2` |
| `x / y` | `push x` + `push y` + `call Math.divide 2` |
| `set(base, member, value)` | 内联 5 条指令（计算地址 + 写入内存） |
| `get(base, member)` | 内联 3 条指令（计算地址 + 读取内存） |
| `int arr[N]` | `push constant N` + `call Memory.alloc 1` |
| `struct T p` | `push constant 成员数` + `call Memory.alloc 1` |

### 运行方式

将生成的 `Main.vm` 连同 Jack OS 库文件一起加载到 VMEmulator 中：

```
Main.vm          ← 编译器生成
Array.vm         ← Jack OS
Keyboard.vm
Math.vm
Memory.vm
Output.vm
Screen.vm
String.vm
Sys.vm
```

---

## 构建与运行

```bash
# 编译编译器
g++ -std=c++17 -Iinclude -o ShiXvn_Compiler.exe \
    main.cpp src/Lexer.cpp src/ErrorHandler.cpp src/SymbolTable.cpp \
    src/Grammar.cpp src/PrecedenceParser.cpp src/SemanticAnalyzer.cpp \
    src/AST.cpp src/CodegenVisitor.cpp

# 编译 Micro-C 源文件
./ShiXvn_Compiler.exe test.txt
```

### 输出文件

| 文件 | 说明 |
|------|------|
| `output/Lexer/tokens.txt` | 词法分析结果（Token 流） |
| `output/Parser/productions.txt` | 产生式列表 |
| `output/Parser/firstvt.txt` | FIRSTVT 集合 |
| `output/Parser/lastvt.txt` | LASTVT 集合 |
| `output/Parser/precedence_table.txt` | 优先关系表 |
| `output/Parser/trace.txt` | 移进-归约过程 |
| `output/Parser/parse_tree.txt` | 语法分析树 |
| `output/Parser/ast.txt` | 抽象语法树（AST） |
| `output/Codegen/quads.txt` | 四元式序列 |
| `output/Codegen/Main.vm` | Jack VM 字节码 |

---

## 项目结构

```
ShiXvn_Compiler/
├── CMakeLists.txt
├── main.cpp                    — 编译器入口
├── test.txt                    — 测试输入文件
├── README.md                   — 本文档
├── include/                    — 头文件
│   ├── Token.h                 — Token 定义
│   ├── ErrorHandler.h          — 错误处理
│   ├── SymbolTable.h           — 词法符号表
│   ├── Lexer.h                 — 词法分析器
│   ├── Grammar.h               — 文法定义
│   ├── PrecedenceParser.h      — 优先语法分析器
│   ├── SemanticAnalyzer.h      — 语义分析器（作用域栈、四元式、VM 生成）
│   ├── AST.h                   — 抽象语法树
│   └── CodegenVisitor.h        — 代码生成访问器
├── src/                        — 源文件
│   ├── Lexer.cpp               — 词法分析（浮点数、字符、注释）
│   ├── ErrorHandler.cpp        — 错误处理
│   ├── SymbolTable.cpp         — 符号表
│   ├── Grammar.cpp             — 文法（产生式、FIRSTVT/LASTVT、优先表）
│   ├── PrecedenceParser.cpp    — 语法分析（移进-归约、AST 构建、语义动作）
│   ├── SemanticAnalyzer.cpp    — 语义分析（作用域栈、四元式、VM 代码生成）
│   ├── AST.cpp                 — AST 实现
│   └── CodegenVisitor.cpp      — 代码生成访问器
└── output/                     — 编译输出
    ├── Lexer/
    ├── Parser/
    └── Codegen/
```
