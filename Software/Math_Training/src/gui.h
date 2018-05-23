#ifndef GUI_H_
#define GUI_H_

#include <string.h>
#include "alt_video_display.h"
#include "touch_spi.h"
#include "touch_spi.h"
#include "terasic_includes.h"
#include "1lnn.h"
#include "geometry.h"

#define SLIDERS_DATA_REG (*(volatile uint32_t*) SLIDERS_BASE)

#define DOT_SIZE    15		//raggio del punto


typedef enum{
	BTN_NONE =0,
    BTN_CLEAR,		//1
    BTN_ENTER,		//2
    BTN_RESULT,		//3
    BTN_NEWGAME,	//4
    RECT_PAINT,		//5
    RECT_EXP		//6
}RECT_ID;


typedef struct{
    RECT rcPaint;						//rettangolo in cui si scrive
    RECT clear;
    RECT enter;
    RECT expression;					//rettangolo in cui compare l'espressione da risolvere
    RECT result;						//rettangolo contenente il risultato
}DESK_INFO;

void Print (RECT_ID rect, DESK_INFO desk, char string[]);
void Resize_image(bool* img, MNIST_Image* mnist_img);
void Update_img(POINT pt, bool* img, DESK_INFO desk);
void GUI_ClearRect(RECT_ID rect, DESK_INFO *pDeskInfo, alt_video_display *pDisplay);
int GUI_CheckButton(DESK_INFO *pDeskInfo, POINT *pt);
void GUI(alt_video_display *pDisplay, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch);
void ShowHelp (alt_video_display *Display, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch);

#endif /*GUI_H_*/
