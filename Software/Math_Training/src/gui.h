#ifndef GUI_H_
#define GUI_H_

#include "alt_video_display.h"
#include "touch_spi.h"
#include "touch_spi.h"
#include "terasic_includes.h"
#include "mnist-utils.h"
#include "1lnn.h"
#include "geometry.h"

#define SLIDERS_DATA_REG (*(volatile uint32_t*) SLIDERS_BASE)

#define DOT_SIZE    15		//raggio del punto


typedef enum{
    BTN_CLEAR = 0,
    BTN_ENTER,		//1
    BTN_RESULT,		//2
    BTN_NEWGAME,	//3
    BTN_NUM,		//4

    BTN_NONE
}BUTTON_ID;


typedef struct{
    RECT rcPaint;						//rettangolo in cui si scrive
    RECT clear;
    RECT enter;
    RECT expression;					//rettangolo in cui compare l'espressione da risolvere
    RECT result;						//rettangolo contenente il risultato
}DESK_INFO;

void Resize_image(bool* img, MNIST_Image* mnist_img);
void Update_img(POINT pt, bool* img, DESK_INFO desk);
void GUI_ClearPaintArea(alt_video_display *pDisplay, DESK_INFO *pDeskInfo);
int GUI_CheckButton(DESK_INFO *pDeskInfo, POINT *pt);
void GUI(alt_video_display *pDisplay, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch);
void ShowHelp (alt_video_display *Display, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch);

#endif /*GUI_H_*/
