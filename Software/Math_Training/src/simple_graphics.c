/***********************************************************************
 *                                                                     *
 * File:     simple_graphics.c                                         *
 *                                                                     *
 * Purpose:  Contains some useful graphics routines                    *
 *                                                                     *
 * Author:   N. Knight                                                 *
 *           Altera Corporation                                        *
 *           Apr 2006                                                  *
 **********************************************************************/
#include "terasic_includes.h"
#include <string.h>
#include <io.h>
#include "alt_video_display.h"
#include "simple_graphics.h"
#include "sys/alt_alarm.h"
#include "sys/alt_cache.h"
#include "system.h"

#define LT24

#ifdef LT24
#include "LCD.h"
#endif


// richard add
void vid_clean_screen(alt_video_display* display, int color){
    vid_paint_block(0 , 0,display->width, display->height, color, display);
}

/******************************************************************
*  Function: vid_draw_line
*
*  Purpose: Draws a line between two end points. First checks
*           to see if the line is horizontal.  If it is, it calls
*           vid_draw_horiz_line(), which is much faster than 
*           vid_draw_sloped_line.
*
******************************************************************/
__inline__ void vid_draw_line(int horiz_start, int vert_start, int horiz_end, int vert_end, int width, int color, alt_video_display* display)
{
 
  if( vert_start == vert_end )
  {
//    c2h_draw_horiz_line( (unsigned short)horiz_start, 
//                         (unsigned short)horiz_end,  
//                         (unsigned short)vert_start,
//                         color,
//                         display->buffer_ptrs[display->buffer_being_written]->buffer);

    vid_draw_horiz_line( (unsigned short)horiz_start, 
                         (unsigned short)horiz_end, 
                         (unsigned short)vert_start,
                         color,
                         display );
  }
  else
  {
    vid_draw_sloped_line( (unsigned short)horiz_start, 
                          (unsigned short)vert_start, 
                          (unsigned short)horiz_end, 
                          (unsigned short)vert_end, 
                          (unsigned short)width, 
                          color,
                          display );
  }
}


/******************************************************************
*  Function: vid_move_block
*
*  Purpose: Moves a block around the screen, backfilling with
*           the backfill_color parameter.
*
******************************************************************/

int vid_move_block(int xbegin, int ybegin, int xend, int yend, int x_distance, int y_distance, int backfill_color, alt_video_display* display)
{
  int read_x, read_y, write_x, write_y;
  short temp_pixel;

  if(x_distance <= 0 && y_distance <= 0) {
    //Move by rows because they are contiguous in memory (could help speed if in SDRAM)
    for (read_y = ybegin; read_y < yend; read_y++) {
      write_y = read_y + y_distance;
      for(read_x = xbegin; read_x < xend; read_x++) {
        write_x = read_x + x_distance;
        temp_pixel = vid_get_pixel(read_x, read_y, display);
        vid_set_pixel(write_x, write_y, temp_pixel);
        if(read_x >= xend + x_distance || read_y >= yend + y_distance)
        {
	        vid_set_pixel(read_x, read_y, backfill_color);
        }
      }
    }
  }
  return (0);
}

/******************************************************************
*  Function: vid_print_string
*
*  Purpose: Prints a string to the specified location of the screen
*           using the specified font and color.
*           Calls vid_print_char
*
******************************************************************/
int vid_print_string(int horiz_offset, int vert_offset, int color, char *font, alt_video_display* display, char string[])
{
  int i = 0;
  int original_horiz_offset;

  original_horiz_offset = horiz_offset;

  // Print until we hit the '\0' char.
  while (string[i]) {
    //Handle newline char here.
    if (string[i] == '\n') {
      horiz_offset = original_horiz_offset;
      vert_offset += 12;
      i++;
      continue;
    }
    // Lay down that character and increment our offsets.
    vid_print_char (horiz_offset, vert_offset, color, string[i], font, display);
    i++;
    horiz_offset += 8;
  }
  return (0);
}

