/*
 * Predictsat.c
 *
 *  Created on: May 24, 2022
 *      Author: eloch
 */
#include "predictsat.h"
#include "rtc.h"
#include "FreeRTOS.h"
#include "gps.h"
#include "RTChandler.h"
#include "cmsis_os.h"
#include <predict/predict.h>
#include <time.h>
extern TinyGPS_t gps ;
struct predict_observation aos_obs;
//struct predict_observation los_obs;

RTC_TimeTypeDef gTime={0};
RTC_DateTypeDef gDate={0};

predict_julian_date_t GetCurrentJuilanTime(){
	predict_julian_date_t JulTim;
	struct tm timeval;
	char str [15];
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);							 	/*Get Current UTC Time*/
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	/* Display date Format: mm-dd-yy */
	sprintf((char*)str, " \r\n %02d-%02d-%2d",gDate.Date, gDate.Month,  2000 + gDate.Year);
	//HAl_uart((char*)str);
	/* Display time Format: hh:mm:ss */
	sprintf((char*)str," %02d:%02d:%02d \r\n",gTime.Hours, gTime.Minutes, gTime.Seconds);
	//uartTransmit((char*)str);
	timeval.tm_year = 2000-1900 + gDate.Year ;//timeval.tm_year = 2000-1900+gDate.Year;
	timeval.tm_mon  = gDate.Month-1;
	timeval.tm_mday = gDate.Date;
	timeval.tm_hour = gTime.Hours;//-SysTimeDate.TimeZone;
	timeval.tm_min  = gTime.Minutes;
	timeval.tm_sec  = gTime.Seconds;
	//validatehoursByTimeZone(SysTimeDate.TimeZone,&timeval.tm_hour,0);
	JulTim=predict_to_julian(mktime(&timeval));   	/*Get Current Julian Time*/
	return JulTim;
}
predict_julian_date_t get_julian_date()
{
	curr_time = GetCurrentJuilanTime() ;
	return 1 ;
}

time_t GetCurrentfromJuilanTime(predict_julian_date_t JulTim){
	time_t timeval;
	timeval = predict_from_julian(JulTim);

	return timeval;
}
struct tm next_pass_aos_sat(time_t time11)
{
	struct tm  ts;


	// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
	ts = *localtime(&time11);
	//strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
	return ts;
}
void print_next_pass_aos(struct tm  ts )
{	char       buf[80];
strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
printf(buf);

}
struct tm sat_next_aos_pass(float Lat , float lon ,float lat, predict_orbital_elements_t *iss)
{	struct tm  ts;
char       buf[80];
predict_observer_t *obs = predict_create_observer("Me", Lat*M_PI/180.0, lon*M_PI/180.0, lat);
struct predict_position iss_orbit;
predict_orbit(iss, &iss_orbit, curr_time);
struct predict_observation aos_obs ;
aos_obs = predict_next_aos(obs, iss, curr_time);
time_t time11 = predict_from_julian(aos_obs.time);

ts = *localtime(&time11);
strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

return ts ;
}
struct tm sat_next_los_pass(float Lat , float lon ,float lat, predict_orbital_elements_t *iss)
{	struct tm  ts;
char       buf[80];
predict_observer_t *obs = predict_create_observer("Me", Lat*M_PI/180.0, lon*M_PI/180.0, 3);
struct predict_position iss_orbit;
predict_orbit(iss, &iss_orbit, curr_time);
struct predict_observation los_obs ;
los_obs = predict_next_los(obs, iss, curr_time);
time_t time11 = predict_from_julian(los_obs.time);
//ts = next_pass_aos_sat(time11);
//strftime(buff, strlen(buff), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
//print_next_pass_aos(ts );
//	return ts ;
ts = *localtime(&time11);
strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
//printf("%s\n", buf);
return ts ;
}
void Prediction_task (void const* pvParameters)
{
	int a = 0  ;
	//	struct tm  ts;
	char msgec[100]={0};
	char msgec1[100]={0};
//	fresult = f_open(&fil, "/telnet/TLE1.txt",FA_READ | FA_OPEN_ALWAYS);
//	fresult = f_read(&fil, msgec ,100,(UINT*) &br);
//	fresult = f_close(&fil);
	const char *tle_line_1 = msgec ;
//	fresult = f_open(&fil, "/telnet/TLE2.txt",FA_READ | FA_OPEN_ALWAYS);
//	fresult = f_read(&fil, msgec1 ,100,(UINT*) &br);
//	fresult = f_close(&fil);
	const char *tle_line_2 = msgec1 ;
	predict_orbital_elements_t *iss = predict_parse_tle(tle_line_1, tle_line_2);
	predict_observer_t *obs = predict_create_observer("Me", 36.8330*M_PI/180.0, 10.2330*M_PI/180.0, 3);
	get_julian_date() ;
	predict_orbit(iss, &iss_orbit, curr_time);

	aos_obs = predict_next_aos(obs, iss, curr_time);
//	los_obs = predict_next_los(obs, iss, curr_time);
	time11 = predict_from_julian(aos_obs.time);
	localtime_r(&time11, &lt);
//	xQueueReceive( Qpredict  , &a , 0 );
	//set_Alarm();
	set_Alarm(lt);
	for(;;)
	{
		get_julian_date() ;
//		xQueueReceive(Qpredict, &a , 0 );
		if(a==1)
		{
			get_julian_date() ;
			iss = predict_parse_tle(tle_line_1, tle_line_2);
			aos_obs = predict_next_aos(obs, iss, curr_time);
//			los_obs = predict_next_los(obs, iss, curr_time);
			time11 = predict_from_julian(aos_obs.time);
			localtime_r(&time11, &lt);
			set_Alarm(lt);
			a= 0 ;
		}
		if (a==2)
		{
			get_julian_date() ;
			predict_destroy_observer(obs);
			obs = predict_create_observer("Me", Location.latitude*M_PI/180.0,Location.longitude*M_PI/180.0, Location.altitude );
//			los_obs = predict_next_los(obs, iss, curr_time);
			aos_obs = predict_next_aos(obs, iss, curr_time);
			time11 = predict_from_julian(aos_obs.time);
			localtime_r(&time11, &lt);
			//set_Alarm();
			set_Alarm(lt);
			a= 0 ;
		}
		if (a==3)
		{
			get_julian_date() ;
			aos_obs = predict_next_aos(obs, iss, curr_time);
//			los_obs = predict_next_los(obs, iss, curr_time);
			time11 = predict_from_julian(aos_obs.time);
//			osDelay(1);
			localtime_r(&time11, &lt);
			//set_Alarm();
			set_Alarm(lt);
			a= 0 ;
		}
		if (a==4)
		{
//			osDelay(30000);
			get_julian_date() ;
//			curr_time += 0.01 ;
			aos_obs = predict_next_aos(obs, iss, curr_time);
//			los_obs = predict_next_los(obs, iss, curr_time);
			osDelay(1);
			time11 = predict_from_julian(aos_obs.time);
			osDelay(1);
//			if (aos_obs.time < predict_next_los(obs, iss, curr_time).time )
//				aos_obs = predict_next_aos(obs, iss, curr_time);
			time11 = predict_from_julian(aos_obs.time);
			osDelay(1);
			localtime_r(&time11, &lt);
//			//set_Alarm();
			set_Alarm(lt);
			a= 0 ;
		}
		predict_orbit(iss, &iss_orbit, curr_time);
		osDelay(1);
	}
}
