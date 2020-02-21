#define _GNU_SOURCE     /* for gregs enum */

# include <stdint.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include "rdebug.h"

/* MODIFY THIS! */
#define THIS_FILE_ID    1

USE_RDEBUG(THIS_FILE_ID);

struct sigaction sa;

#define SETSIG(sa, sig, func) \
    {    memset( &sa, 0, sizeof( struct sigaction ) ); \
         sa.sa_sigaction = func; \
         sa.sa_flags = SA_RESTART | SA_SIGINFO; \
         sigaction(sig, &sa, 0L); \
    }

static void sig_hdlr(int signo, siginfo_t *info, void *context);

void second();
void third();

int main() {

    SETSIG(sa, SIGINT,  sig_hdlr);
    SETSIG(sa, SIGSEGV, sig_hdlr);
    SETSIG(sa, SIGILL,  sig_hdlr);
    SETSIG(sa, SIGBUS,  sig_hdlr);
    SETSIG(sa, SIGQUIT, sig_hdlr);

    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);

    $RD;

    second();

    $RD;

    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);

    $RD;

    third();

    $RD;

    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);

    $RD;

    printf ("%s:%d: end\n",__FUNCTION__,__LINE__);
}

static void sig_hdlr(int signo, siginfo_t *info, void *context)
{
    ucontext_t *ctx = (ucontext_t *)context;

    printf ("\n\nEXCEPTION!!!\n\n===========================\n");
    printf ("PID = %d\n", getpid());
    printf ("signal_code = %d\n", info->si_code);
    printf ("signal_number = %d\n", signo);
    printf ("signal_errno = %d\n", info->si_errno);
    printf ("errno = %d\n", errno);
    printf ("fault address = %p\n", info->si_addr);
    printf ("REGISTERS:\n");
    printf ("EAX = 0x%08x\n", (uint32_t)ctx->uc_mcontext.gregs[REG_RAX]);
    printf ("EBX = 0x%08x    <---- RDEBUG register: FILE_ID: %d, LINE: %d\n", \
            (uint32_t)ctx->uc_mcontext.gregs[REG_RBX], \
            (uint32_t)ctx->uc_mcontext.gregs[REG_RBX] >> 16, \
            (uint32_t)ctx->uc_mcontext.gregs[REG_RBX] & 0xffff);

    printf ("ECX = 0x%08x\n", (uint32_t)ctx->uc_mcontext.gregs[REG_RCX]);
    printf ("EDX = 0x%08x\n", (uint32_t)ctx->uc_mcontext.gregs[REG_RDX]);
    printf ("EIP = 0x%08x\n", (uint32_t)ctx->uc_mcontext.gregs[REG_RIP]);
    printf ("ESP = 0x%08x\n", (uint32_t)ctx->uc_mcontext.gregs[REG_RSP]);

    exit( 1 );
}
