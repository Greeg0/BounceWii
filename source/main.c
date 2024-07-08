#include <grrlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <time.h>
#include "gfx/wii_jpg.h"
#include "gfx/GC_img.h"
#include "gfx/DVD_img.h"
#include "gfx/wiiUimg.h"
#include "gfx/duck2.h"

// RGBA Colours
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

#define SYS_RETURNTOMENU 3  // Directly load the Wii Channels menu, without actually cold-resetting the system
#define SYS_POWEROFF 4	    // Powers off the Wii, automatically choosing Standby or Idle mode depending on the user's configuration

// sets an array for the colour selection.
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

// Callback ----------------------------------------------------

s8 HWButton = -1;

/*
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed()
{
	HWButton = SYS_RETURNTOMENU;
};

/*
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed()
{
	HWButton = SYS_POWEROFF;
};

/*
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan)
{
	HWButton = SYS_POWEROFF;
};

// ---------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand(time(NULL)); // seeds the randomizer for true random position setting.
	
	// Initialise the Graphics & Video subsystem
	
	GRRLIB_Init();
	
	
	// Decided to run all the the loading of textures only once and assigning their pointers into an array. The way it was done before was probably a memory leak.
	GRRLIB_texImg *theme[5];
	theme[0] = GRRLIB_LoadTexture(wii_jpg);
	theme[1] = GRRLIB_LoadTexture(GC_img);
	theme[2] = GRRLIB_LoadTexture(DVD_img);
	theme[3] = GRRLIB_LoadTexture(wiiUimg);
	theme[4] = GRRLIB_LoadTexture(duck2);
	
	float sn = CONF_GetAspectRatio() == CONF_ASPECT_16_9 ? 0.75 : 1; 
	// Detects aspect ratio, if 16:9, horizontal factor is streched by a factor of 0.75¯¹ so the texture's horizontal scale factor will be 0.75
	float xscale = sn;	// just didn't appreciate making sn negative and needed to include math.h for the fabs function.
	
	int width; // this is the width of the Wii logo, the 16:9 changes the width.
	int height; // default height set.
	
	int xDir;		 // Left or right variable.
	int yDir;	 // Up or down variable.
	
	int posx;	 // Initial positions
	int posy;
	
	int offset; // offset for negative scale factors.
	
	bool colourMode;	
	bool flipOnCollision;
	
	int yspeed = 2; // Main speeds.
	int xspeed = 2;

	int colour = rand() % 15; // For the colour adjust system, set to random colour.
	int new = -1; // For the theme adjust system. Set to -1 so I can run changeTheme() to initate the first theme.

	
	// Flip duck function
	void flipDuck()
	{
		xscale *= -1; // image is now backwards.
		
		offset = xscale > 0 ? 0 : width;
	};
	


	void randomPosition()
	{
		posx = rand() % (640-width+1);
		posy = rand() % (480-height+1);
		xDir = (rand() % 2 == 0) ? -1 : 1;
		yDir = (rand() % 2 == 0) ? -1 : 1;
		if (new == 4 && xDir == -1){
			flipDuck();
		};
	};
	
	// Update colour when hit border.
	void updateColour()
	{	
		colour = colourMode ? (colour + 1) % 15 : 14; // When colourmode is on, colour cycles from 0 to 14, and if colourmode is off, colour is set to 14, aka white.
	};
	// changeTheme is now only used to change the parameters for each theme, as the selection of the theme textures can be done with theme[new]	
	void changeTheme()
	{	
		new = (new+1) % 5;
		colour = rand() % 15; // set logo to random colour when switching themes, tbh, fresh start when switching themes is BEST!
	
		switch (new)
		{
		case 0:
			// Switch to Wii
			width = 100;
			height = 44;

			// for reverting back to default from case 4.
			GRRLIB_SetBackgroundColour(0, 0, 0, 1); 
			colourMode = true;
			flipOnCollision = false;
			xspeed = 2;
			xscale = sn; // make CERTAIN xscale is not backwards...
			offset = 0; // make certain offset is zero.
			
			break;
		case 1:
			// Switch to GC
			width = 100;
			height = 144;
			break;
		case 2:
			// Switch to DVD
			width = 100;
			height = 44;
			break;
		case 3:
			// Switch to Wii U
			width = 158;
			height = 44;

			break;

		case 4:
			// Switch to Duck theme
			width = 100;
			height = 100;
			GRRLIB_SetBackgroundColour(37, 65, 120, 0.86); // Set colour to blue! Ducks swim on water!
			xspeed = 4;
			flipOnCollision = true;
			colourMode = false;
			colour = 14;
			break;
			

		};
		width *= sn; 
		randomPosition();
	};
		
	// Initialise the GC Controller
	PAD_Init();

	// Initialise the Wiimotes
	WPAD_Init();

	// Set the callback functions.
	SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
	WPAD_SetPowerButtonCallback(WiimotePowerPressed);
	
	changeTheme(); // configure the default theme to start off with. 
	
	// Loop forever
	while (true)
	{
		WPAD_ScanPads(); // Scan the Wiimotes
		PAD_ScanPads();	 // Scan the gamecube controllers

		// If [HOME] was pressed on the first Wiimote, break out of the loop
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME || PAD_ButtonsDown(0) & PAD_BUTTON_START)
		{
			break;
		};

		// If system callback function was called, break out of the loop
		if (HWButton != -1)
		{
			break;
		};
		
		// Theme switching
		if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_A || PAD_ButtonsDown(0) & PAD_BUTTON_A))
		{
			changeTheme();
		};
		// ---------------------------------------------------------------------

		// move the logo
		posx += xspeed * xDir;
		posy += yspeed * yDir;
		
		// When logo reaches edge reverse direction, and set new colour.
		if (posx < 0 || posx > 640 - width) // The width change should have been accomodated with different aspect ratios for the borders.
		{
			xDir *= -1;
			updateColour();
			
			if (flipOnCollision) {flipDuck();};
		};

		if (posy < 0 || posy > 480 - height)
		{
			yDir *= -1;
			updateColour();
		};
		
		GRRLIB_DrawImg(posx + offset, posy, theme[new], 0, xscale, 1, colours[colour]); // draw
		
		GRRLIB_Render(); // Render the frame buffer to the TV
	};

	GRRLIB_Exit(); // Clears the memory allocated by GRRLIB.

	// System callback function. The loop is asked again in the scenario that the loop was broken by the HOME button.
	if (HWButton != -1)
	{
		SYS_ResetSystem(HWButton, 0, 0);
	};

	exit(0); // Use exit() to exit a program, do not use 'return' from main()
};
