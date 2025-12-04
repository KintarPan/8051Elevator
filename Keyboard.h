#include "utils.h"

enum KeyboardState
{
    Free,
    PrePress,
    Pressed,
    PreRelease,
    Released
};

typedef struct
{
    uint8_t pressedKey;
    uint8_t releasedKey;
    uint8_t prevKey;
    uint8_t state;
} Keyboard;

extern Keyboard keyboard;

void Keyboard_init(Keyboard* self);
void Keyboard_getKey(Keyboard* self);
