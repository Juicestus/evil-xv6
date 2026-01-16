/**
 * ed.c - simple editor for xv6
 * 
 * 
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

#define BUF_SIZE        256
#define MAX_LINE_NUM    256
#define MAX_LINE_LENGTH 256
#define NULL            0


char *strcatn(char *dst, char *src, int len);

int main(int argc, char *argv[])
{
    printf("Hello from editor\n");
}

char *strcatn(char *dst, char *src, int len)
{
    if (len <= 0) 
        return dst;
    int pos = strlen(dst);
    if (len + pos >= MAX_LINE_LENGTH) 
        return dst;
    for (int i = 0; i < len; i++)
        dst[i + pos] = src[i];
    dst[len + pos] = '\0';
    return dst;
}


