/*************************************************************************
    > File Name: test.c
    > Author: wht
    > Mail: vi_pzp@163.com 
    > Created Time: 2016年06月17日 星期五 10时42分21秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool findSessionKey(FILE *fp, const char *session, const char *key, int *pos)
{
	bool foundSession = false;
	bool foundKey = false;
	int lineSize = 0;
	char lineString[1024] = {0};
	char keyString[1024] = {0};
	char valueString[1024] = {0};
	char sessionString[1024]={0};
	
	while(fgets(lineString, 1024, fp))
	{
		//注释行
		if ('#' == lineString[0])
			continue;
		
		lineSize = strlen(lineString);

		//去掉行尾的'\n'
		if('\n' == lineString[strlen(lineString)-1])
			lineString[strlen(lineString)-1] = '\0';
		
		if (!foundSession)
		{
			if (0 == strcmp(lineString, sessionString))
			{
				foundSession = true;
			}
		}
		else
		{
			//读到下一个session
			if ('[' == lineString[0])
				break;
			
			sscanf(lineString, "%[^=]=%s", keyString, valueString);
			if (0 == strcmp(keyString, key))
			{
				foundKey = true;
				fseek(fp, 0 - lineSize, SEEK_CUR);
				*pos = ftell(fp);
				break;
			}
		}
	}

	return foundKey == true ? true : false;
}
bool ReadPrivateProfileString(const char *fileName, const char *session, const char *key, char *value, const int len)
{
	FILE *fp = NULL;
	bool foundSession = false;
	bool foundKey = false;
	char lineString[1024] = {0};
	char keyString[1024] = {0};
	char valueString[1024] = {0};
	char sessionString[1024]={0};

	do
	{
		fp = fopen(fileName, "r");
		if (NULL == fp)
			break;
		
		snprintf(sessionString, 1024, "[%s]",session);

		while(fgets(lineString, 1024, fp))
		{
			//注释行
			if ('#' == lineString[0])
				continue;
			
			//去掉行尾的'\n'
			if('\n' == lineString[strlen(lineString)-1])
				lineString[strlen(lineString)-1] = '\0';
			
			if (!foundSession)
			{
				if (0 == strcmp(lineString, sessionString))
				{
					foundSession = true;
				}
			}
			else
			{
				//读到下一个session
				if ('[' == lineString[0])
					break;
				
				sscanf(lineString, "%[^=]=%s", keyString, valueString);
				if (0 == strcmp(keyString, key))
				{
					foundKey = true;
					int minLen = strlen(valueString) > len ? len : strlen(valueString);
					strncpy(value, valueString, minLen);
					*(value + minLen) = '\0';
					break;
				}
			}
		}

	} while(0);

	fclose(fp);
	return foundKey == true ? true : false;
}

bool WritePrivateProfileString(const char *fileName, const char *session, const char *key, const char *value)
{
	FILE *fp = NULL;
	int lineLen = 0;
	int fbSize = 0;
	int fileSize = 0;
	bool foundSession = false;
	bool foundKey = false;
	char lineString[1024] = {0};
	char keyString[1024] = {0};
	char valueString[1024] = {0};
	char sessionString[1024]={0};

	do
	{
		fp = fopen(fileName, "r");
		if (NULL == fp)
			break;
		
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fbSize = fileSize + 1;
		rewind(fp);

		snprintf(sessionString, 1024, "[%s]",session);

		while(fgets(lineString, 1024, fp))
		{
			//注释行
			if ('#' == lineString[0])
				continue;
		
			lineLen = strlen(lineString);

			//去掉行尾的'\n'
			if('\n' == lineString[strlen(lineString)-1])
				lineString[strlen(lineString)-1] = '\0';
			
			if (!foundSession)
			{
				if (0 == strcmp(lineString, sessionString))
				{
					foundSession = true;
				}
			}
			else
			{
				//读到下一个session
				if ('[' == lineString[0])
					break;
				
				sscanf(lineString, "%[^=]=%s", keyString, valueString);
				if (0 == strcmp(keyString, key))
				{
					foundKey = true;
					fbSize = fbSize + strlen(value) - strlen(valueString);
					char *fileBuffer = malloc(fbSize);
					memset(fileBuffer, 0, fbSize);
					int curr = ftell(fp);
					int used = 0;
					fseek(fp, 0, SEEK_SET);
					fread(fileBuffer, curr - lineLen, 1, fp);
					used = curr - lineLen;
					//printf("before:\n%s\n", fileBuffer);

					snprintf(fileBuffer+used, fbSize - used, "%s=%s\n", key, value);
					used = used + strlen(key) + 2 + strlen(value);
					//printf("current:\n%s\n", fileBuffer);
					
					fseek(fp, curr, SEEK_SET);
					fread(fileBuffer + used, fileSize - curr, 1, fp);
					//printf("end:\n%s\n", fileBuffer);
				
					fp = freopen(fileName, "w", fp);
					fputs(fileBuffer, fp);
					break;
				}
			}
		}

	} while(0);

	fclose(fp);
	return foundKey == true ? true : false;
}

int main(int argc, char **argv)
{
	const char *file = "./config.ini";
	char value[1024] = "xxxxxxxxxxxxxxxxxxxxx";
	char *valueNew = "x";
	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	
	if (WritePrivateProfileString(file, "session2", "key3", valueNew))
		printf("valueNew = %s\n", valueNew);

	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	return 0;
}
