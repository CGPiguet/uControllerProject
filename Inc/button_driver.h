#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"

#define INPUT_LOW   ((uint16_t)  0)
#define INPUT_HIGH  ((uint16_t)  65535)


#ifndef EDGE
#define EDGE
typedef enum{FALLING_EDGE, NO_EDGE, RISING_EDGE}edge;
#endif // EDGE

typedef struct InputFilter_{
    uint16_t rawInputSet;
    bool inputState;
    edge pendingEdge;
    GPIO_TypeDef * port;
    uint16_t pin;
    void (*sample)(struct InputFilter_ *const);
    edge (*getEdge)(struct InputFilter_ *const);
}InputFilter;

uint8_t InputFilter_init(InputFilter *const _this, GPIO_TypeDef *const port, uint16_t pin);

#endif // BUTTON_DRIVER_H
