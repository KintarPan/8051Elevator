#include "NumberInput.h"
#include "display.h"
#include "initialize.h"
#include "keyboard.h"
#include "utils.h"
#include <reg51.h>

#ifdef __VSCODE_C51__
#define INTERRUPT(x)
#else
#define INTERRUPT(x) interrupt x
#endif

enum WorkState
{
    WS_GetPassword,
    WS_VarifyPassword,
    WS_PasswordWrong,
    WS_GetMaxPerson,
    WS_GetMaxWeight,
    WS_Finish,
    WS_Free
};

void getPassword();
void getMaxPerson();
void getMaxWeight();
void varifyPassword();
void wrongPasswordDelay();
void finishDelay();

const uint8_t castTable[] = {
    DC_0, DC_1, DC_2, DC_3, DC_4, DC_5, DC_6, DC_7, DC_8, DC_9, DC_A, DC_B, DC_C, DC_D, DC_E, DC_F
};
const uint8_t truePassword[] = {1, 1, 4, 5, 1, 4};
const uint8_t personPrompt[] = {DC_N, DC_O& DS_Dot};
const uint8_t weightPrompt[] = {DC_R, DC_L& DS_Dot};
const uint8_t finishPrompt[] = {DC_F, DC_1, DC_N, DC_1, DC_5, DC_H};
uint8_t errorPrompt[] = {DC_E, DC_R, DC_R, DC_O, DC_R, DS_Disabled, DS_Disabled, DS_Disabled};
uint8_t wrongPasswordDelayTime;
uint8_t finishDelayTime;
uint8_t maxPerson;
uint8_t maxWeight;
uint8_t passwordTrialCount;
uint8_t workState;

int main()
{
    init();
    workState = WS_GetPassword;
    while (1)
        ;
}

void onTimer0Timeout() INTERRUPT(1)
{
    EA = 0;
    TH0 = (-10000) / 0x0100; // 10 ms in 12MHz
    TL0 = (-10000) % 0x0100;
    switch (workState)
    {
    case WS_GetPassword:
        getPassword();
        break;
    case WS_VarifyPassword:
        varifyPassword();
        break;
    case WS_PasswordWrong:
        wrongPasswordDelay();
        break;
    case WS_GetMaxPerson:
        getMaxPerson();
        break;
    case WS_GetMaxWeight:
        getMaxWeight();
        break;
    case WS_Finish:
        finishDelay();
        break;
    case WS_Free:
        break;
    }
    Display_refreshDisplay(&display);
    EA = 1;
}

void getPassword()
{
    Display_promptInput(&display, numberInput.currentIndex);
    Keyboard_getKey(&keyboard);
    if (keyboard.state != Released)
        return;
    if (keyboard.releasedKey == SK_Enter)
    {
        workState = WS_VarifyPassword;
        return;
    }
    if (keyboard.releasedKey == SK_Backspace)
    {
        display.displayBuffer[numberInput.currentIndex] = DS_Disabled;
        NumberInput_backspace(&numberInput);
        return;
    }
    if (numberInput.currentIndex == 6)
        return;
    if (keyboard.releasedKey >= 10)
        return;
    display.displayBuffer[numberInput.currentIndex] = castTable[keyboard.releasedKey];
    Display_delayDisappear(&display, numberInput.currentIndex, 50);
    NumberInput_append(&numberInput, keyboard.releasedKey);
}

void getMaxPerson()
{
    uint8_t i = 0;
    Display_promptInput(&display, 2 + numberInput.currentIndex);
    Keyboard_getKey(&keyboard);
    if (keyboard.state != Released)
        return;
    if (keyboard.releasedKey == SK_Enter)
    {
        maxPerson = NumberInput_getNumber(&numberInput);
        NumberInput_clear(&numberInput);
        workState = WS_GetMaxWeight;
        Display_setPrompt(&display, weightPrompt, 2);
        return;
    }
    if (keyboard.releasedKey == SK_Backspace)
    {
        display.displayBuffer[2 + numberInput.currentIndex] = DS_Disabled;
        NumberInput_backspace(&numberInput);
        return;
    }
    if (numberInput.currentIndex == 3)
        return;
    if (keyboard.releasedKey >= 10)
        return;
    display.displayBuffer[2 + numberInput.currentIndex] = castTable[keyboard.releasedKey];
    NumberInput_append(&numberInput, keyboard.releasedKey);
}

void getMaxWeight()
{
    uint8_t i = 0;
    Display_promptInput(&display, 2 + numberInput.currentIndex);
    Keyboard_getKey(&keyboard);
    if (keyboard.state != Released)
        return;
    if (keyboard.releasedKey == SK_Enter)
    {
        maxWeight = NumberInput_getNumber(&numberInput);
        NumberInput_clear(&numberInput);
        workState = WS_Finish;
        Display_setPrompt(&display, finishPrompt, 6);
        return;
    }
    if (keyboard.releasedKey == SK_Backspace)
    {
        display.displayBuffer[2 + numberInput.currentIndex] = DS_Disabled;
        NumberInput_backspace(&numberInput);
        return;
    }
    if (numberInput.currentIndex == 3)
        return;
    if (keyboard.releasedKey >= 10)
        return;
    display.displayBuffer[2 + numberInput.currentIndex] = castTable[keyboard.releasedKey];
    NumberInput_append(&numberInput, keyboard.releasedKey);
}

void varifyPassword()
{
    uint8_t i = 0;
    bit passwordIsCorrect = 1;
    passwordTrialCount--;
    errorPrompt[7] = castTable[passwordTrialCount];
    if (numberInput.currentIndex != 6)
        passwordIsCorrect = 0;
    else
    {
        for (; i != 6; i++)
        {
            if (numberInput.inputBuffer[i] != truePassword[i])
            {
                passwordIsCorrect = 0;
                break;
            }
        }
    }
    if (!passwordIsCorrect)
    {
        Display_setPrompt(&display, errorPrompt, 8);
        wrongPasswordDelayTime = 100;
        workState = WS_PasswordWrong;
    } else
    {
        Display_setPrompt(&display, personPrompt, 2);
        NumberInput_clear(&numberInput);
        workState = WS_GetMaxPerson;
    }
    Display_resetDelayDisappear(&display);
}

void wrongPasswordDelay()
{
    if (passwordTrialCount == 0)
        return;
    if (wrongPasswordDelayTime == 0)
    {
        Display_clear(&display);
        NumberInput_clear(&numberInput);
        workState = WS_GetPassword;
        return;
    }
    wrongPasswordDelayTime--;
}

void finishDelay()
{
    uint8_t i = 0;
    if (finishDelayTime == 0)
    {
        for (; i != 8; i++)
            display.displayBuffer[i] = DS_Disabled;
        workState = WS_Free;
    }
    finishDelayTime--;
}
