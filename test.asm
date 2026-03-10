# this are the values
string msg1 "hello\n"
string msg2 "message from loop\n"

# here we do a comparation and decide in wich label the cpu should jmp
mov 10 R2
cmp 10 R2
je label2

.label1
    mov 1 R0
    mov R0 R1
    add 2 R1
    hlt

.label2
    # the cpu jumped on the label2 because the R2 is equal to 10 and we print the first value "hello\n"
    mov 1 R0
    mov msg1 R1
    mov 6 R2
    syscall

    # we store manualy the word "hi\n" in the code section after the "hello\n"
    store 104 536
    store 105 537
    store 10 538

    # we now print the "hi\n"
    mov 1 R0
    mov 536 R1
    mov 3 R2
    syscall

    # we set a counter variable
    mov 0 R3

.startloop
    # we compare the counter variable to 10 and when it hits 10 then we end the loop
    cmp 10 R3
    jge endloop

    # we print the message "message from loop\n"
    mov 1 R0
    mov msg2 R1
    mov 18 R2
    syscall

    # increasing the counter variable
    inc R3
    jmp startloop

.endloop
    # a simple calculation that results to 10
    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1

    # R4 should be 105 now because this is the value stored in the address of 537
    load 537 R4

    hlt
