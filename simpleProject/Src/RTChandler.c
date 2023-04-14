/*
 * RTChandler.c
 *
 *  Created on: Jun 6, 2022
 *      Author: eloch
 */
#include "RTChandler.h"
#include "IHMMGR.h"
#include "predictsat.h"
#include "gps.h"
#include "freertos.h"
#include "cmsis_os.h"

void set_Alarm (struct tm lt)
{
	RTC_AlarmTypeDef qsAlarm = {0};
	if(lt.tm_min + 15 > 60)
	{
		if (lt.tm_hour + 1 > 24)
			{qsAlarm.AlarmDateWeekDay = lt.tm_mday+1 ;
		qsAlarm.AlarmTime.Hours = lt.tm_hour - 23  ;}
		else
			{qsAlarm.AlarmDateWeekDay = lt.tm_mday ;
		qsAlarm.AlarmTime.Hours = lt.tm_hour + 1 ;}
		qsAlarm.AlarmTime.Minutes = lt.tm_min - 45 ;
	}
	else
	{
	qsAlarm.AlarmDateWeekDay = lt.tm_mday ;
	qsAlarm.AlarmTime.Hours = lt.tm_hour ;
	qsAlarm.AlarmTime.Minutes = lt.tm_min + 15 ;
	}
//	qsAlarm.AlarmTime.Minutes = lt.tm_min ;
	qsAlarm.AlarmTime.Seconds = lt.tm_sec  ;
	qsAlarm.AlarmTime.SubSeconds = 0 ;
	qsAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	qsAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	qsAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	qsAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	qsAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	qsAlarm.Alarm = RTC_ALARM_B;
	HAL_RTC_SetAlarm_IT(&hrtc, &qsAlarm, RTC_FORMAT_BIN);
	if (HAL_RTC_SetAlarm_IT(&hrtc, &qsAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}


}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	int a = 0 ;
	a= 4 ;

//	xQueueGenericSend(Qpredict,&a ,0,queueSEND_TO_BACK ) ;
//	xQueueGenericSendFromISR(Qpredict, &a, 0, queueSEND_TO_BACK);
	//void E22_Send(uint16_t address, uint8_t channel, uint8_t *buf ) ;
}
