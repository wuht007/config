/*************************************************************************
    > File Name: config.h
    > Author: wht
    > Mail: vi_pzp@163.com 
    > Created Time: 2016年06月20日 星期一 14时18分56秒
 ************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

bool ReadPrivateProfileString(const char *fileName, const char *session, const char *key, char *value, const int valueBufferSize);

bool RevisePrivateProfileString(const char *fileName, const char *session, const char *key, const char*newValue);

#endif
