# 测试说明

## 测试文件

| 文件 | 说明 | 测试方式 |
|------|------|----------|
| `test_flow.txt` | 流程验证测试（计算 1+2+...+10） | VMEmulator + 硬件 CPU |
| `test_bubble.txt` | 冒泡排序测试（数组、IO） | 仅 VMEmulator |

---

## 测试1：test_flow.txt（完整硬件流程）

### 测试内容
- 变量声明和赋值
- 算术运算（加法）
- 循环（while）
- 条件判断（if-goto）
- 函数调用和返回

### 预期结果
- `RAM[256] = 55`（1+2+...+10=55）

### 测试步骤

#### 步骤1：编译 mico C 源代码

```bash
cd D:\c_cpp\ShiXvn_Compiler
.\ShiXvn_Compiler.exe test\test_flow.txt
```

输出文件：
- `output\Codegen\Main.vm`

#### 步骤2：VMTranslator 转换

```bash
.\VMTranslator.exe output\Codegen\Main.vm output\Codegen\Main.asm
```

输出文件：
- `output\Codegen\Main.asm`

#### 步骤3：Assembler 汇编

```bash
D:\nand2tetris\nand2tetris\tools\Assembler.bat output\Codegen\Main.asm
```

输出文件：
- `output\Codegen\Main.hack`

#### 步骤4：CPUEmulator 测试

**方式A：手动操作**

1. 打开 `D:\nand2tetris\nand2tetris\tools\CPUEmulator.bat`
2. File → Load Program → 选择 `output\Codegen\Main.hack`
3. **不需要手动设置 RAM[0]**（程序会自动初始化 SP=256）
4. 点击 Run（或按 F5）
5. 查看 RAM[256] 应该显示 55

**方式B：自动测试**

在 `output\Codegen` 目录下创建 `test.tst` 文件：
```
load Main.hack,
output-file test.out,
output-list RAM[0]%D2.6.2 RAM[256]%D2.6.2;

repeat 5000 {
  ticktock;
}
output;
```

运行：
```bash
cd output\Codegen
D:\nand2tetris\nand2tetris\tools\CPUEmulator.bat test.tst
```

预期输出：
```
|  RAM[0]  | RAM[256] |
|     257  |      55  |
```

**注意：不需要 `set RAM[0] 256`，因为程序开头会自动初始化栈指针。**

---

## 测试2：test_bubble.txt（仅 VMEmulator）

### 测试内容
- 数组声明和操作（set/get）
- 嵌套循环
- 字符输出（writeChar）
- 整数输出（write）
- 换行（println）
- 冒泡排序算法

### 预期输出
```
Begin
9 8 7 6 5 4 3 2 1 0
End
0 1 2 3 4 5 6 7 8 9
```

### 测试步骤

#### 步骤1：编译 mico C 源代码

```bash
cd D:\c_cpp\ShiXvn_Compiler
.\ShiXvn_Compiler.exe test\test_bubble.txt
```

输出文件：
- `output\Codegen\Main.vm`

#### 步骤2：使用 VMEmulator 运行

1. 打开 `D:\nand2tetris\nand2tetris\tools\VMEmulator.bat`
2. 点击 **Load Program**
3. 选择以下文件（按住 Ctrl 多选）：
   - `OS\Array.vm`
   - `OS\Math.vm`
   - `OS\Memory.vm`
   - `OS\Output.vm`
   - `OS\Screen.vm`
   - `OS\Keyboard.vm`
   - `OS\String.vm`
   - `OS\Sys.vm`
   - `output\Codegen\Main.vm`
4. 点击 **Run**（或按 F5）
5. 在输出区域查看结果

### 为什么不能用硬件 CPU？

| 项目 | 限制 |
|------|------|
| Hack 计算机 ROM 容量 | 32768 条指令 |
| bubble 排序 + OS 库 | 约 40000 条指令 |
| **超出** | **约 7000 条** |

bubble 排序使用了 Memory、Output 等 OS 库，这些库本身就很大，加上函数调用开销，总代码量超过了 ROM 容量。

---

## 工具链总结

```
mico C 源代码 (.txt)
    ↓ ShiXvn_Compiler
VM 代码 (.vm)
    ↓ VMTranslator
汇编代码 (.asm)
    ↓ Assembler
机器码 (.hack)
    ↓ CPUEmulator / VMEmulator
执行结果
```

### 工具位置

| 工具 | 位置 |
|------|------|
| ShiXvn_Compiler | `D:\c_cpp\ShiXvn_Compiler\ShiXvn_Compiler.exe` |
| VMTranslator | `D:\c_cpp\ShiXvn_Compiler\VMTranslator.exe` |
| Assembler | `D:\nand2tetris\nand2tetris\tools\Assembler.bat` |
| CPUEmulator | `D:\nand2tetris\nand2tetris\tools\CPUEmulator.bat` |
| VMEmulator | `D:\nand2tetris\nand2tetris\tools\VMEmulator.bat` |
| OS 库 | `D:\c_cpp\ShiXvn_Compiler\OS\` |

---

## 常见问题

### Q1：CPUEmulator 显示 "Comparison failure"

检查：
- RAM[0] 是否设置为 256
- 循环次数是否足够（使用 repeat 5000）
- 期望输出格式是否正确

### Q2：CPUEmulator 显示 "Illegal memory address"

检查：
- 程序是否访问了超出范围的内存地址
- 是否使用了 OS 库函数（需要 VMEmulator）

### Q3：VMEmulator 显示错误

检查：
- 是否加载了所有必要的 OS 库文件
- Main.vm 是否正确定义了 Main.main 函数
