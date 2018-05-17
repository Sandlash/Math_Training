#include "terasic_includes.h"
#include "simple_text.h"
#include "simple_graphics.h"

#define LT24

/******************************************************************
*  Function: vid_print_string_alpha
*
*  Purpose: Prints a string to the specified location of the screen
*           using the specified font and color.
*           Calls vid_print_char_alpha
*
*           The color formats are as follows:
*           16 bit - R(5):G(6):B(5)  (packed)
*           24 bit - R(8):G(8):B(8)  (packed)
*           32 bit - 0 : R(8):G(8):B(8) (unpacked)
******************************************************************/

int vid_print_string_alpha(int horiz_offset, int vert_offset, int color, int background_color, struct abc_font_struct font[], alt_video_display * display, char string[])
{
  int i = 0;
  int tab;
  int original_horiz_offset;

  original_horiz_offset = horiz_offset;

  // Print until we hit the '\0' char.
  while (string[i]) {
    //Handle newline char here.
    if (string[i] == '\n') {
      horiz_offset = original_horiz_offset;
      vert_offset += font['|' - 33].bounds_height;  // we'll use "|" to add the line to line spacing
      i++;
      continue;
    }
    // Lay down that character and increment our offsets.
    if(string[i] == 32)  // this is a space in ASCII
    {
      if(background_color != CLEAR_BACKGROUND)  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
      {
        vid_print_char_alpha (horiz_offset, vert_offset, background_color, '-', background_color, font, display);
      }
      horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
    }
    else if(string[i] == '\t')  // this is a tab
    {
      for( tab = 0; tab < TAB_SPACING; tab++ )
      {
        if(background_color != CLEAR_BACKGROUND)  // need to fill in this spot (setting foreground to background color so that it looks like a drawn in box)
        {
          vid_print_char_alpha (horiz_offset, vert_offset, background_color, '-', background_color, font, display);
        }
        horiz_offset += font[45 - 33].bounds_width;  // using the width of the '-' character for the space width since it's not present in the font
      }
    }
    else
    {
      vid_print_char_alpha (horiz_offset, vert_offset, color, string[i], background_color, font, display);
      horiz_offset += font[string[i] - 33].bounds_width;
    }
    i++;
  }
  return (0);
}





/******************************************************************
*  Function: seperate_colour_channels
* 
*  Author:  JCJB
*
*  Purpose: Takes a single colour pointer and break the value into
*           seperate channels.  It supports 16/24(packed/unpacked)
*           formats as inputs.  The results are passed back by
*           the function working on pointers.
*
******************************************************************/

int seperate_color_channels(int color_depth, unsigned char * color, unsigned char * red, unsigned char * green, unsigned char * blue)
{
  int return_value;  // error checking in case anyone is listening
  unsigned short temp_color = 0;
  
  if (color_depth == 16)  // R(5):G(6):B(5)
  {
    temp_color = *color++;        // get the first half of the 16 bit color
    temp_color += (*color << 8);  // get the second half of the 16 bit color
    *blue = temp_color & 0x1F; 
    *green = (temp_color >> 5) & 0x3F;
    *red = (temp_color >> 11) & 0x1F;
    return_value = 1;
  }
  else if ((color_depth == 24) || (color_depth == 32))  // R(8):G(8):B(8) packed/unpacked
  {
    *blue = *color++;
    *green = *color++;
    *red = *color;   
    return_value = 1;
  }
  else  // not a valid color depth for this function
  {
    *red = 0;
    *green = 0;
    *blue = 0;    
    return_value = 0;    
  }
  return return_value;
}

/******************************************************************
*  Function: merge_color_channels
* 
*  Author:  JCJB
*
*  Purpose: Takes the seperate RGB channels and merges them into
*           a single 16/24/32 bit location.  The caller must
*           use an unsigned char * pointer to the destination
*           color allowing the same function to be used for
*           different color formats and packing.
*
******************************************************************/

int merge_color_channels(int color_depth, unsigned char red, unsigned char green, unsigned char blue, unsigned char * color)
{
  int return_value;  // error checking in case anyone is listening
  unsigned short temp_color;
    
  if (color_depth == 16)  // R(5):G(6):B(5)
  {
    temp_color = (blue & 0x1F) | ((green & 0x3F) << 5) | ((red & 0x1F) << 11);  // 16 bit color format
    *color++ = temp_color & 0xFF;
    *color = (temp_color >> 8) & 0xFF;
    return_value = 1;
  }
  else if ((color_depth == 24) || (color_depth == 32))  // R(8):G(8):B(8) packed/unpacked
  {
    *color++ = blue;
    *color++ = green;
    *color = red;
    return_value = 1;
  }
  else  // not a valid color depth for this function
  {
    *color++ = 0;
    *color++ = 0;
    *color = 0;
    return_value = 0;    
  }
  return return_value;  
}


