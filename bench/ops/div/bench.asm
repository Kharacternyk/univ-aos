include "../macro/bench.asm"
mov bx, 7
bench
mov ax, -42
mov dx, 3
div bx
endbench
