include "../macro/bench.asm"
mov ebx, 7
bench
xadd eax, ebx
endbench
