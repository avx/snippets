/*
    cc -rdynamic main.c -ldl
*/

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    Dl_info dl;

    dladdr(__builtin_return_address(0), &dl);

    printf ("%s():%d ret addr=%p caller=%p(%s) %s obj=%s\n", __func__, __LINE__, \
            __builtin_return_address(0), dl.dli_saddr, dl.dli_sname, (dl.dli_saddr == main)?"recursive":"", dl.dli_fname);

    if (argc == 1) {
        return 0;
    }

    main(argc-1, argv);
}
