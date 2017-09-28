/*
    cc -static -nostdlib static.c
*/
#include <stdarg.h>

#define SYSCALL_MAX_ARGS        6

#define SYS_EXIT                1
#define SYS_WRITE               4
#define SYS_GETPID              20
#define SYS_NANOSLEEP           162

#define MIN(a,b)                ((a)<(b)?(a):(b))
#define ABS(x)                  ((x)<0?-(x):(x))

int syscall0(int syscall) {
    int ret;

    __asm__ ("movl %1,%%eax\n\t"
             "int $0x80\n\t"
             "movl %%eax,%0\n\t"
             : "=r" (ret)
             : "m" (syscall)
             : "eax");

    return ret;
}

int syscall1(int syscall, int arg) {
    int ret;

    __asm__ ("movl %[syscall],%%eax\n\t"
             "movl %[arg],%%ebx\n\t"
             "int $0x80\n\t"
             "movl %%eax,%[ret]\n\t"
             : [ret] "=r" (ret)
             : [syscall] "m" (syscall), 
               [arg] "m" (arg)
             : "eax", "ebx");

    return ret;
}

int syscall2(int syscall, int arg0, int arg1) {
    int ret;

    __asm__ ("movl %[syscall],%%eax\n\t"
             "movl %[arg0],%%ebx\n\t"
             "movl %[arg1],%%ecx\n\t"
             "int $0x80\n\t"
             "movl %%eax,%[ret]\n\t"
             : [ret] "=r" (ret)
             : [syscall] "m" (syscall),
               [arg0] "m" (arg0),
               [arg1] "m" (arg1)
             : "eax", "ebx", "ecx");

    return ret;
}

int syscall3(int syscall, int arg0, int arg1, int arg2) {
    int ret;

    __asm__ ("movl %[syscall],%%eax\n\t"
             "movl %[arg0],%%ebx\n\t"
             "movl %[arg1],%%ecx\n\t"
             "movl %[arg2],%%edx\n\t"
             "int $0x80\n\t"
             "movl %%eax,%[ret]\n\t"
             : [ret] "=r" (ret)
             : [syscall] "m" (syscall), 
               [arg0] "m" (arg0), 
               [arg1] "m" (arg1), 
               [arg2] "m" (arg2)
             : "eax", "ebx", "ecx", "edx");

    return ret;
}

int syscall4(int syscall, int arg0, int arg1, int arg2, int arg3) {
    int ret;

    __asm__ ("movl %[syscall],%%eax\n\t"
             "movl %[arg0],%%ebx\n\t"
             "movl %[arg1],%%ecx\n\t"
             "movl %[arg2],%%edx\n\t"
             "movl %[arg3],%%esi\n\t"
             "int $0x80\n\t"
             "movl %%eax,%[ret]\n\t"
             : [ret] "=r" (ret)
             : [syscall] "m" (syscall),
               [arg0] "m" (arg0),
               [arg1] "m" (arg1),
               [arg2] "m" (arg2),
               [arg3] "m" (arg3)
             : "eax", "ebx", "ecx", "edx", "esi");

    return ret;
}

const syscall_args[] = {
    [SYS_EXIT] = 1,
    [SYS_WRITE] = 3,
    [SYS_NANOSLEEP] = 1,
    [SYS_GETPID] = 0
};

/* system call implementaion */
int syscall(int syscall, ...) {
    int arg[SYSCALL_MAX_ARGS] = {0};
    int ret;
    va_list ap;

    va_start (ap, syscall);

    switch (syscall_args[syscall]) {
        case 0:
            ret = syscall0(syscall);
            break;
        case 1:
            ret = syscall1(syscall, va_arg(ap,int));
            break;
        case 2:
            arg[0] = va_arg(ap,int);
            arg[1] = va_arg(ap,int);
            ret = syscall2(syscall, arg[0], arg[1]);
            break;
        case 3:
            arg[0] = va_arg(ap,int);
            arg[1] = va_arg(ap,int);
            arg[2] = va_arg(ap,int);
            ret = syscall3(syscall, arg[0], arg[1], arg[2]);
            break;
        case 4:
            arg[0] = va_arg(ap,int);
            arg[1] = va_arg(ap,int);
            arg[2] = va_arg(ap,int);
            arg[4] = va_arg(ap,int);
            ret = syscall4(syscall, arg[0], arg[1], arg[2], arg[3]);
            break;
        default:
            break;
    }

    va_end (ap);
    return ret;
}

