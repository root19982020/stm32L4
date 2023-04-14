
#include "rtc.h"
#include "time.h"

void set_Alarm (struct tm lt) ;
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc) ;
