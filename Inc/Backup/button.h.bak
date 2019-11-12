#ifndef BUTTON_H
#define BUTTON_H

#include "stdint.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

typedef struct InputFilter_ InputFilter;

#ifndef EDGE
#define EDGE
typedef enum{FALLING_EDGE, NO_EDGE, RISING_EDGE}edge;
#endif // EDGE

typedef struct Button_{
    InputFilter * filter;
    void (*sample)(struct Button_ *const);
    edge (*getEdge)(struct Button_ *const);
}Button;

uint8_t Button_init(Button *const _this, GPIO_TypeDef * const port, uint16_t pin);

#endif // BUTTON_H
