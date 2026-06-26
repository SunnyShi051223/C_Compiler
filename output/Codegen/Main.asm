// function Main.main 66
(Main.main)
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1

// push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1

// call Memory.alloc 1
@RETURN_0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Memory.alloc
0;JMP
(RETURN_0)

// pop local 0
@LCL
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 1
@LCL
D=M
@1
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 0
@SP
AM=M-1
D=M
@R5
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
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

// push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 2
@SP
AM=M-1
D=M
@R7
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 3
@SP
AM=M-1
D=M
@R8
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 5
@5
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 4
@SP
AM=M-1
D=M
@R9
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 5
@5
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 5
@SP
AM=M-1
D=M
@R10
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 6
@SP
AM=M-1
D=M
@R11
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 7
@7
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

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 7
@SP
AM=M-1
D=M
@R12
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 15
@LCL
D=M
@15
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 16
@LCL
D=M
@16
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1

// call Memory.alloc 1
@RETURN_1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Memory.alloc
0;JMP
(RETURN_1)

// pop local 5
@LCL
D=M
@5
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 66
@66
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 17
@LCL
D=M
@17
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 101
@101
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 18
@LCL
D=M
@18
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
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

// push constant 103
@103
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 19
@LCL
D=M
@19
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 105
@105
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 20
@LCL
D=M
@20
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 110
@110
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 21
@LCL
D=M
@21
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 5
@5
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 22
@LCL
D=M
@22
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 23
@LCL
D=M
@23
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 23
@LCL
D=M
@23
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_2)

// pop local 24
@LCL
D=M
@24
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 25
@LCL
D=M
@25
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 25
@LCL
D=M
@25
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_3)

// pop local 26
@LCL
D=M
@26
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 27
@LCL
D=M
@27
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 27
@LCL
D=M
@27
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_4
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_4)

// pop local 28
@LCL
D=M
@28
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 3
@3
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 29
@LCL
D=M
@29
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 29
@LCL
D=M
@29
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_5
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_5)

// pop local 30
@LCL
D=M
@30
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 5
@LCL
D=M
@5
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 4
@4
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 31
@LCL
D=M
@31
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 31
@LCL
D=M
@31
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_6
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_6)

// pop local 32
@LCL
D=M
@32
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// call Output.println 0
@RETURN_7
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.println
0;JMP
(RETURN_7)

// pop local 33
@LCL
D=M
@33
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// label L0
(Main.main$L0)

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE_8
D;JLT
@SP
A=M-1
M=0
@END_9
0;JMP
(TRUE_8)
@SP
A=M-1
M=-1
(END_9)

// pop local 34
@LCL
D=M
@34
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 34
@LCL
D=M
@34
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// not
@SP
A=M-1
M=!M

// if-goto L1
@SP
AM=M-1
D=M
@Main.main$L1
D;JNE

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 2
@LCL
D=M
@2
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 35
@LCL
D=M
@35
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 35
@LCL
D=M
@35
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printInt 1
@RETURN_10
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printInt
0;JMP
(RETURN_10)

// pop temp 0
@SP
AM=M-1
D=M
@R5
M=D

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 36
@LCL
D=M
@36
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 36
@LCL
D=M
@36
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// goto L0
@Main.main$L0
0;JMP

// label L1
(Main.main$L1)

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// label L5
(Main.main$L5)

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE_11
D;JLT
@SP
A=M-1
M=0
@END_12
0;JMP
(TRUE_11)
@SP
A=M-1
M=-1
(END_12)

// pop local 37
@LCL
D=M
@37
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 37
@LCL
D=M
@37
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// not
@SP
A=M-1
M=!M

// if-goto L6
@SP
AM=M-1
D=M
@Main.main$L6
D;JNE

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 3
@LCL
D=M
@3
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// label L3
(Main.main$L3)

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 9
@9
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE_13
D;JLT
@SP
A=M-1
M=0
@END_14
0;JMP
(TRUE_13)
@SP
A=M-1
M=-1
(END_14)

// pop local 38
@LCL
D=M
@38
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 38
@LCL
D=M
@38
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// not
@SP
A=M-1
M=!M

