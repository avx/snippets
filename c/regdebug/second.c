#include <stdio.h>
#include "rdebug.h"

#define THIS_FILE_ID    2

USE_RDEBUG(THIS_FILE_ID);

void second() {
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
    *((int *)0x100) = 1;
    $RD;
    printf ("%s:%d: end\n",__FUNCTION__,__LINE__);
}