#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

/* repeat macros set */
#define rep0(x)
#define rep1(x)                 x
#define rep2(x)                 x x
#define rep3(x)                 x x x
#define rep4(x)                 x x x x
#define rep5(x)                 x rep4(x)
#define rep6(x)                 x rep5(x)
#define rep7(x)                 x rep6(x)
#define rep8(x)                 x rep7(x)
#define rep9(x)                 x rep8(x)
#define rep10(x)                x rep9(x)
#define rep100(x)               rep10(rep10(x))
#define rep1000(x)              rep10(rep100(x))
#define rep10000(x)             rep100(rep100(x))
#define rep100000(x)            rep1000(rep100(x))
#define rep(a,b,c,d,e,f,x)      rep ## f(rep1(x)) \
                                rep ## e(rep10(x)) \
                                rep ## d(rep100(x)) \
                                rep ## c(rep1000(x)) \
                                rep ## b(rep10000(x)) \
                                rep ## a(rep100000(x))
#define rep_semi(a,b,c,d,e,f,x) rep(a,b,c,d,e,f,x;)


extern char * __progname;

int main() {
    pid_t pid = getpid();
    char cmd[128];

    sprintf(cmd,"grep -B2 Rss /proc/%d/smaps | grep -A2 r-xp | grep -A2 %s", pid, __progname);

    system(cmd);

    sleep(1);

    printf("\nexecute 256000 nops\n");

    rep_semi(2,5,6,0,0,0,asm("nop"));

    system(cmd);

    sleep(1);

    printf("\nexecute 512000 nops\n");

    rep_semi(5,1,2,0,0,0,asm("nop"));

    system(cmd);

    return 0;
}

