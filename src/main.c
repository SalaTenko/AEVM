#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "myConsole.h"
#include "myTerm.h"
#include "myReadkey.h"
#include "mysimple.h"

struct itimerval nval, oval;

void sighandler(int signum)
{
    if (signum == SIGUSR1)
    {
        mt_clrscr();
        exit(0);
    }
    if (signum == SIGALRM)
    {
        int freq_flg = 0,
            comm_flg = 0,
            inst_curr = 0;
        sc_regGet(FREQ_ERR, &freq_flg);
        sc_regGet(COMMAND_ERR, &comm_flg);
        sc_instGet(&inst_curr);

        if (comm_flg)
            sc_regSet(FREQ_ERR, 1);

        if (!freq_flg)
        {
            step();
            interface();
        }

        if (inst_curr == 99)
            sc_regSet(FREQ_ERR, 1);
    }
}

int main()
{
    //signal(SIGALRM, sighandler);
    signal(SIGUSR1, sighandler);
    signal(SIGALRM, Execution);

    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 80000;
    nval.it_value.tv_sec = 0;
    nval.it_value.tv_usec = 50000;
    setitimer(ITIMER_REAL, &nval, &oval);

    init();

    simple_computer();

    write(1, "\e[0m", 4);
    rk_mytermrestore();
    mt_clrscr();
    return 0;
}