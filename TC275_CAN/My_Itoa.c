#include "My_Itoa.h"

#include "IfxAsclin_Asc.h"
#include "IfxPort.h"
#include "Ifx_Console.h"
#include "Ifx_Shell.h"
#include "Ifx_Types.h"

void my_itoa(int value, char* str)
{
    char* p = str;
    char* p1, *p2;
    int negative = 0;

    if (value < 0)
    {
        negative = 1;
        value = -value;
    }

    do {
        *p++ = (value % 10) + '0';
        value /= 10;
    } while (value);

    if (negative)
        *p++ = '-';

    *p = '\0';

    // Reverse the string
    p1 = str;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void my_itoa_hex(unsigned int value, char* str)
{
    char* p = str;
    char hex_chars[] = "0123456789ABCDEF";

    do {
        *p++ = hex_chars[value % 16];
        value /= 16;
    } while (value);

    *p = '\0';

    // Reverse the string
    char* p1 = str;
    char* p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}
