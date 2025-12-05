#include "display.h"
#include "utils.h"
#include <reg51.h>

xdata Display display;

void shortDelay(uint8_t time)
{
    while (time--)
        ;
}

void Display_init(Display* self)
{
    uint8_t i = 0;
    self->currentPromptValue = DS_Bottom;
    for (; i < 8; i++)
    {
        self->displayBuffer[i] = DS_Disabled;
        self->disappearCounter[i] = -1;
    }
    self->promptCount = 0;
}

void Display_displayCharacter(Display* self, uint8_t item, uint8_t index)
{
    P2 = 0;
    P0 = DS_Disabled;
    shortDelay(10);
    P2 = index;
    P0 = item;
    shortDelay(100);
}

void Display_displayString(Display* self, uint8_t* str)
{
    int i = 0;
    int currentDigit = 0x01;
    for (i = 0; i < 8; i++)
    {
        Display_displayCharacter(self, str[i], currentDigit);
        currentDigit <<= 1;
    }
}

void Display_refreshDisplay(Display* self)
{
    uint8_t i = 0;
    for (; i < 8; i++)
    {
        if (self->disappearCounter[i] == 0)
            self->displayBuffer[i] = DS_Middle;
        if (self->disappearCounter[i] >= 0)
            self->disappearCounter[i]--;
    }
    Display_displayString(self, self->displayBuffer);
}

void Display_promptInput(Display* self, uint8_t index)
{
    self->displayBuffer[index] = self->currentPromptValue;
    self->promptCount++;
    if (self->promptCount == 20)
    {
        self->promptCount = 0;
        if (self->currentPromptValue == DS_Bottom)
            self->currentPromptValue = DS_Disabled;
        else
            self->currentPromptValue = DS_Bottom;
    }
}

void Display_delayDisappear(Display* self, uint8_t index, int8_t time)
{
    self->disappearCounter[index] = time;
}

void Display_resetDelayDisappear(Display* self)
{
    uint8_t i = 0;
    for (; i < 8; i++)
        self->disappearCounter[i] = -1;
}
