#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <jpeg/jpgogc.h>
#include <wiiuse/wpad.h>
#include <unistd.h>



static u32 *xfb;
static GXRModeObj *rmode;
extern char picdata[];
extern int  piclength;
//-------------------------------------------------


void Initialize() {

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	switch(VIDEO_GetCurrentTvMode()) {
		case VI_NTSC:
			rmode = &TVNtsc480IntDf;
			break;
		case VI_PAL:
			rmode = &TVPal528IntDf;
			break;
		case VI_MPAL:
			rmode = &TVMpal480IntDf;
			break;
		default:
			rmode = &TVNtsc480IntDf;
			break;
	}

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	//rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

}



void display_jpeg(JPEGIMG jpeg, int x, int y) {
	unsigned int *jpegout = (unsigned int *) jpeg.outbuffer;
	int i,j;
	int height = jpeg.height;
	int width = jpeg.width/2;
	for(i=0;i<=width;i++)
	for(j=0;j<=height-2;j++)
	xfb[(i+x)+320*(j+16+y)]=jpegout[i+width*j];
	free(jpeg.outbuffer);
}

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
	
	JPEGIMG wii;
	memset(&wii, 0, sizeof(JPEGIMG));
	wii.inbuffer = picdata;
	wii.inbufferlength = piclength;
	JPEG_Decompress(&wii);
	
	
	Initialize();
	
	
	
	int updown=2;
	int leftright=2;
	int movelr=0;
	int moveud=0;
	
	while(1) {
		VIDEO_ClearFrameBuffer (rmode, xfb, COLOR_BLACK);
		
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) exit(0);

		// Wait for the next frame
		
		
		movelr=movelr+leftright;
		moveud=moveud+updown;
		
		 if ( moveud>420-wii.height) {
			updown=updown*-1;
		} else if (moveud<0){
			updown=updown*-1;
		}
		
		 if ( movelr>390-wii.width ){
			leftright=leftright*-1;
		} else if (movelr<0){
			leftright=leftright*-1;
		} 
		
		display_jpeg(wii, movelr, moveud);
		VIDEO_WaitVSync();
	}

	return 0;
}
