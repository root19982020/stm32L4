/*
 * gps.c
 *
 *  Created on: May 23, 2022
 *      Author: eloch
 */
#include <stdint.h>
#include <math.h>
#include "gps.h"
#include <string.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"
const float GPS_INVALID_F_LONGITUDE_ANGLE = 181.0;
const float GPS_INVALID_F_LATITUDE_ANGLE = 91.0;
const float GPS_INVALID_F_ALTITUDE = 1000000.0;
const float GPS_INVALID_F_SPEED = -1.0;
#define _GPRMC_TERM   "GPRMC"
#define _GPGGA_TERM   "GPGGA"

uint32_t millis (void)
{
	return osKernelSysTick();
	//return SysTick_Config(1)
	//return HAL_Delay(1);
}
void GPSOutputInit(GPSOutput_t *myCfg)
{
	myCfg->Altitude =0 ;
	myCfg->Date = 010100;
	myCfg->Time = 0;
	myCfg->Latitude = 0;
	myCfg->Longitude = 0;
	myCfg->course = 0;
	myCfg->Speed = 0;
	myCfg->NumSats = 0;

}
void TinyGPSCreate(TinyGPS_t *gps)
{
	gps->_time = GPS_INVALID_TIME;
	gps->_date = GPS_INVALID_DATE;
	gps->_latitude = GPS_INVALID_ANGLE;
	gps->_longitude = GPS_INVALID_ANGLE;
	gps->_altitude = GPS_INVALID_ALTITUDE;
	gps->_speed = GPS_INVALID_SPEED;
	gps->_course = GPS_INVALID_ANGLE;
	gps->_hdop = GPS_INVALID_HDOP;
	gps->_numsats = GPS_INVALID_SATELLITES;
	gps->_last_time_fix = GPS_INVALID_FIX_TIME;
	gps->_last_position_fix = GPS_INVALID_FIX_TIME;
	gps->_parity = 0;
	gps->_is_checksum_term = false;
	gps->_sentence_type = _GPS_SENTENCE_OTHER;
	gps->_term_number = 0;
	gps->_term_offset = 0;
	gps->_gps_data_good = false;
#ifndef _GPS_NO_STATS
	gps->_encoded_characters = 0;
	gps->_good_sentences = 0;
	gps->_failed_checksum = 0;
#endif
	{
		gps->_term[0] = '\0';
	}
}
//
// public methods
//
float degrees (float value)
{
	return ((value*360)/TWO_PI);
}
float radians(float value)
{
	return ((TWO_PI*value)/360);
}
int library_version(void)
{
	return _GPS_VERSION;
}
unsigned long hdop(TinyGPS_t *gps)
{
	return gps->_hdop;
}
unsigned short satellites(TinyGPS_t *gps)
{
	return gps->_numsats;
}
bool encode(TinyGPS_t *gps, char c)
{
	bool valid_sentence = false;

#ifndef _GPS_NO_STATS
	++gps->_encoded_characters;
#endif
	switch(c)
	{
	case ',': // term terminators
		gps->_parity ^= c;
	case '\r':
	case '\n':
	case '*':
		if (gps->_term_offset < sizeof(gps->_term))
		{
			gps->_term[gps->_term_offset] = 0;
			valid_sentence = term_complete(gps);
		}
		++gps->_term_number;
		gps->_term_offset = 0;
		gps->_is_checksum_term = c == '*';
		return valid_sentence;

	case '$': // sentence begin
		gps->_term_number = gps->_term_offset = 0;
		gps->_parity = 0;
		gps->_sentence_type = _GPS_SENTENCE_OTHER;
		gps->_is_checksum_term = false;
		gps->_gps_data_good = false;
		return valid_sentence;
	}

	// ordinary characters
	if (gps->_term_offset < sizeof(gps->_term) - 1)
		gps->_term[gps->_term_offset++] = c;
	if (!gps->_is_checksum_term)
		gps->_parity ^= c;

	return valid_sentence;
}



#ifndef _GPS_NO_STATS
void stats(TinyGPS_t *gps,unsigned long *chars, unsigned short *sentences, unsigned short *failed_cs)
{
	if (chars) *chars = gps->_encoded_characters;
	if (sentences) *sentences = gps->_good_sentences;
	if (failed_cs) *failed_cs = gps->_failed_checksum;
}
#endif

//
// internal utilities
//
int from_hex(char a)
{
	if (a >= 'A' && a <= 'F')
		return a - 'A' + 10;
	else if (a >= 'a' && a <= 'f')
		return a - 'a' + 10;
	else
		return a - '0';
}

