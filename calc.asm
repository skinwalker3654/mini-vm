string panel "1) add\n2) sub\n3) mul\n4) div\n" 
number panellen size panel 

string question "\nPick one: " 
number questionlen size question 

string invalidnumber "Error: Invalid Number\n" 
number invalidnumberlen size invalidnumber 

string firstnum "Enter first number: "
number firstnumlen size firstnum

string secondnum "Enter second number: "
number secondnumlen size secondnum

string result "Result: "
number resultlen size result

string divisionerror "Error: Division by 0\n"
number divisionerrorlen size divisionerror

unitialized pick 1
unitialized num1 1
unitialized num2 1


# print the panel 
mov 1 R0
mov panel R1
load panellen R2
syscall 

# print the question 
mov 1 R0
mov question R1
load questionlen R2 
syscall 

# take users input 
mov 2 R0 
mov pick R1
mov 1 R2 
syscall 

load pick R3 
sub 48 R3

cmp 1 R3 
jl invalidnum 

cmp 4 R3 
jg invalidnum 

# take first number
mov 1 R0
mov firstnum R1
load firstnumlen R2
syscall

mov 2 R0
mov num1 R1
mov 1 R2
syscall

load num1 R4
sub 48 R4

# number validation
cmp 0 R4
jl invalidnum
cmp 9 R4
jg invalidnum

# take second number
mov 1 R0
mov secondnum R1
load secondnumlen R2
syscall

mov 2 R0
mov num2 R1
mov 1 R2
syscall

load num2 R5
sub 48 R5

# number validation
cmp 0 R5
jl invalidnum
cmp 9 R5
jg invalidnum
    
# checks the option
cmp 1 R3
je add

cmp 2 R3
je sub

cmp 3 R3
je mul

cmp 4 R3
je div

.add
    add R4 R5
    add 48 R5
    store R5 600
    store 10 601

    mov 1 R0
    mov result R1
    load resultlen R2
    syscall

    mov 1 R0
    mov 600 R1
    mov 2 R2
    syscall

    hlt

.sub
    sub R5 R4
    add 48 R4
    store R4 600
    store 10 601

    mov 1 R0
    mov result R1
    load resultlen R2
    syscall

    mov 1 R0
    mov 600 R1
    mov 2 R2
    syscall

    hlt

.mul
    mul R4 R5
    add 48 R5
    store R5 600
    store 10 601

    mov 1 R0
    mov result R1
    load resultlen R2
    syscall

    mov 1 R0
    mov 600 R1
    mov 2 R2
    syscall

    hlt

.div
    cmp 0 R5
    je divisionbyzero

    div R4 R5
    add 48 R5
    store R5 600
    store 10 601

    mov 1 R0
    mov result R1
    load resultlen R2
    syscall

    mov 1 R0
    mov 600 R1
    mov 2 R2
    syscall

    hlt

.divisionbyzero
    mov 1 R0
    mov divisionerror R1
    load divisionerrorlen R2
    syscall

    hlt

.invalidnum 
    mov 1 R0
    mov invalidnumber R1 
    load invalidnumberlen R2
    syscall

    hlt 

