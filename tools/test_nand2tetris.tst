// 测试脚本：验证斐波那契程序
// 预期结果：RAM[0] (SP) = 257, RAM[256] = 5 (fib(5) = 5)

load test_nand2tetris.hack,
output-file test_nand2tetris.out,
compare-to test_nand2tetris.cmp,
output-list RAM[0]%D2.6.2 RAM[256]%D2.6.2;

set RAM[0] 256,
repeat 1000 {
  ticktock;
}
output;
