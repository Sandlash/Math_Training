/*
 * ILI9341.h
 *
 *  Created on: 2014-2-13
 *      Author: Administrator
 */

#ifndef ILI9341_H_
#define ILI9341_H_

//#include "system.h"
//#include "type.h"
//#include "altera_avalon_pio_regs.h"
#include "terasic_includes.h"

void LCD_Init();
void LCD_Clear(alt_u16 Color);
void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color );
//void LCD_ReadPoint(DESK_INFO *desk, uint16_t *img, alt_video_display *display);

#endif /* ILI9341_H_ */
