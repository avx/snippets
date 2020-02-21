#ifndef _RDEBUG_H_
#define _RDEBUG_H_

#if defined __x86_64__ || defined __i386__
# define RDEBUG_REGISTER "ebx"
#elif defined __arm__ || defined __thumb__
# define RDEBUG_REGISTER "r3"
#else
# error "Please define RDEBUG_REGISTER"
//#define RDEBUG_REGISTER "a0"
#endif

#define USE_RDEBUG(_file_id)    \
    volatile register unsigned int rdebug_file_line_reg asm(RDEBUG_REGISTER); \
    static const unsigned short rdebug_file_id = _file_id;

#define _RD(_line)              \
    do { rdebug_file_line_reg = (((rdebug_file_id)<<16)|((_line)&0xffff)); } while(0)

#define $RD      _RD(__LINE__)

//#define $P       do { printf ("rdebug_register = 0x%08x\n", rdebug_file_line_reg); } while(0)

#endif /* _RDEBUG_H_ */
