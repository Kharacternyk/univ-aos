include "../macro/bench.asm"
mov al, 7
mov ah, -42
bench
mul al
endbench
