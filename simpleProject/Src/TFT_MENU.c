/*
 * TFT_MENU.c
 *
 *  Created on: Nov 13, 2020
 *      Author: meh
 */

#include "TFT_MENU.h"
#include "stm32l4xx_hal.h"
#include "fonts.h"
#include "gpio.h"
#include "predictsat.h"
#include "cmsis_os.h"
#include "GUI_Paint.h"
#include "DEV_Config.h"
#include <time.h>
#include <string.h>

// Menu buttons defines
int homemenu=0,page2menu=0,testmenu=0,testbutton=0,page2button=0,backbutton=0,ledonbutton=0,ledoffbutton=0,page3menu=0,page4menu=0,page5menu = 0,currtimebut =0 , capbutton = 0,page6menu = 0,page7menu = 0;

// physical buttons defines
int downbutton=0, upbutton=0, enter=0,accmeunu=0;
uint8_t initial=1;
uint8_t hh,mm,ss;
//	uint16_t x00=0, x11=0, y00=0, y11=0;
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;
uint16_t osx=120, osy=120, omx=120, omy=120, ohx=120, ohy=120;  // Saved H, M, S x & y coords
float sdeg=0, mdeg=0, hdeg=0;
RTC_DateTypeDef gDate;
RTC_TimeTypeDef gTime;
int local = 0 ;
// "TEST ALL" button on home (unselected)
void Test_Button_unselect (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, BLUE);
	//	fillRoundRect(21, 75, 65, 13, 8, WHITE);
	//	ST7735_WriteString(22, 77  , "SYSTEM", Font_7x10, BLUE, WHITE );
	//	testbutton = 0;  // TEST ALL button is not selected
}

// "TEST ALL" button on home (selected)
void Test_Button_select (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, RED);
	//	fillRoundRect(21, 75, 65, 13, 8, BLUE);
	//	ST7735_WriteString(22, 77 , "SYSTEM", Font_7x10, WHITE, BLUE);
	//	testbutton = 1;  // TEST ALL button is selected
}

void SYS_Button_unselect (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, BLUE);
	//	fillRoundRect(21, 75, 65, 13, 8, WHITE);
	//	ST7735_WriteString(24, 77  , "  CAP  ", Font_7x10, BLUE, WHITE );
	//	testbutton = 0;  // TEST ALL button is not selected
}

// "TEST ALL" button on home (selected)
void SYS_Button_select (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, RED);
	//	fillRoundRect(21, 75, 65, 13, 8, BLUE);
	//	ST7735_WriteString(24, 77 , "  CAP  ", Font_7x10, WHITE, BLUE);
	//	testbutton = 1;  // TEST ALL button is selected
}
// "PAGE 2" button on home (unselected)
void Page2_Button_unselect (void)
{
	//	drawRoundRect(83, 95, 53, 13, 8, BLUE);
	//	fillRoundRect(83, 96, 53, 13, 8, WHITE);
	//	ST7735_WriteString(84, 97, "PREDICT", Font_7x10, BLUE, WHITE);
	//	page2button = 0;  // "PAGE 2" button is not selected
}

// "PAGE 2" button on home (selected)
void Page2_Button_select (void)
{
	//	drawRoundRect(83, 95, 53, 13, 8, RED);
	//	fillRoundRect(83, 96, 53, 13, 8, BLUE);
	//	ST7735_WriteString(84, 97, "PREDICT", Font_7x10, WHITE, BLUE);
	//	page2button = 1;  // "PAGE 2" button is selected
}
void print_CurTme (void)
{
	//	char msg[50]={0};
	//	RTC_TimeTypeDef sTimeRTC= {0};
	//	RTC_DateTypeDef sDateRTC = {0};
	//
	//	HAL_RTC_GetTime(&hrtc, &sTimeRTC, RTC_FORMAT_BIN);
	//	HAL_RTC_GetDate(&hrtc, &sDateRTC, RTC_FORMAT_BIN);
	//
	//	sprintf(msg, " %02d/%02d:%02d:%02d", sDateRTC.Date,sTimeRTC.Hours, sTimeRTC.Minutes, sTimeRTC.Seconds );
	//	ST7735_WriteString(14 , 111, msg, Font_7x10, BLUE, WHITE);
}
// "GO BACK" button (unselected)
void back_Button_unselect (void)
{
	//	drawRoundRect(38, 104, 53, 14, 8, BLACK);
	//	fillRoundRect(39, 105, 53, 14, 8, WHITE);
	//	ST7735_WriteString(40, 106, "GO BACK", Font_7x10, BLUE, WHITE);
	//	backbutton = 0;  // "GO BACK" button is not selected
}

