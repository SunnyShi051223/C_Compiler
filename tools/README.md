# VMTranslator 使用说明

## 概述

VMTranslator 是一个将 nand2tetris VM 代码（.vm 文件）转换为 Hack 汇编代码（.asm 文件）的工具。

## 编译

```bash
g++ -std=c++17 -o VMTranslator.exe tools/VMTranslator.cpp
```

或者在项目根目录下使用 CMake 构建（如果已配置）：

```bash
cmake -B build
cmake --build build --target VMTranslator
```

## 使用方法

### 基本用法

```bash
./VMTranslator input.vm
```

这将生成 `input.asm` 文件。

### 指定输出文件

```bash
./VMTranslator input.vm output.asm
```

### 翻译多个文件

```bash
./VMTranslator file1.vm file2.vm output.asm
```

当翻译多个文件时，会自动生成初始化代码（调用 Sys.init）。

## 支持的 VM 指令

### 算术/逻辑指令

- `add` - 加法
- `sub` - 减法
- `neg` - 取负
- `eq` - 等于比较
- `gt` - 大于比较
- `lt` - 小于比较
- `and` - 逻辑与
- `or` - 逻辑或
- `not` - 逻辑非

### 内存访问指令

- `push constant i` - 压入常数 i
- `push local i` - 压入局部变量 i
- `push argument i` - 压入参数 i
- `push this i` - 压入 this 段 i
- `push that i` - 压入 that 段 i
- `push temp i` - 压入临时变量 i (R5-R12)
- `push pointer 0/1` - 压入 THIS/THAT
- `push static i` - 压入静态变量 i
- `pop local i` - 弹出到局部变量 i
- `pop argument i` - 弹出到参数 i
- `pop this i` - 弹出到 this 段 i
- `pop that i` - 弹出到 that 段 i
- `pop temp i` - 弹出到临时变量 i
- `pop pointer 0/1` - 弹出到 THIS/THAT
- `pop static i` - 弹出到静态变量 i

### 程序流程指令

- `label label` - 定义标签
- `goto label` - 无条件跳转
- `if-goto label` - 条件跳转（栈顶非零时跳转）

### 函数指令

- `function name nLocals` - 定义函数，nLocals 为局部变量数量
- `call name nArgs` - 调用函数，nArgs 为参数数量
- `return` - 从函数返回

## 示例

### 输入文件 (test.vm)

```vm
// 计算 1 + 2
function Simple.add 0
push constant 1
push constant 2
add
return
```

### 输出文件 (test.asm)

```asm
// function Simple.add 0
(Simple.add)

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// return
@LCL
D=M
@R13
M=D
@5
A=D-A
D=M
@R14
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@R13
AM=M-1
D=M
@THAT
M=D
@R13
AM=M-1
D=M
@THIS
M=D
@R13
AM=M-1
D=M
@ARG
M=D
@R13
AM=M-1
D=M
@LCL
M=D
@R14
A=M
0;JMP
```

## 测试

### 使用 CPUEmulator 测试

1. 翻译 VM 文件：
   ```bash
   ./VMTranslator test.vm
   ```

2. 使用 Assembler 汇编：
   ```bash
   ./Assembler test.asm
   ```

3. 使用 CPUEmulator 运行：
   ```bash
   ./CPUEmulator test.tst
   ```

### 测试脚本示例 (test.tst)

```
load test.hack,
output-file test.out,
compare-to test.cmp,
output-list RAM[0]%D2.6.2 RAM[256]%D2.6.2;

set RAM[0] 256,
repeat 20 {
  ticktock;
}
output;
```

## 注意事项

1. **栈初始化**：VMTranslator 不会自动初始化栈指针（SP=256）。如果需要，可以在生成的 .asm 文件开头添加初始化代码，或者使用 `--init` 选项（如果实现了的话）。

2. **静态变量**：静态变量使用 `@FileName.i` 格式，其中 FileName 是输入文件名（不含扩展名），i 是索引。

3. **函数标签**：所有标签都使用 `FunctionName$Label` 格式，以避免不同函数间的标签冲突。

4. **多文件翻译**：当翻译多个文件时，会自动生成初始化代码并调用 Sys.init。

## 与 ShiXvn_Compiler 集成

1. 使用 ShiXvn_Compiler 编译 mico C 源代码：
   ```bash
   ./ShiXvn_Compiler source.c
   ```

2. 使用 VMTranslator 翻译生成的 .vm 文件：
   ```bash
   ./VMTranslator output/Codegen/Main.vm
   ```

3. 使用 Assembler 汇编生成的 .asm 文件：
   ```bash
   ./Assembler output/Codegen/Main.asm
   ```

4. 使用 CPUEmulator 测试生成的 .hack 文件：
   ```bash
   ./CPUEmulator test.tst
   ```

## 故障排除

### 常见问题

1. **"Cannot open file" 错误**
   - 检查输入文件路径是否正确
   - 确保文件存在且有读取权限

2. **生成的汇编代码不正确**
   - 检查 VM 代码语法是否正确
   - 确保所有函数都有正确的返回语句

3. **CPUEmulator 测试失败**
   - 检查测试脚本是否正确
   - 确保 .hack 文件已正确生成
   - 检查期望输出文件（.cmp）是否存在

## 扩展功能

### 可能的改进

1. 添加 `--init` 选项自动生成栈初始化代码
2. 支持从标准输入读取 VM 代码
3. 添加调试输出选项
4. 支持更多的错误检查和报告
5. 优化生成的汇编代码

## 许可证

此工具是 ShiXvn_Compiler 项目的一部分，仅供学习和研究使用。
