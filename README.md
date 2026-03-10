Virtual CPU + Assembler writen in (C)

this is a simple cpu writen in C that has
0-9 registers memory with size of 1024 64-bit-integetrs
and pc.

# instructions
```bash
mov
add
sub
mul
div
inc
dec
store
load
jmp
je
jl
jle
jg
jge
jne
cmp
hlt
```
## example of program
```bash
# we skip the label1 because the R2 is equal to 10 so the comparation is true
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
# now the register R1 is 10 not 3, because we skiped the label1, the R4 is 105 because we loaded the value from the address 536
# and it also prints to the screen hello and hi(because we store the characters manually) 
# beacause of the sys_write AND it loops through 10 and prints "message for loop"
# also the comments dosent work in the real assembler, i have only puted them in the README.md
```

## compile
```bash
# firstly open the file vm.c with any text editor you want, example
vim vm.c

# go to the end of the file and remove the lines
# /*we check if the program skiped the label1 and we check if the value on register1 is 10 and not 3 and if register 4 has loaded the 105 succesfully*/
printf("%ld\n",cpu.regs[1]);
printf("%ld\n",cpu.regs[4]);

# and lastly run
gcc vm.c -o vm

# now to run it you simply do
./vm assembly_file # you put the name of the file with the assembly code
```

## run test
```bash
make test
```

## delete test
```bash
make clean
```

### notes:
1. it uses at&t like syntax without the suffixes
2. it has build in assembler and byte_code executer

im gonna add more instructions + the marion interpreter from my other projects BUT it will now be a compiler for the
custom assembly language i have build on this project so the marion language can be cross-platform like the java VM but lighter. In the future.
# mini-vm
