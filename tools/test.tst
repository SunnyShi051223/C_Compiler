load test.hack,
output-file test.out,
compare-to test.cmp,
output-list RAM[0]%D2.6.2 RAM[256]%D2.6.2;

set RAM[0] 256,
repeat 10000 {
  ticktock;
}
output;
