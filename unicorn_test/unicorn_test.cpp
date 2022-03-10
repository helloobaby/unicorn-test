#include"unicorn_include/unicorn/unicorn.h"

#include<iostream>
#include<algorithm>
#include<string>
#include<cassert>


using namespace std;

void print_regs(uc_engine* uc)
{
    uint64_t temp_reg;
    uc_reg_read(uc, UC_X86_REG_RAX, &temp_reg);
    printf("RAX -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RBX, &temp_reg);
    printf("RBX -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RCX, &temp_reg);
    printf("RCX -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RDX, &temp_reg);
    printf("RDX -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RBP, &temp_reg);
    printf("RBP -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RSP, &temp_reg);
    printf("RSP -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RSI, &temp_reg);
    printf("RSI -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_RDI, &temp_reg);
    printf("RDI -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R8, &temp_reg);
    printf("R8 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R9, &temp_reg);
    printf("R9 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R10, &temp_reg);
    printf("R10 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R11, &temp_reg);
    printf("R11 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R12, &temp_reg);
    printf("R12 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R13, &temp_reg);
    printf("R13 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R14, &temp_reg);
    printf("R14 -> %llx\n", temp_reg);
    uc_reg_read(uc, UC_X86_REG_R15, &temp_reg);
    printf("R15 -> %llx\n\n", temp_reg);
}

const int code_start = 0;

int main()
{
    LPSTR argv = GetCommandLineA();
    char uc_insn_buf[1000]{}; //max instuction byte count support
    int insn_buf_pos = 0;

    uc_engine* uc;
    string command_line(argv);

    size_t pos;
    if ((pos = command_line.find("-x64")) == string::npos)
    {
        cout << "example : -x64 0F A2\n"; //cpuid
        return -1;
    }
    
    pos += 5;

    string insn(command_line.substr(pos));
    
    while ((pos = insn.find(" ")) != string::npos)
    {
        uc_insn_buf[insn_buf_pos++] = stoi(insn.substr(0, pos),0 ,16);
        insn.erase(0, pos+1);
    }
    uc_insn_buf[insn_buf_pos++] = stoi(insn.substr(0, pos), 0, 16);

    uc_err error = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
    if (error != UC_ERR_OK)
        return -1;

    uint64_t stack = 0x5000;
    assert(uc_reg_write(uc, UC_X86_REG_RSP, &stack) == UC_ERR_OK);
    //alloc stack (0 - 0x5fff)
    assert(uc_mem_map(uc, (uint64_t)0, 0x10000, UC_PROT_ALL) == UC_ERR_OK);

    assert(uc_mem_write(uc, code_start, uc_insn_buf, insn_buf_pos) == UC_ERR_OK);

    if (uc_emu_start(uc, (uint64_t)code_start, (uint64_t)code_start + insn_buf_pos, 0, 0) != UC_ERR_OK)
    {
        cout << "Emulate failed! \n";
        return -1;
    }
    cout << "after emu\n";
    print_regs(uc);
    


    return 0;
}


