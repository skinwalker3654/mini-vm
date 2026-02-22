#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE 1024
#define REG_COUNT 10

typedef struct cpu_t {
    uint64_t memory[MEM_SIZE];
    uint64_t regs[REG_COUNT];
    uint64_t pc;
} cpu_t;

/*Instructions I = imidiate, R = register*/
#define MOVI 1
#define MOVR 2
#define ADDI 3
#define ADDR 4
#define SUBI 5
#define SUBR 6
#define HLT 7
#define SYSCALL 8

void execute_byte_code(cpu_t *cpu) {
    /*regr = register to put value, regv = register to use value: r = register, v = value*/
    int running = 1;
    while(running) {
        uint64_t opcode = cpu->memory[cpu->pc++];
        switch(opcode) {
            case MOVI: {
                uint64_t value = cpu->memory[cpu->pc++];
                uint64_t reg = cpu->memory[cpu->pc++];
                cpu->regs[reg] = value;
                break;
            }
            case MOVR: {
                uint64_t regv = cpu->memory[cpu->pc++];
                uint64_t regr = cpu->memory[cpu->pc++];
                cpu->regs[regr] = cpu->regs[regv];
                break;
            }
            case ADDI: {
                uint64_t value = cpu->memory[cpu->pc++];
                uint64_t reg = cpu->memory[cpu->pc++];
                cpu->regs[reg] += value;
                break;
            } 
            case ADDR: {
                uint64_t regv = cpu->memory[cpu->pc++];
                int regr = cpu->memory[cpu->pc++];
                cpu->regs[regr] += cpu->regs[regv];
                break;
            }
            case SUBI: {
                uint64_t value = cpu->memory[cpu->pc++];
                uint64_t reg = cpu->memory[cpu->pc++];
                cpu->regs[reg] -= value;
                break;
            }
            case SUBR: {
                uint64_t regv = cpu->memory[cpu->pc++];
                uint64_t regr = cpu->memory[cpu->pc++];
                cpu->regs[regr] -= cpu->regs[regv];
                break;
            }
            case HLT: {
                running = 0;
                break;
            }
        }
    }
}

int register_identity(char *string) {
    if(strlen(string) != 2) return -1;
    if(*string == 'R') {
        string++;
        char *endPtr;
        int number = strtol(string,&endPtr,10);
        if(*endPtr != '\0') {
            printf("Error: Invalid register %s\n",string);
            return -1;
        }

        if(number < 0 || number > 9) {
            printf("Error: Register R%d does not exists\n",number);
            return -1;
        }

        return number;
    }

    return -1;
}


void assembler(cpu_t *cpu,char *file_name) {
    FILE *file = fopen(file_name,"r");
    if(!file) {
        printf("Cant open the file '%s'\n",file_name);
        return;
    }

    char line[256];
    int pc = 0;
    while(fgets(line,sizeof(line),file)) {
        char *tokens[10];
        int counter = 0;

        char *token = strtok(line," ");
        while(token) {
            tokens[counter++] = token;
            token = strtok(NULL," ");
        }

        if(strcmp(tokens[0],"mov")==0) {
            if(tokens[1][0] == 'R') {
                int regv = register_identity(tokens[1]);
                if(regv == -1) {
                    printf("Invalid register '%s'\n",tokens[1]);
                    return;
                }

                tokens[2][strcspn(tokens[2],"\n")] = '\0';

                int regr = register_identity(tokens[2]);
                if(regr == -1) {
                    printf("Invalid register '%s'\n",tokens[2]);
                    return;
                }

                cpu->memory[pc++] = MOVR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;

                continue;
            }

            char *endPtr;
            int number = strtol(tokens[1],&endPtr,10);
            if(*endPtr != '\0') {
                printf("Error: Invalid number '%s'\n",tokens[1]);
                return;
            }

            tokens[2][strcspn(tokens[2],"\n")] = '\0';

            int reg = register_identity(tokens[2]);
            if(reg == -1) {
                printf("Invalid register '%s'\n",tokens[2]);
                return;
            }

            cpu->memory[pc++] = MOVI;
            cpu->memory[pc++] = number;
            cpu->memory[pc++] = reg;

            continue;
        }
        
        if(strcmp(tokens[0],"add")==0) {
            if(tokens[1][0] == 'R') {
                int regv = register_identity(tokens[1]);
                if(regv == -1) {
                    printf("Invalid register '%s'\n",tokens[1]);
                    return;
                }

                tokens[2][strcspn(tokens[2],"\n")] = '\0';

                int regr = register_identity(tokens[2]);
                if(regr == -1) {
                    printf("Invalid register '%s'\n",tokens[2]);
                    return;
                }

                cpu->memory[pc++] = ADDR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;

                continue;
            }

            char *endPtr;
            int number = strtol(tokens[1],&endPtr,10);
            if(*endPtr != '\0') {
                printf("Error: Invalid number '%s'\n",tokens[1]);
                return;
            }

            tokens[2][strcspn(tokens[2],"\n")] = '\0';

            int reg = register_identity(tokens[2]);
            if(reg == -1) {
                printf("Invalid register '%s'\n",tokens[2]);
                return;
            }

            cpu->memory[pc++] = ADDI;
            cpu->memory[pc++] = number;
            cpu->memory[pc++] = reg;

            continue;
        }

        if(strcmp(tokens[0],"sub")==0) {
            if(tokens[1][0] == 'R') {
                int regv = register_identity(tokens[1]);
                if(regv == -1) {
                    printf("Invalid register '%s'\n",tokens[1]);
                    return;
                }

                tokens[2][strcspn(tokens[2],"\n")] = '\0';

                int regr = register_identity(tokens[2]);
                if(regr == -1) {
                    printf("Invalid register '%s'\n",tokens[2]);
                    return;
                }

                cpu->memory[pc++] = SUBR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;

                continue;
            }

            char *endPtr;
            int number = strtol(tokens[1],&endPtr,10);
            if(*endPtr != '\0') {
                printf("Error: Invalid number '%s'\n",tokens[1]);
                return;
            }

            tokens[2][strcspn(tokens[2],"\n")] = '\0';

            int reg = register_identity(tokens[2]);
            if(reg == -1) {
                printf("Invalid register '%s'\n",tokens[2]);
                return;
            }

            cpu->memory[pc++] = SUBI;
            cpu->memory[pc++] = number;
            cpu->memory[pc++] = reg;

            continue;
        }


        tokens[0][strcspn(tokens[0],"\n")] = '\0';

        if(strcmp(tokens[0],"hlt")==0) {
            cpu->memory[pc++] = HLT;
            continue;
        }

        printf("Invlaid opcode '%s'\n",tokens[0]);
        break;
    }
}

int main(void) {
    cpu_t cpu = {0};
    assembler(&cpu,"test.asm");

    execute_byte_code(&cpu);
    printf("%ld\n",cpu.regs[1]); // we check if the register R1 is 7

    return 0;
}
