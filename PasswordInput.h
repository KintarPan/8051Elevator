#ifndef __PASSWORD_INPUT_H__
#define __PASSWORD_INPUT_H__

#include "utils.h"

typedef struct
{
    uint8_t passwordInput[6];
    uint8_t currentIndex;
} PasswordInput;

extern PasswordInput passwordInput;

void PasswordInput_init(PasswordInput* self);
void PasswordInput_setCurrentDigit(PasswordInput* self, uint8_t n);

#endif // __PASSWORD_INPUT_H__
