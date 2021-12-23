#include <grrlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include "mxml.h"
#include "gfx/wii_jpg.h"
#include "gfx/GC_img.h"
#include "gfx/DVD_img.h"
// RGBA Colours
<<<<<<< HEAD
#define GRRLIB_BLACK    0x000000FF
#define GRRLIB_MAROON   0x800000FF
#define GRRLIB_GREEN    0x008000FF
#define GRRLIB_OLIVE    0x808000FF
#define GRRLIB_NAVY     0x000080FF
#define GRRLIB_PURPLE   0x800080FF
#define GRRLIB_TEAL     0x008080FF
#define GRRLIB_GRAY     0x808080FF
#define GRRLIB_SILVER   0xC0C0C0FF
#define GRRLIB_RED      0xFF0000FF
#define GRRLIB_LIME     0x00FF00FF
#define GRRLIB_YELLOW   0xFFFF00FF
#define GRRLIB_BLUE     0x0000FFFF
#define GRRLIB_FUCHSIA  0xFF00FFFF
#define GRRLIB_AQUA     0x00FFFFFF
#define GRRLIB_WHITE    0xFFFFFFFF
=======
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

#define SYS_RETURNTOMENU 3 /*!< Directly load the Wii Channels menu, without actually cold-resetting the system */
#define SYS_POWEROFF 4 /*!< Powers off the Wii, automatically choosing Standby or Idle mode depending on the user's configuration */
>>>>>>> parent of 6dd0166 (stuff)


//sets an array for the colour selection, I have moved similar looking colors apart from each other.
int colours[] = {
	GRRLIB_MAROON,
	GRRLIB_GREEN,
	GRRLIB_OLIVE, 
	GRRLIB_NAVY,    
	GRRLIB_PURPLE,  
	GRRLIB_TEAL,   
	GRRLIB_GRAY,    
	GRRLIB_RED,     
	GRRLIB_LIME,    
	GRRLIB_YELLOW,  
	GRRLIB_SILVER, 
	GRRLIB_BLUE,    
	GRRLIB_FUCHSIA, 
	GRRLIB_AQUA,    
	GRRLIB_WHITE,
};

  
int x=0;
void newColour(){
	if (x==14){
		x=-1;
	}
	x++;
}



//Callback ----------------------------------------------------
s8 HWButton = -1;

/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
}

/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF;
}

/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF;
}

//---------------------------------------------------------------------


int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();
    
    
    //load texture
    GRRLIB_texImg *theme = GRRLIB_LoadTexture(wii_jpg); //theme "0" for default.
    
    	int sn=1; //This is the 4:3 to 16:9 variable.
	int width=100; //this is the width of the Wii logo, the 16:9 changes the width.
    	int height=44; //default height set.
    	int lr=1; //Left or right variable.
    	int ud=1; //Up or down variable.
   	int posx=100; // Initial positions 
        int posy=100; 
        int new=0; //For the theme adjust system.
	 if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
    	sn=0.75;
    	width=75; //the image width changes to 75 due to scaling to 75% for the 16:9 fix.	
   }
    
    // Initialise the GC Controller
    PAD_Init();
    
    
    // Initialise the Wiimotes
    WPAD_Init();	
    
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);

    // Loop forever
    
    while(1) {
 	
        WPAD_ScanPads();  // Scan the Wiimotes
 	PAD_ScanPads(); //Scan the gamecube controllers
        
        
        // If [HOME] was pressed on the first Wiimote, break out of the loop
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME || PAD_ButtonsDown(0) & PAD_BUTTON_START)  break;
 
	// If system callback function was called, break out of the loop
	if (HWButton != -1){
		break;
	}
	
	
        //New Theme select System. Please give me suggestions on how to improve this.
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A || PAD_ButtonsDown(0) & PAD_BUTTON_A){
         GRRLIB_FreeTexture(theme);
                new=new+1;
        	if (new==3){
			new=0;
		}
		
		posx=100; posy=100; lr=1; ud=1;
			if (new==0){
		//Switch to wii
			     GRRLIB_texImg *theme = GRRLIB_LoadTexture(wii_jpg);
			     height=44;
			  } else if (new==1){
		//Switch to GC
			     GRRLIB_texImg *theme = GRRLIB_LoadTexture(GC_img);
			     height=144;
			  } else if (new==2){ 
		//Switch to DVD
			     GRRLIB_texImg *theme = GRRLIB_LoadTexture(DVD_img);
			     height=44;
			  }
        }    
        // ---------------------------------------------------------------------
        
        //move the logo
        posx=posx+2*lr;
        posy=posy+2*ud;
        
        
        //When logo reaches edge reverse direction, and set new colour.
        if (posx<0 || posx>640-width) {
        	lr=lr*-1;
        	newColour();
        }
        
        if (posy<0 || posy>480-height) {
        	ud=ud*-1;
        	newColour();
        }
        
        // for some odd reason, GRRLIB_DrawImg does not like having variables in scaling factors, so this had to be made this way.
        
        
       if (sn==1) {
        GRRLIB_DrawImg(posx, posy, theme, 0, 1, 1, colours[x]);  // draw
        } else {
        	GRRLIB_DrawImg(posx, posy, theme, 0, 0.75, 1, colours[x]);  // draw
        }
        
   
        // ---------------------------------------------------------------------
        
        GRRLIB_Render();  // Render the frame buffer to the TV
    }
 
    GRRLIB_Exit(); // Clears the memory allocated by GRRLIB.
    
    //System callback function. The loop is asked again in the scenario that the loop was broken by the HOME button.
    if(HWButton != -1)
	{
		SYS_ResetSystem(HWButton, 0, 0);
	}

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
   }
