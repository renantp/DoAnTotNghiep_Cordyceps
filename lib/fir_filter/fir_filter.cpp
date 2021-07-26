/*
 * fir_filter.c
 *
 *  Created on: 10 Jun 2021
 *      Author: Hung
 */

#include "fir_filter.h"

//static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = { -0.0017759037265000722,
//		-0.006755662691079017, -0.011585386337898967, -0.004290132599689081,
//		0.030413106273274496, 0.09639553721112855, 0.1743359233031604,
//		0.22794247370314313, 0.22794247370314313, 0.1743359233031604,
//		0.09639553721112855, 0.030413106273274496, -0.004290132599689081,
//		-0.011585386337898967, -0.006755662691079017, -0.0017759037265000722 };

/*

 FIR filter designed with
 http://t-filter.appspot.com

 sampling frequency: 18000 Hz

 * 0 Hz - 500 Hz
 gain = 1
 desired ripple = 1 dB
 actual ripple = 0.021056469722792705 dB

 * 4500 Hz - 9000 Hz
 gain = 0
 desired attenuation = -40 dB
 actual attenuation = -61.965918967833986 dB

 */
static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = { -0.0030307304347347557,
		-0.011033152035782564, -0.01967342340561219, -0.014907440676854953,
		0.022093208431463767, 0.09589444016822785, 0.18422131999496114,
		0.24525414740192017, 0.24525414740192017, 0.18422131999496114,
		0.09589444016822785, 0.022093208431463767, -0.014907440676854953,
		-0.01967342340561219, -0.011033152035782564, -0.0030307304347347557 };

void FIRFilter_Init(FIRFilter_t *fir){
	unsigned char i;
	for ( i = 0; i < FIR_FILTER_LENGTH; i++) {
		fir->buf[i] = 0;
	}

	fir->bufIndex = 0;

	fir->out = 0.0f;
}
float FIRFilter_Update(FIRFilter_t *fir, float inp) {
	unsigned char i, sumIndex;
	/* Store latest sample in buffer */
	fir->buf[fir->bufIndex] = inp;
	/* Increment buffer index and wrap around if necessary */
	fir->bufIndex++;
	if (fir->bufIndex == FIR_FILTER_LENGTH)
		fir->bufIndex = 0;
	/* Compute new output sample (via convoluion) */
	fir->out = 0.0f;
	sumIndex = fir->bufIndex;

	for (i = 0; i < FIR_FILTER_LENGTH; i++) {

		if (sumIndex > 0) {
			sumIndex--;
		} else {
			sumIndex = FIR_FILTER_LENGTH - 1;
		}
		/* Multiply impulse response with shifted input sample and add to output  */
		fir->out += FIR_IMPULSE_RESPONSE[i] * (fir->buf[sumIndex]);
	}
	return fir->out;
}