unsigned long parse_decimal(TinyGPS_t *gps)
{
	char *p = gps->_term;
	bool isneg = *p == '-';
	if (isneg) ++p;
	unsigned long ret = 100UL * gpsatol(p);
	while (gpsisdigit(*p)) ++p;
	if (*p == '.')
	{
		if (gpsisdigit(p[1]))
		{
			ret += 10 * (p[1] - '0');
			if (gpsisdigit(p[2]))
				ret += p[2] - '0';
		}
	}
	return isneg ? -ret : ret;
}

// Parse a string in the form ddmm.mmmmmmm...
unsigned long parse_degrees(TinyGPS_t *gps)
{
	char *p;
	unsigned long left_of_decimal = gpsatol(gps->_term);
	unsigned long hundred1000ths_of_minute = (left_of_decimal % 100UL) * 100000UL;
	for (p=gps->_term; gpsisdigit(*p); ++p);
	if (*p == '.')
	{
		unsigned long mult = 10000;
		while (gpsisdigit(*++p))
		{
			hundred1000ths_of_minute += mult * (*p - '0');
			mult /= 10;
		}
	}
	return (left_of_decimal / 100) * 1000000 + (hundred1000ths_of_minute + 3) / 6;
}

#define COMBINE(sentence_type, term_number) (((unsigned)(sentence_type) << 5) | term_number)

// Processes a just-completed term
// Returns true if new sentence has just passed checksum test and is validated
bool term_complete(TinyGPS_t *gps)
{
	if (gps->_is_checksum_term)
	{
		uint8_t checksum;
		checksum = 16 * from_hex(gps->_term[0]) + from_hex(gps->_term[1]);
		if (checksum == gps->_parity)
		{
			if (gps->_gps_data_good)
			{
#ifndef _GPS_NO_STATS
				++gps->_good_sentences;
#endif
				gps->_last_time_fix = gps->_new_time_fix;
				gps->_last_position_fix = gps->_new_position_fix;

				switch(gps->_sentence_type)
				{
				case _GPS_SENTENCE_GPRMC:
					gps->_time      = gps->_new_time;
					gps->_date      = gps->_new_date;
					gps->_latitude  = gps->_new_latitude;
					gps->_longitude = gps->_new_longitude;
					gps->_speed     = gps->_new_speed;
					gps->_course    = gps->_new_course;
					break;
				case _GPS_SENTENCE_GPGGA:
					gps->_altitude  = gps->_new_altitude;
					gps->_time      = gps->_new_time;
					gps->_latitude  = gps->_new_latitude;
					gps->_longitude = gps->_new_longitude;
					gps->_numsats   = gps->_new_numsats;
					gps->_hdop      = gps->_new_hdop;
					break;
				}

				return true;
			}
		}

#ifndef _GPS_NO_STATS
		else
			++gps->_failed_checksum;
#endif
		return false;
	}

	// the first term determines the sentence type
	if (gps->_term_number == 0)
	{
		if (!gpsstrcmp(gps->_term, _GPRMC_TERM))
			gps->_sentence_type = _GPS_SENTENCE_GPRMC;
		else if (!gpsstrcmp(gps->_term, _GPGGA_TERM))
			gps->_sentence_type = _GPS_SENTENCE_GPGGA;
		else
			gps->_sentence_type = _GPS_SENTENCE_OTHER;
		return false;
	}

	if (gps->_sentence_type != _GPS_SENTENCE_OTHER && gps->_term[0])
		switch(COMBINE(gps->_sentence_type, gps->_term_number))
		{
		case COMBINE(_GPS_SENTENCE_GPRMC, 1): // Time in both sentences
		case COMBINE(_GPS_SENTENCE_GPGGA, 1):
		gps->_new_time = parse_decimal(gps);
		//gps->_new_time_fix = millis();
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 2): // GPRMC validity
    		gps->_gps_data_good = gps->_term[0] == 'A';
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 3): // Latitude
		case COMBINE(_GPS_SENTENCE_GPGGA, 2):
		gps->_new_latitude = parse_degrees(gps);
		//gps->_new_position_fix = millis();
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 4): // N/S
		case COMBINE(_GPS_SENTENCE_GPGGA, 3):
		if (gps->_term[0] == 'S')
			gps->_new_latitude = -gps->_new_latitude;
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 5): // Longitude
		case COMBINE(_GPS_SENTENCE_GPGGA, 4):
		gps->_new_longitude = parse_degrees(gps);
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 6): // E/W
		case COMBINE(_GPS_SENTENCE_GPGGA, 5):
		if (gps->_term[0] == 'W')
			gps->_new_longitude = -gps->_new_longitude;
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 7): // Speed (GPRMC)
    		gps->_new_speed = parse_decimal(gps);
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 8): // Course (GPRMC)
    		gps->_new_course = parse_decimal(gps);
		break;
		case COMBINE(_GPS_SENTENCE_GPRMC, 9): // Date (GPRMC)
    		gps->_new_date = gpsatol(gps->_term);
		break;
		case COMBINE(_GPS_SENTENCE_GPGGA, 6): // Fix data (GPGGA)
    		gps->_gps_data_good = gps->_term[0] > '0';
		break;
		case COMBINE(_GPS_SENTENCE_GPGGA, 7): // Satellites used (GPGGA)
    		gps->_new_numsats = (unsigned char)atoi(gps->_term);
		break;
		case COMBINE(_GPS_SENTENCE_GPGGA, 8): // HDOP
    		gps->_new_hdop = parse_decimal(gps);
		break;
		case COMBINE(_GPS_SENTENCE_GPGGA, 9): // Altitude (GPGGA)
    		gps->_new_altitude = parse_decimal(gps);
		break;
		}

	return false;
}

