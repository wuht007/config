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

bool findSessionKey(FILE *fp, const char *session, const char *key, char *oldValue, int oldValueSize, int *pos)
{
	bool foundSession = false;
	bool foundKey = false;
	int lineSize = 0;
	char lineString[1024] = {0};
	char keyString[1024] = {0};
	char valueString[1024] = {0};
	char sessionString[1024]={0};

	snprintf(sessionString, 1024, "[%s]", session);

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
				int minSize = strlen(valueString) > oldValueSize ? oldValueSize : strlen(valueString);
				strncpy(oldValue, valueString, minSize);
				*(oldValue + minSize) = '\0';
				break;
			}
		}
	}

	return foundKey == true ? true : false;
}

bool ReadPrivateProfileString(const char *fileName, const char *session, const char *key, char *value, const int len)
{
	bool result = false;
	FILE *fp = NULL;
	int pos = 0;

	do
	{
		fp = fopen(fileName, "r");
		if (NULL == fp)
			break;

		result = findSessionKey(fp, session, key, value, len, &pos);
	} while(0);

	fclose(fp);
	return result;
}

bool WritePrivateProfileString(const char *fileName, const char *session, const char *key, const char *newValue)
{
	bool result = false;
	FILE *fp = NULL;
	int pos = 0;
	int fbSize = 0;
	int fileSize = 0;
	char lineString[1024] = {0};
	char oldValue[1024] = {0};
	

	do
	{
		fp = fopen(fileName, "r");
		if (NULL == fp)
			break;
		
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fbSize = fileSize + 1;
		rewind(fp);

		result = findSessionKey(fp, session, key, oldValue, 1024, &pos);
		if (result)
		{
			fbSize = fbSize + strlen(newValue) - strlen(oldValue);
			char *fileBuffer = malloc(fbSize);
			int bufferUsed = 0;
			memset(fileBuffer, 0, fbSize);
			fseek(fp, 0, SEEK_SET);
			fread(fileBuffer, pos, 1, fp);
			bufferUsed += pos;
			//printf("before:\n%s\n", fileBuffer);
			snprintf(fileBuffer+bufferUsed, fbSize - bufferUsed, "%s=%s\n", key, newValue);
			bufferUsed = bufferUsed + strlen(key) + 2 + strlen(newValue);
			//printf("current:\n%s\n", fileBuffer);		
			
			//要修改的行原来的数据
			fgets(lineString, 1024, fp);
			
			int curr = ftell(fp);
			fread(fileBuffer + bufferUsed, fileSize - curr, 1, fp);
			bufferUsed = bufferUsed + fileSize - curr;
			//printf("end:\n%s\n", fileBuffer);
				
			fp = freopen(fileName, "w", fp);
			fputs(fileBuffer, fp);
			free(fileBuffer);
		}

	} while(0);

	fclose(fp);
	return result;
}

int main(int argc, char **argv)
{
	const char *file = "./config.ini";
	char value[1024] = "xxxxxxxxxxxxxxxxxxxxx";
	char *valueNew = "xxxxxxx";
	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	
	if (WritePrivateProfileString(file, "session2", "key3", valueNew))
		printf("valueNew = %s\n", valueNew);

	if (ReadPrivateProfileString(file, "session2", "key3", value, 1024))
		printf("value = %s\n", value);
	return 0;
}
