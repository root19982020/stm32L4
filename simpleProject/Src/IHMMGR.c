/*
 * IHMMGR.c
 *
 *  Created on: 11 avr. 2022
 *      Author: eloch
 */

#define _SVID_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "IHMMGR.h"
#include "tim.h"
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
#include "predictsat.h"
#include <time.h>


const char TXPSETHELP[]="TXP SET HELP MSG\nTXP VALUES RANGE [00..20]dbm";
const char SFSETHELP[]="SF SET HELP MSG\nSF VALUES RANGE [8..12]";
const char CRCSETHELP[]="CRC SET HELP MSG\nCRC VALUES IS BOOLEAN TYPE 0 for FALSE and 1 for TRUE";
const char BWDSETHELP[]="BANDWIDTH SET HELP MSG\nBANDWISTH VALUES RANGE [000.000..500.000]KHz";
const char PAYSETHELP[]="PAYLOAD SET HELP MSG\nPAYLOAD LENTH [000..255]";
const char PRESETHELP[]="PREAMBLE SET HELP MSG\nPREAMBLE LENTH [10..65535]";
const char CRSETHELP[]="CRC SET HELP MSG\nCRC VALUES IS BOOLEAN TYPE 0 for FALSE and 1 for TRUE";
const char FCSETHELP[]="FREAQUENCY SET HELP MSG\nFREQUENCY VALUES RANGE [000.000..915.000]MHz";
const char TIMEDATESETHELP[]="TIME and DATE SET HELP MSG\nTIME and date format YYYY/MT/DD/HH/MM/SS/MS";
const char IDSETHELP[]="ID SET HELP MSG\nID format XXXXXXXXXXXXXXXX (16) ";
const char TLESETHELP[]="TWO LINE ELEMENT (TLE) SET HELP MSG\nID format XXXXXXXX(line1)\nXXXXXXXX(line2) ";
const char LOCATIONSETHELP[]="LOCATION SET HELP\nLOCATION FORMAT LANGITUDE,LATITUDE,ALTITUDE";
const char STATUSHELP[]="SYSTEM STATUS HELP\nGET SYSTEM STATUS BATTERY HEALTH , LORA CONNECTION , SD CARD STATUS";
const char GTXPSETHELP[]="TXP GET HELP MSG\nTXP VALUES RANGE [00..20]dbm";
const char GSFSETHELP[]="SF GET HELP MSG\nSF VALUES RANGE [8..12]";
const char GCRCSETHELP[]="CRC GET HELP MSG\nCRC VALUES IS BOOLEAN, 0 for FALSE and 1 for TRUE";
const char GBWDSETHELP[]="BANDWIDTH GET HELP MSG\nBANDWISTH VALUES RANGE [000.000..500.000]KHz";
const char GPAYSETHELP[]="PAYLOAD GET HELP MSG\nPAYLOAD LENTH [000..255]";
const char GPRESETHELP[]="PREAMBLE GET HELP MSG\nPREAMBLE LENTH [10..65535]";
const char GCRSETHELP[]="CR GET HELP MSG\nCRC VALUES IS BOOLEAN, 0 for FALSE and 1 for TRUE";
const char GFCSETHELP[]="FREAQUENCY GET HELP MSG\nFREQUENCY VALUES RANGE [000.000..915.000]MHz";
const char GTIMEDATESETHELP[]="TIME and DATE GET HELP MSG\nTIME and date format YYYY/MT/DD/HH/MM/SS/MS";
const char GIDSETHELP[]="ID SET HELP MSG\nID format XXXXXXXXXXXXXXXX (16) ";
const char GLOCATIONSETHELP[]="LOCATION GET HELP\nLOCATION FORMAT LANGITUDE,LATITUDE,ALTITUDE";
const char SECHHELP[]="Set periode d'echantill (secondes)";
const char GECHHELP[]="Get periode d'echantill (secondes)";
const char PredictionHELP[]="\r\n********** WELCOME TO PREDICTION MENU **********\r\n\r\n\r\nCMD LIST\r\nSETTIMEDATE : TIME and date YYMTDDHHMMSS\r\nGETTIMEDATE : HH:MM:SS DD/MT/YYYY\r\nSETLOCATION : LOCATION FORMAT ALTITUDE,LATITUDE,LANGITUDE\r\nGETLOCATION : ALT: // LAT:// LON:\r\nGETSATINFO : GET SATELLITE INFO\r\nSETTLE1 : SET TLE LIGNE 1\r\nSETTLE2 : SET TLE LIGNE 2\r\nGETTLE1 :  GET TLE LINE 1\r\nGETTLE2 : GET TLE LINE 2\r\nGETNEXTPASS : GET NEXT APPEARANCE OF SIGNAL\r\n\r\n";
const char LORAHELP[]="\r\n********** WELCOME TO LoRa MENU **********\r\n\r\n\r\nCMD LIST\r\n\r\nSETSF : SET Spreding Factor \r\nSETTXP : SET TRANSMITION POWER VALUE BETWEEN 0 AND 30 dBm\r\nSETCRC : TAKE 0 OR 1 AS ARGMS\r\nSETBWD : SET BANDWIDTH VALUE(Hz)\r\nSETFC : SET CENTRAL FREQUENCY VALUE(MHz)\r\nSETCR : SET CODING RATE VALUE(BOOL)\r\nSETPAY : SET PAYLOAD VALUE\r\nSETPRE : SET PREAMBLE \r\nSETID : SET DEVICE ID\r\nGETSF : GET Spreding Factor VALUE\r\nGETTXP : GET TRANSMITION POWER\r\nGETCRC : GET CRC ON/OFF\r\nGETBWD : GET BANDWIDTH VALUE(Hz)\r\nGETFC : GET CENTRAL FREQUENCY VALUE(MHz)\r\nGETCR : GET CODING RATE VALUE\r\nGETPAY : GET PAYLOAD VALUE\r\nGETPRE : GET PREAMBLE \r\nGETID : SET DEVICE ID\r\n\r\n";
const char SYSHELP[]="\r\n********** WELCOME TO SYSTEME MENU **********\r\n\r\nCMD LIST\r\n\r\nDATATRANS : GET DATA TRANSMITTED \r\nDATATRAIT : GET DATA TRAITED \r\nDATAACQ : GET DATA ACQUIRED\r\n\r\n";
const char CAPHELP[]="\r\n********** WELCOME TO CAPTEURS MENU **********\r\n\r\n\r\nCMD LIST\r\n\r\nSETECH : SET eECHANT PERIODE(min)\r\nGETECH : GET ECHANT PERIODE(min)\r\nGETCO2 : GET CO2 value(PPM)\r\nGETDEBIT : GET DEBIT VAL(L/min)\r\nGETPLU : GET RAIN VAL(mm/m²)\r\nGETHUMSOL : GET SOL MOISTURE(%)\r\nGETVVENT : GET WIND SPEED (km/h)\r\nGETHUM : GET HUM VAL(%)\r\nGETTEMP : GET TEMPRETURE(°C)\r\nGETRAY : GET IRRADIATION (W/m²)\r\nGETPH : GET WATER PH\r\n\r\n";
const char SATPOS[]="corrent sat info ";
const char TLESET[]="TLE is SET into COrrected file TLE.txt ";
const char CO2_GET[]= "";
const char DEBIT_GET[]= "";
const char PLU_GET[]= "";
const char HUMSOL_GET[]= "";
const char VVENT_GET[]="" ;
const char HUM_GET[]= "";
const char TEMP_GET[]= "";
const char RAY_GET[]= "";
const char TLEGET2[]="";
const char TLEGET1[]="";
const char GETNEXTPASSHELP[]="";
const char ACQHELP[]="";
const char TRAITHELP[]="";
const char TRANSHELP[]="";
const char PH_GET[]="";


