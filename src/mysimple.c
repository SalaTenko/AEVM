#include "mysimple.h"

int sc_memoryInit()
{
    for (int i = 0; i < 100; i++)
        memory[i] = 0;
    sc_regSet(OUT_OF_MEMORY, 0);
    return OK;
}

int sc_memorySet(int address, int value)
{
    if (address >= 0 && address < 100)
    {
        memory[address] = value;
        return OK;
    }
    else
    {
        sc_regSet(OUT_OF_MEMORY, 1);
        return OOM;
    }
}

int sc_memoryGet(int address, int *value)
{
    if (address >= 0 && address < 100)
    {
        *value = memory[address];
        return OK;
    }
    else
    {
        sc_regSet(OUT_OF_MEMORY, 1);
        return OOM;
    }
}

int sc_memorySave(char *filename)
{
    int code = OK;
    FILE *file = fopen(filename, "w");
    if (!file)
        code = OPEN_ERR;
    if (!fwrite(memory, sizeof(int), 100, file))
        code = WRITE_ERR;
    fclose(file);
    return code;
}

int sc_memoryLoad(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return OPEN_ERR;
    sc_memoryInit();
    if (!fread(memory, sizeof(int), 100, file))
        return READ_ERR;
    fclose(file);
    return OK;
}

int sc_regInit()
{
    reg_flag = 0;
    return OK;
}

int sc_regSet(int regist, int value)
{
    if (regist >= 0x01 && regist <= 0x10)
    {
        if (value == 0)
            reg_flag &= ~regist;
        else if (value == 1)
            reg_flag |= regist;
        else
            return WRONG_VALUE;
    }
    else
        return WRONG_REGISTER;
    return OK;
}

int sc_regGet(int regist, int *value)
{
    if (regist >= 0x01 && regist <= 0x10)
        *value = (reg_flag & regist) > 0 ? 1 : 0;
    else
        return WRONG_REGISTER;
    return OK;
}

int sc_commandEncode(int command, int operand, int *value)
{
    if (operand > 0 && operand <= 100)
    {
        int flag = 0;
        int commands[] = {10, 11, 20, 21, 30, 31, 32, 33, 40, 41, 42, 43};
        for (int i = 0; i < 13; i++)
        {
            if (command == commands[i])
            {
                flag++;
                break;
            }
        }
        for (int i = 51; i < 77; i++)
        {
            if (flag || command == i)
            {
                flag++;
                break;
            }
        }
        if (!flag)
        {
            return 0;
        }
    }
    *value = 16383 & ((command << 7) | operand);
    return 1;
}

int sc_commandDecode(int value, int *command, int *operand)
{
    if ((value & 32768) != 0)
    {
        sc_regSet(COMMAND_ERR, 1);
        return 0;
    }
    *operand = value & 127;
    value >>= 7;
    *command = value & 127;
    return 1;
}

int sc_instGet(int *value)
{
    *value = inst_cnt;
    return OK;
}

int sc_instSet(int value)
{
    if (value >= 100 || value < 0)
        return WRONG_VALUE;
    inst_cnt = value;
    return OK;
}

int sc_accumGet(int *value)
{
    *value = accum;
    return OK;
}

int sc_accumSet(int value)
{
    if (value > 127 || value < 0)
        return WRONG_VALUE;
    accum = value;
    return OK;
}