/*
    cc -c mcount.c
    cc -pg -c return-macro.c
    cc mcount.o return-macro.o
*/
#include <stdio.h>

#define return  printf("debug: " "%s:%d: " "return to %p" "\n",__func__,__LINE__,__builtin_return_address(0));return

int func1() {
    return 1;
}

void func2() {
    func1();
    return;
}

int main() {
    func1();
    func2();
    return 0;
}
