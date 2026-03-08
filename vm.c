#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MEM_SIZE 1024
#define REG_COUNT 10
#define BUFF_SIZE 256
#define MAX_LABELS 100
#define MAX_VALUES 100
#define VPC 512

typedef struct value_t {
    char value_name[BUFF_SIZE];
    int value_address;
} value_t;

typedef struct label_t {
    char label_name[BUFF_SIZE];
    int label_address;
} label_t;

typedef struct values {
    value_t values[MAX_VALUES];
    int counter;
} values;

typedef struct labels {
    label_t labels[MAX_LABELS];
    int counter;
} labels;

typedef struct cpu_t {
    uint64_t memory[MEM_SIZE];
    uint64_t regs[REG_COUNT];
    uint64_t cpc;
    uint64_t vpc;
    labels labels;
    values values;
    int zflag;
    int sflag;
    int running;
} cpu_t;

/*Instructions I = immediate, R = register*/
#define JE 9
#define JMP 1
#define HLT 2
#define MOVI 3
#define MOVR 4
#define ADDI 5
#define ADDR 6
#define SUBI 7
#define SUBR 8
#define JNE 10
#define JL 11
#define JLE 12
#define JG 13
#define JGE 14
#define CMPI 15
#define CMPR 16
#define MULI 17
#define MULR 18
#define DIVI 19
#define DIVR 20
#define INC 24
#define DEC 25
#define STOREI 22
#define STORER 23
#define SYSCALL 21

#define SYS_WRITE 1

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

int get_value_pc(cpu_t *cpu,const char *name) {
    for(int i=0; i<cpu->values.counter; i++) {
        if(strcmp(cpu->values.values[i].value_name,name)==0) {
            return cpu->values.values[i].value_address;
        }
    }

    return -1;
}

int get_label_pc(cpu_t *cpu, const char *name) {
    for(int i=0;i<cpu->labels.counter;i++) {
        if(strcmp(cpu->labels.labels[i].label_name,name)==0) {
            return cpu->labels.labels[i].label_address;
        }
    }

    return -1;
}

