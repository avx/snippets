#include <stdio.h>
#include "rdebug.h"

#define THIS_FILE_ID    3

USE_RDEBUG(THIS_FILE_ID);

void third() {
    char * ptr = 0;
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
    *ptr=111;
    printf ("%s:%d: something\n",__FUNCTION__,__LINE__);
    $RD;
}