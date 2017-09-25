/*
    gcc -rdynamic -finstrument-functions instr.c -ldl
*/

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

#define no_instrument_func __attribute__ ((no_instrument_function))

/* or gcc -finstrument-functions-exclude-function-list=__cyg_profile_func_enter,__cyg_profile_func_exit,tf ... */
//#define no_instrument_func

static no_instrument_func inline void tf(void * this, void * callsite, int enter) {
    Dl_info this_dl, callsite_dl;
    dladdr(this, &this_dl);
    dladdr(callsite, &callsite_dl);
    printf("%16p %s %16p : %s %s %s\n", (int *)callsite, enter?"->":"<-", (int *)this, callsite_dl.dli_sname, enter?"->":"<-", this_dl.dli_sname);
}

void no_instrument_func __cyg_profile_func_enter(void *this, void *callsite)
{
    tf(this, callsite, 1);
}


void no_instrument_func __cyg_profile_func_exit(void *this, void *callsite)
{
    tf(this, callsite, 1);
}


int func1(int i) {
    printf ("%s\n",__func__);
    return i;
}

int func2(int i) {
    printf ("%s\n",__func__);
    i++;
    return func1(i);
}

int main() {
    printf ("%s\n",__func__);
    return func1(5) + func2(5);
}