long gpsatol(const char *str)
{
	long ret = 0;
	while (gpsisdigit(*str))
		ret = 10 * ret + *str++ - '0';
	return ret;
}

int gpsstrcmp(const char *str1, const char *str2)
{
	while (*str1 && *str1 == *str2)
		++str1, ++str2;
	return *str1;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts decimal degrees to radians             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
  return (deg * PI / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  This function converts radians to decimal degrees             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
  return (rad * 180 / PI);
}


double distance(double lat1, double lon1, double lat2, double lon2) {
  double theta, dist;
  if ((lat1 == lat2) && (lon1 == lon2)) {
    return 0;
  }
  else {
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515;
    dist = dist * 1.609344;  //en km
    return (dist);
  }
}

float course_to (float lat1, float long1, float lat2, float long2)
{
	// returns course in degrees (North=0, West=270) from position 1 to position 2,
	// both specified as signed decimal-degrees latitude and longitude.
	// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
	// Courtesy of Maarten Lamers
	float dlon = radians(long2-long1);
	lat1 = radians(lat1);
	lat2 = radians(lat2);
	float a1 = sin(dlon) * cos(lat2);
	float a2 = sin(lat1) * cos(lat2) * cos(dlon);
	a2 = cos(lat1) * sin(lat2) - a2;
	a2 = atan2(a1, a2);
	if (a2 < 0.0)
	{
		a2 += TWO_PI;
	}
	return degrees(a2);
}

char cardinal (float course)
{
	static const char* directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
	int direction;
	direction = (int)((course + 11.25f) / 22.5f);
	return directions[(direction % 16)];
}

// lat/long in MILLIONTHs of a degree and age of fix in milliseconds
// (note: versions 12 and earlier gave this value in 100,000ths of a degree.
void get_position(TinyGPS_t *gps,long *latitude, long *longitude, unsigned long *fix_age)
{
	if (latitude) *latitude = gps->_latitude;
	if (longitude) *longitude = gps->_longitude;
	if (fix_age) *fix_age = (gps->_last_position_fix == GPS_INVALID_FIX_TIME) ?   GPS_INVALID_AGE : (millis() - gps->_last_position_fix < 0)?  (millis() - gps->_last_position_fix + MAX_COUNTER_VALUE ) :(millis() - gps->_last_position_fix) ;
}

// date as ddmmyy, time as hhmmsscc, and age in milliseconds
void get_datetime(TinyGPS_t *gps,unsigned long *date, unsigned long *time, unsigned long *age)
{
	if (date) *date = (((gps->_date/ 100) % 100 <=0)||((gps->_date/ 100) % 100 >12)||(gps->_date/10000<=0)||(gps->_date/10000>31)||(gps->_date % 100>99)||(gps->_date % 100<0))?GPS_INVALID_DATE:gps->_date;
	if (time) *time = ((gps->_time<0)||((gps->_time>23595999)))?GPS_INVALID_TIME:gps->_time;
	if (age) *age = gps->_last_time_fix == GPS_INVALID_FIX_TIME ? GPS_INVALID_AGE : millis() - gps->_last_time_fix;
}

void f_get_position(TinyGPS_t *gps, float *latitude, float *longitude, unsigned long *fix_age)
{
	long lat, lon;
	get_position(gps,&lat, &lon, fix_age);
	*latitude = (lat == GPS_INVALID_ANGLE) ? GPS_INVALID_F_LATITUDE_ANGLE : (lat / 1000000.0);
	*longitude = (lon == GPS_INVALID_ANGLE) ? GPS_INVALID_F_LONGITUDE_ANGLE : (lon / 1000000.0);
	if (abs(*latitude)>90.0)
		*latitude  =GPS_INVALID_F_LATITUDE_ANGLE;
	if (abs(*longitude)>180.0)
		*longitude = GPS_INVALID_F_LONGITUDE_ANGLE;

}

bool crack_datetime(TinyGPS_t *gps,int *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *hundredths, unsigned long *age)
{
	unsigned long date, time;

	get_datetime(gps,&date, &time, age);

	if (date!=GPS_INVALID_DATE)
	{
		if (year)
		{
			*year = date % 100;
			*year += *year > 80 ? 1900 : 2000;
		}
		if (month) *month = (date / 100) % 100;
		if (day) *day = date / 10000;

	}
	else
	{
		return false;

	}

	if (time!=GPS_INVALID_TIME)
	{
		if (hour) *hour = time / 1000000;
		if (minute) *minute = (time / 10000) % 100;
		if (second) *second = (time / 100) % 100;
		if (hundredths) *hundredths = time % 100;

	}
	else
	{

		return false;
	}
	return true;
}

float f_altitude(TinyGPS_t *gps)
{
	return gps->_altitude = ((gps->_altitude >= GPS_INVALID_ALTITUDE)|| (gps->_altitude<0))? GPS_INVALID_F_ALTITUDE : gps->_altitude / 100.0;
}

float f_course(TinyGPS_t *gps)
{
	return gps->_course == GPS_INVALID_ANGLE ? 1000.0 : gps->_course / 100.0;
}

float f_speed_knots(TinyGPS_t *gps)
{
	return gps->_speed == GPS_INVALID_SPEED ? GPS_INVALID_F_SPEED : gps->_speed / 100.0;
}

float f_speed_mph(TinyGPS_t *gps)
{
	float sk = f_speed_knots(gps);
	return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : _GPS_MPH_PER_KNOT * sk;
}

float f_speed_mps(TinyGPS_t *gps)
{
	float sk = f_speed_knots(gps);
	return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : _GPS_MPS_PER_KNOT * sk;
}

float f_speed_kmph(TinyGPS_t *gps)
{
	float sk = f_speed_knots(gps);
	return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : _GPS_KMPH_PER_KNOT * sk;
}
bool gpsisdigit(char c)
{
	return c >= '0' && c <= '9';
}

void print_float(float val, float invalid, int len, int prec)
{
	char str[16];
	if (val == invalid)
	{
		while (len-- > 1)
			//SendStringWithUSB("*");
		SendStringWithUSB(" ");
	}
	else
	{
		sprintf(str,"%.2d",val);
		//fprintf(str, "%.2f", val);
//		SendStringWithUSB(str);
		int vi = abs((int)val);
		int flen = prec + (val < 0.0 ? 2 : 1); // . and -
		flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
		for (int i=flen; i<len; ++i)
			SendStringWithUSB(" ");
	}
}

void print_int(unsigned long val, unsigned long invalid, int len)
{
	char sz[32];
	if (val == invalid)
		strcpy(sz, "*******\r\n");
	else
		sprintf(sz, "%ld\r\n", val);
//	SendStringWithUSB(sz);
	sz[len] = 0;
	for (int i=strlen(sz); i<len; ++i)
		sz[i] = ' ';
	if (len > 0)
		sz[len-1] = ' ';

}
//bool print_date(TinyGPS_t *gps)
//{
//  uint8_t hundredths;
//  unsigned long age;
//  bool state = false;
//  state = crack_datetime(gps, &GpsOutput.Year, &GpsOutput.Month,&GpsOutput.Day, &GpsOutput.Hour, &GpsOutput.Minute, &GpsOutput.Second, &hundredths, &age);
//  if (!state)
//	return false;
//  else {
//	if (age == GPS_INVALID_AGE) {
//	  //SendStringWithUSB("********** ********\r\n");
//	  return false ;
//	}
//	else {
//	  char sz[32];
//	  sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d\r\n", GpsOutput.Month,
//		  GpsOutput.Day, GpsOutput.Year, GpsOutput.Hour,
//		  GpsOutput.Minute, GpsOutput.Second);
////	  SendStringWithUSB(sz);
//	}
//  }
//  print_int(age, GPS_INVALID_AGE, 5);
//  return true ;
//}

void print_str( char *str, int len)
{
	int slen = strlen(str);
	for (int i=0; i<len; ++i)
	{
		if(i<slen)
			SendStringWithUSB(str);
		else
			SendStringWithUSB(" \r\n");
	}
}

int FindTimeZone()
{
	int offset;
	int direction;
	direction = (GpsOutput.Latitude < 0.0 ? -1 : 1);
	offset = direction * round( GpsOutput.Longitude * 24.0 / 360.0);
	return offset;
}
