#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "button.h"


typedef enum{JOYSTICK_NEUTRAL, JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_CENTER}joystickDirection;

typedef struct Joystick_{
    Button *up;
    Button *down;
    Button *left;
    Button *right;
    Button *center;
    void (*sample)(struct Joystick_ *const);
    joystickDirection(*getDirection)(struct Joystick_ *const);
}Joystick;

uint8_t Joystick_init(Joystick *const _this);

#endif // JOYSTICK_H

