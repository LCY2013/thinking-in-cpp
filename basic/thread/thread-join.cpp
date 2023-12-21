//
// Created by fufeng on 2023/12/21.
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define TIME_FILENAME "time.txt"

void* fileThreadFunc(void* arg)
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timeStr[32] = {0};
    snprintf(timeStr, 32, "%04d/%02d/%02d %02d:%02d:%02d",
             t->tm_year+1900,
             t->tm_mon+1,
             t->tm_mday,
             t->tm_hour,
             t->tm_min,
             t->tm_sec);
    //文件不存在，则创建；存在，则覆盖。
    FILE* fp = fopen(TIME_FILENAME, "w");
    if (fp == NULL)
    {
        printf("Failed to create time.txt.\n");
        return NULL;
    }

    size_t sizeToWrite = strlen(timeStr) + 1;
    size_t ret = fwrite(timeStr, 1, sizeToWrite, fp);
    if (ret != sizeToWrite)
    {
        printf("Write file error.\n");
    }

    fclose(fp);
    return NULL;
}

int main()
{
    pthread_t fileThreadID;
    int ret = pthread_create(&fileThreadID, NULL, fileThreadFunc, NULL);
    if (ret != 0)
    {
        printf("Failed to create fileThread.\n");
        return -1;
    }

    int* retval;
    pthread_join(fileThreadID, (void**)&retval);

    //使用r选项，要求文件必须存在
    FILE* fp = fopen(TIME_FILENAME, "r");
    if (fp == NULL)
    {
        printf("open file error.\n");
        return -2;
    }

    char buf[32] = {0};
    int sizeRead = fread(buf, 1, 32, fp);
    if (sizeRead == 0)
    {
        printf("read file error.\n");
        fclose(fp);
        return -3;
    }

    printf("Current Time is: %s.\n", buf);

    fclose(fp);

    return 0;
}
