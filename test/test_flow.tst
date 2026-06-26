load output/Codegen/Main.hack,
output-file test/test_flow.out,
output-list RAM[0]%D2.6.2 RAM[256]%D2.6.2;

set RAM[0] 256,
repeat 5000 {
  ticktock;
}
output;
