#include "keyboard.h"
#include "utils.h"
#include <reg51.h>

Keyboard keyboard;

void Keyboard_getKey(Keyboard* self)
{
    uint8_t currentKey = 0xFF;
    uint8_t rowMask = 0x01;
    uint8_t colMask = 0x10;
    uint8_t row = 0;
    uint8_t col = 0;

    for (; row != 4; row++)
    {
        P1 = ~rowMask;
        if (P1 != ~rowMask)
            break;
        rowMask <<= 1;
    }
    for (; col != 4; col++)
    {
        P1 = ~colMask;
        if (P1 != ~colMask)
            break;
        colMask <<= 1;
    }
    if (row != 4 && col != 4)
        currentKey = row * 4 + col;

    switch (self->state)
    {
    case Free:
        if (currentKey != (uint8_t)0xFF)
        {
            self->state = PrePress;
            self->pressedKey = currentKey;
        }
        break;
    case PrePress:
        if (currentKey == (uint8_t)0xFF)
        {
            self->state = Free;
            break;
        }
        if (currentKey == self->prevKey)
        {
            self->pressedKey = currentKey;
            self->state = Pressed;
            break;
        }
        break;
    case Pressed:
        if (currentKey == (uint8_t)0xFF)
        {
            self->state = PreRelease;
            self->releasedKey = self->pressedKey;
            break;
        }
        if (currentKey != self->pressedKey)
        {
            self->state = PrePress;
            break;
        }
        break;
    case PreRelease:
        if (currentKey == (uint8_t)0xFF)
        {
            self->state = Released;
            break;
        }
        if (currentKey == self->prevKey)
        {
            self->pressedKey = currentKey;
            self->state = Pressed;
            break;
        }
        break;
    case Released:
        if (currentKey == (uint8_t)0xFF)
        {
            self->state = Free;
            break;
        }
        self->state = PreRelease;
        break;
    }
    self->prevKey = currentKey;
}
