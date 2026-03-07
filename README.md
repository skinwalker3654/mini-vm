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
mov 10 R2
cmp 10 R2
je label2

.label1
    mov 1 R0
    mov R0 R1
    add 2 R1
    hlt

.label2
    mov 2 R0
    mov R0 R1
    add 2 R1
    div 2 R1
    mul 5 R1
    hlt
# now the register R1 is 10 not 3, because we skiped the label1
# also the comments dosent work in the real assembler, i have only puted them in the README.md
```

## compile and run
```bash
gcc vm.c -o vm
./vm
```

### notes:
1. it uses at&t like syntax without the suffixes
2. it has build in assembler and byte_code executer

im gonna add more instructions + syscalls + the maroin interpreter from my other projects BUT it will now be a  compiler for the
custom assembly language i have build on this project so the marion language can be cross-platoform like java VM but light in the future.
# mini-vm
