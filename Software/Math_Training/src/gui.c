/*
 * gui_2.0.c
 *
 *  Created on: 09/mag/2018
 *      Author: emilio
 */

#include "gui.h"
#include "simple_graphics.h"


void GUI_ShowWelcome(alt_video_display *pDisplay){
	int x, y;

	x = 7;
	y = pDisplay->height / 2 - 10;

	vid_print_string_alpha(x, y, BLUE_24, BLACK_24, tahomabold_32, pDisplay, "Math Training");

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
	else if (IsPtInRect(pt, &pDeskInfo->expression))
		ButtonId = BTN_NEWGAME;

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
	vid_print_string_alpha(rc.left+30, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, RED_24, tahomabold_20, pDisplay, "Clear");

	//disegno il pulsante di enter
	RectCopy(&rc, &(pDeskInfo->enter));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, GREEN, DO_FILL, pDisplay);
	vid_print_string_alpha(rc.left+30, rc.top+(RectHeight(&rc)-22)/2, BLACK_24, GREEN, tahomabold_20, pDisplay, "Done");

	//disegno il campo expression
	RectCopy(&rc, &(pDeskInfo->expression));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);

	//disegno il campo result
	RectCopy(&rc, &(pDeskInfo->result));
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);

}


void GUI_ClearPaintArea(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    RECT rc;
    RectCopy(&rc, &pDeskInfo->rcPaint);
    RectInflate(&rc, -1, -1);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
}


void Update_img(POINT pt, bool* img, DESK_INFO desk){
	int x_start = pt.x-DOT_SIZE;
	int y_start = pt.y-DOT_SIZE;
	int x_min = desk.rcPaint.left+1;		//9
	int y_min = desk.rcPaint.top+1;		//49

	for(int y=y_start; y<y_start+2*DOT_SIZE; y++)			//"disegno" un quadrato di lato 30 intorno al punto toccato sul touchscreen
		for(int x=x_start; x<x_start+2*DOT_SIZE; x++){
			if(x >= x_min && y >= y_min)					//per evitare numeri negativi
				img[(x-x_min)+DRAW_WIDTH*(y-y_min)]=1;		//aggiorno l'immagine tenendo conto dei bordi di rcPaint
		}
}

void Resize_image(bool* img, MNIST_Image* mnist_img){
	int row=0;
	int column=0;

	for(int k=0; k<28*28; k++)
		mnist_img->pixel[k]=0;

	for(int i=0; i<224*224; i++){
		row=i/224;
		column=i%224;
		mnist_img->pixel[(column/8)+(row/8)*28] += (uint8_t)img[i];
	}
	for(int j=0; j<28*28; j++){
		if(j%28 == 0)
			printf("\n");
		if(mnist_img->pixel[j] >= 32){
			mnist_img->pixel[j]=1;
			printf("x");
		}
		else{
			mnist_img->pixel[j]=0;
			printf("o");
		}
	}
}

void GUI(alt_video_display *pDisplay, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch){
    //const int nDotSize = DOT_SIZE;

    // Schermata iniziale
	vid_clean_screen(pDisplay, BLACK_24);
	GUI_ShowWelcome(pDisplay);
	usleep(1*1000*1000);
	vid_clean_screen(pDisplay, BLACK_24);

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);

    GUI_DeskInit(pDisplay, DeskInfo);		//imposta i vari rettangoli
    GUI_DeskDraw(pDisplay, DeskInfo);		//colora e riempie tutto

}

void ShowHelp (alt_video_display *Display, DESK_INFO *DeskInfo, TOUCH_HANDLE *pTouch){
	vid_clean_screen(Display, BLUE_24);
	vid_print_string_alpha(30, 10, WHITE_24, BLUE_24, tahomabold_20, Display, "How to...");
	vid_print_string_alpha(30, 40, WHITE_24, BLUE_24, tahomabold_20, Display, ">Draw digit");
	vid_print_string_alpha(30, 70, WHITE_24, BLUE_24, tahomabold_20, Display, ">Enter to check");
	vid_print_string_alpha(30, 100, WHITE_24, BLUE_24, tahomabold_20, Display, "  clear otherwise");
	vid_print_string_alpha(30, 130, WHITE_24, BLUE_24, tahomabold_20, Display, ">Touch result");
	vid_print_string_alpha(30, 160, WHITE_24, BLUE_24, tahomabold_20, Display, "  to validate digit");
	vid_print_string_alpha(30, 190, WHITE_24, BLUE_24, tahomabold_20, Display, "  clear otherwise");
	vid_print_string_alpha(30, 220, WHITE_24, BLUE_24, tahomabold_20, Display, ">Done to confirm");
	vid_print_string_alpha(30, 250, WHITE_24, BLUE_24, tahomabold_20, Display, ">New Game");
	while(SLIDERS_DATA_REG & 0x01){

	};
	// init GUI again
	GUI(Display, DeskInfo, pTouch);
}

