format elf64 executable 3
segment executable readable
entry $

macro bench {
    xor ecx, ecx
    bench:
}

macro endbench {
    inc ecx
    cmp ecx, count
    jb bench

    mov rdi, 0
    mov eax, 60
    syscall
}