static int sleep(int seconds) {
    struct {
        unsigned int tv_sec;
        unsigned int tv_usec;
    } ts;

    ts.tv_sec = seconds;
    ts.tv_usec = 0;

    return syscall(SYS_NANOSLEEP, (int)&ts);
}

int strlen(char * str) {
    int i=0;
    while(*str++)
        i++;
    return i;
}

#define PRINTF_BUFFER   256

#define STDIN           0
#define STDOUT          1
#define STDERR          2


int strcopy(char * dst, char * src) {
    int i = 0;
    while (*src) {
        *dst++=*src++;
        i++;
    }
    return i;
}

static int puts(char * str) {
    return syscall(SYS_WRITE, STDOUT, (int)str, strlen(str));
}


int printf(char * fmt, ...) {
    const char printf_overflow_message[] = "!@#$%>>> error: printf buffer overflowed!\n";
    unsigned char pbuf[PRINTF_BUFFER];
    register int c=0, c2=0, c3=0, max_c = PRINTF_BUFFER - sizeof(printf_overflow_message);
    char t=0;
    va_list ap;
    va_start (ap, fmt);

#define inc(c)     ({ if (c > max_c) goto fail; c++;})

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                case 'i':
                    {
                        int arg;
                        arg = va_arg(ap,int);

                        if (arg < 0)
                            pbuf[inc(c)] = '-';
                        else if (arg == 0)
                            pbuf[inc(c)] = '0';

                        c2=c;
                        while (arg) {
                            pbuf[inc(c)] = ABS(arg%10) + '0';
                            arg = arg/10;
                        }

                        c3=c-1;

                        while (c2 < c3) {
                            pbuf[c2] ^= pbuf[c3];
                            pbuf[c3] ^= pbuf[c2];
                            pbuf[c2] ^= pbuf[c3];
                            c2++;
                            c3--;
                        }

                        break;
                    }
                case 'u':
                    {
                        unsigned int arg;
                        arg = va_arg(ap,unsigned int);

                        if (arg == 0)
                            pbuf[inc(c)] = '0';

                        c2=c;
                        while (arg) {
                            pbuf[inc(c)] = arg%10 + '0';
                            arg = arg/10;
                        }
                        c3=c-1;

                        while (c2 < c3) {
                            pbuf[c2] ^= pbuf[c3];
                            pbuf[c3] ^= pbuf[c2];
                            pbuf[c2] ^= pbuf[c3];
                            c2++;
                            c3--;
                        }

                        break;
                    }
                case 'c':
                    {
                        int arg;
                        arg = va_arg(ap,int);
                        pbuf[inc(c)]=(unsigned char)arg;
                        break;
                    }
                case 's':
                    {
                        char * arg;
                        arg = va_arg(ap,char *);
                        while(*arg) {
                            pbuf[inc(c)] = *arg;
                            arg++;
                        }
                        break;
                    }
                case '%':
                        pbuf[inc(c)] = '%';
                        break;
                default :
                        pbuf[inc(c)] = '%';
                        pbuf[inc(c)] = *fmt;
                        break;

            }
        }
        else {
            pbuf[c] = *fmt;
            inc(c);
        }

        fmt++;
    }

end:
    pbuf[c]=0;

    puts(pbuf);

    va_end (ap);

    return c;

fail:
    c += strcopy(&pbuf[c], (char *)printf_overflow_message);
    goto end;
}

int main() {
    int pid = syscall(SYS_GETPID);

    printf("pid = %d ; sleep a while before exit\n", pid);

    sleep(30);

    return 0;
}

void __attribute__((noreturn)) _start() {
    syscall(SYS_EXIT, main());

loop: /* never reach */
    goto loop;
}