// "GO BACK" button (selected)
void back_Button_select (void)
{
	//	drawRoundRect(38, 104, 53, 14, 8, RED);
	//	fillRoundRect(38, 105, 53, 14, 8, BLUE);
	//	ST7735_WriteString(40, 106, "GO BACK", Font_7x10, BLACK, BLUE);
	//	backbutton = 1;  // "GO BACK" button is selected
}
void back_Button_unselect2 (void)
{
	//	drawRoundRect(38, 114, 53, 14, 8, BLACK);
	//	fillRoundRect(39, 115, 53, 14, 8, WHITE);
	//	ST7735_WriteString(40, 116, "GO BACK", Font_7x10, BLUE, WHITE);
	//	backbutton = 0;  // "GO BACK" button is not selected
}

// "GO BACK" button (selected)
void back_Button_select2 (void)
{
	//	drawRoundRect(38, 114, 53, 14, 8, RED);
	//	fillRoundRect(39, 115, 53, 14, 8, BLUE);
	//	ST7735_WriteString(40, 116, "GO BACK", Font_7x10, BLACK, BLUE);
	//	backbutton = 1;  // "GO BACK" button is selected
}

// "LED ON" button (unselected)
void ledon_Button_unselect (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, BLUE);
	//	fillRoundRect(21, 75, 65, 13, 8, WHITE);
	//	ST7735_WriteString(22, 77  , " ORBIT ", Font_7x10, BLUE, WHITE );
	//	ledonbutton = 0;  // "LED ON" button is not selected
}

// "LED ON" button (selected)
void ledon_Button_select (void)
{
}

// "LED OFF" button (unselected)
void ledoff_Button_unselect (void)
{

}

// "LED OFF" button (selected)
void ledoff_Button_select (void)
{
	//	drawRoundRect(83, 86, 53, 13, 8, RED);
	//	fillRoundRect(83, 87, 53, 13, 8, BLUE);
	//	ST7735_WriteString(84, 88, "  AOS  ", Font_7x10, WHITE, BLUE);
	//	ledoffbutton = 1;  // "LED OFF" button is selected
}
void AOS_Button_unselect (void)
{
	//	drawRoundRect(83, 86, 53, 13, 8, BLUE);
	//	fillRoundRect(83, 87, 53, 13, 8, WHITE);
	//	ST7735_WriteString(84, 88, "  AOS  ", Font_7x10, BLUE, WHITE);
	//	ledoffbutton = 0;  // "LED OFF" button is not selected
}
void AOS_Button_select (void)
{
	//	drawRoundRect(83, 86, 53, 13, 8, RED);
	//	fillRoundRect(83, 87, 53, 13, 8, BLUE);
	//	ST7735_WriteString(84, 88, "  AOS  ", Font_7x10, WHITE, BLUE);
	//	ledoffbutton = 1;  // "LED OFF" button is selected
}
void ORBIT_Button_unselect (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, BLUE);
	//	fillRoundRect(21, 75, 65, 13, 8, WHITE);
	//	ST7735_WriteString(22, 77  , " ORBIT ", Font_7x10, BLUE, WHITE );
	//	ledonbutton = 0;  // "LED ON" button is not selected
}
void cap_Button_unselect (void)
{
	//	drawRoundRect(83, 86, 53, 13, 8, BLUE);
	//	fillRoundRect(83, 87, 53, 13, 8, WHITE);
	//	ST7735_WriteString(84, 88 , " CAP ", Font_7x10, BLUE, WHITE );
	//	capbutton = 0;  // "LED ON" button is not selected
}
void cap_Button_select (void)
{
	//	drawRoundRect(83, 86, 53, 13, 8, RED);
	//	fillRoundRect(83, 87, 53, 13, 8, BLUE);
	//	ST7735_WriteString(84, 88, " CAP ", Font_7x10,WHITE , BLUE );
	//	capbutton = 1;  // "LED ON" button is selected
}
void Time_Button_unselect (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, BLUE);
	//	fillRoundRect(21, 75, 65, 13, 8, WHITE);
	//	ST7735_WriteString(22, 77  , "Curr.Time", Font_7x10, BLUE, WHITE );
	//	currtimebut = 0;  // "LED ON" button is not selected
}
void Time_Button_select (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, RED );
	//	fillRoundRect(21, 75, 65, 13, 8, BLUE);
	//	ST7735_WriteString(22, 77  , "Curr.Time", Font_7x10,WHITE , BLUE );
	//	currtimebut = 1;  // "LED ON" button is selected
}
// "LED ON" button (selected)
void ORBIT_Button_select (void)
{
	//	drawRoundRect(20, 74, 65, 13, 8, RED );
	//	fillRoundRect(21, 75, 65, 13, 8, BLUE);
	//	ST7735_WriteString(22 , 77  , " ORBIT ", Font_7x10,WHITE , BLUE );
	//	ledonbutton = 1;  // "LED ON" button is selected
}
/******Home Menu******/
void Acceuille (void)
{
	//	ST7735_SetRotation(3);  // set horizontal
	//	//	fillScreen(BLACK);  // fill black
	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image2 );
	//	Page2_Button_unselect();  // draw unselected "PAGE 2" button
	//	SYS_Button_unselect();  // draw unselected "TEST ALL" button
	//	homemenu = 1;  // "HomeMenu" is selected
	//	page2menu = 0;  // "Page2Menu" is not selected
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
}
void HomeMenu (void)
//	ST7735_SetRotation(3);  // set horizontal
//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image );
//	homemenu = 0;  // "HomeMenu" is selected
//	page2menu = 0;  // "Page2Menu" is not selected
//	testmenu = 0;// "TestMenu" is not selected
//	page5menu = 0;
//	accmeunu = 1 ;
//	page4menu = 0 ;
//	page3menu = 0 ;

