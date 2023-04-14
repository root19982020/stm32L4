/*
 * MQ135.h
 *
 *  Created on: May 31, 2022
 *      Author: eloch
 */

#ifndef SRC_MQ135_H_
#define SRC_MQ135_H_

 #include "main.h"
#include "math.h"

/// The load resistance on the board
#define RLOAD 22.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 5804.99
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 411.29

float get_MQ_resistance(float adc_val) ;
float get_ppm_val (float RES) ;
float getCorrectionFactor(float t, float h) ;
float getCorrectedResistance(float t, float h,float MQ135_val) ;
float get_corrected_ppm_val ( float corrected_resist);

#endif /* SRC_MQ135_H_ */
