#ifndef GUI_H_
#define GUI_H_

#include "alt_video_display.h"
#include "touch_spi.h"
#include "geometry.h"
#include "1lnn.h"

void GUI(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch, Layer *nn_layer);

/*typedef enum{
    BTN_CLEAR = 0,
    BTN_WHITE,		//1
    BTN_BLUE,		//2
    BTN_GREEN,		//3
    BTN_RED,		//4
    BTN_NUM,		//5

    BTN_NONE
}BUTTON_ID;

typedef struct{
    RECT rc;
    int  color;
}PALETTE_INFO;

typedef struct{
    int Paint_Index;					//indice pulsante (colore) selezionato
    RECT rcPaint;						//rettangolo in cui si scrive
    PALETTE_INFO szPalette[BTN_NUM];	//vettore di pulsanti colorati
}DESK_INFO;*/

#endif /*GUI_H_*/
