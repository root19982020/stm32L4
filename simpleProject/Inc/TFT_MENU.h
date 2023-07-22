/*
 * TFT_MENU.h
 *
 *  Created on: Nov 13, 2020
 *      Author: meh
 */

#ifndef INC_TFT_MENU_H_
#define INC_TFT_MENU_H_
#include "GUI_Paint.h"
#include "fonts.h"
#include "image.h"
#include "LCD_Test.h"
//void HomeMenu (void);
//void TestMenu (void);
//void Page2Menu (void);
//void Page3Menu (void) ;
//void Page4Menu (void) ;


//void ORBIT_Button_select (void) ;
//void ORBIT_Button_unselect (void) ;
//void AOS_Button_select (void) ;
//void AOS_Button_unselect (void) ;

void Menu_Handler (void);
void AFF_task(void const * argument);
//void test (void);
int downbutton, upbutton, enter,accmeunu;
#endif /* INC_TFT_MENU_H_ */
