#ifndef MYCONSOLE_H
#define MYCONSOLE_H

#include <stdio.h>

int x, y;

void print_mem();
void print_accum();
void print_instcnt();
void print_operation();
void print_flg();
void print_membc();
void print_keys();
void load_mem();
void load_mem();
void save_mem();
void set_accum();
void set_instcnt();
void set_mem();
void step();
void init();
void refresh();
void key_handler();
void interface();
void simple_computer();

void RCCL();
void RCR();
void LOGRC();

int Execution();
int ALU(int command, int operand);
int CU(int command, int operand);

#endif