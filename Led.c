#include "Led.h"
#include <reg51.h>

void Led_turnOn(uint8_t index)
{
    uint8_t flag = 0xFE;
    flag <<= index;
    P3 = flag;
}
