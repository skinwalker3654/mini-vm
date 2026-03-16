# this are the values
string msg1 "hello\n"
string msg2 "message from loop\n"

number len1 size msg1
number len2 size msg2
number len3 3

unitialized buffer 3

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
    load len1 R2
    syscall

    # we store manualy the word "hi\n" in the code section after the "hello\n"
    store 104 372
    store 105 373
    store 10 374

    # we now print the "hi\n"
    mov 1 R0
    mov 372 R1
    load len3 R2
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
    load len2 R2
    syscall

    # increasing the counter variable
    inc R3
    jmp startloop

.endloop
    # R4 should be 105 now because this is the value stored in the address of 539
    load 373 R4
    load len1 R5
    load len2 R6
    store 2 len1

    # we get users input
    mov 2 R0
    mov buffer R1
    mov 3 R2
    syscall

    # we put a new line at the end
    store 10 371

    # we print users input
    mov 1 R0
    mov buffer R1
    mov 4 R2
    syscall

    # a simple calculation that results to 10
    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1
 
    push 20
    pop R5

    call func1
    call func2

    hlt

# function1
.func1
    mov 600 R7
    ret

# function2
.func2
    mov 700 R8
    ret

# this are the values that the R7 and R8 should have if the call opcode does not work corectly
mov 1000 R7
mov 2000 R8
