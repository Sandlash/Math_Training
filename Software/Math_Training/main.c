/**
 * @file main.c
 *
 * @mainpage MNIST 1-Layer Neural Network
 *
 * @brief Main characteristics: Only 1 layer (= input layer), no hidden layer.  Feed-forward only.
 * No Sigmoid activation function. No back propagation.\n
 *
 * @details Learning is achieved simply by incrementally updating the connection weights based on the comparison
 * with the desired target output (supervised learning).\n
 *
 * Its performance (success rate) of 85% is far off the state-of-the-art techniques (surprise, surprise) 
 * but close the Yann Lecun's 88% when using only a linear classifier.
 *
 * @see [Simple 1-Layer Neural Network for MNIST Handwriting Recognition](http://mmlind.github.io/Simple_1-Layer_Neural_Network_for_MNIST_Handwriting_Recognition/)
 * @see http://yann.lecun.com/exdb/mnist/
 * @version [Github Project Page](http://github.com/mmlind/mnist-1lnn/)
 * @author [Matt Lind](http://mmlind.github.io)
 * @date July 2015
 *
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "src/mnist-utils.h"
#include "src/1lnn.h"

#include "src/LCD.h"
#include "src/gui.h"
#include "src/touch_spi.h"
#include "src/simple_graphics.h"

#include "src/random_gen.h"

#include "system.h"

typedef enum{
	NO_IMG=0,
	IMG_READY,
	CONFIRM,
	FINISHED,
	NEW_GAME
}STATUS;

int main() {
	STATUS status=NO_IMG;

    Layer nn_layer;
    DESK_INFO DeskInfo;
    TOUCH_HANDLE *pTouch;
    alt_video_display Display;

    RECT rcTouch;
	POINT Pt;

	int X, Y;
	int ButtonId;
	MNIST_Image mnist_img;
	bool img[DRAW_WIDTH*DRAW_WIDTH];

	expression exp;
	uint8_t result[3]={0,0,0};			//risultato immesso dall'utente
	uint8_t answer=0;
	uint8_t digits=0;					//numero di cifre inserite dall'utente
	bool red_digit=0;					//flag di presenza di cifre rosse (non confermate)
	bool writing=FALSE;

	for(int i=0; i<DRAW_WIDTH*DRAW_WIDTH; i++)
	    img[i]=0;

    initLayer(&nn_layer);

    usleep(40000);

   // init touch
   pTouch = Touch_Init(TOUCH_PANEL_SPI_BASE, TOUCH_PANEL_PEN_IRQ_N_BASE, TOUCH_PANEL_PEN_IRQ_N_IRQ);
   if (!pTouch)
	   printf("Failed to init touch\r\n");
   else
	   printf("Init touch successfully\r\n");


   // init LCD
   LCD_Init();
   LCD_Clear(0X0000);

   // init display
   Display.interlace = 0;
   Display.bytes_per_pixel = 2;
   Display.color_depth = 16;
   Display.height = SCREEN_HEIGHT;
   Display.width = SCREEN_WIDTH;

   // init GUI
   GUI(&Display, &DeskInfo, pTouch);
   RectCopy(&rcTouch, &DeskInfo.rcPaint);
   RectInflate(&rcTouch, -DOT_SIZE-2, -DOT_SIZE-2);


   exp=NewExpression();
   char expression[] = {exp.value1/10==0 ? '_':(char)(exp.value1/10+48),
      									 (char)(exp.value1%10+48),
      									 exp.operator,
      									 exp.value2/10==0 ? '_':(char)(exp.value2/10+48),
      									 (char)(exp.value2%10+48), '=', '\0'};
   vid_print_string_alpha(DeskInfo.expression.left+30, DeskInfo.expression.top+9, BLACK_24, WHITE_24, tahomabold_20, &Display, expression);

   while(1){
	   if(SLIDERS_DATA_REG & 0x01){
		   ShowHelp(&Display, &DeskInfo, pTouch);
		   //exp=NewExpression();
		   char expression[] = {exp.value1/10==0 ? '_':(char)(exp.value1/10+48),
		      									 (char)(exp.value1%10+48),
		      									 exp.operator,
		      									 exp.value2/10==0 ? '_':(char)(exp.value2/10+48),
		      									 (char)(exp.value2%10+48), '=', '\0'};
		   vid_print_string_alpha(DeskInfo.expression.left+30, DeskInfo.expression.top+9, BLACK_24, WHITE_24, tahomabold_20, &Display, expression);
	   }

   		if (Touch_GetXY(pTouch, &X, &Y)){

   			PtSet(&Pt, X, Y);
   			ButtonId = GUI_CheckButton(&DeskInfo, &Pt);

   			/*se c'è tocco nell'area buona, coloro il punto toccato*/
   			if(IsPtInRect(&Pt, &rcTouch)){
   				vid_draw_circle(Pt.x, Pt.y, DOT_SIZE, WHITE_24, DO_FILL, &Display);
   				Update_img(Pt, img, DeskInfo);
   				if(writing==FALSE){
   					vid_draw_box (DeskInfo.enter.left, DeskInfo.enter.top, DeskInfo.enter.right, DeskInfo.enter.bottom, GREEN, DO_FILL, &Display);
   					vid_print_string_alpha(DeskInfo.enter.left+30, DeskInfo.enter.top+9, BLACK_24, GREEN, tahomabold_20, &Display, "Enter");
   				}

   				writing=TRUE;
   			}

   			/*se ho cliccato clear, pulisco*/
   			else if (ButtonId == BTN_CLEAR){
   				if(status != NEW_GAME)
   					status=NO_IMG;
   				GUI_ClearPaintArea(&Display, &DeskInfo);
   				Touch_EmptyFifo(pTouch);

				if(red_digit){
					vid_draw_box (DeskInfo.result.left, DeskInfo.result.top, DeskInfo.result.right, DeskInfo.result.bottom, WHITE_24, DO_FILL, &Display);
					for(int n=0; n<digits; n++)
						vid_print_char_alpha(DeskInfo.result.left+20+13*n, DeskInfo.result.top+9, BLACK_24, (char)(result[n]+48), WHITE_24, tahomabold_20, &Display);
				}
				for(int i=0; i<DRAW_WIDTH*DRAW_WIDTH; i++)
				   	img[i]=0;

   			}

   			/*se do conferma, avvio la rete neurale*/
   			else if (ButtonId == BTN_ENTER && status==NO_IMG){

   				if(!red_digit && writing==FALSE){				//confermo il risultato finale
   					switch(digits){
   						case 0:
   							break;
   						case 1:
   							answer=result[0];
   							break;
   						case 2:
   							answer = result[0]*10+result[1];
   							break;
   						case 3:
							answer = result[0]*100+result[1]*10+result[2];
							break;
   					}

   	   				GUI_ClearPaintArea(&Display, &DeskInfo);
   	   				if(answer==exp.answer && digits!=0)
   	   					vid_print_string_alpha(DeskInfo.rcPaint.left+75, DeskInfo.rcPaint.top+90, GREEN, BLACK_24, tahomabold_32, &Display, "Nice");
   	   				else
   	   					vid_print_string_alpha(DeskInfo.rcPaint.left+75, DeskInfo.rcPaint.top+80, RED_24, BLACK_24, tahomabold_32, &Display, "Nope");
   	   				status=FINISHED;
				}
   				else{
   					if(digits>=3){
   						status=FINISHED;
   	   	   				GUI_ClearPaintArea(&Display, &DeskInfo);
   	   					vid_print_string_alpha(DeskInfo.rcPaint.left+30, DeskInfo.rcPaint.top+80, PINK_24, BLACK_24, tahomabold_32, &Display, "Un c'entra!");
   					}
   					else{
						Resize_image(img, &mnist_img);				//catturo l'immagine da mandare alla rete neurale
						status = IMG_READY;
						for(int i=0; i<DRAW_WIDTH*DRAW_WIDTH; i++)
							img[i]=0;
   					}
   				}
   			}

   			else if(ButtonId==BTN_RESULT && status==CONFIRM){
   				writing=FALSE;
				red_digit=FALSE;
	   			GUI_ClearPaintArea(&Display, &DeskInfo);
				vid_print_char_alpha(DeskInfo.result.left+20+13*digits, DeskInfo.result.top+9, BLACK_24, (char)(result[digits]+48), WHITE_24, tahomabold_20, &Display);
				digits++;
				status=NO_IMG;
				vid_draw_box (DeskInfo.enter.left, DeskInfo.enter.top, DeskInfo.enter.right, DeskInfo.enter.bottom, GREEN, DO_FILL, &Display);
				vid_print_string_alpha(DeskInfo.enter.left+30, DeskInfo.enter.top+9, BLACK_24, GREEN, tahomabold_20, &Display, "Done");
   			}

   			else if(ButtonId==BTN_NEWGAME && status==NEW_GAME){
   				status=NO_IMG;
   				exp=NewExpression();
   				char expression[] = {exp.value1/10==0 ? '_':(char)(exp.value1/10+48),
   									 (char)(exp.value1%10+48),
   									 exp.operator,
   									 exp.value2/10==0 ? '_':(char)(exp.value2/10+48),
   									 (char)(exp.value2%10+48), '=', '\0'};
   				vid_draw_box (DeskInfo.expression.left, DeskInfo.expression.top, DeskInfo.expression.right, DeskInfo.expression.bottom, WHITE_24, DO_FILL, &Display);
   				vid_print_string_alpha(DeskInfo.expression.left+30, DeskInfo.expression.top+9, BLACK_24, WHITE_24, tahomabold_20, &Display, expression);
   				vid_draw_box (DeskInfo.result.left, DeskInfo.result.top, DeskInfo.result.right, DeskInfo.result.bottom, WHITE_24, DO_FILL, &Display);
	   			GUI_ClearPaintArea(&Display, &DeskInfo);
	   			result[0]=0;
	   			result[1]=0;
	   			result[2]=0;
	   			digits=0;
   			}

   		} // if touch

   		if(status==IMG_READY){
   			result[digits]=testLayer(&nn_layer, &mnist_img);
   			/*visualizzo in rosso la cifra intuita dalla rete neurale*/
   			vid_print_char_alpha(DeskInfo.result.left+20+13*digits, DeskInfo.result.top+9, RED_24, (char)(result[digits]+48), WHITE_24, tahomabold_20, &Display);
   			status=CONFIRM;
   			red_digit=TRUE;
   		}

   		if(status==FINISHED){
   			red_digit=FALSE;
   			writing=FALSE;
   			status=NEW_GAME;
			vid_draw_box (DeskInfo.expression.left, DeskInfo.expression.top, DeskInfo.expression.right, DeskInfo.expression.bottom, YELLOW_24, DO_FILL, &Display);
			vid_print_string_alpha(DeskInfo.expression.left+10, DeskInfo.expression.top+9, BLACK_24, YELLOW_24, tahomabold_20, &Display, "New game!");
   		}

   } // while
   return 0;
}


