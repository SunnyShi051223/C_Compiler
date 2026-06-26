# 测试总结

## 测试结果

| 测试 | 测试方式 | 结果 |
|------|----------|------|
| test_flow.txt | CPUEmulator | ✅ 通过（RAM[256]=55） |
| test_bubble.txt | VMEmulator | ✅ 通过（输出排序结果） |

## 项目文件结构

```
D:\c_cpp\ShiXvn_Compiler\
├── ShiXvn_Compiler.exe    # mico C 编译器
├── VMTranslator.exe       # VM 转汇编工具
├── CMakeLists.txt         # 构建配置
├── OS\                    # nand2tetris OS 库
│   ├── Array.vm
│   ├── Math.vm
│   ├── Memory.vm
│   ├── Output.vm
│   ├── Screen.vm
│   ├── Keyboard.vm
│   ├── String.vm
│   └── Sys.vm
├── test\                  # 测试文件
│   ├── README.md          # 测试说明
│   ├── test_flow.txt      # 流程验证测试
│   └── test_bubble.txt    # 冒泡排序测试
├── output\Codegen\        # 编译输出
│   ├── Main.vm            # VM 代码
│   ├── Main.asm           # 汇编代码
│   └── Main.hack          # 机器码
└── tools\VMTranslator.cpp # VMTranslator 源码
```

## 工具链

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

## 工具位置

| 工具 | 位置 |
|------|------|
| ShiXvn_Compiler | `D:\c_cpp\ShiXvn_Compiler\ShiXvn_Compiler.exe` |
| VMTranslator | `D:\c_cpp\ShiXvn_Compiler\VMTranslator.exe` |
| Assembler | `D:\nand2tetris\nand2tetris\tools\Assembler.bat` |
| CPUEmulator | `D:\nand2tetris\nand2tetris\tools\CPUEmulator.bat` |
| VMEmulator | `D:\nand2tetris\nand2tetris\tools\VMEmulator.bat` |
