#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "myConsole.h"
#include "mysimple.h"
#include "myTerm.h"
#include "myReadkey.h"
#include "myBigChars.h"

int *alph = NULL;

void print_mem()
{
    for (int i = 0; i < 100; i++)
    {
        mt_gotoXY(2 + 6 * (i % 10), 2 + (i / 10));
        if (x > 9)
            x = 0;
        if (x < 0)
            x = 9;
        if (y > 9)
            y = 0;
        if (y < 0)
            y = 9;
        if (i % 10 == y && i / 10 == x)
            mt_setbgcolor(red);
        int command, operand, value;
        sc_memoryGet(i, &value);
        sc_commandDecode(value, &command, &operand);
        char tmp1[10];
        if (command < 16 && operand < 16)
            sprintf(tmp1, "+0%x0%x", command, operand);
        else if (command < 16 && operand >= 16)
            sprintf(tmp1, "+0%x%x", command, operand);
        else if (command >= 16 && operand < 16)
            sprintf(tmp1, "+%x0%x", command, operand);
        else
            sprintf(tmp1, "+%x%x", command, operand);
        write(1, tmp1, strlen(tmp1));
        mt_setbgcolor(deflt);
    }
}

void print_accum()
{
    mt_gotoXY(71, 2);
    int acc = 0;
    sc_accumGet(&acc);
    char tmp[4];
    sprintf(tmp, "+%d", acc);
    write(1, tmp, strlen(tmp));
}

void print_instcnt()
{
    mt_gotoXY(71, 5);
    int inst = 0;
    sc_instGet(&inst);
    char tmp[4];
    sprintf(tmp, "+%d", inst);
    write(1, tmp, strlen(tmp));
}

void print_operation()
{
    int tmp = 0;
    sc_memoryGet(y + x * 10, &tmp);
    char tmp1[10];
    int cmd, opr;
    sc_commandDecode(tmp, &cmd, &opr);
    sprintf(tmp1, "+%x:%x", cmd, opr);
    mt_gotoXY(69, 8);
    write(1, tmp1, strlen(tmp1));
}

void print_flg()
{
    char tmp1[] = "POMTE";
    mt_gotoXY(67, 11);
    for (int i = 0; i < 5; i++)
    {
        int value;
        sc_regGet(1 << i, &value);
        if (value)
            mt_setfgcolor(red);
        char tmp2[3];
        sprintf(tmp2, "%c ", tmp1[i]);
        write(1, tmp2, strlen(tmp2));
        mt_setfgcolor(deflt);
    }
}

void print_membc()
{
    int value, command, operand;
    sc_memoryGet(10 * x + y, &value);
    sc_commandDecode(value, &command, &operand);
    int big[] = {alph[16 * 2], alph[16 * 2 + 1]};
    bc_printbigchar(big, 2 + 10 * 0, 14, deflt, deflt);
    int big1[] = {alph[command / 16 * 2], alph[command / 16 * 2 + 1]};
    bc_printbigchar(big1, 2 + 10 * 1, 14, deflt, deflt);
    int big2[] = {alph[command % 16 * 2], alph[command % 16 * 2 + 1]};
    bc_printbigchar(big2, 2 + 10 * 2, 14, deflt, deflt);
    int big3[] = {alph[operand / 16 * 2], alph[operand / 16 * 2 + 1]};
    bc_printbigchar(big3, 2 + 10 * 3, 14, deflt, deflt);
    int big4[] = {alph[operand % 16 * 2], alph[operand % 16 * 2 + 1]};
    bc_printbigchar(big4, 2 + 10 * 4, 14, deflt, deflt);
}

void print_keys()
{
    bc_box(51, 13, 33, 10);
    mt_gotoXY(52, 13);
    const char tmp[] = " Keys: ";
    write(1, tmp, strlen(tmp));
    const char *tmp1[] = {"l - load", "s - save", "r - run", "t - step",
                          "i - reset", "F5 - accumulator", "F6 - instructionCounter"};
    for (int i = 0; i < 7; i++)
    {
        mt_gotoXY(52, 14 + i);
        write(1, tmp1[i], strlen(tmp1[i]));
    }
}

void load_mem()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(24, 7);
    write(1, "Load\n", strlen("Load\n"));
    char tmp[255] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 255);
    if (sc_memoryLoad(tmp))
    {
        bc_box(20, 6, 20, 5);
        mt_gotoXY(23, 7);
        write(1, "Failed to open\n", strlen("Failed to open\n"));
        mt_gotoXY(29, 9);
        mt_setbgcolor(red);
        write(1, "OK", strlen("OK"));
        mt_setbgcolor(deflt);
        mt_gotoXY(30, 9);
        read(1, tmp, 1);
    }
    refresh();
}