void execute_byte_code(cpu_t *cpu) {
    cpu->running = 1;
    while(cpu->running) {
        uint64_t opcode = cpu->memory[cpu->cpc++];
        switch(opcode) {
            case MOVI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg] = value;
                break;
            }
            case MOVR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                cpu->regs[regr] = cpu->regs[regv];
                break;
            }
            case ADDI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg] += value;
                break;
            }
            case ADDR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                cpu->regs[regr] += cpu->regs[regv];
                break;
            }
            case SUBI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg] -= value;
                break;
            }
            case SUBR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                cpu->regs[regr] -= cpu->regs[regv];
                break;
            }
            case MULI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg] *= value;
                break;
            }
            case MULR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                cpu->regs[regr] *= cpu->regs[regv];
                break;
            }
            case DIVI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg] /= value;
                break;
            }
            case DIVR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                cpu->regs[regr] /= cpu->regs[regv];
                break;
            }
            case INC: {
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg]++;
                break;
            }
            case DEC: {
                uint64_t reg = cpu->memory[cpu->cpc++];
                cpu->regs[reg]--;
                break;
            }
            case STOREI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t address = cpu->memory[cpu->cpc++];
                cpu->memory[address] = value;
                break;
            }
            case STORER: {
                uint64_t reg = cpu->memory[cpu->cpc++];
                uint64_t address = cpu->memory[cpu->cpc++];
                cpu->memory[address] = cpu->regs[reg];
                break;
            }
            case CMPI: {
                uint64_t value = cpu->memory[cpu->cpc++];
                uint64_t reg = cpu->memory[cpu->cpc++];
                int result = cpu->regs[reg] - value;
                cpu->zflag = (result == 0);
                cpu->sflag = (result < 0);
                break;
            }
            case CMPR: {
                uint64_t regv = cpu->memory[cpu->cpc++];
                uint64_t regr = cpu->memory[cpu->cpc++];
                int result = cpu->regs[regr] - cpu->regs[regv];
                cpu->zflag = (result == 0);
                cpu->sflag = (result < 0);
                break;
            }
            case JMP: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                cpu->cpc = addr;
                break;
            }
            case JE: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->zflag == 1) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case JLE: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->zflag == 1 || cpu->sflag == 1) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case JL: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->sflag == 1) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case JG: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->zflag == 0 && cpu->sflag == 0) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case JGE: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->sflag == 0) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case JNE: {
                uint64_t addr = cpu->memory[cpu->cpc++];
                if(cpu->zflag == 0) {
                    cpu->cpc = addr;
                    break;
                }
                break;
            }
            case HLT: {
                cpu->running = 0;
                break;
            }
            case SYSCALL: {
                switch(cpu->regs[0]) {
                    case SYS_WRITE: {
                        int addr = cpu->regs[1];
                        int len = cpu->regs[2];

                        for(int i=0; i<len; i++) {
                            printf("%c",(char)cpu->memory[addr + i]);
                        }

                        break;
                    }

                    default:
                        printf("Invalid syscall %ld\n",cpu->regs[0]);
                        cpu->running = 0;
                        break;
                }
                break;
            }
            default:
                printf("Invalid opcode %lu at PC=%lu\n", opcode, cpu->cpc-1);
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
    int temp_cpc = 0;
    int cpc = 0;

    // --- FIRST PASS: collect labels ---
    while(fgets(line,sizeof(line),file)) {
        char tokens[10][256];
        memset(tokens,0,sizeof(tokens));
        int counter = 0;

        int pos = 0;
        while(line[pos] != '\n' && line[pos] != '\0') {
            while(isspace(line[pos])) pos++;

            if(line[pos] == '\0') break;

            char buffer[256];
            int buffer_counter = 0;

            if(line[pos] == '.') {
                buffer[buffer_counter++] = line[pos++];

                while(isalnum(line[pos])) {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);

                continue;
            }

            if(isalpha(line[pos])) {
                while(isalnum(line[pos]) || line[pos] == '_') {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);
                continue;
            }

            if(isdigit(line[pos])) {

                while(isdigit(line[pos])) {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);
                continue;
            }

            if(line[pos] == '"') {
                pos++;
                while(line[pos] != '"' && line[pos] != '\0') {
                    if(line[pos] == '\\' && line[pos+1] == 'n') {
                        buffer[buffer_counter++] = '\n';
                        pos+=2;
                        continue;
                    }
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                if(line[pos] == '"') pos++;
                    strcpy(tokens[counter++], buffer);
                    continue;
            }

            pos++;
        }

        if(counter == 0) continue;

        if(tokens[0][0]=='.') {
            char label_name[BUFF_SIZE];
            strncpy(label_name, tokens[0]+1, BUFF_SIZE);

            label_name[strcspn(label_name,"\n")] = '\0';
            strcpy(cpu->labels.labels[cpu->labels.counter].label_name, label_name);

            cpu->labels.labels[cpu->labels.counter].label_address = temp_cpc;
            cpu->labels.counter++;

            continue;
        }

        if(strcmp(tokens[0],"string")==0) {
            char name[256];
            strcpy(name,tokens[1]);
            int address = cpu->vpc;

            for(int i=0; i<strlen(tokens[2]); i++) {
                cpu->memory[cpu->vpc] = (int)(tokens[2][i]);
                cpu->vpc++;
            }

            strcpy(cpu->values.values[cpu->values.counter].value_name,name);
            cpu->values.values[cpu->values.counter].value_address = address;
            cpu->values.counter++;

            continue;
        }

        // increment temp_cpc according to instruction size
        tokens[counter-1][strcspn(tokens[counter-1],"\n")] = '\0';

        if(!strcmp(tokens[0],"mov") 
                || !strcmp(tokens[0],"add") 
                || !strcmp(tokens[0],"sub") 
                || !strcmp(tokens[0],"div")
                || !strcmp(tokens[0],"mul")
                || !strcmp(tokens[0],"store")
                || !strcmp(tokens[0],"cmp")) temp_cpc+=3; 
        else if(!strcmp(tokens[0],"jmp") || 
                !strcmp(tokens[0],"je") ||
                !strcmp(tokens[0],"jl") ||
                !strcmp(tokens[0],"jle") ||
                !strcmp(tokens[0],"jg") ||
                !strcmp(tokens[0],"jge") ||
                !strcmp(tokens[0],"jne") ||
                !strcmp(tokens[0],"inc") ||
                !strcmp(tokens[0],"dec")) temp_cpc+=2;
        else if(strcmp(tokens[0],"hlt")==0 || strcmp(tokens[0],"syscall")==0) temp_cpc+=1;
        else { 
            printf("Invalid opcode '%s'\n",tokens[0]);
            fclose(file);
            return; 
        }
    }

    rewind(file);

    // --- SECOND PASS: write memory ---
    while(fgets(line,sizeof(line),file)) {
        char tokens[10][256];
        memset(tokens,0,sizeof(tokens));
        int counter = 0;

        int pos = 0;
        while(line[pos] != '\n' && line[pos] != '\0') {
            while(isspace(line[pos])) pos++;

            if(line[pos] == '\0') break;

            char buffer[256];
            int buffer_counter = 0;

            if(line[pos] == '.') {
                buffer[buffer_counter++] = line[pos++];

                while(isalnum(line[pos])) {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);
                continue;
            }

            if(isalpha(line[pos])) {
                while(isalnum(line[pos]) || line[pos] == '_') {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);
                continue;
            }

            if(isdigit(line[pos])) {
                while(isdigit(line[pos])) {
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';
                strcpy(tokens[counter++], buffer);
                continue;
            }

            if(line[pos] == '"') {
                pos++;

                while(line[pos] != '"' && line[pos] != '\0') {
                    if(line[pos] == '\\' && line[pos+1] == 'n') {
                        buffer[buffer_counter++] = '\n';
                        pos+=2;
                        continue;
                    }
                    buffer[buffer_counter++] = line[pos++];
                }

                buffer[buffer_counter] = '\0';

                if(line[pos] == '"') pos++;

                strcpy(tokens[counter++], buffer);
                continue;
            }

            pos++;
        }

        if(counter == 0) continue;

        if(tokens[0][0] == '.') continue; 
        if(strcmp(tokens[0],"string")==0) continue;

        if(strcmp(tokens[0],"syscall")==0) {
            cpu->memory[cpc++] = SYSCALL;
            continue;
        }

        if(strcmp(tokens[0],"inc")==0) {
            int reg = register_identity(tokens[1]);
            if(reg == -1) {
                printf("Invalid register '%s'\n",tokens[1]);
                return;
            }

            cpu->memory[cpc++] = INC;
            cpu->memory[cpc++] = reg;

            continue;
        }

        if(strcmp(tokens[0],"dec")==0) {
            int reg = register_identity(tokens[1]);
            if(reg == -1) {
                printf("Invalid register '%s'\n",tokens[1]);
                return;
            }

            cpu->memory[cpc++] = DEC;
            cpu->memory[cpc++] = reg;

            continue;
        }

        if(strcmp(tokens[0],"je")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JE;
            cpu->memory[cpc++] = addr;

            continue;
        }


        if(strcmp(tokens[0],"jl")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JL;
            cpu->memory[cpc++] = addr;

            continue;
        }


        if(strcmp(tokens[0],"jle")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JLE;
            cpu->memory[cpc++] = addr;

            continue;
        }


        if(strcmp(tokens[0],"jg")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JG;
            cpu->memory[cpc++] = addr;

            continue;
        }


        if(strcmp(tokens[0],"jge")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JGE;
            cpu->memory[cpc++] = addr;

            continue;
        }

        if(strcmp(tokens[0],"jne")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) {
                printf("Label '%s' not found\n",tokens[1]);
                fclose(file);
                return;
            }

            cpu->memory[cpc++] = JNE;
            cpu->memory[cpc++] = addr;

            continue;
        }

        if(strcmp(tokens[0],"jmp")==0) {
            int addr = get_label_pc(cpu,tokens[1]);
            if(addr == -1) { 
                printf("Label '%s' not found\n",tokens[1]); 
                fclose(file);
                return; 
            }

            cpu->memory[cpc++] = JMP;
            cpu->memory[cpc++] = addr;

            continue;
        }


        if(strcmp(tokens[0],"cmp")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s' or '%s'\n",tokens[1],tokens[2]);
                    fclose(file); 
                    return; 
                }


                cpu->memory[cpc++] = CMPR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                int value = atoi(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]); 
                    fclose(file);
                    return; 
                }

                cpu->memory[cpc++] = CMPI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"mov")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s' or '%s'\n",tokens[1],tokens[2]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = MOVR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                char *endPtr;
                int value = strtol(tokens[1],&endPtr,10);
                if(*endPtr != '\0') {
                    int address = get_value_pc(cpu,tokens[1]);
                    if(address == -1) {
                        printf("Error: value '%s' does not exists\n",tokens[1]);
                        return;
                    }

                    int regr = register_identity(tokens[2]);
                    if(regr == -1) { 
                        printf("Invalid register '%s'\n",tokens[2]); 
                        fclose(file);
                        return; 
                    }

                    cpu->memory[cpc++] = MOVI;
                    cpu->memory[cpc++] = address;
                    cpu->memory[cpc++] = regr;

                    continue;
                }

                int regr = register_identity(tokens[2]);
                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]); 
                    fclose(file);
                    return; 
                }

                cpu->memory[cpc++] = MOVI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"store")==0) {
            if(tokens[1][0]=='R') {
                int reg = register_identity(tokens[1]);

                if(reg == -1) {
                    printf("Invalid register '%s'\n",tokens[1]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = STORER;
                cpu->memory[cpc++] = reg;
                cpu->memory[cpc++] = atoi(tokens[2]);
            } else {
                int value = atoi(tokens[1]);
                int address = atoi(tokens[2]);

                cpu->memory[cpc++] = STOREI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = address;
            }

            continue;
        }

        if(strcmp(tokens[0],"add")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s' or '%s'\n",tokens[1],tokens[2]); 
                    fclose(file); 
                    return; \
                }

                cpu->memory[cpc++] = ADDR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                int value = atoi(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = ADDI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"sub")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s' or '%s'\n",tokens[1],tokens[2]);
                    fclose(file);
                    return; 
                }

                cpu->memory[cpc++] = SUBR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                int value = atoi(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = SUBI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"mul")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s' or '%s'\n",tokens[1],tokens[2]);
                    fclose(file); 
                    return; \
                }

                cpu->memory[cpc++] = MULR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                int value = atoi(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = MULI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"div")==0) {
            if(tokens[1][0]=='R') {
                int regv = register_identity(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regv == -1 || regr == -1) {
                    printf("Invalid register '%s or '%s\n",tokens[1],tokens[2]); 
                    fclose(file); 
                    return; \
                }

                cpu->memory[cpc++] = DIVR;
                cpu->memory[cpc++] = regv;
                cpu->memory[cpc++] = regr;
            } else {
                int value = atoi(tokens[1]);
                int regr = register_identity(tokens[2]);

                if(regr == -1) { 
                    printf("Invalid register '%s'\n",tokens[2]);
                    fclose(file); 
                    return; 
                }

                cpu->memory[cpc++] = DIVI;
                cpu->memory[cpc++] = value;
                cpu->memory[cpc++] = regr;
            }

            continue;
        }

        if(strcmp(tokens[0],"hlt")==0) { 
            cpu->memory[cpc++] = HLT;
            continue; 
        }

        printf("Invalid opcode '%s' at PC=%ld\n",tokens[0],cpu->cpc-1);
        fclose(file);

        return;
    }

    fclose(file);
}

int main(void) {
    cpu_t cpu = {-1};
    cpu.vpc = VPC;

    assembler(&cpu,"test.asm");
    execute_byte_code(&cpu);
    
    /*we check if the program skiped the label1 and we check if the value on register1 is 10 and not 3*/
    printf("%ld\n",cpu.regs[1]);
    return 0;
}