extern RTC_TimeTypeDef sTime ;
extern RTC_DateTypeDef sDate ;
int echant ;
char RxChar;
char RxBuffer[MAX_COMMAND_LENGTH]={0};


bool CMDReceived=false;

Status_t stat ;

#define TXP_min 0
#define TXP_max 31
#define BWD_min 0
#define BWD_max 500000
#define Fc_min 0
#define Fc_max 915000
#define PAYLOAD_min 0
#define PAYLOAD_max 255
#define SF_min 7
#define SF_max 12
#define Preamble_min 10
#define Preamble_max 65535
#define year_min 0
#define year_max 99
#define month_min 1
#define month_max 12
#define date_min 1
#define date_max 31
#define hours_min 0
#define hours_max 23
#define minutes_min 0
#define minutes_max 59
#define secondes_min 0
#define secondes_max 59
#define Msecondes_min 0
#define Msecondes_max 99
#define CR_min 4
#define CR_max 8

void TXP_set(char* args){



	int val = atoi(args);
	if((val>=TXP_min) && (val<=TXP_max))
	{RF1.TXP= val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"TXP SET\r\n", 9,20);}
}
void SF_SET(char* args){
	int val = atoi(args);
	if((val >= SF_min) && (val<=SF_max))
	{RF1.SF= val ;
	HAL_UART_Transmit(&huart2,(uint8_t *)"SF SET\r\n", 8,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
}
void CRC_SET(char* args){
	int i= atoi(args);
	bool val;
	if ((i==0)||(i==1))
	{	val = i ;
	if(val)
	{RF1.CRCO = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"CRC SET\r\n", 10,20);}
	else
	{HAL_UART_Transmit(&huart2,(uint8_t*)"CRC RSET\r\n", 12,20);}
	}
	else
	{
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
	}

}
void BWD_SET(char* args){
	float val = atof(args);
	if((val >= BWD_min) && (val<= BWD_max))
	{RF1.bandwidth = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"BANDWIDTH SET\r\n", 15,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
}
void FC_SET(char* args){
	int val = atoi(args);
	if((val >= Fc_min) && (val<= Fc_max))
	{RF1.Fc = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"frequency SET\r\n", 16,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
}
void CR_SET(char* args){
	int val = atoi(args);
	if((val>=CR_min)&&(val<=CR_max))
	{RF1.CR = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"CR SET\r\n", 8,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
}
void PRE_SET(char* args){
	int val = atoi(args);
	if((val>=Preamble_min) && (val<=Preamble_max))
	{RF1.PReamble = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"PREAMBLE SET\r\n", 15,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);

}
void PAY_SET(char* args){
	int val = atoi(args);
	if((val>=PAYLOAD_min) && (val<=PAYLOAD_max))
	{RF1.Payload = val ;
	HAL_UART_Transmit(&huart2,(uint8_t*)"PAYLOAD SET\r\n", 14,20);}
	else
		HAL_UART_Transmit(&huart2,(uint8_t*)"SORRY WRONG INPUT\r\n",20,20);
}
void TIMEDATE_SET(char* args){
	int a = 0 ;
	RTC_TimeTypeDef timeRTC = {0};
	RTC_DateTypeDef dateRTC = {0};
	char chTD[12] ;
	int years , month , date , hours , minutes , secondes ;
	if (strlen(args) == 12)
	{memcpy(chTD,args,strlen(args));
	char* Cyearsx = substringfunc(chTD, 0 , 2);
	char* Cmonth = substringfunc(chTD, 2 , 4);
	char* Cdate = substringfunc(chTD, 4 , 6);
	char* Chours = substringfunc(chTD, 6 , 8);
	char* Cminutes = substringfunc(chTD, 8 , 10);
	char* Csecondes = substringfunc(chTD, 10 , 12);
	years = atoi(Cyearsx);
	month = atoi(Cmonth);
	date = atoi(Cdate);
	hours = atoi(Chours);
	minutes = atoi(Cminutes);
	secondes = atoi(Csecondes);
	if ((hours>=hours_min)&&(hours<=hours_max))
		timeRTC.Hours = hours ;
	if ((minutes>=minutes_min)&&(minutes<=minutes_max))
		timeRTC.Minutes = minutes ;
	if ((secondes>=secondes_min)&&(secondes<=secondes_max))
		timeRTC.Seconds = secondes ;
	if ((years>=year_min)&&(years<=year_max))
		dateRTC.Year = years ;
	if ((month>=month_min)&&(month<=month_max))
		dateRTC.Month = month ;
	if ((date>=date_min)&&(date<=date_max))
		dateRTC.Date = date ;
	dateRTC.WeekDay = 0 ;
	HAL_RTC_SetTime(&hrtc, &timeRTC, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&dateRTC, RTC_FORMAT_BIN) ;
	curr_timeIHM = GetCurrentJuilanTime() ;
	time_t time1122 = predict_from_julian(curr_timeIHM);
//	lt22 = *localtime(&time1122);
	localtime_r(&time1122, &lt22);
	timeRTC.Hours = lt22.tm_hour;
	timeRTC.Minutes = lt22.tm_min ;
	timeRTC.Seconds = lt22.tm_sec ;
	dateRTC.Year = lt22.tm_year-100 ;
	dateRTC.Month = lt22.tm_mon+1 ;
	dateRTC.Date = lt22.tm_mday ;
	dateRTC.WeekDay = lt22.tm_wday ;
	HAL_RTC_SetTime(&hrtc, &timeRTC, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&dateRTC, RTC_FORMAT_BIN) ;
	HAL_UART_Transmit(&huart2,(uint8_t *)"time and date SET\r\n",20,40);
	a = 3 ;
	xQueueGenericSendFromISR(Qpredict, &a, 0 , queueSEND_TO_BACK );}
	else
		HAL_UART_Transmit(&huart2,(uint8_t *)"SORRY WRONG INPUT\r\n",20,40);
}
void ID_SET(char* args){
	char ch[20]={0} ;
	int i , l=0 ;
	long int result1 ;
	if (strlen(args)==8)
	{
		memcpy(ch,args,strlen(args));
		for (i=0; i<= 8 ; i++)
		{
			if( isxdigit((int)ch[i]) )
				l++ ;

		}

		if (l==8)
		{

			result1=(int)strtol(ch, NULL, 16);
			ID= result1 ;

		}
		else
			HAL_UART_Transmit(&huart2,(uint8_t*)"WRONG ID\r\n",12,40);

	}
	else
		HAL_UART_Transmit(&huart2,(uint8_t *)"ID SIZE MUST BE 8\r\n",20,40);
}

void LOCATION_SET (char* args){
	int a = 0 ;
	char* tok;
	tok = strtok_r(args, "," ,&args);
	Location.altitude = atof(tok);
	tok = strtok_r(args, "," ,&args);
	Location.latitude= atof(tok);
	Location.longitude= atof(args);
	HAL_UART_Transmit(&huart2,(uint8_t *)"Location SET\r\n",15,40);
	a = 2 ;
	xQueueGenericSend(Qpredict, &a, 0 , queueSEND_TO_BACK );
}
void TXP_GET(){
	char TXP[3]={0} ;
	sprintf(TXP, "%d", RF1.TXP);
	HAL_UART_Transmit(&huart2, (uint8_t*) TXP, strlen(TXP), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void SF_GET(){
	char SF_s[6]={0} ;
	sprintf(SF_s, "%d", RF1.SF);
	HAL_UART_Transmit(&huart2, (uint8_t*) SF_s, strlen(SF_s), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void CRC_GET(){
	char CRCOO[6]={0} ;
	sprintf(CRCOO, "%d", RF1.CRCO);
	HAL_UART_Transmit(&huart2, (uint8_t*) CRCOO, strlen(CRCOO), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void BWD_GET(){
	char bandwidth[12]={0} ;
	sprintf(bandwidth, "%d", RF1.bandwidth);
	HAL_UART_Transmit(&huart2, (uint8_t*) bandwidth, strlen(bandwidth), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void FC_GET(){
	char FC[9]={0} ;
	sprintf(FC, "%d", RF1.Fc);
	HAL_UART_Transmit(&huart2, (uint8_t*) FC, strlen(FC), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void CR_GET(){
	char CR[6]={0} ;
	sprintf(CR, "%d", RF1.CR);
	HAL_UART_Transmit(&huart2, (uint8_t *) "4/", 2, 10);
	HAL_UART_Transmit(&huart2, (uint8_t*) CR, strlen(CR), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}

void PRE_GET(){
	char Preamble[9]={0} ;
	sprintf(Preamble, "%d", RF1.PReamble);
	HAL_UART_Transmit(&huart2, (uint8_t*) Preamble, strlen(Preamble), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void PAY_GET(){
	char Payload[5]={0} ;
	sprintf(Payload, "%d", RF1.Payload);
	HAL_UART_Transmit(&huart2, (uint8_t*) Payload, strlen(Payload), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void TIMEDATE_GET(){
	char msg[50]={0};
	RTC_TimeTypeDef sTimeRTC= {0};
	RTC_DateTypeDef sDateRTC = {0};

	HAL_RTC_GetTime(&hrtc, &sTimeRTC, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDateRTC, RTC_FORMAT_BIN);

	sprintf(msg, "%02d:%02d:%02d %02d/%02d/%04d \r\n", sTimeRTC.Hours, sTimeRTC.Minutes, sTimeRTC.Seconds, sDateRTC.Date, sDateRTC.Month, sDateRTC.Year+2000 );
	HAL_UART_Transmit(&huart2,(uint8_t*) msg, strlen(msg), 30);

}
void ID_GET(){
	char ch1[12]={0} ;
	sprintf(ch1, "%x",ID);
	HAL_UART_Transmit(&huart2,(uint8_t*) ch1, strlen(ch1), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);
}
void get_sat_info ()
{
	char bu1[30]={0} ,bu2[20]={0}  ,bu3[20]={0}, bu4[20]={0} ,bu5[20]={0} ,bu6[20]={0} , bu7[20]={0} ,bu8[20]={0}  ,bu9[20]={0}, bu10[20]={0} ,bu11[20]={0} ,bu12[20]={0};
	printfloat(iss_orbit.altitude , 8 , bu1 );
	printfloat(iss_orbit.latitude , 8 , bu2 );
	printfloat(iss_orbit.longitude , 8 , bu3 );
	printfloat(iss_orbit.eclipse_depth , 8 , bu4 );
	printfloat(iss_orbit.footprint , 8 , bu5 );
	printfloat(iss_orbit.argument_of_perigee , 8 , bu6 );
	printfloat(iss_orbit.inclination , 8 , bu7 );
	printfloat(iss_orbit.phase , 8 , bu8 );
	//	orbit_velocity = (int) iss_orbit.velocity;
	//	orbit_velocity1 = (iss_orbit.velocity - orbit_velocity )*100000000 ;
	//	printfloat(iss_orbit.velocity , 8 , bu9 );

	printfloat(iss_orbit.decayed , 1 , bu9 );
	printfloat(iss_orbit.revolutions , 1 , bu11 );
	printfloat(iss_orbit.right_ascension , 8 , bu10 );
	printfloat(iss_orbit.eclipsed , 8 , bu12 );
	HAL_UART_Transmit(&huart2,(uint8_t *) "Argument of perigee :", strlen("Argument of perigee :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu6, strlen(bu6), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "Decayed :", strlen("Decayed :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu9, strlen(bu9), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "Eclipsed :", strlen("Eclipsed :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu12, strlen(bu12), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "Revolutions :", strlen("Revolutions :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu11, strlen(bu11), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "ALT :", strlen("ALT :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu1, strlen(bu1), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "LAT :", strlen("LAT :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu2, strlen(bu2), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "LON :", strlen("LON :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu3, strlen(bu3), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "DEPTH :", strlen("DEPTH :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu4, strlen(bu4), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "FOOTPRINT :", strlen("FOOTPRINT :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu5, strlen(bu5), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "INCLINATION :", strlen("INCLINATION :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu7, strlen(bu7), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "PHASE :", strlen("PHASE :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu8, strlen(bu8), 120);
	//	sprintf(bu12,"VELOCITY : %d.%d\r\n",orbit_velocity,orbit_velocity1);
	//	HAL_UART_Transmit(&huart2,(uint8_t *) bu12, strlen(bu12), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) "RIGHT ASCENSION :", strlen("RIGHT ASCENSION :"), 120);
	HAL_UART_Transmit(&huart2,(uint8_t *) bu10, strlen(bu10), 120);
}
void getNEXTPASS ()
{
//	char buffff[20]={0};
	//struct tm  ts ;
//	ts = *localtime(time11) ;
//	strftime(buffff, sizeof(buffff), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
	volatile char ahmed[30]={0};
	localtime_r(&time11, &lt);
//	sprintf(ahmed , "%ld" , time11);
	strftime(ahmed, sizeof(ahmed), "%a %Y-%m-%d %H:%M:%S \r\n", &lt);
//	sprintf(buffff , "%ld\r\n" , time11);
	HAL_UART_Transmit(&huart2,(uint8_t *) ahmed, strlen(ahmed), 120);
}
void STATUS_fun(char* args){
	int x=0 ;
	if (stat.Intern_temp < 50)
		x=1 ;
	if(!stat.LoRa)
		x=2;
	if (stat.SD)
		x=3;
	if(stat.battery<20)
		x=4;
	if (!stat.capt_cnx)
		x=5;
	HAL_UART_Transmit(&huart2, (uint8_t*) "code error \n1:Intern temp\n2:LoRa\n3:SD\n4:battery low level\n5:capteur connectivity\n" , sizeof("code error \n1:Intern temp\n2:LoRa\n3:SD\n4:battery low level\n5:capteur connectivity\n"), 60);
	HAL_UART_Transmit(&huart2, (uint8_t*) x , sizeof(x), 40);
	HAL_UART_Transmit(&huart2, (uint8_t *) "\r\n", 2, 10);

}
void LOCATION_GET(){
	char ALT[20]={0} , LAT[20]={0} ,LON[20]={0} ;
	printfloat(Location.altitude , 8 , ALT);
	printfloat(Location.longitude , 8 , LON);
	printfloat(Location.latitude , 8 , LAT);
	HAL_UART_Transmit(&huart2, (uint8_t *) "ALT :", 5 , 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) ALT, strlen(ALT), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "LAT :", 5 , 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) LAT, strlen(LAT), 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) "LON :", 5 , 30);
	HAL_UART_Transmit(&huart2, (uint8_t *) LON, strlen(LON), 30);
}
void UnknownCMDHandler(){
	HAL_UART_Transmit(&huart2,(uint8_t*)"CMD invalide\r\n", sizeof("CMD invalide\r\n"), 30);

}


Command_t AllCommandsList[MAX_COMMAND_NUMBER]={{"SETTXP",TXPSETHELP,TXP_set},{"SETSF",SFSETHELP,SF_SET},{"SETCRC",CRCSETHELP,CRC_SET},{"SETBWD",BWDSETHELP,BWD_SET},{"SETPAY",PAYSETHELP,PAY_SET},{"SETPRE",PRESETHELP,PRE_SET},{"SETCR",CRSETHELP,CR_SET},{"SETFC",FCSETHELP,FC_SET},{"SETTIMEDATE",TIMEDATESETHELP,TIMEDATE_SET},{"SETID",IDSETHELP,ID_SET},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET},{"STATUS",STATUSHELP,STATUS_fun},{"GETTXP",GTXPSETHELP,TXP_GET},{"GETSF",GSFSETHELP,SF_GET},{"GETCRC",GCRCSETHELP,CRC_GET},{"GETBWD",GBWDSETHELP,BWD_GET},{"GETPAY",GPAYSETHELP,PAY_GET},{"GETPRE",GPRESETHELP,PRE_GET},{"GETCR",GCRSETHELP,CR_GET},{"GETFC",GFCSETHELP,FC_GET},{"GETTIMEDATE",GTIMEDATESETHELP,TIMEDATE_GET},{"GETID",GIDSETHELP,ID_GET},{"GETLOCATION",GLOCATIONSETHELP,LOCATION_GET},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET}};
/* Menu */
Command_t CAPCMDList[MAX_COMMAND_NUMBER]={};
Command_t SYSCMDList[MAX_COMMAND_NUMBER]={};
Command_t LoRaCMDList[MAX_COMMAND_NUMBER]={{"SETTXP",TXPSETHELP,TXP_set},{"SETSF",SFSETHELP,SF_SET},{"SETCRC",CRCSETHELP,CRC_SET},{"SETBWD",BWDSETHELP,BWD_SET},{"SETPAY",PAYSETHELP,PAY_SET},{"SETPRE",PRESETHELP,PRE_SET},{"SETCR",CRSETHELP,CR_SET},{"SETFC",FCSETHELP,FC_SET},{"SETID",IDSETHELP,ID_SET}};
Command_t PREDCMDList[MAX_COMMAND_NUMBER]={{"SETTIMEDATE",TIMEDATESETHELP,TIMEDATE_SET},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET},{"GETLOCATION",GLOCATIONSETHELP,LOCATION_GET}};
//Command_t AllCommandsList[MAX_COMMAND_NUMBER]={{"SETTXP",TXPSETHELP,TXP_set},{"SETSF",SFSETHELP,SF_SET},{"SETCRC",CRCSETHELP,CRC_SET},{"SETBWD",BWDSETHELP,BWD_SET},{"SETPAY",PAYSETHELP,PAY_SET},{"SETPRE",PRESETHELP,PRE_SET},{"SETCR",CRSETHELP,CR_SET},{"SETFC",FCSETHELP,FC_SET},{"SETTIMEDATE",TIMEDATESETHELP,TIMEDATE_SET},{"SETID",IDSETHELP,ID_SET},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET},{"STATUS",STATUSHELP,STATUS_fun},{"GETTXP",GTXPSETHELP,TXP_GET},{"GETSF",GSFSETHELP,SF_GET},{"GETCRC",GCRCSETHELP,CRC_GET},{"GETBWD",GBWDSETHELP,BWD_GET},{"GETPAY",GPAYSETHELP,PAY_GET},{"GETPRE",GPRESETHELP,PRE_GET},{"GETCR",GCRSETHELP,CR_GET},{"GETFC",GFCSETHELP,FC_GET},{"GETTIMEDATE",GTIMEDATESETHELP,TIMEDATE_GET},{"GETID",GIDSETHELP,ID_GET},{"GETLOCATION",GLOCATIONSETHELP,LOCATION_GET},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET},{"SETECH",SECHHELP,ECH_SET},{"GETECH",GECHHELP,ECH_GET}};
Menu_t AllMenuList[MAX_MENU_NUMBER]={{"PREDICT",PredictionHELP,{{"SETTIMEDATE",TIMEDATESETHELP,TIMEDATE_SET},{"GETTIMEDATE",GTIMEDATESETHELP,TIMEDATE_GET},{"GETNEXTPASS",GETNEXTPASSHELP,getNEXTPASS},{"SETLOCATION",LOCATIONSETHELP,LOCATION_SET},{"GETLOCATION",GLOCATIONSETHELP,LOCATION_GET},{"GETSATINFO",SATPOS,get_sat_info}}},{"LORA",LORAHELP,{{"GETID",GIDSETHELP,ID_GET},{"SETTXP",TXPSETHELP,TXP_set},{"SETSF",SFSETHELP,SF_SET},{"SETCRC",CRCSETHELP,CRC_SET},{"SETBWD",BWDSETHELP,BWD_SET},{"SETPAY",PAYSETHELP,PAY_SET},{"SETPRE",PRESETHELP,PRE_SET},{"SETCR",CRSETHELP,CR_SET},{"SETFC",FCSETHELP,FC_SET},{"SETID",IDSETHELP,ID_SET},{"GETTXP",GTXPSETHELP,TXP_GET},{"GETSF",GSFSETHELP,SF_GET},{"GETCRC",GCRCSETHELP,CRC_GET},{"GETBWD",GBWDSETHELP,BWD_GET},{"GETPAY",GPAYSETHELP,PAY_GET},{"GETPRE",GPRESETHELP,PRE_GET},{"GETCR",GCRSETHELP,CR_GET},{"GETFC",GFCSETHELP,FC_GET}}},{"SYS",SYSHELP,{}},{"CAP",CAPHELP,{}}};

int ParseCMD(char * CMD,char *Args,Command_t *commandlist)
{
	uint8_t counter;
	char *cmdName;

	cmdName=strtok_r(CMD," ",&CMD);
	memcpy(Args,CMD,strlen(CMD));

	for(counter=0;counter<MAX_COMMAND_NUMBER;counter++)
		if(!strcmp(cmdName,commandlist[counter].name))
			return counter;

	return -1;

}

void ProcessCMD(Command_t *commandlist,char* Args, int index)
{
	if(index<0)
		UnknownCMDHandler();
	else
		if(commandlist[index].hdl!=NULL)
			commandlist[index].hdl(Args);
	memset(Args, '\0', strlen(Args));
}
int ParseMenu(char* CMD , Menu_t *MenuList  )
{
	uint8_t counter;
	char *MenuName;

	MenuName=strtok_r(CMD,"}",&CMD);

	for(counter=0;counter<MAX_MENU_NUMBER;counter++)
		if(!strcmp(MenuName,MenuList[counter].name))
			return counter;
	return -1;
}
void IHM_task(void* pvParameters)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)"************ WELCOME TO AGRICULTURE LoRa NODE ************\r\nPredict : GET INTO PREDICTION MENU\r\nLoRa : GET INTO LoRa MENU\r\nSys : GET INTO SYSTEME MENU\r\nCAP : GET INTO CAPTEURS MENU\r\n\r\n\r\n", strlen("************ WELCOME TO AGRICULTURE LoRa NODE ************\r\nPredict : GET INTO PREDICTION MENU\r\nLoRa : GET INTO LoRa MENU\r\nSys : GET INTO SYSTEME MENU\r\nCAP : GET INTO CAPTEURS MENU\r\n\r\n\r\n"), 120);
	int CMDIndex=-1 ;
	char RetArgs[MAX_COMMAND_LENGTH];
	int MenuIndex=-1 ;
	int indexIHM  ;

	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{

		if(CMDReceived){

			MenuIndex  = ParseMenu(RxBuffer , AllMenuList );
			//Print Menu CMD and wait to parse cmd
			if (MenuIndex != -1)
			{
				indexIHM = MenuIndex ;
				HAL_UART_Transmit(&huart2, (uint8_t *)AllMenuList[indexIHM].helper, strlen(AllMenuList[indexIHM].helper), 160);
				HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 60);
			}
			else if (strcmp(RxBuffer,"00"))
			{

				CMDIndex =  ParseCMD(RxBuffer,RetArgs,AllMenuList[indexIHM].CMD_list) ;
				ProcessCMD(AllMenuList[indexIHM].CMD_list,RetArgs, CMDIndex) ;
			}
			if (!strcmp(RxBuffer,"00"))
			{indexIHM=-1;
			HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"*****************************", strlen("*****************************"), 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"       OUT OF ", 14, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"MENU", 4, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, 60);
			HAL_UART_Transmit(&huart2, (uint8_t *)"************ WELCOME TO AGRICULTURE LoRa NODE ************\r\nPredict : GET INTO PREDICTION MENU\r\nLoRa : GET INTO LoRa MENU\r\nSys : GET INTO SYSTEME MENU\r\nCAP : GET INTO CAPTEURS MENU\r\n\r\n\r\n", strlen("************ WELCOME TO AGRICULTURE LoRa NODE ************\r\nPredict : GET INTO PREDICTION MENU\r\nLoRa : GET INTO LoRa MENU\r\nSys : GET INTO SYSTEME MENU\r\nCAP : GET INTO CAPTEURS MENU\r\n\r\n\r\n"), 120);

			}

			memset(RxBuffer,0,sizeof(RxBuffer));
			CMDReceived=false;
		}
		osDelay(1);

	}
}
void printfloat(float alt , int pow10 ,char buff[20])
{
	int ALT1 ,ALT2;
	ALT1 = (int) alt ;
	if (ALT1 <= -1)
	{
		ALT2 = ( alt  - ALT1 )* pow(10, pow10 ) ;
		ALT2 = abs(ALT2);
	}
	else
		ALT2 = ( alt  - ALT1 )* pow(10, pow10 ) ;
	sprintf(buff , "%d.%d\r\n",ALT1,ALT2) ;
}
void printfloat2(float alt , int pow10 ,char buff[20])
{
	int ALT1 ,ALT2;
	ALT1 = (int) alt ;
	if (ALT1 <= -1)
	{
		ALT2 = ( alt  - ALT1 )* pow(10, pow10 ) ;
		ALT2 = abs(ALT2);
	}
	else
		ALT2 = ( alt  - ALT1 )* pow(10, pow10 ) ;
	sprintf(buff , "%d.%d",ALT1,ALT2) ;
}

char* substringfunc(const char *src, int m, int n)
{
	// get the length of the destination string
	int len = n - m;

	// allocate (len + 1) chars for destination (+1 for extra null character)
	char *dest = (char*)pvPortMalloc(sizeof(char) * (len + 1));

	// extracts characters between m'th and n'th index from source string
	// and copy them into the destination string
	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}

	// null-terminate the destination string
	*dest = '\0';

	// return the destination string
	return dest - len;
}