//void analog_watch()
{
	homemenu = 1;  // "HomeMenu" is selected
	page2menu = 0;  // "Page2Menu" is not selected
	testmenu = 0;  // "TestMenu" is not selected
	page5menu = 0;

	//	RTC_DateTypeDef sDate;
	//	RTC_TimeTypeDef sTime;

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	//  Paint_DrawTime(153, 123, &time_s, &Font12 , BLACK, GREEN) ;
	DEV_Module_Init();
	//  Paint_DrawImage(gImage_70X70, 85, 85, 70, 70);
	//  Paint_DrawImage(gImage_60X60, 85, 25, 70, 70);
	//  Paint_DrawString_CN(56,140, "΢ѩ����",   &Font24CN,BLACK,  WHITE);
	//  Paint_DrawString_EN(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
	//  Paint_DrawString_CN(56,140, "OCHI",   &Font24,BLACK,  WHITE);
	//  Paint_DrawString_EN(153, 123, "NEOBIONICS",&Font8,  BLACK, GREEN);
	//	Paint_DrawLine  (120, 120, 70, 70,YELLOW ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (120, 120, 176, 64,BLUE ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (120, 120, 120, 210,RED ,DOT_PIXEL_2X2,LINE_STYLE_SOLID);
	//	DEV_Delay_ms(1000);
	ss=gTime.Seconds;
	mm=gTime.Minutes;
	hh=gTime.Hours;


	//	sdeg = ss*6;                  // 0-59 -> 0-354
	//	mdeg = mm*6+sdeg*0.01666667;  // 0-59 -> 0-360 - includes seconds
	//	hdeg = hh*30+mdeg*0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
	//	hx =  cos((hdeg-90)*0.0174532925);
	//	hy =  sin((hdeg-90)*0.0174532925);
	//	mx =  cos((mdeg-90)*0.0174532925);
	//	my =  sin((mdeg-90)*0.0174532925);
	//	sx = cos((sdeg-90)*0.0174532925);
	//	sy =  sin((sdeg-90)*0.0174532925);

	sdeg = ss*6;                  // 0-59 -> 0-354
	mdeg = mm*6;  // 0-59 -> 0-360 - includes seconds
	hdeg = hh*30;  // 0-23 -> 0-360 - includes minutes and seconds
	hx =  cos((hdeg-90)*0.0174532925)/3;
	hy =  sin((hdeg-90)*0.0174532925)/3;
	mx =  cos((mdeg-90)*0.0174532925)/3;
	my =  sin((mdeg-90)*0.0174532925)/3;
	//	hdeg=(hdeg/57.29577951) ;
	//	mdeg=(mdeg/57.29577951) ;
	//	hx=cos((sdeg-90)*0.0174532925);
	//	hy=sin((sdeg-90)*0.0174532925);
	//	mx=cos((sdeg-90)*0.0174532925);
	//	my=sin((sdeg-90)*0.0174532925);
	sx = cos((sdeg-90)*0.0174532925)/3;
	sy =  sin((sdeg-90)*0.0174532925)/3;
	//	hx = cos(hdeg)/400;
	//	hy = sin(hdeg)/400;
	//	mx = cos(hdeg)/200;
	//	my = sin(hdeg)/200;
	//	sx = cos(hdeg)/200;
	//	sy = sin(hdeg)/200;
	//	Paint_DrawLine  (hx, hy, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (mx, my, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (sx, sy, 120, 121,BLACK ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (hx, hy, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (mx, my, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (sx, sy, 120, 121,RED ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (hx, hy, 120, 121,YELLOW ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	//	Paint_DrawLine  (mx, hy, 120, 121,BLUE ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	if (ss==0 || initial) {
		initial = 0;
		//		// Erase hour and minute hand positions every minute
		//
		//		Paint_DrawLine  (ohx, ohy, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
		Paint_DrawCircle(120, 121, 45, BLACK ,DOT_PIXEL_8X8,1);

		Paint_DrawLine  (omx, omy, 120, 121,BLACK ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);

		//		ohx = hx*62+121;
		//		ohy = hy*62+121;
		ohx = (hx*55+121);
		ohy = (hy*55+121);
		//	}



		//		omx = mx*84+120;
		//		omy = my*84+121;
		omx = (mx*90+121);
		omy = (my*90+121);
		//	}
	}
	Paint_DrawLine  (osx, osy, 120, 121,BLACK ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	osx = (sx*90+121);
	osy = (sy*90+121);
	Paint_DrawLine  (osx, osy, 120, 121,RED ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	Paint_DrawLine  (ohx, ohy, 120, 121,YELLOW ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	Paint_DrawLine  (omx, ohy, 120, 121,BLUE ,DOT_PIXEL_1X1,LINE_STYLE_SOLID);
	printf("quit...\r\n");
	local = 0 ;
	//	DEV_Module_Exit();
	//	HAL_Delay(1000);

}

/******Test Menu******/
void TestMenu (void)
{ char phstring[4],tempstring[4],humstring[4],shumstring[4],CO2string[4],VVENTSTRING[4];
//homemenu = 0;  // "HomeMenu" is not selected
homemenu = 1;
page2menu = 0;  // "Page2Menu is not selected
page4menu = 0 ;
page3menu = 0 ;
accmeunu = 0 ;

//testmenu = 1;  // "TestMenu" is selected
page5menu = 0;
if (local = 0)
Paint_Clear(BLACK);
else
//Paint_DrawCircle(120,120, 40, BLACK ,DOT_PIXEL_2X2,DRAW_FILL_FULL);
Paint_DrawImage(image_data_Image, 105, 85, 40, 80);

//Paint_DrawLine  (120, 90, 120, 85,YELLOW ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
//Paint_DrawLine  (120, 150 , 120, 155,YELLOW ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
//Paint_DrawLine  (0, 120, 12, 120,YELLOW ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
//Paint_DrawLine  (228, 120, 240, 120,YELLOW ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
//Paint_NewImage(80,80, 0, BLACK);

//	testAll();  // Perform All the Tests
//
//	ST7735_SetRotation(3);  // set Horizontal
//	fillScreen(WHITE);  // fill WHITE
////	ST7735_DrawImage(0, 0, 160, 128, image_data_Image2 );
//
//	ST7735_WriteString(9, 19, "PH : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(ph1, 2 , phstring ) ;
//	ST7735_WriteString(59, 19, phstring, Font_7x10, BLUE, WHITE);
//	ST7735_WriteString(9, 29, "TEMP : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(temp, 2 , tempstring ) ;
//	ST7735_WriteString(59, 29, tempstring, Font_7x10, BLUE, WHITE);
//	ST7735_WriteString(9, 39, "HUM : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(hum, 2 , humstring ) ;
//	ST7735_WriteString(59, 39, humstring, Font_7x10, BLUE, WHITE);
//	ST7735_WriteString(9, 49, "S.HUM : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(sol_hum, 2 , shumstring ) ;
//	ST7735_WriteString(59, 49, shumstring, Font_7x10, BLUE, WHITE);
//	ST7735_WriteString(9, 59, "CO2 : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(CO2, 2 , CO2string ) ;
//	ST7735_WriteString(59, 59, CO2string, Font_7x10, BLUE, WHITE);
//	ST7735_WriteString(9, 69, "V.VENT : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
//	printfloat2(V_vent, 2 , VVENTSTRING ) ;
//	ST7735_WriteString(59, 69, VVENTSTRING, Font_7x10, BLUE, WHITE);
//	back_Button_unselect();  // draw unselected "GO BACK" button
}

/******Page 2  Menu******/
void Page2Menu (void)
{
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 1;  // "Page2Menu" is selected
	//	page4menu = 0 ;
	//	page3menu = 0 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
	//	ST7735_SetRotation(3);  // set horizontal
	//	//	fillScreen(BLACK);  // fill black
	////	ST7735_DrawImage(0, 0, 160, 128, image_data_Image2 );
	//
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	//	ORBIT_Button_unselect () ;
	//	//	AOS_Button_unselect();  // draw unslected "LED OFF" button
	//	AOS_Button_unselect() ;
	//	back_Button_unselect();  // draw unslected "GO BACK" button
}
void Page3Menu (void)
{
	//	char bu1[30]={0} ,bu2[20]={0}  ,bu3[20]={0}, bu4[20]={0} ,bu5[20]={0} ,bu6[20]={0} , bu7[20]={0} ,bu8[20]={0}  ,bu9[20]={0}, bu10[20]={0} ,bu11[20]={0} ,bu12[20]={0};
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 0;  // "Page2Menu" is selected
	//	page4menu = 0 ;
	//	page3menu = 1 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
	//	ST7735_SetRotation(3);  // set horizontal
	////	fillScreen(WHITE);  // fill black
	//	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image44 );
	//
	//	printfloat2(iss_orbit.altitude , 8 , bu1 );
	//	printfloat2(iss_orbit.latitude , 8 , bu2 );
	//	printfloat2(iss_orbit.longitude , 8 , bu3 );
	//	printfloat2(iss_orbit.eclipse_depth , 8 , bu4 );
	//	printfloat2(iss_orbit.footprint , 8 , bu5 );
	//	printfloat2(iss_orbit.argument_of_perigee , 8 , bu6 );
	//	printfloat2(iss_orbit.inclination , 8 , bu7 );
	//	printfloat2(iss_orbit.phase , 8 , bu8 );
	//	printfloat2(iss_orbit.decayed , 1 , bu9 );
	//	printfloat2(iss_orbit.revolutions , 1 , bu11 );
	//	printfloat2(iss_orbit.right_ascension , 8 , bu10 );
	//	printfloat2(iss_orbit.eclipsed , 8 , bu12 );
	//
	//
	//	ST7735_WriteString(9, 9, "ALT : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 9, bu1, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 29, "LAT : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 29, bu2, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 39, "LON : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 39, bu3, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 49, "DEPTH : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 49, bu4, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 59, "FootP : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 59, bu5, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 69, "INCLI : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 69, bu7, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 79, "ARG.P : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 79, bu6, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 89, "PHASE : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 89, bu8, Font_7x10, BLUE, WHITE);
	////	ST7735_WriteString(9, 99, "DECAYED : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	////	ST7735_WriteString(69, 99, bu9, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 19, "REVO : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 19, bu11, Font_7x10, BLUE, WHITE);
	//	ST7735_WriteString(9, 99, "ECLIP : ", Font_7x10, BLUE, WHITE);  // Write "test completed"
	//	ST7735_WriteString(65, 99, bu12, Font_7x10, BLUE, WHITE);
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	//	//	ORBIT_Button_unselect () ;
	//	//	AOS_Button_unselect();  // draw unslected "LED OFF" button
	//	//	AOS_Button_unselect() ;
	//	back_Button_unselect2();  // draw unslected "GO BACK" button
}
void Page4Menu (void)
{
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 0;  // "Page2Menu" is selected
	//	page4menu = 1 ;
	//	page3menu = 0 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
	//	ST7735_SetRotation(3);  // set horizontal
	//	//	fillScreen(BLACK);  // fill black
	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image2 );
	//
	//	ST7735_WriteString(22, 77  , "NEXT AOS:", Font_7x10, BLUE, WHITE );
	//	volatile char ahmed[30]={0};
	//	localtime_r(&time11, &lt);
	//	//	sprintf(ahmed , "%ld" , time11);
	//	strftime(ahmed, sizeof(ahmed), "%a %Y-%m-%d %H:%M:%S", &lt);
	//	//	 ah = substringfunc(ahmed, 5 , 24 ) ;
	//	ST7735_WriteString(15, 88 , substringfunc(ahmed, 6 , 24 ) , Font_7x10, BLUE, WHITE );
	//
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	//	//	ORBIT_Button_unselect () ;
	//	//	AOS_Button_unselect();  // draw unslected "LED OFF" button
	//	//	AOS_Button_unselect() ;
	//	back_Button_unselect();  // draw unslected "GO BACK" button
}
void Page5Menu (void)
{
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 0;  // "Page2Menu" is selected
	//	page4menu = 0 ;
	//	page3menu = 0 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 1;
	//	ST7735_SetRotation(3);  // set horizontal
	//	//	fillScreen(BLACK);  // fill black
	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image2 );
	//
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	////	cap_Button_unselect () ;
	////	Time_Button_select() ;
	//	back_Button_unselect();  // draw unslected "GO BACK" button
}
void Page6Menu (void)
{
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 0;  // "Page2Menu" is selected
	//	page4menu = 0 ;
	//	page6menu = 1 ;
	//	page7menu = 0 ;
	//	//	page5menu = 0 ;
	//	page3menu = 0 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
	//	ST7735_SetRotation(3);  // set horizontal
	//	fillScreen(WHITE);  // fill black
	//	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image44 );
	//
	//
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	//	//	ORBIT_Button_unselect () ;
	//	//	AOS_Button_unselect();  // draw unslected "LED OFF" button
	//	//	AOS_Button_unselect() ;
	//	back_Button_unselect();  // draw unslected "GO BACK" button
}
void Page7Menu (void)
{
	//	homemenu = 0;  // "HomeMenu" is not selected
	//	page2menu = 0;  // "Page2Menu" is selected
	//	page4menu = 0 ;
	//	page6menu = 0 ;
	//	page7menu = 1 ;
	//	//	page5menu = 0 ;
	//	page3menu = 0 ;
	//	accmeunu = 0 ;
	//	testmenu = 0;  // "TestMenu" is not selected
	//	page5menu = 0;
	//	ST7735_SetRotation(3);  // set horizontal
	//	fillScreen(WHITE);  // fill black
	//	//	ST7735_DrawImage(0, 0, 160, 128, image_data_Image44 );
	//
	//
	//	//	ORBIT_Button_unselect();  // draw unslected "LED ON" button
	//	//	ORBIT_Button_unselect () ;
	//	//	AOS_Button_unselect();  // draw unslected "LED OFF" button
	//	//	AOS_Button_unselect() ;
	//	back_Button_unselect();  // draw unslected "GO BACK" button
}
/*******
 *  EXTI callback
 * PA0 is ENTER
 * PA1 is DOWN
 * PA4 is UP
 *******/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_11)  // If the PA0 (ENTER) is pressed
	{
		downbutton = 0;  // downbutton set to 0
		upbutton = 0;  // upbutton set to 0
		enter = 1;  // enter set to 1
	}

	if (GPIO_Pin == GPIO_PIN_12)  // If the PA1 (DOWN) is pressed
	{
		downbutton = 1;  // downbutton set to 1 and others set to 0
		upbutton = 0;
		enter = 0;
	}

	if (GPIO_Pin == GPIO_PIN_1)  // If the PA4 (UP) is pressed
	{
		downbutton = 0;
		upbutton = 1;  // upbutton set to 1 and others set to 0
		enter = 0;
	}
}
void test (void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)!=1)
	{downbutton = 1 ;}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)!=1)
	{enter = 1 ;}
}

