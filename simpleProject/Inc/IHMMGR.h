/*
 * IHMMGR.h
 *
 *  Created on: 11 avr. 2022
 *      Author: eloch
 */

#ifndef INC_IHMMGR_H_
#define INC_IHMMGR_H_

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define MAX_COMMAND_LENGTH 1000
#define MAX_COMMAND_NAME_LENGTH 25
#define MAX_COMMAND_NUMBER 27
#define MAX_MENU_NUMBER 27


typedef void (*handle_t)(char* Args);
typedef struct rang_s{
	int min ;
	int max ;
}rang_t;
typedef struct Command_s{

	char name[MAX_COMMAND_NAME_LENGTH];
	const char *helper;
	handle_t hdl;
}Command_t;

/* Menu struct */
typedef struct Menu_s{

	char name[MAX_COMMAND_NAME_LENGTH];
	const char *helper;
	Command_t CMD_list[MAX_COMMAND_NUMBER] ;
}Menu_t;


typedef struct RF_s{

	int Fc ;
	int bandwidth;
	uint8_t CR;
	bool header;
	uint8_t Payload;
	uint8_t TXP;
	uint16_t PReamble;
	bool CRCO;
	uint8_t SF;
}RF_t;
typedef struct Status_s{

	bool SD ;
	bool LoRa ;
	uint8_t battery ;
	uint8_t Intern_temp ;
	bool capt_cnx ;
}Status_t;
typedef struct Location_s{

	float longitude ;
	float latitude ;
	float altitude ;
}Location_t;

extern char RxChar;
extern char RxBuffer[MAX_COMMAND_LENGTH];
extern bool CMDReceived;
extern Command_t AllCommandsList[MAX_COMMAND_NUMBER];
RF_t RF , RF1 ;

Location_t Location ;
long int ID ;
typedef bool (*validityhdl_t)(uint32_t val,uint32_t* range,uint32_t inc);

extern int echant ;
int ParseCMD(char * CMD,char *Args,Command_t *commandlist);
void ProcessCMD(Command_t *commandlist,char* Args, int index) ;
void LOCATION_SET(char* args) ;
void TLE_SET(char* args);
void ID_SET(char* args);
void TIMEDATE_SET(char* args);
void PAY_SET(char* args) ;
void PRE_SET(char* args) ;
void CR_SET(char* args) ;
void FC_SET(char* args) ;
void BWD_SET(char* args) ;
void CRC_SET(char* args) ;
void SF_SET(char* args) ;
void TXP_set(char* args) ;
void STATUS_fun();
void LOCATION_GET() ;
void ID_GET();
void TIMEDATE_GET();
void PAY_GET() ;
void PRE_GET() ;
void CR_GET() ;
void FC_GET() ;
void BWD_GET() ;
void CRC_GET() ;
void SF_GET() ;
void TXP_GET() ;
void UnknownCMDHandler();
void Set(uint32_t * variable,char* args,validityhdl_t checkvalidity);
void printfloat(float alt , int pow10 ,char buff[20]) ;
void printfloat2(float alt , int pow10 ,char buff[20]);
//char* substr(const char *src, int m, int n)
//void printfloatinFreeRTOS (float a) ;
char* substringfunc(const char *src, int m, int n);



#endif /* INC_IHMMGR_H_ */
