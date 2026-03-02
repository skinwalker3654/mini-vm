#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE 1024
#define REG_COUNT 10
#define BUFF_SIZE 256
#define MAX_LABELS 100

typedef struct label {
    char name[BUFF_SIZE];
    int pc;
} label;

typedef struct cpu_t {
    uint64_t memory[MEM_SIZE];
    uint64_t regs[REG_COUNT];
    uint64_t pc;
    label labels[MAX_LABELS];
    int lab_counter;
    int running;
} cpu_t;

/*Instructions I = immediate, R = register*/
#define JMP 1
#define HLT 2
#define MOVI 3
#define MOVR 4
#define ADDI 5
#define ADDR 6
#define SUBI 7
#define SUBR 8

int register_identity(char *string) {
    if(strlen(string) != 2) return -1;

    if(*string == 'R') {
        string++;
        char *endPtr;
        int number = strtol(string,&endPtr,10);

        if(*endPtr != '\0' || number < 0 || number > 9)
            return -1;

        return number;
    }

    return -1;
}

int get_label_pc(cpu_t *cpu, const char *name) {
    for(int i=0;i<cpu->lab_counter;i++) {
        if(strcmp(cpu->labels[i].name,name)==0) {
            return cpu->labels[i].pc;
        }
    }

    return -1;
}

void execute_byte_code(cpu_t *cpu) {
    cpu->running = 1;
    while(cpu->running) {
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
                uint64_t regr = cpu->memory[cpu->pc++];
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
            case JMP: {
                uint64_t addr = cpu->memory[cpu->pc++];
                cpu->pc = addr;
                break;
            }
            case HLT:
                cpu->running = 0;
                break;
            default:
                printf("Invalid opcode %lu at PC=%lu\n", opcode, cpu->pc-1);
                cpu->running = 0;
                break;
        }
    }
}

void assembler(cpu_t *cpu, const char *file_name) {
    FILE *file = fopen(file_name,"r");
    if(!file) {
        printf("Cannot open file '%s'\n",file_name); 
        return; 
    }

    char line[BUFF_SIZE];
    int temp_pc = 0;
    int pc = 0;

    // --- FIRST PASS: collect labels ---
    while(fgets(line,sizeof(line),file)) {
        if(line[0]=='\n' || line[0]=='\0') continue;

        char *tokens[10];
        int counter=0;

        char *token = strtok(line," ");
        while(token) {
            tokens[counter++] = token; 
            token = strtok(NULL," "); 
        }

        if(counter==0) continue;

        if(tokens[0][0]=='.') {
            char label_name[BUFF_SIZE];
            strncpy(label_name, tokens[0]+1, BUFF_SIZE);

            label_name[strcspn(label_name,"\n")] = '\0';
            strcpy(cpu->labels[cpu->lab_counter].name, label_name);

            cpu->labels[cpu->lab_counter].pc = temp_pc;
            cpu->lab_counter++;

            continue;
        }

        // increment temp_pc according to instruction size
        tokens[counter-1][strcspn(tokens[counter-1],"\n")] = '\0';

        if(!strcmp(tokens[0],"mov") || !strcmp(tokens[0],"add") || !strcmp(tokens[0],"sub")) temp_pc+=3;
        else if(strcmp(tokens[0],"jmp")==0) temp_pc+=2;
        else if(strcmp(tokens[0],"hlt")==0 || strcmp(tokens[0],"syscall")==0) temp_pc+=1;
        else { 
            printf("Invalid opcode '%s'\n",tokens[0]);
            fclose(file);
            return; 
        }
    }

    rewind(file);

    // --- SECOND PASS: write memory ---
    while(fgets(line,sizeof(line),file)) {
        if(line[0]=='\n' || line[0]=='\0') continue;

        char *tokens[10];
        int counter=0;

        char *token = strtok(line," ");
        while(token) { 
            tokens[counter++] = token; 
            token = strtok(NULL," "); 
        }

        if(counter == 0) continue;
        tokens[counter-1][strcspn(tokens[counter-1],"\n")] = '\0';

        if(tokens[0][0] == '.') continue; 

        if(strcmp(tokens[0],"jmp")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) { 
                printf("Label '%s' not found\n",tokens[1]); 
                fclose(file);
                return; 
            }

            cpu->memory[pc++] = JMP;
            cpu->memory[pc++] = addr;

            continue;
        }

        if(strcmp(tokens[0],"mov")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register\n");
                    fclose(file); 
                    return; 
                }

                cpu->memory[pc++] = MOVR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;
            } else {
                int regr = register_identity(tokens[2]);
                int value = atoi(tokens[1]);

                if(regr == -1) { 
                    printf("Invalid register\n"); 
                    fclose(file);
                    return; 
                }

                cpu->memory[pc++] = MOVI;
                cpu->memory[pc++] = value;
                cpu->memory[pc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"add")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register\n"); 
                    fclose(file); 
                    return; \
                }

                cpu->memory[pc++] = ADDR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;
            } else {
                int regr = register_identity(tokens[2]);
                int value = atoi(tokens[1]);

                if(regr == -1) { 
                    printf("Invalid register\n");
                    fclose(file); 
                    return; 
                }

                cpu->memory[pc++] = ADDI;
                cpu->memory[pc++] = value;
                cpu->memory[pc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"sub")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register\n");
                    fclose(file);
                    return; 
                }

                cpu->memory[pc++] = SUBR;
                cpu->memory[pc++] = regv;
                cpu->memory[pc++] = regr;
            } else {
                int regr = register_identity(tokens[2]);
                int value = atoi(tokens[1]);
                if(regr == -1) { 
                    printf("Invalid register\n");
                    fclose(file); 
                    return; 
                }

                cpu->memory[pc++] = SUBI;
                cpu->memory[pc++] = value;
                cpu->memory[pc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"hlt")==0) { 
            cpu->memory[pc++] = HLT;
            continue; 
        }

        printf("Invalid opcode '%s'\n",tokens[0]);
        fclose(file);

        return;
    }

    fclose(file);
}

int main(void) {
    cpu_t cpu = {0};

    assembler(&cpu,"test.asm");
    execute_byte_code(&cpu);
    
    /* we check if the mov 10 R2 is skiped by the jmp and we also check if 
     * R1 is 7 because we did mov 2 R0, mov R0 R1 and add 5 R1*/
    printf("%ld\n",cpu.regs[2]);
    printf("%ld\n",cpu.regs[1]);

    return 0;
}