void Menu_Handler (void)
{
	if (homemenu)  // If the Home Menu is called
	{
		//HomeMenu();  // Draw the Home Menu
		TestMenu();
		while (homemenu)  // While the control is inside the Home Menu
		{
			//HomeMenu();//			  test() ;
			TestMenu();
			if (downbutton)  // if the down button is pressed
			{
				osDelay(200);  // wait for some time to avoid error click
				if (testbutton)  // if the "TEST ALL" button is already selected
				{
					Page2_Button_select();  // draw selected "PAGE 2" button
					SYS_Button_unselect(); // draw unselected "TEST ALL" button
					downbutton = 0;  // reset the down button or else it will keep selecting in a loop
					testbutton = 0;  // reset the test button also
				}

				else if (page2button)  // If the "PAGE 2" button is selected
				{
					Page2_Button_unselect();  // draw unselected "PAGE 2" button
					SYS_Button_select();  // draw selected "TEST ALL" button
					downbutton = 0;  // reset the down button or else it will keep selecting in a loop
					page2button = 0;  // reset the page 2 button
				}

				else  // If none of the buttons are selected
				{
					Page2_Button_unselect();  // draw unselected "PAGE 2" button
					SYS_Button_select();  // draw selected "TEST ALL" button
					downbutton = 0;  // reset the down button or else it will keep selecting in a loop
				}
			}

			else if (upbutton) // if the UP button is pressed
			{
				osDelay(200);
				if (testbutton)  // If the "TEST ALL" is already selected
				{
					// select the "PAGE 2" button and unselect the "TSET ALL" button
					Page2_Button_select();
					Test_Button_unselect();
					upbutton = 0;
					testbutton = 0;
				}

				else if (page2button)  // If the "PAGE 2" button is already selected
				{
					// select the "TEST ALL" button and unselect the "PAGE 2" button
					Page2_Button_unselect();
					Test_Button_select();
					upbutton = 0;
					page2button = 0;
				}

				else  // if none of the buttons are selected
				{
					// select the "PAGE 2" button and unselect the "TEST ALL" button
					Page2_Button_select();
					Test_Button_unselect();
					upbutton = 0;
				}
			}
			else if (enter)  // if the ENTER button is pressed
			{
				if (testbutton)  // if the "TEST ALL" button is already pressed
				{
					testmenu = 1;  // Select the TestMenu
					homemenu = 0;  // Reset the HomeMenu
					enter = 0;  // reset the enter button or else it will keep selecting in a loop
					testbutton = 0;  // reset the down button
				}

				else if (page2button)  // if the "PAGE 2" button is already pressed
				{
					page2menu = 1;  // Select the Page2Menu
					homemenu = 0;  // Reset the HomeMenu
					enter = 0;  // reset the enter button or else it will keep selecting in a loop
					page2button = 0;  // reset the up button
					testmenu = 0 ;
				}

				else;  // if none of the buttons are selected, then do nothing
			}
			else ;
			print_CurTme() ;
			osDelay(1);
		}
	}

	else if (testmenu)  // If the Test Menu is called
	{
		TestMenu();  // Display the Test Menu
		while (testmenu)  // While the control is in the Test Menu
		{
			if (downbutton )  // Since there is only a Back Button, so either up or down pressed, it will result the same
			{
				osDelay(200);
				back_Button_select();
				downbutton = 0 ;
			}

			if (enter)  // If the enter is pressed
			{
				if (backbutton)  // If the Back button is selected
				{
					homemenu = 1;  // "HomeMenu" is not selected
					page2menu = 0;  // "Page2Menu" is selected
					page4menu = 0 ;
					page3menu = 0 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0;
				}
				else ;
			}

			else ;
			osDelay(1);
		}
	}

	else if (page2menu)  // If the Page 2 Menu is called
	{
		Page2Menu();  // Display the Page 2 Menu
		while (page2menu)  // While the control is in the Page Menu
		{
			if (downbutton)  // If down button is pressed
			{
				osDelay(200);
				if (ledonbutton)  // If the "LED ON" is selected
				{
					ORBIT_Button_unselect();  // Unselect "LED ON"
					back_Button_unselect();  // unselect "GO BACK"
					AOS_Button_select();  // select "LED OFF"
					downbutton = 0;  // Reset the down button
				}

				else if (ledoffbutton)
				{
					AOS_Button_unselect();
					back_Button_select();
					ORBIT_Button_unselect();
					downbutton = 0;
				}

				else if (backbutton)
				{
					AOS_Button_unselect();
					back_Button_unselect();
					ORBIT_Button_select();
					downbutton = 0;
				}

				else  // If none of the buttons were selected
				{
					// Select the "LED ON" button
					AOS_Button_unselect();
					back_Button_unselect();
					ORBIT_Button_select();
					downbutton = 0;
				}
			}
			// Similar to down button
			else if (upbutton)
			{
				osDelay(200);
				if (ledonbutton)
				{
					ORBIT_Button_unselect();
					back_Button_select();
					AOS_Button_unselect();
					upbutton = 0;
				}

				else if (ledoffbutton)
				{
					AOS_Button_unselect();
					back_Button_unselect();
					ORBIT_Button_select();
					upbutton = 0;
				}

				else if (backbutton)
				{
					AOS_Button_select();
					back_Button_unselect();
					ORBIT_Button_unselect();
					upbutton = 0;
				}

				else
				{
					back_Button_select();
					AOS_Button_unselect();
					ORBIT_Button_unselect();
					upbutton = 0;
				}
			}

			// If the enter is pressed
			else if (enter)
			{
				if (ledonbutton)  // If "LED ON" is selected
				{
					homemenu = 0;  // "HomeMenu" is not selected
					page2menu = 0;  // "Page2Menu" is selected
					page4menu = 0 ;
					page3menu = 1 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0; // Turn the LED ON
					enter = 0;
				}

				else if (ledoffbutton)  // If "LED OFF" is selected
				{
					homemenu = 0;  // "HomeMenu" is not selected
					page2menu = 0;  // "Page2Menu" is selected
					page4menu = 1 ;
					page3menu = 0 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0;
					enter = 0;
				}

				else if (backbutton)  // If "GO BACK" is selected
				{
					homemenu = 1;  // Go back to the home menu
					page2menu = 0;  // "Page2Menu" is selected
					page4menu = 0 ;
					page3menu = 0 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0;
					enter = 0;
				}

			}
			osDelay(1);
		}
	}
	else if (page3menu)  // If the Page 2 Menu is called
	{
		Page3Menu();  // Display the Page 2 Menu
		while (page3menu)  // While the control is in the Page Menu
		{
			if (downbutton)  // If down button is pressed
			{

				osDelay(200);
				back_Button_select2();
				downbutton = 0 ;
			}

			// If the enter is pressed
			else if (enter)
			{

				if (backbutton)  // If "GO BACK" is selected
				{
					homemenu = 0;  // "HomeMenu" is not selected
					page2menu = 1;  // "Page2Menu" is selected
					page4menu = 0 ;
					page3menu = 0 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0;
					//					enter = 0;
				}

			}
			osDelay(1);
		}
	}
	else if (page4menu)  // If the Page 2 Menu is called
	{
		Page4Menu();  // Display the Page 2 Menu
		while (page4menu)  // While the control is in the Page Menu
		{
			if (downbutton)  // If down button is pressed
			{

				osDelay(200);
				back_Button_select();
				downbutton = 0 ;
			}
			// If the enter is pressed
			else if (enter)
			{

				if (backbutton)  // If "GO BACK" is selected
				{
					homemenu = 0;  // "HomeMenu" is not selected
					page2menu = 1;  // "Page2Menu" is selected
					page4menu = 0 ;
					page3menu = 0 ;
					accmeunu = 0 ;
					testmenu = 0;  // Turn the LED ON
					enter = 0;
					enter = 0;
				}

			}
			osDelay(1);
		}
	}

}

