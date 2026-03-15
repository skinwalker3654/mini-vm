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

## sys_calls
```bash
sys_write 1
sys_read 2
```

## example of program
```bash
string msg1 "hello\n"
string msg2 "message from loop\n"

number len1 size msg1
number len2 size msg2
number len3 3

unitialized buffer 3

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
    load len1 R2
    syscall

    store 104 543
    store 105 544
    store 10 545

    mov 1 R0
    mov 543 R1
    load len3 R2
    syscall

    mov 0 R3

.startloop
    cmp 10 R3
    jge endloop

    mov 1 R0
    mov msg2 R1
    load len2 R2
    syscall

    inc R3
    jmp startloop

.endloop
    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1

    load 544 R4
    load len1 R5
    load len2 R6
    store 2 len1

    mov 2 R0
    mov buffer R1
    mov 3 R2
    syscall

    store 10 542

    mov 1 R0
    mov buffer R1
    mov 4 R2
    syscall

    hlt

# now the register R1 is 10 not 3, because we skiped the label1, the R4 is 105 because we loaded the value from the address 536
# and it also prints to the screen hello and hi(because we store the characters manually) 
# beacause of the sys_write AND it loops through 10 and prints "message for loop" and now the registers R4 R5 and R6 are equal to 105 6 18
```

## compile
```bash
# firstly open the file vm.c with any text editor you want, example
vim vm.c

# then go to the bottom of the file and remove this lines of code
    /*we check if the program skiped the label1 and we check if the value on register1 is 10 and not 3 and if register 4 has loaded the 105 succesfully*/
    printf("%ld\n",cpu.regs[1]);
    printf("%ld\n",cpu.regs[4]);

    /*we check if the registers 5 and 6 have the values of 1 and 2 because we loaded them from the memory*/
    printf("%ld\n",cpu.regs[5]);
    printf("%ld\n",cpu.regs[6]); 

    /*after the store we print the value on the address 536*/
    printf("%ld\n",cpu.memory[536]);

# and lastly run
gcc vm.c -o vm

# now to run it you simply do
./vm assembly_file # you put the name of the file with the assembly code
```

## install to your system
```bash
# to install it on you system you must first compile the project by using the guidline you show right above and then you do
sudo mv vm /usr/bin/

# this requires sudo administration so you have to use your sudo password to move the executable in the bins folder
# after that you can completely delete the folder mini-vm and execute assembly programs in the custom cpu with

vm file.asm

# make sure you dont have an alias with the name vm on you bashrc otherwise you either change the alias OR change the name of the executable and use that instead
# example

new_name file.asm

# if you want to remove it from your system you should do
sudo rm /usr/bin/vm #OR 
sudo rm /usr/bin/new_name

# that operation also requires the sudo password
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