/******************************************************************
*  Function: vid_draw_box
*
*  Purpose: Draws a box on the screen with the specified corner
*  points.  The fill parameter tells the function whether or not
*  to fill in the box.  1 = fill, 0 = do not fill.
*
******************************************************************/
int vid_draw_box (int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, alt_video_display* display)
{

  // If we want to fill in our box
  if (fill) {
     vid_paint_block (horiz_start, vert_start, horiz_end, vert_end, color, display);
  // If we're not filling in the box, just draw four lines.
  } else {
    vid_draw_line(horiz_start, vert_start, horiz_start, vert_end-1, 1, color, display);
    vid_draw_line(horiz_end-1, vert_start, horiz_end-1, vert_end-1, 1, color, display);
    vid_draw_line(horiz_start, vert_start, horiz_end-1, vert_start, 1, color, display);
    vid_draw_line(horiz_start, vert_end-1, horiz_end-1, vert_end-1, 1, color, display);
  }

  return (0);
}


/******************************************************************
*  Function: vid_print_char
*
*  Purpose: Prints a character to the specified location of the
*           screen using the specified font and color.
*
******************************************************************/

int vid_print_char (int horiz_offset, int vert_offset, int color, char character, char *font, alt_video_display* display)
{

  int i, j;
  
  char temp_char, char_row;

  // Convert the ASCII value to an array offset
  temp_char = (character - 0x20);

  //Each character is 8 pixels wide and 11 tall.
  for(i = 0; i < 11; i++) {
      char_row = *(font + (temp_char * FONT_10PT_ROW) + i);
    for (j = 0; j < 8; j++) {
      //If the font table says the pixel in this location is on for this character, then set it.
      if (char_row & (((unsigned char)0x80) >> j)) {
        vid_set_pixel((horiz_offset + j), (vert_offset + i), color); // plot the pixel
      }
    }
  }
  return(0);
}


/******************************************************************
*  Function: vid_set_pixel
*
*  Purpose: Sets the specified pixel to the specified color.
*           Sets one pixel although frame buffer consists of
*           two-pixel words.  Therefore this function is not
*           efficient when painting large areas of the screen.
*
******************************************************************/

void vid_set_pixel(int horiz, int vert, unsigned int color)
{
	alt_u16 color16;


	// encode to RGB  5 6 5
	color16 = (color & 0xFF) >> 3; // blue
	color16 |= (color & 0xFC00) >> 5; // green
	color16 |= (color & 0xF80000) >> 8; // blue
	LCD_DrawPoint(horiz, vert, color16);

}

/******************************************************************
*  Function: vid_get_pixel
*
*  Purpose: Reads the color of the pixel at the given coordinates
*
******************************************************************/
short vid_get_pixel(int horiz, int vert, alt_video_display* display)
{
	printf("vid_get_pixel is not implemented\r\n");
	/*LCD_ReadPoint(horiz, vert);*/
	return 0;
}


/******************************************************************
*  Function: vid_paint_block
*
*  Purpose: Paints a block of the screen the specified color.
*           Note: works with two pixels at a time to maintain high
*           bandwidth.  Therefore, corner points must be even
*           numbered coordinates.  Use vid_draw_solid_box() for
*           areas with odd-numbered corner points.
*           The color parameter must contain two pixel's worth
*           (32 bits).
*
******************************************************************/
void vid_paint_block (int Hstart,int Vstart, int Hend, int Vend, int color, alt_video_display* display)
{
	int x,y;
	for(y=Vstart;y<Vend;y++){
		for(x=Hstart;x<Hend;x++){
			vid_set_pixel(x, y, color);
		}
	}
}


/******************************************************************
*  Function: vid_draw_horiz_line
*
*  Purpose: Draws a horizontal line on the screen quickly.
*           Good for filling stuff.
*
******************************************************************/
void vid_draw_horiz_line (short Hstart, short Hend, int V, int color, alt_video_display* display)
{
	int x;
	for(x=Hstart;x<Hend;x++){
		vid_set_pixel(x, V, color);
	}
}



