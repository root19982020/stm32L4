/*
 * MQ135.c
 *
 *  Created on: May 31, 2022
 *      Author: eloch
 */


#include "MQ135.h"
#include "math.h"

float get_MQ_resistance(float adc_val)
{float MQ135_val ;
MQ135_val = (float) adc_val*(3.3/4096);
return MQ135_val ;
}
float get_ppm_val (float RES)
{ float ppm_val ;
ppm_val = PARA * pow((RES/RZERO),-PARB);
	return ppm_val;
}
float getCorrectionFactor(float t, float h) {
  return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
}
float getCorrectedResistance(float t, float h,float MQ135_val)
{
	return MQ135_val /getCorrectionFactor(t, h);
}
float get_corrected_ppm_val ( float corrected_resist)
{float  corr_ppm ;

		corr_ppm =	 PARA * pow((corrected_resist/RZERO), -PARB);
		return corr_ppm ;
}
