/*
 * gui_2.0.c
 *
 *  Created on: 09/mag/2018
 *      Author: emilio
 */

#include "terasic_includes.h"
#include "gui.h"
#include "alt_video_display.h"
#include "simple_graphics.h"
#include "geometry.h"
#include "touch_spi.h"
#include "mnist-utils.h"


#define DOT_SIZE    15


typedef enum{
    BTN_CLEAR = 0,
    BTN_ENTER,		//1
    BTN_RESULT,		//2
    BTN_NUM,		//3

    BTN_NONE
}BUTTON_ID;


typedef struct{
    RECT rcPaint;						//rettangolo in cui si scrive
    RECT clear;
    RECT enter;
    RECT expression;					//rettangolo in cui compare l'espressione da risolvere
    RECT result;						//rettangolo contenente il risultato
}DESK_INFO;


void GUI_ShowWelcome(alt_video_display *pDisplay){
	int x, y;

	x = pDisplay->width / 2 - 60;
	y = pDisplay->height / 2 - 10;

	vid_print_string_alpha(x, y, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Math Training");

}

void GUI_DeskInit(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    /*dimensioni pulsanti*/
    const int BoxH = 40;
    const int BoxW = 110;
    const int BoxW_quiz = 140;

    const int GapH = 5;
    const int GapW = 4;
    const int DrawBorder = 8;

    RectSet(&pDeskInfo->clear, DrawBorder, DrawBorder+BoxW, pDisplay->height-BoxH-GapH, pDisplay->height-GapH);

    RectSet(&pDeskInfo->enter, DrawBorder+BoxW+GapW, DrawBorder+2*BoxW+GapW, pDisplay->height-BoxH-GapH, pDisplay->height-GapH);

    RectSet(&pDeskInfo->expression, DrawBorder, DrawBorder+BoxW_quiz, GapH, GapH+BoxH);

    RectSet(&pDeskInfo->result, 12+BoxW_quiz, 12+BoxW_quiz+2*BoxH, GapH, GapH+BoxH);

    // desk drawing area
    RectSet(&pDeskInfo->rcPaint, DrawBorder, pDisplay->width-DrawBorder, BoxH+DrawBorder, pDisplay->height-DrawBorder-BoxH);

}

int GUI_CheckButton(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE;

	if (IsPtInRect(pt, &pDeskInfo->clear))
		ButtonId = BTN_CLEAR;
	else if (IsPtInRect(pt, &pDeskInfo->enter))
		ButtonId = BTN_ENTER;
	else if (IsPtInRect(pt, &pDeskInfo->result))
		ButtonId = BTN_RESULT;

    return ButtonId;
}

void GUI_DeskDraw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    RECT rc;

    // draw border (bordo bianco che delimita l'area di scrittura)
    RectCopy(&rc, &pDeskInfo->rcPaint);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);


	//disegno il pulsante di clear
	RectCopy(&rc, &(pDeskInfo->clear));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, RED_24, DO_FILL, pDisplay);
	vid_print_string_alpha(rc.left+30, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Clear");

	//disegno il pulsante di enter
	RectCopy(&rc, &(pDeskInfo->enter));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, GREEN, DO_FILL, pDisplay);
	vid_print_string_alpha(rc.left+30, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Enter");

	//disegno il campo expression
	RectCopy(&rc, &(pDeskInfo->expression));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
	vid_print_string_alpha(rc.left+30, rc.top+(RectHeight(&rc)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "2x3=");

	//disegno il campo result
	RectCopy(&rc, &(pDeskInfo->result));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);

}


bool IsContinuedPoint(POINT *ptPre, POINT *ptNew){
    bool bYes = TRUE;
    const int nMax = 50;
    if (abs(ptPre->x - ptNew->x) > nMax)
        bYes = FALSE;
    else if (abs(ptPre->y - ptNew->y) > nMax)
        bYes = FALSE;

    return bYes;
}


void GUI_ClearPaintArea(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    RECT rc;
    RectCopy(&rc, &pDeskInfo->rcPaint);
    RectInflate(&rc, -1, -1);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
}