/******************************************************************
*  Function: read_from_frame
* 
*  Author:  JCJB
*
*  Purpose: Reads a pixel value from an existing frame buffer and
*           separates the color channels.  This is useful for alpha
*           blending where the previously rendored pixel value is
*           needed to calculate the new one.
*
******************************************************************/

int read_from_frame (int horiz, int vert, unsigned char *red, unsigned char *green, unsigned char *blue, alt_video_display * display)
{
	printf("read_from_frame is not implemented!\r\n");
	return -1;
}


/******************************************************************
*  Function: alpha_blending
*
*  Purpose: Takes in the alpha value (0-255) and uses that to mix
*           the font color with the background color.
*
******************************************************************/

int alpha_blending (int horiz_offset, int vert_offset, int background_color, unsigned char alpha, unsigned char *red, unsigned char *green, unsigned char *blue, alt_video_display * display)
{
  int return_value;
  unsigned char background_red, background_green, background_blue;
  unsigned int blended_red, blended_green, blended_blue;  // short since there may be overflow to be clipped
  
  // set red, green, and blue of the background color
  if(background_color == CLEAR_BACKGROUND)
  {
    read_from_frame (horiz_offset, vert_offset, &background_red, &background_green, &background_blue, display);
  }
  else
  {
    seperate_color_channels(display->color_depth, (unsigned char *)&background_color, &background_red, &background_green, &background_blue);
  }

  // these blended colors may need to be clipped to the maximum amounts the color depth supports
  blended_red = (((*red) * alpha) + (background_red * (255 - alpha)))/255;
  blended_green = (((*green) * alpha) + (background_green * (255 - alpha)))/255;
  blended_blue = (((*blue) * alpha) + (background_blue * (255 - alpha)))/255;
  

  if(display->color_depth == 16)
  {
    *red = (blended_red > 0x1F)? 0x1F: blended_red;
    *green = (blended_green > 0x3F)? 0x3F: blended_green;
    *blue = (blended_blue > 0x1F)? 0x1F: blended_blue; 
    return_value = 1;        
  }
  else if((display->color_depth == 24) || (display->color_depth == 32))
  {
    *red = (blended_red > 0xFF)? 0xFF: blended_red;
    *green = (blended_green > 0xFF)? 0xFF: blended_green;
    *blue = (blended_blue > 0xFF)? 0xFF: blended_blue; 
    return_value = 1;            
  }
  else
  {
    return_value = 0;     
  }  
 
  return return_value;
} 



/******************************************************************
*  Function: vid_print_char_alpha
*
*  Purpose: Prints a character to the specified location of the
*           screen using the specified font and color.
*
******************************************************************/

int vid_print_char_alpha (int horiz_offset, int vert_offset, int color, char character, int background_color, struct abc_font_struct font[], alt_video_display * display)
{
  int i, j;
  unsigned char * alpha;

  // Assign the pointer of the font bitmap
  alpha = font[character-33].char_alpha_map;

  for(i = 0; i < font[character-33].bounds_height; i++) {
    for (j = 0; j < font[character-33].bounds_width; j++) {

    	if (*alpha >= 0x80){
    		//center pixels filled with color
    		vid_set_pixel((horiz_offset + j), (vert_offset + i), color, display);

    		alpha++;
		}
    	else{
    		//extern pixels filled with background_color
		    vid_set_pixel((horiz_offset + j), (vert_offset + i), background_color, display);

		    alpha++;
    	}
    }
  }

  return(0);
}

/******************************************************************
*  Function: vid_string_pixel_length_alpha
*
*  Purpose: Gets the length in pixels of a string in the specified
*           font.  Useful for centering text, and determining the
*           appropriate size of a text box or button.
*
******************************************************************/
int vid_string_pixel_length_alpha( struct abc_font_struct font[], char string[] )
{
  int i = 0;
  int length = 0;

  // Count the length of the string
  for( i = 0; string[i] != '\0'; i++ )
  {
    if( string[i] == ' ' )
    {
      // Here we just add the length of the '-' character
      length += font[45 - 33].bounds_width;
    }
    else
    {
      length += font[string[i] - 33].bounds_width;
    }
  }
  return( length );
}

