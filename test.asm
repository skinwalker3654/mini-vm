string name "hello\n"

mov 10 R2
cmp 10 R2
je label2

.label1
    mov 1 R0
    mov R0 R1
    add 2 R1
    hlt

.label2
    mov 1 R0
    mov name R1
    mov 6 R2
    syscall

    store 104 520
    store 105 521
    store 10 522

    mov 1 R0
    mov 520 R1
    mov 3 R2
    syscall

    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1

    hlt
