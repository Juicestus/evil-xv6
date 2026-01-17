#ifndef _CC_UTIL_H_
#define _CC_UTIL_H_

#include "user/user.h"

bool iswhitespace(const char ch) 
{
    return (ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r');
}

bool isnumeric(const char ch) 
{
    return (ch >= '0') && (ch <= '9');
}

bool isnumber(char* s) 
{
    while (*(s++))
    {
        if (!isnumeric(*s)) 
            return false;
    }
    return true;
}

bool ishexadecimal(const char ch) 
{
    return ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f')) ||
           ((ch >= 'A') && (ch <= 'F'));
}

bool isalpha(const char ch) 
{
    return ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) ||
           ch == '_';
}

bool isidentstring(const char* s) 
{
    if (!isalpha(*s)) return false;
    while (*s) 
    {
        if (!(isalpha(*s) || isnumeric(*s))) return false;
        s++;
    }
    return true;
}

void printn(char* s, int n)
{
    for (int i = 0; i < n; i++)
    {
        putchar(s[i]);
    }
}

#endif