/******************************************************************
*  Function: vid_merge_colors
*
*  Purpose: Takes 5-bit color values for each red, green, and blue
*           and merges them into one 16-bit color value.
*
******************************************************************/
int vid_merge_colors(int red, int green, int blue)
{
  // Green actually has 6-bits, but we'll make it's LSB 1 to be consistent.
  return ((blue) | (((green << 1) | 0x1) << 5) | (red << 11));
}

/******************************************************************
*  Function: vid_color_convert24_16
*
*  Purpose: Takes a pointer to a 24-bit (3-byte) 24-bit RGB color 
*           sample and converts it to 16-bits, displayable by the 
*           VGA controller in 16-bit mode
*
******************************************************************/
unsigned short vid_color_convert24_16(char* color24)
{
	unsigned char red, green, blue;
	unsigned short output;
	
	red = *(color24 + 0) & 0xF8;
	green = *(color24 + 1) & 0xFC; // green is actualy 6 bits
	blue = *(color24 + 2) & 0xF8;

	output = ((blue >> 3) | (green << 3) | (red << 8));  
	return output;
}


/******************************************************************
*  Function: vid_color_convert24_16
*
*  Purpose: Takes a pointer to a 24-bit (3-byte) 24-bit RGB color 
*           sample and converts it to 16-bits, displayable by the 
*           VGA controller in 16-bit mode
*
******************************************************************/
int vid_color_convert16_24(unsigned short color16, char* color24)
{
	*(color24 + 0) = color16 >> 11;
	*(color24 + 1) = ((color16 & 0x3E) >> 5);
	*(color24 + 2) = (color16 & 0x1F);
	
	return (0);
}


/******************************************************************
*  Function: vid_draw_sloped_line
*
*  Purpose: Draws a line between two end points using
*           Bresenham's line drawing algorithm.
*           width parameter is not used.  
*           It is reserved for future use.
*
******************************************************************/
void vid_draw_sloped_line( unsigned short horiz_start, 
                           unsigned short vert_start, 
                           unsigned short horiz_end, 
                           unsigned short vert_end, 
                           unsigned short width, 
                           int color, 
                           alt_video_display* display)
{
  // Find the vertical and horizontal distance between the two points
  int horiz_delta = abs(horiz_end-horiz_start);
  int vert_delta = abs(vert_end-vert_start);

  // Find out what direction we are going
  int horiz_incr, vert_incr;
  if (horiz_start > horiz_end) { horiz_incr=-1; } else { horiz_incr=1; }
  if (vert_start > vert_end) { vert_incr=-1; } else { vert_incr=1; }

  // Find out which axis is always incremented when drawing the line
  // If it's the horizontal axis
  if (horiz_delta >= vert_delta) {
    int dPr   = vert_delta<<1;
    int dPru  = dPr - (horiz_delta<<1);
    int P     = dPr - horiz_delta;

    // Process the line, one horizontal point at at time
    for (; horiz_delta >= 0; horiz_delta--) {
      // plot the pixel
      vid_set_pixel(horiz_start, vert_start, color);
      // If we're moving both up and right
      if (P > 0) {
        horiz_start+=horiz_incr;
        vert_start+=vert_incr;
        P+=dPru;
      } else {
        horiz_start+=horiz_incr;
        P+=dPr;
      }
    }
  // If it's the vertical axis
  } else {
    int dPr   = horiz_delta<<1;
    int dPru  = dPr - (vert_delta<<1);
    int P     = dPr - vert_delta;

    // Process the line, one vertical point at at time
    for (; vert_delta>=0; vert_delta--) {
      // plot the pixel
      vid_set_pixel(horiz_start, vert_start, color);
      // If we're moving both up and right
      if (P > 0) {
        horiz_start+=horiz_incr;
        vert_start+=vert_incr;
        P+=dPru;
      } else {
        vert_start+=vert_incr;
        P+=dPr;
      }
    }
  }
}


