/*
 * predictsat.h
 *
 *  Created on: May 24, 2022
 *      Author: eloch
 */

#ifndef SRC_PREDICTSAT_H_
#define SRC_PREDICTSAT_H_
#define BUFFER_SIZE 20

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <predict/predict.h>
#include "rtc.h"
#include <time.h>
#include "FreeRTOS.h"
#include "queue.h"
#include <time.h>
#include "IHMMGR.h"

char       buf[80];
char       buffff[80];
struct tm  ts;
typedef struct read_s{

	char* adress;
	int status;
}read_t;
typedef struct write_s{

	char* adress;
	char* contenue ;
	int status;
}write_t;
char buffer[BUFFER_SIZE] ;
RTC_TimeTypeDef gTime;
RTC_DateTypeDef gDate;
predict_orbital_elements_t *iss ;
predict_observer_t *obs ;
struct predict_observation iss_obs;
predict_julian_date_t  curr_time , curr_timeIHM ;
QueueHandle_t Qpredict , Qread , Qwrite , Qtransmit ;
struct predict_position iss_orbit;
struct predict_observation aos_obs ;
struct predict_observation los_obs;
time_t time11 ;
struct tm lt  , lt22 ;
//void TIMEDATE_SET() ;
predict_julian_date_t GetCurrentJuilanTime() ;
time_t GetCurrentfromJuilanTime(predict_julian_date_t JulTim) ;
predict_julian_date_t get_julian_date();
struct tm next_pass_aos_sat(time_t time11) ;
void print_next_pass_aos(struct tm  ts) ;
struct tm sat_next_aos_pass(float Lat , float lon ,float lat, predict_orbital_elements_t *iss) ;
struct tm sat_next_los_pass(float Lat , float lon ,float lat, predict_orbital_elements_t *iss) ;
void Prediction_task (void const* pvParameters) ;

#endif /* SRC_PREDICTSAT_H_ */
