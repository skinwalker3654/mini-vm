Virtual CPU + Assembler writen in (C)

this is a simple cpu writen in C that has
0-9 registers memory with size of 1024 64-bit-integetrs
and pc.

# instructions
```bash
mov
add
sub
hlt
```
## example of program
```bash
mov 2 R0
mov R0 R1
add 5 R1
hlt
# now the register R1 is 7
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
