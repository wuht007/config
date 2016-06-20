/*************************************************************************
    > File Name: test.c
    > Author: wht
    > Mail: vi_pzp@163.com 
    > Created Time: 2016年06月17日 星期五 10时42分21秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include "config.h"

int main(int argc, char **argv)
{
	const char *file = "./config.ini";
	char value[1024] = "xxxxxxxxxxxxxxxxxxxxx";
	char *valueNew = "xxxxxxx";
	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	
	if (RevisePrivateProfileString(file, "session2", "key3", valueNew))
		printf("valueNew = %s\n", valueNew);

	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	return 0;
}