// if-goto L4
@SP
AM=M-1
D=M
@Main.main$L4
D;JNE

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 3
@LCL
D=M
@3
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 39
@LCL
D=M
@39
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 40
@LCL
D=M
@40
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 40
@LCL
D=M
@40
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 41
@LCL
D=M
@41
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 39
@LCL
D=M
@39
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 41
@LCL
D=M
@41
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE_15
D;JGT
@SP
A=M-1
M=0
@END_16
0;JMP
(TRUE_15)
@SP
A=M-1
M=-1
(END_16)

// pop local 42
@LCL
D=M
@42
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 42
@LCL
D=M
@42
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// not
@SP
A=M-1
M=!M

// if-goto L2
@SP
AM=M-1
D=M
@Main.main$L2
D;JNE

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 3
@LCL
D=M
@3
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 43
@LCL
D=M
@43
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 43
@LCL
D=M
@43
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 4
@LCL
D=M
@4
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 44
@LCL
D=M
@44
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 44
@LCL
D=M
@44
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 45
@LCL
D=M
@45
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 45
@LCL
D=M
@45
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 46
@LCL
D=M
@46
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 47
@LCL
D=M
@47
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 47
@LCL
D=M
@47
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 4
@LCL
D=M
@4
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 48
@LCL
D=M
@48
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// label L2
(Main.main$L2)

// push local 3
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 49
@LCL
D=M
@49
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 49
@LCL
D=M
@49
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 3
@LCL
D=M
@3
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// goto L3
@Main.main$L3
0;JMP

// label L4
(Main.main$L4)

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 50
@LCL
D=M
@50
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 50
@LCL
D=M
@50
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// goto L5
@Main.main$L5
0;JMP

// label L6
(Main.main$L6)

// call Output.println 0
@RETURN_17
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.println
0;JMP
(RETURN_17)

// pop local 51
@LCL
D=M
@51
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// call Memory.alloc 1
@RETURN_18
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Memory.alloc
0;JMP
(RETURN_18)

// pop local 6
@LCL
D=M
@6
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 69
@69
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 52
@LCL
D=M
@52
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 110
@110
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 53
@LCL
D=M
@53
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
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

// push constant 100
@100
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 54
@LCL
D=M
@54
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 3
@3
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 1
@SP
AM=M-1
D=M
@R6
M=D

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push temp 1
@R6
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop that 0
@THAT
D=M
@0
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 55
@LCL
D=M
@55
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 0
@0
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 56
@LCL
D=M
@56
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 56
@LCL
D=M
@56
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_19
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_19)

// pop local 57
@LCL
D=M
@57
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 58
@LCL
D=M
@58
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 58
@LCL
D=M
@58
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_20
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_20)

// pop local 59
@LCL
D=M
@59
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 6
@LCL
D=M
@6
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 60
@LCL
D=M
@60
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 60
@LCL
D=M
@60
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printChar 1
@RETURN_21
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printChar
0;JMP
(RETURN_21)

// pop local 61
@LCL
D=M
@61
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// call Output.println 0
@RETURN_22
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.println
0;JMP
(RETURN_22)

// pop local 62
@LCL
D=M
@62
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// label L7
(Main.main$L7)

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@TRUE_23
D;JLT
@SP
A=M-1
M=0
@END_24
0;JMP
(TRUE_23)
@SP
A=M-1
M=-1
(END_24)

// pop local 63
@LCL
D=M
@63
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 63
@LCL
D=M
@63
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// not
@SP
A=M-1
M=!M

// if-goto L8
@SP
AM=M-1
D=M
@Main.main$L8
D;JNE

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 2
@LCL
D=M
@2
A=D+A
D=M
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

// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D

// push that 0
@THAT
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 64
@LCL
D=M
@64
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 64
@LCL
D=M
@64
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// call Output.printInt 1
@RETURN_25
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@6
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Output.printInt
0;JMP
(RETURN_25)

// pop temp 0
@SP
AM=M-1
D=M
@R5
M=D

// push local 2
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push constant 1
@1
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

// pop local 65
@LCL
D=M
@65
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// push local 65
@LCL
D=M
@65
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// pop local 2
@LCL
D=M
@2
D=D+A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D

// goto L7
@Main.main$L7
0;JMP

// label L8
(Main.main$L8)

// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

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

