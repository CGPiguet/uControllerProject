#ifndef rgb_H
#define rgb_H

#include "main.h"




typedef struct RGB{
	uint32_t Red;
	uint32_t Green;
	uint32_t Blue;
}RGB;

void setRGB(RGB rgb, int tempCelsius)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	tempCelsius = tempCelsius-20;


	// Red condition
	if (tempCelsius<20) {
		rgb.Red = 100;
	}
	else if ((tempCelsius>20 && tempCelsius<40)) {
		rgb.Red = (-49/50)*tempCelsius + 197;
	}
	else if (tempCelsius>40) {
		rgb.Red = 1;
	}

	// Green condition
	if (tempCelsius<0) {
		rgb.Green = 100;
	}
	else if ((tempCelsius>0 && tempCelsius<30)) {
		rgb.Green = (-98/25)*tempCelsius + (593/5);
	}
	else if (tempCelsius > 30) {
		rgb.Green = 100;
	}

	// Blue condition
	if (tempCelsius<-10) {
		rgb.Blue = 1;
	}
	else if ((tempCelsius>-10 && tempCelsius<5)) {
		rgb.Blue = (98/15)*tempCelsius + (199/3);
	}
	else if (tempCelsius>5) {
		rgb.Blue = 100;
	}


	htim3.Instance->CCR1 = rgb.Red;
	htim3.Instance->CCR2 = rgb.Green;
	htim1.Instance->CCR2 = rgb.Blue;


}

#endif // JOYSTICK_H

