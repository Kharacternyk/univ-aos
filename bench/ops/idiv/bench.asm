include "../macro/bench.asm"
mov bx, 7
bench
mov dx, 0
mov ax, -42
idiv bx
endbench