void update_img(POINT pt, bool* img, DESK_INFO* desk){
	int x_start = pt.x-DOT_SIZE;
	int y_start = pt.y-DOT_SIZE;
	int x_min = desk->rcPaint.left+1;		//9
	int y_min = desk->rcPaint.top+1;		//49

	for(int y=y_start; y<y_start+2*DOT_SIZE; y++)			//"disegno" un quadrato di lato 30 intorno al punto toccato sul touchscreen
		for(int x=x_start; x<x_start+2*DOT_SIZE; x++){
			if(x >= x_min && y >= y_min)					//per evitare numeri negativi
				img[(x-x_min)+DRAW_WIDTH*(y-y_min)]=1;		//aggiorno l'immagine tenendo conto dei bordi di rcPaint
		}
}

void resize_image(bool* img, MNIST_Image* mnist_img){
	//int row=0;
	//int column=0;
	int average=0;

	/*for(int i=0; i<224*224; i++){
		row=i/224;
		column=i%224;
		mnist_img[(column%28)+(row/8)*28] += img[i];
	}
	for(int j=0; j<28*28; j++){
		if(j%28 == 0)
			printf("\n");
		if(mnist_img[j] >= 32){
			mnist_img[j]=1;
			printf("x");
		}
		else{
			mnist_img[j]=0;
			printf("o");
		}
	}*/
	for(int j=0; j<28; j++){
		for(int i=0; i<28; i++){
			for(int y=0; y<8; y++){
				for(int x=0; x<8; x++){
					average+=img[x+DRAW_WIDTH*y];
				}
			}
			if(average >= 32){
				average=0;
				mnist_img->pixel[i+28*j]=1;
				printf("x");
			}
			else{
				average=0;
				mnist_img->pixel[i+28*j]=0;
				printf("o");
			}
		}
	printf("\n");
	}
}

void GUI(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch){
    // video
    DESK_INFO DeskInfo;
    int X, Y;
    POINT Pt;
    const int nDotSize = DOT_SIZE;
    RECT rcTouch;
    int ColorPen, ButtonId;

    bool img[DRAW_WIDTH*DRAW_WIDTH];
    MNIST_Image mnist_img;
    bool img_ready;

    for(int i=0; i<DRAW_WIDTH*DRAW_WIDTH; i++)
    	img[i]=0;

    // Schermata iniziale
	GUI_ShowWelcome(pDisplay);
	usleep(1*1000*1000);
	vid_clean_screen(pDisplay, BLACK_24);

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);

    GUI_DeskInit(pDisplay, &DeskInfo);		//imposta i vari rettangoli
    GUI_DeskDraw(pDisplay, &DeskInfo);		//colora e riempie tutto


    RectCopy(&rcTouch, &DeskInfo.rcPaint);
    RectInflate(&rcTouch, -nDotSize-2, -nDotSize-2);

    ColorPen = WHITE_24;


    while(1){
		// touch
		if (Touch_GetXY(pTouch, &X, &Y)){

			PtSet(&Pt, X, Y);
			ButtonId = GUI_CheckButton(&DeskInfo, &Pt);

			/*se c'è tocco nell'area buona, coloro il punto toccato*/
			if ( IsPtInRect(&Pt, &rcTouch)){
				vid_draw_circle(Pt.x, Pt.y, nDotSize, ColorPen, DO_FILL, pDisplay);
				update_img(Pt, img, &DeskInfo);
			}
			/*se ho cliccato clear, pulisco*/
			else if (ButtonId == BTN_CLEAR){
				GUI_ClearPaintArea(pDisplay, &DeskInfo);
				Touch_EmptyFifo(pTouch);
			}
			/*se do conferma, avvio la rete neurale*/
			else if (ButtonId == BTN_ENTER){
				//LCD_GetImage(&DeskInfo &img, pDisplay);
				resize_image(img, &mnist_img);
				img_ready = 1;
			}

		} // if touch
    } // while

}



