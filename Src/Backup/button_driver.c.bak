//$dnd changed to "button_driver.h"
#include "button_driver.h"

static void InputFilter_sample(InputFilter *const _this)
{
    GPIO_PinState currentInputState = GPIO_PIN_RESET;
    currentInputState = HAL_GPIO_ReadPin(_this->port, _this->pin);

    _this->rawInputSet = _this->rawInputSet << 1;
    _this->rawInputSet |= currentInputState;

    if (_this->inputState == true) {
        if (_this->rawInputSet == INPUT_LOW) {
            _this->pendingEdge = FALLING_EDGE;
            _this->inputState = false;
        }
    }
    else {
        if (_this->rawInputSet == INPUT_HIGH) {
            _this->pendingEdge = RISING_EDGE;
            _this->inputState = true;
        }
    }
}

static edge InputFilter_getEdge(InputFilter *const _this)
{
    edge pendingEdge = _this->pendingEdge;
    _this->pendingEdge = NO_EDGE;
    return pendingEdge;
}

uint8_t InputFilter_init(InputFilter *const _this, GPIO_TypeDef *const port, uint16_t pin)
{
    _this->rawInputSet = 0;
    _this->inputState = false;
    _this->pendingEdge = NO_EDGE;
    _this->port = port;
    _this->pin = pin;
    _this->sample = InputFilter_sample;
    _this->getEdge = InputFilter_getEdge;
    return EXIT_SUCCESS;
}