void save_mem()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);
    write(1, "Save to\n", strlen("Save to\n"));
    char tmp[255] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 255);
    if (sc_memorySave(tmp))
    {
        bc_box(20, 6, 20, 5);
        mt_gotoXY(23, 7);
        write(1, "Failed to save\n", strlen("Failed to save\n"));
        mt_gotoXY(29, 9);
        mt_setbgcolor(red);
        write(1, "OK", strlen("OK"));
        mt_setbgcolor(deflt);
        mt_gotoXY(30, 9);
        read(1, tmp, 1);
    }
    refresh();
}

void set_accum()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);
    write(1, "Set accum to\n", strlen("Set accum to\n"));
    char tmp[5] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 5);
    int new_acc = atoi(tmp);
    sc_accumSet(new_acc);
    refresh();
}

void set_instcnt()
{
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    write(1, "Set instructionCounter to\n", strlen("Set instructionCounter to\n"));
    char tmp[5] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 5);
    tmp[strlen(tmp) - 1] = '\0';
    int new_inst = atoi(tmp);
    sc_instSet(new_inst);
    refresh();
}

void set_mem()
{
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    write(1, "Set memory(dec) to\n", strlen("Set memory(dec) to\n"));
    char tmp[11] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 10);
    tmp[strlen(tmp) - 1] = '\0';
    int tmp1 = atoi(tmp);

    sc_memorySet(10 * x + y, tmp1);

    refresh();
}

void step()
{
    int inst_curr = 0;
    sc_instGet(&inst_curr);
    y = inst_curr % 10;
    x = inst_curr / 10;
    sc_instSet(inst_curr + 1);
}

