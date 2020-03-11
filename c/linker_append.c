/*
    cc -Wl,--verbose -Wl,--script=append.ld test.c

    append.ld:
    ----
    PROVIDE(text_start = LOADADDR(.text));
    PROVIDE(data_start = LOADADDR(.data));
    PROVIDE(bss_start = LOADADDR(.bss));

    PROVIDE(text_end = text_start + SIZEOF(.text));
    PROVIDE(data_end = data_start + SIZEOF(.data));
    PROVIDE(bss_end = bss_start + SIZEOF(.bss));

    INSERT BEFORE .text;
    ----
 */
#include <stdio.h>
#include <sys/mman.h>

extern char text_start, text_end, data_start, data_end, bss_start, bss_end;

int main()
{
    printf("start of .text = %p, %p, %d\n", &text_start, &text_end, &text_end-&text_start);
    if (mlock(&text_start, &text_end - &text_start))
        printf("mlock for .text: %m\n");

    printf("start of .data = %p, %p, %d\n", &data_start, &data_end, &data_end-&data_start);
    if (mlock(&data_start, &data_end - &data_start))
        printf("mlock for .data: %m\n");

    printf("start of .bss  = %p, %p, %d\n", &bss_start, &bss_end, &bss_end-&bss_start);
    if (mlock(&bss_start, &bss_end - &bss_start))
        printf("mlock for .bss: %m\n");

    sleep(100);
}
