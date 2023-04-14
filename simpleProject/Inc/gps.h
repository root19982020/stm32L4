/*
 * gps.h
 *
 *  Created on: May 27, 2022
 *      Author: eloch
 */

#ifndef GPS_H_
#define GPS_H_
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define _GPS_VERSION 13 // software version of this library
#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001
#define MAX_COUNTER_VALUE   4294967296
extern const float GPS_INVALID_F_LONGITUDE_ANGLE ;
extern const float GPS_INVALID_F_LATITUDE_ANGLE ;
extern const float GPS_INVALID_F_ALTITUDE ;
extern const float GPS_INVALID_F_SPEED ;
#define PI 3.142857
#define TWO_PI (3.14 * 2)

typedef struct GPSOutput_s
{
  long Latitude;
  long Longitude;
  long Altitude;
  unsigned long Date;
  unsigned long Time;
  unsigned long Speed;
  unsigned long course;
  unsigned short NumSats;
}GPSOutput_t;

enum
 {
    GPS_INVALID_AGE = 0xFFFFFFFF,
    GPS_INVALID_ANGLE = 999999999,
    GPS_INVALID_ALTITUDE = 18000,
    GPS_INVALID_DATE = 10100,
    GPS_INVALID_TIME = 0,
    GPS_INVALID_SPEED = 999999999,
    GPS_INVALID_FIX_TIME = 0xFFFFFFFF,
    GPS_INVALID_SATELLITES = 0xFF,
    GPS_INVALID_HDOP = 0xFFFFFFFF
  };
 enum  {
   _GPS_SENTENCE_GPGGA = 0,
   _GPS_SENTENCE_GPRMC,
   _GPS_SENTENCE_OTHER
 };
typedef struct TinyGPS_s
{
  unsigned long _time, _new_time;
  unsigned long _date, _new_date;
  long _latitude, _new_latitude;
  long _longitude, _new_longitude;
  long _altitude, _new_altitude;
  unsigned long  _speed, _new_speed;
  unsigned long  _course, _new_course;
  unsigned long  _hdop, _new_hdop;
  unsigned short _numsats, _new_numsats;

  unsigned long _last_time_fix, _new_time_fix;
  unsigned long _last_position_fix, _new_position_fix;

  // parsing state variables
  uint8_t _parity;
  bool _is_checksum_term;
  char _term[15];
  uint8_t _sentence_type;
  uint8_t _term_number;
  uint8_t _term_offset;
  bool _gps_data_good;

#ifndef _GPS_NO_STATS
  // statistics
  unsigned long _encoded_characters;
  unsigned short _good_sentences;
  unsigned short _failed_checksum;
  unsigned short _passed_checksum;
#endif

}TinyGPS_t;

extern TinyGPS_t TinyGPS;
extern GPSOutput_t GpsOutput;
//uint32_t millis (void);
void TinyGPSCreate(TinyGPS_t *gps);
void GPSOutputInit (GPSOutput_t *myCfg);
int library_version(void);
unsigned long hdop(TinyGPS_t *gps) ;
unsigned short satellites(TinyGPS_t *gps);
bool encode(TinyGPS_t *gps, char c);
void stats(TinyGPS_t *gps,unsigned long *chars, unsigned short *sentences, unsigned short *failed_cs);
int from_hex(char a);
unsigned long parse_decimal(TinyGPS_t *gps);
unsigned long parse_degrees(TinyGPS_t *gps);
bool term_complete(TinyGPS_t *gps);
long gpsatol(const char *str);
int gpsstrcmp(const char *str1, const char *str2);
double distance(double lat1, double lon1, double lat2, double lon2);
float course_to (float lat1, float long1, float lat2, float long2);
char cardinal (float course);
void get_position(TinyGPS_t *gps,long *latitude, long *longitude, unsigned long *fix_age);
void get_datetime(TinyGPS_t *gps,unsigned long *date, unsigned long *time, unsigned long *age);
void f_get_position(TinyGPS_t *gps, float *latitude, float *longitude, unsigned long *fix_age);
bool crack_datetime(TinyGPS_t *gps,int *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *hundredths, unsigned long *age);
float f_altitude(TinyGPS_t *gps);
float f_course(TinyGPS_t *gps);
float f_speed_knots(TinyGPS_t *gps);
float f_speed_mph(TinyGPS_t *gps);
float f_speed_mps(TinyGPS_t *gps);
float f_speed_kmph(TinyGPS_t *gps);
void print_float(float val, float invalid, int len, int prec);
void print_int(unsigned long val, unsigned long invalid, int len);
bool print_date(TinyGPS_t *gps);
void print_str( char *str, int len);
bool gpsisdigit(char c) ;
int FindTimeZone();


#endif /* GPS_H_ */
