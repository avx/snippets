/*
    cc -c mcount.c
    cc -pg -c any.other.c
    cc mcount.o any.other.o
*/

void mcount() {
    printf("debug: " "mcount: " "%p" "\n",__builtin_return_address(0));
    return;
}
