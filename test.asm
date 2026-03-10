string msg1 "hello\n"
string msg2 "message for loop\n"

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
    mov msg1 R1
    mov 6 R2
    syscall

    store 104 535
    store 105 536
    store 10 537

    mov 1 R0
    mov 535 R1
    mov 3 R2
    syscall

    mov 0 R3
.startloop
    cmp 10 R3
    jge endloop

    mov 1 R0
    mov msg2 R1
    mov 17 R2
    syscall

    inc R3
    jmp startloop

.endloop
    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1

    load 536 R4
    hlt
