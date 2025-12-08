#include "display.h"
#include "utils.h"
#include <reg51.h>

Display display;

void Display_init(Display* self)
{
    self->currentPromptValue = DS_Bottom;
    Display_clear(self);
    Display_resetDelayDisappear(self);
    self->promptCount = 0;
}

void Display_displayCharacter(uint8_t item, uint8_t index)
{
    uint8_t delayTime = 100;
    P2 = 0;
    P0 = DS_Disabled;
    P2 = index;
    P0 = item;
    shortDelay(delayTime);
}

void Display_refreshDisplay(Display* self)
{
    uint8_t currentDigit = 0x01;
    uint8_t i = 0;
    for (; i != 8; i++)
    {
        if (self->disappearCounter[i] == 0)
            self->displayBuffer[i] = DS_Middle;
        if (self->disappearCounter[i] != -1)
            self->disappearCounter[i]--;

        Display_displayCharacter(self->displayBuffer[i], currentDigit);
        currentDigit <<= 1;
    }
}

void Display_promptInput(Display* self, uint8_t index)
{
    self->displayBuffer[index] = self->currentPromptValue;
    self->promptCount++;
    if (self->promptCount == 20)
    {
        self->promptCount = 0;
        self->currentPromptValue = ~(self->currentPromptValue ^ DS_Bottom);
    }
}

void Display_resetDelayDisappear(Display* self)
{
    uint8_t i = 0;
    for (; i != 8; i++)
        self->disappearCounter[i] = -1;
}

void Display_clear(Display* self)
{
    uint8_t i = 0;
    for (; i != 8; i++)
        self->displayBuffer[i] = DS_Disabled;
}

void Display_setPrompt(Display* self, uint8_t* prompt, uint8_t size)
{
    uint8_t i = 0;
    for (; i != size; i++)
        self->displayBuffer[i] = prompt[i];
    for (; i != 8; i++)
        self->displayBuffer[i] = DS_Disabled;
}