/******************************************************************
*  Function: vid_draw_circle
*
*  Purpose: Draws a circle on the screen with the specified center
*  and radius.  Draws symetric circles only.  The fill parameter
*  tells the function whether or not to fill in the box.  1 = fill,
*  0 = do not fill.
*
******************************************************************/
int vid_draw_circle(int Hcenter, int Vcenter, int radius, int color, char fill, alt_video_display* display)
{
  int x = 0;
  int y = radius;
  int p = (5 - radius*4)/4;

  // Start the circle with the top, bottom, left, and right pixels.
  vid_round_corner_points(Hcenter, Vcenter, x, y, 0, 0, color, fill, display);

  // Now start moving out from those points until the lines meet
  while (x < y) {
    x++;
    if (p < 0) {
      p += 2*x+1;
    } else {
      y--;
      p += 2*(x-y)+1;
    }
    vid_round_corner_points(Hcenter, Vcenter, x, y, 0, 0, color, fill, display);
  }
  return (0);
}


/******************************************************************
*  Function: vid_draw_round_corner_box
*
*  Purpose: Draws a box on the screen with the specified corner
*  points.  The fill parameter tells the function whether or not
*  to fill in the box.  1 = fill, 0 = do not fill.
*
******************************************************************/
int vid_draw_round_corner_box ( int horiz_start, int vert_start, int horiz_end, int vert_end, 
                                int radius, int color, int fill, alt_video_display* display)
{
  unsigned int x, y;
  int p;
  int diameter;
  int temp;
  unsigned int width, height, straight_width, straight_height;

  // Make sure the start point us up and left of the end point
  if( horiz_start > horiz_end )
  {
    temp = horiz_end;
    horiz_end = horiz_start;
    horiz_start = temp;
  }
  
  if( vert_start > vert_end )
  {
    temp = vert_end;
    vert_end = vert_start;
    vert_start = temp;
  }
  
  // These are the overall dimensions of the box
  width = horiz_end - horiz_start;
  height = vert_end - vert_start;

  // Make sure our radius isnt more than the shortest dimension 
  // of the box, or it'll screw us all up
  if( radius > ( width / 2 ))
    radius = width / 2;

  if( radius > ( height / 2 ))
    radius = height / 2;
  
  // We use the diameter for some calculations, so we'll pre calculate it here.
  diameter = ( radius * 2 );

  // These are the lengths of the straight portions of the box edges.
  straight_width = width - diameter;
  straight_height = height - diameter;

  x = 0;
  y = radius;
  p = (5 - radius*4)/4;
   
  // Start the corners with the top, bottom, left, and right pixels.
  vid_round_corner_points( horiz_start + radius, vert_start + radius, x, y, 
                           straight_width, straight_height, color, fill, display );
  
  // Now start moving out from those points until the lines meet
  while (x < y) {
    x++;
    if (p < 0) {
      p += 2*x+1;
    } else {
      y--;
      p += 2*(x-y)+1;
    }
    vid_round_corner_points( horiz_start + radius, vert_start + radius, x, y, 
                             straight_width, straight_height, color, fill, display);
  }

  // If we want to fill in our box
  if (fill) {
     vid_paint_block (horiz_start, vert_start + radius, horiz_end, vert_end - radius, color, display);
  // If we're not filling in the box, just draw four lines.
  } else {
    vid_draw_line(horiz_start, vert_start + radius, horiz_start, vert_end - radius , 1, color, display); //left
    vid_draw_line(horiz_end, vert_start + radius, horiz_end, vert_end - radius , 1, color, display); //right
    vid_draw_line(horiz_start + radius, vert_start, horiz_end - radius , vert_start, 1, color, display); //top
    vid_draw_line(horiz_start + radius, vert_end, horiz_end - radius , vert_end, 1, color, display); //bottom
  }

  return (0);
}


