#ifndef rgb_H
#define rgb_H

#include "main.h"




struct RGB{
	uint32_t Red,Green,Blue;
};
typedef struct RGB rgbStruct;

struct linearVariable{
	float a,b;
};
typedef struct linearVariable lvStruct;
lvStruct findLinear(int min, int max)
{

	lvStruct s;
	s.a = -98/(max-min);
	s.b = -(-99*max+min)/(max-min);
	return s;
}

void setRGB(int tempCelsius)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
//	tempCelsius = tempCelsius-20;


	int redMax = 40;
	int redMin = 10;
	int greenMinStart = 0;
	int greenMaxMedium = 15;
	int greenMinEnd = 30;
	int blueMax = -20;
	int blueMin = 0;
	lvStruct redValue 			= findLinear(redMin, redMax);
	lvStruct greenValueFirst 	= findLinear(greenMinStart, greenMaxMedium);
	lvStruct greenValueSecond	= findLinear(greenMinEnd, greenMaxMedium);
	lvStruct blueValue 			= findLinear(blueMin, blueMax);
	rgbStruct rgb;

	// Red condition
	if (tempCelsius<redMin) {
		rgb.Red = 100;
	}
	else if ((tempCelsius>redMin && tempCelsius<redMax)) {
		rgb.Red = redValue.a*tempCelsius + redValue.b;
	}
	else if (tempCelsius>redMax) {
		rgb.Red = 1;
	}

	// Green condition
	if (tempCelsius<greenMinStart) {
		rgb.Green = 100;
	}
	else if ((tempCelsius>greenMinStart && tempCelsius<greenMaxMedium)) {
		rgb.Green = greenValueFirst.a*tempCelsius + greenValueFirst.b;
	}
	else if ((tempCelsius>greenMaxMedium && tempCelsius<greenMinEnd)) {
		rgb.Green = greenValueSecond.a*tempCelsius + greenValueSecond.b;
	}
	else if (tempCelsius > greenMinEnd) {
		rgb.Green = 100;
	}

	// Blue condition
	if (tempCelsius<blueMax) {
		rgb.Blue = 1;
	}
	else if ((tempCelsius>blueMax && tempCelsius<blueMin)) {
		rgb.Blue = blueValue.a*tempCelsius + blueValue.b;
	}
	else if (tempCelsius>blueMin) {
		rgb.Blue = 100;
	}


	htim3.Instance->CCR1 = rgb.Red;
	htim3.Instance->CCR2 = rgb.Green;
	htim1.Instance->CCR2 = rgb.Blue;


}


#endif // JOYSTICK_H

