/*
 * fir_filter.h
 *
 *  Created on: 10 Jun 2021
 *      Author: Hung
 */

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

#include "math.h"

#define FIR_FILTER_LENGTH 16

typedef struct
{
	float buf[FIR_FILTER_LENGTH];
	unsigned char bufIndex;

	float out;
} FIRFilter_t;

void FIRFilter_Init(FIRFilter_t *fir);
float FIRFilter_Update(FIRFilter_t *fir, float inp);

#endif /* FIR_FILTER_H_ */
