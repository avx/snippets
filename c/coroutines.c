#include <stdio.h>

#define crBegin static int state=0; switch(state) { case 0:
#define crFinish state = 0; }
#define crReturn(n) do { state=__LINE__; return n; \
                         case __LINE__:; } while (0)

int flag;

int task1() {
    crBegin;

    while(1)
    {
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(0);
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(1);
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(2);
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(3);
    }
    crFinish;
}

int task2() {
    crBegin;

    while(1)
    {
	while(flag!=4)
	    crReturn(0);
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(0);
	
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(1);

	printf("%s:%d\n", __func__, __LINE__);
	crReturn(2);
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(3);
    }
    crFinish;
}

int task3() {
    crBegin;

    while(1)
    {
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(0);
	flag=1;
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(1);
	flag=2;
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(2);
	flag=3;
	printf("%s:%d\n", __func__, __LINE__);
	crReturn(3);
	flag=4;
    }
    crFinish;
}

int main()
{
    while(1)
    {
	task1();
	task2();
	task3();
    }
}