void init()
{
    int fd = open("font", O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int cnt = 0;
    alph = (int *)malloc(sizeof(int) * 2 * 17);
    if (bc_bigcharread(fd, alph, 17, &cnt) == -1 && cnt < 17)
    {
        fprintf(stderr, "Failed to load font\n");
        exit(EXIT_FAILURE);
    }

    sc_regSet(OVERFLOW, 0);
    sc_regSet(ZERO_ERR, 0);
    sc_regSet(OUT_OF_MEMORY, 0);
    sc_regSet(FREQ_ERR, 1);
    sc_regSet(COMMAND_ERR, 0);
    sc_memoryInit();
    sc_instSet(0);
    sc_accumSet(0);

    mt_clrscr();
}

void refresh()
{
    bc_box(1, 1, 61, 12);
    mt_gotoXY(30, 0);
    char tmp[] = " Memory ";
    write(1, tmp, sizeof(tmp));
    bc_box(62, 1, 22, 3);
    mt_gotoXY(66, 1);
    char tmp1[] = " accumulator ";
    write(1, tmp1, sizeof(tmp1));
    bc_box(62, 4, 22, 3);
    mt_gotoXY(63, 4);
    char tmp2[] = " instructionCounter ";
    write(1, tmp2, sizeof(tmp2));
    bc_box(62, 7, 22, 3);
    mt_gotoXY(68, 7);
    char tmp3[] = " Operation ";
    write(1, tmp3, sizeof(tmp3));
    bc_box(62, 10, 22, 3);
    mt_gotoXY(69, 10);
    char tmp4[] = " Flags ";
    write(1, tmp4, strlen(tmp4));
    bc_box(1, 13, 50, 10);
    print_keys();
    interface();
}

void key_handler(int *exit)
{
    enum keys key = none;
    rk_readkey(&key);
    if (key == r)
    {
        int tmp;
        sc_regGet(FREQ_ERR, &tmp);
        tmp = (tmp) ? 0 : 1;
        sc_regSet(FREQ_ERR, tmp);
    }

    int freq_flg;
    sc_regGet(FREQ_ERR, &freq_flg);
    if (!freq_flg)
        return;

    if (key == q)
        *exit = 1;
    if (key == l)
        load_mem();
    if (key == s)
        save_mem();
    if (key == up)
        x--;
    if (key == down)
        x++;
    if (key == left)
        y--;
    if (key == right)
        y++;
    if (key == i)
    {
        init();
        x = 0, y = 0;
        refresh();
    }
    if (key == f5)
        set_accum();
    if (key == f6)
        set_instcnt();
    if (key == t)
        step();
    if (key == enter)
    {
        set_mem();
    }
    if (key == z)
        RCCL();
}

void interface()
{
    print_mem();
    print_accum();
    print_instcnt();
    print_operation();
    print_flg();
    print_membc();
}

void simple_computer()
{
    int exit = 0;
    x = y = 0;
    refresh();
    while (!exit)
    {
        interface();
        key_handler(&exit);
        //mt_clrscr();
        refresh();
    }
}

void RCCL()
{
    int accum;
    sc_accumGet(&accum);
    int memory_content = memory[10 * x + y];
    for (int i = 0; i < accum; i++)
    {
        memory_content <<= 1;
        int tmp = (memory_content & 128) >> 7;
        memory_content &= 127;
        memory_content |= tmp;
    }
    sc_accumSet(memory_content);
}

void RCR()
{
    int memory_content = memory[10 * x + y];
    int tmp = (memory_content & 1);
    memory_content >>= 1;
    tmp <<= 6;
    memory_content |= tmp;
    sc_accumSet(memory_content);
}

void LOGRC()
{
    int accum;
    sc_accumGet(&accum);
    int memory_content = memory[10 * x + y];
    for (int i = 0; i < accum; i++)
        memory_content >>= 1;
    sc_accumSet(memory_content);
}

int Execution()
{
    int flag, command, operand;

    sc_regGet(FREQ_ERR, &flag);

    if (!flag)
    {
        mt_gotoXY(21, 9);
        if (inst_cnt > 99 || inst_cnt < 0)
        {
            sc_regSet(OUT_OF_MEMORY, 1);
            sc_regSet(FREQ_ERR, 1);
            return -1;
        }
        else if (sc_commandDecode(memory[inst_cnt], &command, &operand) == 0)
        {
            sc_regSet(COMMAND_ERR, 0);
            x = inst_cnt % 10;
            y = inst_cnt / 10;
            if (command >= 30 && command <= 33)
            {
                if (ALU(command, operand) == 0)
                {
                    inst_cnt++;
                    y += 1;
                    if (y == 10)
                    {
                        y = 0;
                        x++;
                    }
                }
            }
            else
            {
                if (CU(command, operand) == 0)
                {
                    inst_cnt++;
                    y += 1;
                    if (y == 10)
                    {
                        y = 0;
                        x++;
                    }
                }
            }
        }
        else
        {
            sc_regSet(COMMAND_ERR, 1);
            sc_regSet(FREQ_ERR, 1);
            return -1;
        }
    }
    return 0;
}

int ALU(int command, int operand)
{
    switch (command)
    {
    case 30:
        if (accum + memory[operand - 1] > 99999)
        {
            sc_regSet(OVERFLOW, 1);
            sc_regSet(FREQ_ERR, 1);
            return -1;
        }
        else
        {
            accum = memory[operand - 1] + accum;
        }
        break;
    case 31:
        accum = accum - memory[operand - 1];
        break;
    case 32:
        if (memory[operand - 1] == 0)
        {
            sc_regSet(ZERO_ERR, 1);
            return -1;
        }
        accum = accum / memory[operand - 1];
        break;
    case 33:
        if (accum * memory[operand - 1] > 99999)
        {
            sc_regSet(OVERFLOW, 1);
            sc_regSet(FREQ_ERR, 1);
            return -1;
        }
        else
            accum = accum * memory[operand - 1];
        break;
    }
    return 0;
}

int CU(int command, int operand)
{

    if (operand > 99 || operand < 0)
    {
        sc_regSet(OUT_OF_MEMORY, 1);
        return -1;
    }

    switch (command)
    {
    case 10:
        refresh();
        sc_regSet(FREQ_ERR, 1);
        rk_mytermrestore();
        rk_mytermregime(1, 0, 0, 1, 0);

        mt_gotoXY(21, 9);
        printf("Memory[%d]>", operand);
        scanf("%d", &memory[operand - 1]);
        mt_gotoXY(21, 9);
        printf("                     ");

        rk_mytermsave();
        rk_mytermregime(0, 1, 0, 0, 0);
        sc_regSet(FREQ_ERR, 0);
        break;
    case 11:
        mt_gotoXY(21, 9);
        printf("%d", memory[operand - 1]);
        break;
    case 20:
        accum = memory[operand - 1];
        break;
    case 21:
        memory[operand - 1] = accum;
        break;

    case 40:
        inst_cnt = operand - 1;
        y = inst_cnt % 10;
        x = inst_cnt / 10;
        break;
    case 41:
        if (accum < 0)
        {
            inst_cnt = operand - 1;
            y = inst_cnt % 10;
            x = inst_cnt / 10;
        }
        break;
    case 42:
        if (accum == 0)
        {
            inst_cnt = operand - 1;
            y = inst_cnt % 10;
            x = inst_cnt / 10;
        }
        break;
    case 43:
        sc_regSet(FREQ_ERR, 1);
        return 1;
        break;
    }
    return 0;
}