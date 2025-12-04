#include "PasswordInput.h"

PasswordInput passwordInput;

void PasswordInput_init(PasswordInput* self)
{
    uint8_t i = 0;
    self->currentIndex = 0;
    for (; i < 6; i++)
        self->passwordInput[i] = 0xFF;
}

void PasswordInput_setCurrentDigit(PasswordInput* self, uint8_t n)
{
    self->passwordInput[self->currentIndex] = n;
}
