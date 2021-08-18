#include <grrlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "gfx/wii_jpg.h"
	
// RGBA Colors
#define GRRLIB_BLACK   0x000000FF
#define GRRLIB_MAROON  0x800000FF
#define GRRLIB_GREEN   0x008000FF
#define GRRLIB_OLIVE   0x808000FF
#define GRRLIB_NAVY    0x000080FF
#define GRRLIB_PURPLE  0x800080FF
#define GRRLIB_TEAL    0x008080FF
#define GRRLIB_GRAY    0x808080FF
#define GRRLIB_SILVER  0xC0C0C0FF
#define GRRLIB_RED     0xFF0000FF
#define GRRLIB_LIME    0x00FF00FF
#define GRRLIB_YELLOW  0xFFFF00FF
#define GRRLIB_BLUE    0x0000FFFF
#define GRRLIB_FUCHSIA 0xFF00FFFF
#define GRRLIB_AQUA    0x00FFFFFF
#define GRRLIB_WHITE   0xFFFFFFFF


int colours[] = {
	GRRLIB_MAROON,
	GRRLIB_GREEN,
	GRRLIB_OLIVE, 
	GRRLIB_NAVY,    
	GRRLIB_PURPLE,  
	GRRLIB_TEAL,   
	GRRLIB_GRAY,    
	GRRLIB_SILVER,  
	GRRLIB_RED,     
	GRRLIB_LIME,    
	GRRLIB_YELLOW,  
	GRRLIB_BLUE,    
	GRRLIB_FUCHSIA, 
	GRRLIB_AQUA,    
	GRRLIB_WHITE,
};
int x=0;
void newColour(){
	if (x==14){
		x=0;
	}
	x++;
}



int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();
    
    
    //load texture
    GRRLIB_texImg *tex_wii_jpg = GRRLIB_LoadTexture(wii_jpg); 
    	
    	int sn=1; //This is the 4:3 to 16:9 variable, very shitty temporary fix.
	int width=100; //this is the width of the Wii logo, the 16:9 changes the width.
    
	 if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
    	sn=0.75;
    	width=75; //the image width changes to 75 due to scaling to 75% for the 16:9 fix.	
   }
    
    // Initialise the GC Controller
    PAD_Init();
    
    
    // Initialise the Wiimotes
    WPAD_Init();
    
    int lr=1;
    int ud=1;
    int posx=100;
    int posy=100;	
    // Loop forever
    while(1) {
 	
        WPAD_ScanPads();  // Scan the Wiimotes
 	PAD_ScanPads(); //Scan the gamecube controllers
        // If [HOME] was pressed on the first Wiimote, break out of the loop
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME || PAD_ButtonsDown(0) & PAD_BUTTON_START)  break;
        // ---------------------------------------------------------------------
        
        
        //move the logo
        posx=posx+2*lr;
        posy=posy+2*ud;
        
        
        //collision detecting reverse direction.
        if (posx<0 || posx>640-width) {
        	lr=lr*-1;
        	newColour();
        }
        
        if (posy<0 || posy>480-44) {
        	ud=ud*-1;
        	newColour();
        }
        
        // for some odd reason, GRRLIB_DrawImg does not like having variables in scaling factors, so this had to be made this way.
        
        
        if (sn==1) {
        GRRLIB_DrawImg(posx, posy, tex_wii_jpg, 0, 1, 1, colours[x]);  // draw
        } else {
        	GRRLIB_DrawImg(posx, posy, tex_wii_jpg, 0, 0.75, 1, colours[x]);  // draw
        }
        
        
        
        // ---------------------------------------------------------------------
 
        GRRLIB_Render();  // Render the frame buffer to the TV
    }
 
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
 
    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
   }
