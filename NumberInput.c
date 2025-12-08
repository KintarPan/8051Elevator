#include "NumberInput.h"

NumberInput numberInput;

uint8_t NumberInput_getNumber(NumberInput* self)
{
    uint8_t i = 0;
    uint8_t result = 0;
    for (; i != self->currentIndex; i++)
        result = result * 10 + self->inputBuffer[i];
    return result;
}