/******************************************************************
*  Function: vid_round_corner_points
*
*  Purpose: Called by vid_draw_round_corner_box() and 
*  vid_draw_circle() to plot the actual points of the round corners.
*  Draws horizontal lines to fill the shape.
*
******************************************************************/

void vid_round_corner_points( int cx, int cy, int x, int y, 
                              int straight_width, int straight_height, int color, 
                              char fill, alt_video_display* display)
{

    // If we're directly above, below, left and right of center (0 degrees), plot those 4 pixels
    if (x == 0) {
        // bottom
        vid_set_pixel(cx, cy + y + straight_height, color);
        vid_set_pixel(cx + straight_width, cy + y + straight_height, color);
        // top
        vid_set_pixel(cx, cy - y, color);
        vid_set_pixel(cx + straight_width, cy - y, color);

        if(fill) {
          vid_draw_line(cx - y, cy, cx + y + straight_width, cy, 1, color, display);
          vid_draw_line(cx - y, cy + straight_height, cx + y + straight_width, cy + straight_height, 1, color, display);
        } else {
          //right
          vid_set_pixel(cx + y + straight_width, cy, color);
          vid_set_pixel(cx + y + straight_width, cy + straight_height, color);
          //left
          vid_set_pixel(cx - y, cy, color);
          vid_set_pixel(cx - y, cy + straight_height, color);
        }

    } else
    // If we've reached the 45 degree points (x=y), plot those 4 pixels
    if (x == y) {
      if(fill) {
        vid_draw_line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color, display); // lower
        vid_draw_line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color, display); // upper
        
      } else {
        vid_set_pixel(cx + x + straight_width, cy + y + straight_height, color); // bottom right
        vid_set_pixel(cx - x, cy + y + straight_height, color); // bottom left
        vid_set_pixel(cx + x + straight_width, cy - y, color); // top right
        vid_set_pixel(cx - x, cy - y, color); // top left
      }
    } else
    // If we're between 0 and 45 degrees plot 8 pixels.
    if (x < y) {
        if(fill) {
          vid_draw_line(cx - x, cy + y + straight_height, cx + x + straight_width, cy + y + straight_height, 1, color, display);
          vid_draw_line(cx - y, cy + x + straight_height, cx + y + straight_width, cy + x + straight_height, 1, color, display);
          vid_draw_line(cx - y, cy - x, cx + y + straight_width, cy - x, 1, color, display);
          vid_draw_line(cx - x, cy - y, cx + x + straight_width, cy - y, 1, color, display);
        } else {
          vid_set_pixel(cx + x + straight_width, cy + y + straight_height, color);
          vid_set_pixel(cx - x, cy + y + straight_height, color);
          vid_set_pixel(cx + x + straight_width, cy - y, color);
          vid_set_pixel(cx - x, cy - y, color);
          vid_set_pixel(cx + y + straight_width, cy + x + straight_height, color);
          vid_set_pixel(cx - y, cy + x + straight_height, color);
          vid_set_pixel(cx + y + straight_width, cy - x, color);
          vid_set_pixel(cx - y, cy - x, color);
        }
    }
}


/******************************************************************
*  Function: min3
*
*  Purpose:  Returns the minimum value of 3 parameters
*            Used for drawing filled shapes
*
******************************************************************/
int max3( int a, int b, int c )
{
  if( a < b )
    a = b;
  if( a < c )
    a = c;
  
  return a;
}

/******************************************************************
*  Function: min3
*
*  Purpose:  Returns the minimum value of 3 parameters
*            Used for drawing filled shapes.
*
******************************************************************/
int min3( int a, int b, int c )
{
  if( a > b )
    a = b;
  if( a > c )
    a = c;
  
  return a;
}

/******************************************************************
*  Function: max_diff3
*
*  Purpose:  Returns the positive max difference between 3 
*            parameters.  Used for drawing filled shapes
*
******************************************************************/
int max_diff3(int a, int b, int c)
{
  int max, min;
    
  max = max3( a, b, c );
  min = min3( a, b, c );
  return (max - min);
}



