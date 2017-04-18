//*****************************************************************************
//
// Display Bitmap and String in OLED Output using 
// Input Switch in LM3S811 ARM Cortex M3-Microcontroller
// Author: MD Farhadur Reza
// University of Louisiana at Lafayette

//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "drivers/display96x16x1.h"
#include "inc/lm3s811.h"
//#include "game.h"
#include "globals.h"
#include "random.h"
//#include "screen_saver.h"
#include "Output.h"
#include<string.h>


//
//
//     xxx.......xxxx....xxxxxxxxxxxxx....xxx....xxx..........
//     xxx.....xxxx......xxxxxxxxxxxxx....xxx....xxx..........
//     xxx...xxxx........xxx..............xxx....xxx..........
//     xxxxxxxx..........xxxxxxxxxxxxx....xxx....xxx..........
//     xxxxxxxx..........xxxxxxxxxxxxx....xxx....xxx..........
//     xxx...xxxx........xxx..............xxx....xxx..........
//     xxx.....xxxx......xxxxxxxxxxxxx....xxx....xxxxxxxxxxxxx
//     xxx.......xxxx....xxxxxxxxxxxxx....xxx....xxxxxxxxxxxxx
//     .......................................................
//     .......................................................
//     .x.........x..xx..x...x....xx..........................
//     x.x.......x.x.x.x.xx.xx...x............................
//     xxx.xx....xxx.xx..x.x.x...x....x..xx.x..xx...xx.xx..x.x
//     x.x.x.x...x.x.x.x.x...x...x...x.x.x.x.x.x.x.x.x.x.x.x.x
//     x.x.x.x...x.x.x.x.x...x....xx..x..x.x.x.xx...xx.x.x..x.
//     ........................................x...........x..
//
//*****************************************************************************
#if defined(rvmdk) || defined(__ARMCC_VERSION)
static const unsigned char g_pucKeilLogo[152] =
{
    0xff, 0x02, 0x04, 0x09, 0x13, 0x25, 0x49, 0x91, 0x21, 0x41, 0x81, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x81, 0x81, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb, 0xdb, 0xdb, 0xdb, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,
    0x7f, 0x20, 0x10, 0x48, 0x64, 0x52, 0x49, 0x44, 0x42, 0x41, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x7f, 0x00, 0x00, 0x00, 0x78, 0x14, 0x78,
    0x00, 0x70, 0x10, 0x60, 0x00, 0x00, 0x00, 0x78, 0x14, 0x78, 0x00, 0x7c,
    0x14, 0x68, 0x00, 0x7c, 0x08, 0x10, 0x08, 0x7c, 0x00, 0x00, 0x00, 0x38,
    0x44, 0x44, 0x00, 0x20, 0x50, 0x20, 0x00, 0x70, 0x10, 0x60, 0x10, 0x60,
    0x00, 0xf0, 0x50, 0x20, 0x00, 0x20, 0x50, 0x70, 0x00, 0x70, 0x10, 0x60,
    0x00, 0xb0, 0x40, 0x30
};
static const unsigned char g_pucSmileLogo[194] =
{
    0x00,
	  0x00, 0x00, 0xf8, 0x04,0x02,0x39,0x39,0x01,
	  0x39, 0x39, 0x02, 0x04, 0xf8,0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  
	  0x00, 0x00, 0x1f,0x20, 0x42, 0x86,0x8a,0x8a,
	  0x8a, 0x86, 0x42,0x20, 0x1f, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  

	   
};


static const unsigned char g_pucSurpriseLogo[193] =
{
    0x00,
	  0x00, 0xf0, 0x08, 0x04, 0x02,0x19,0x99,0x81,
	  0x99, 0x19, 0x02, 0x04, 0x08,0xf0,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  
	  0x00, 0x0f, 0x10,0x24, 0x4a,0x89,0x88,0x88,
	  0x88, 0x89, 0x4a,0x24, 0x10, 0x0f,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  

	   
};



static const unsigned char g_pucSadLogo[193] =
{
    0x00,
	  0x00, 0xf0, 0x08, 0x04, 0x02,0x31,0x01,0x01,
	  0x01, 0x31, 0x82, 0x04, 0x08,0xf0,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  
	  0x00, 0x0f, 0x10,0x20, 0x48,0x84,0x82,0x82,
	  0x82, 0x84, 0x48,0x20, 0x10, 0x0f,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  

	   
};


static const unsigned char g_pucDisappointedLogo[193] =
{
    0x00,
	  0xe0, 0x10, 0x08, 0x24, 0x1a,0x39,0x31,0x01,
	  0x31, 0x39, 0x1a, 0x24, 0x08,0x10,0xf0,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  
	  0x07, 0x08, 0x10,0x20, 0x48,0x84,0x82,0x82,
	  0x82, 0x84, 0x48,0x20, 0x10, 0x08,0x07,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00,0x00,
	  

	   
};






#endif

//*****************************************************************************
//
// A bitmap for the CodeSourcery logo.  The bitmap is as follows:
//
//    .....xxx....xx.........
//    ...xx......x..x........
//    ..x.......x............
//    .x.......x.............
//    .x......x..............
//    x......x...............
//    x.....x....xxx.........
//    x......xxxx...x........
//    x.............x........
//    .x...........x.........
//    .x..........x..........
//    ..x........x...........
//    ...xx.....x............
//    .....x...x.............
//    ........x..............
//    ........x..............
//
// Continued...
//
//    ............................................................
//    ...x..................x.....................................
//    ..x..................x......................................
//    .x..................x.......................................
//    .x....xx..xx...xxxx.x.....xx..x..x.xxx.....x.xxxx.xxx..x...x
//    x....x..x.x.x..x.....x...x..x.x..x.x..x...x..x....x..x.x...x
//    x....x..x.x..x.x.....x...x..x.x..x.x..x..x...x....x..x..x.x.
//    x....x..x.x..x.xxx...x...x..x.x..x.x..x..x...xxx..x..x...x..
//    x....x..x.x..x.x......x..x..x.x..x.xxx..x....x....xxx....x..
//    x....x..x.x..x.x......x..x..x.x..x.x..x.x....x....x..x...x..
//    x....x..x.x..x.x.......x.x..x.x..x.x..x.x....x....x..x...x..
//    x....x..x.x..x.x.......x.x..x.x..x.x..x.x....x....x..x...x..
//    .x...x..x.x.x..x.......x.x..x.x..x.x..x..x...x....x..x...x..
//    ..xx..xx..xx...xxxx...x...xx...xx..x..x...xx.xxxx.x..x...x..
//    ....................xx......................................
//    ............................................................
//
//*****************************************************************************
#if defined(sourcerygxx)
static const unsigned char g_pucCodeSourceryLogo[166] =
{
    0xe0, 0x18, 0x04, 0x02, 0x02, 0x01, 0x41, 0xa1, 0x90, 0x88, 0x84, 0x42,
    0x41, 0x41, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
    0x18, 0x04, 0x02, 0x00, 0xe0, 0x10, 0x10, 0xe0, 0x00, 0xf0, 0x10, 0x20,
    0xc0, 0x00, 0xf0, 0x90, 0x90, 0x10, 0x00, 0x18, 0xe4, 0x02, 0x00, 0x00,
    0xe0, 0x10, 0x10, 0xe0, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0xf0, 0x10,
    0x10, 0xe0, 0x00, 0x00, 0xc0, 0x20, 0x10, 0x00, 0xf0, 0x90, 0x90, 0x10,
    0x00, 0xf0, 0x10, 0x10, 0xe0, 0x00, 0x30, 0x40, 0x80, 0x40, 0x30,
    0x01, 0x06, 0x08, 0x10, 0x10, 0x20, 0x00, 0x00, 0xc0, 0x20, 0x10, 0x08,
    0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
    0x10, 0x20, 0x20, 0x00, 0x1f, 0x20, 0x20, 0x1f, 0x00, 0x3f, 0x20, 0x10,
    0x0f, 0x00, 0x3f, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x23, 0x1c, 0x00,
    0x1f, 0x20, 0x20, 0x1f, 0x00, 0x1f, 0x20, 0x20, 0x1f, 0x00, 0x3f, 0x01,
    0x01, 0x3e, 0x00, 0x0f, 0x10, 0x20, 0x20, 0x00, 0x3f, 0x20, 0x20, 0x20,
    0x00, 0x3f, 0x01, 0x01, 0x3e, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00
};
#endif

//*****************************************************************************
//
// A bitmap for the IAR logo.  The bitmap is as follows:
//
//     .......xxx................................
//     .....xxxxxxx..............................
//     ..x.xxxxxxxxx.x...........................
//     .xx.xxxxxxxxx.xx...xx....xxx....xxxxxx....
//     .xx...........xx...xx....xxx....xx...xx...
//     xxx.xxxxxxxxx.xxx..xx...xx.xx...xx...xx...
//     xxx.xxxxxxxxx.xxx..xx...xx.xx...xx...xx...
//     xxx...........xxx..xx...xx.xx...xx...xx...
//     xxx.xxxxxxxxx.xxx..xx..xx...xx..xxxxxx....
//     xxx...........xxx..xx..xx...xx..xx..xx....
//     xxx..xxxxxxx..xxx..xx..xxxxxxx..xx...xx...
//     .xxx.........xxx...xx.xx.....xx.xx...xx...
//     .xxxxx.....xxxxx...xx.xx.....xx.xx....xx..
//     ..xxxxxxxxxxxxx...........................
//     ....xxxxxxxxx.............................
//     ......xxxxx...............................
//
// Continued...
//
//     ..................................................
//     ..................................................
//     ..................................................
//     .xxxxx............................................
//     xx...xx................x..........................
//     xx....................xx..........................
//     xxx.....xx...xx.xxxx.xxxx..xxxx..xx.xxx.xx...xxxx.
//     .xxxx...xx...xxxx..xx.xx..xx..xx.xxx.xxx.xx.xx..xx
//     ..xxxx...xx.xx.xxx....xx..xx..xx.xx..xx..xx.xxx...
//     ....xxx..xx.xx..xxxx..xx..xxxxxx.xx..xx..xx..xxxx.
//     .....xx..xx.xx....xxx.xx..xx.....xx..xx..xx....xxx
//     xx...xx...xxx..xx..xx.xx..xx..xx.xx..xx..xx.xx..xx
//     .xxxxx....xxx...xxxx...xx..xxxx..xx..xx..xx..xxxx.
//     ..........xxx.....................................
//     ..........xx......................................
//     .........xx.......................................
//
//*****************************************************************************
#if defined(ewarm)
static const unsigned char g_pucIarLogo[184] = {
    0xe0, 0xf8, 0xfc, 0x00, 0x6c, 0x6e, 0x6e, 0x6f, 0x6f, 0x6f, 0x6e, 0x6e,
    0x6c, 0x00, 0xfc, 0xf8, 0xe0, 0x00, 0x00, 0xf8, 0xf8, 0x00, 0x00, 0x00,
    0xe0, 0xf8, 0x18, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x08, 0x08,
    0x08, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x70, 0xf8, 0xc8, 0x88, 0x88, 0x18,
    0x10, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x80, 0xc0, 0x40,
    0x40, 0xc0, 0x80, 0x40, 0xe0, 0xf0, 0x40, 0x00, 0x80, 0xc0, 0x40, 0x40,
    0xc0, 0x80, 0x00, 0xc0, 0xc0, 0x80, 0x40, 0xc0, 0xc0, 0x80, 0x40, 0xc0,
    0x80, 0x00, 0x80, 0xc0, 0x40, 0x40, 0xc0, 0x80,
    0x07, 0x1f, 0x3f, 0x38, 0x71, 0x75, 0xe5, 0xe5, 0xe5, 0xe5, 0xe5, 0x75,
    0x71, 0x38, 0x3f, 0x1f, 0x07, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x18, 0x1f,
    0x07, 0x04, 0x04, 0x04, 0x07, 0x1f, 0x18, 0x00, 0x1f, 0x1f, 0x01, 0x01,
    0x03, 0x0f, 0x1c, 0x10, 0x00, 0x00, 0x08, 0x18, 0x11, 0x11, 0x13, 0x1f,
    0x0e, 0x00, 0x00, 0x87, 0xff, 0x78, 0x3f, 0x07, 0x00, 0x09, 0x1b, 0x13,
    0x16, 0x1e, 0x0c, 0x00, 0x0f, 0x1f, 0x10, 0x00, 0x0f, 0x1f, 0x12, 0x12,
    0x1b, 0x0b, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x1f,
    0x1f, 0x00, 0x09, 0x1b, 0x13, 0x16, 0x1e, 0x0c
};
#endif

//*****************************************************************************
//
// A bitmap for the code_red logo.  The bitmap is as follows:
//
//     ............................xxx...................
//     ............................xxx...................
//     ............................xxx...................
//     ............................xxx...................
//     ............................xxx...................
//     ...xxx....xxxxx.......xxxxx.xxx....xxxxx..........
//     .xxxxx...xxxxxxxx....xxxxxxxxxx...xx...xx.........
//     .xx.....xxx....xxx..xxx.....xxx..xx.....xx........
//     xxx.....xx......xx..xx......xxx..xxxxxxxxx........
//     xxx.....xx......xx..xx......xxx..xx...............
//     .xx.....xxx....xxx..xxx.....xxx..xx...............
//     .xxxxx...xxxxxxxx....xxxxxxxxxx...xx...xxx........
//     ...xxx....xxxxxx......xxxx..xxx....xxxxx..........
//     ..................................................
//     .........................................xxxxxxxx.
//     ..................................................
//
// Continued...
//
//     ..........................xxx
//     ..........................xxx
//     ..........................xxx
//     ..........................xxx
//     ..........................xxx
//     xx.xx....xxxxx......xxxxx.xxx
//     xxxxx...xx...xx....xxxxxxxxxx
//     xxx....xx.....xx..xxx.....xxx
//     xx.....xxxxxxxxx..xx......xxx
//     xx.....xx.........xx......xxx
//     xx.....xxx........xxx.....xxx
//     xx......xxxxxxx....xxxxxxxxxx
//     xx.......xxxx........xxxx.xxx
//     .............................
//     .............................
//     .............................
//
//*****************************************************************************
#if defined(codered)
static const unsigned char g_pucCodeRedLogo[156] =
{
    0x00, 0xc0, 0xc0, 0x60, 0x60, 0x60, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x60,
    0x60, 0x60, 0x60, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x60,
    0x60, 0x60, 0x40, 0xff, 0xff, 0xff, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x20,
    0x20, 0x20, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xe0, 0xe0, 0xc0, 0x60, 0x60, 0x00, 0x00, 0x80, 0xc0, 0x60,
    0x20, 0x20, 0x20, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x60,
    0x60, 0x60, 0x40, 0xff, 0xff, 0xff,
    0x03, 0x0f, 0x0f, 0x18, 0x18, 0x18, 0x00, 0x00, 0x07, 0x0f, 0x1c, 0x18,
    0x18, 0x18, 0x18, 0x1c, 0x0f, 0x07, 0x00, 0x00, 0x07, 0x0f, 0x1c, 0x18,
    0x18, 0x18, 0x08, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x07, 0x0f, 0x19, 0x11,
    0x11, 0x11, 0x19, 0x09, 0x09, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0f, 0x19,
    0x11, 0x11, 0x11, 0x19, 0x09, 0x09, 0x00, 0x00, 0x07, 0x0f, 0x1c, 0x18,
    0x18, 0x18, 0x08, 0x1f, 0x1f, 0x1f
};
#endif

//*****************************************************************************
//
// A bitmap for the Code Composer Studio logo type.  The bitmap is as follows:
//
//    123456781234567812345678123456781234567812345678123456781234567812345678
//  0 ........................................................................
//  1 ........................................................................
//  2 ........................................................................
//  3 ..xxx..........x..........xxx.........................................xx
//  4 .x...x.........x.........x...x.......................................x..
//  5 x....x.........x........x....x.......................................x..
//  6 x.......xx...x.x..xx....x.......xx..x.x..xx.x.x...xx...xx...xx..x.x..x..
//  7 x......x..x.x.xx.x..x...x......x..x.xx.xx.x.xx.x.x..x.x..x.x..x.xx....xx
//  0 x......x..x.x..x.xxxx...x......x..x.x..x..x.x..x.x..x..x...xxxx.x.......
//  1 x....x.x..x.x..x.x......x....x.x..x.x..x..x.x..x.x..x...x..x....x....x..
//  2 .x...x.x..x.x.xx.x..x....x...x.x..x.x..x..x.xx.x.x..x.x..x.x..x.x....x..
//  3 ..xxx...xx...x.x..xx......xxx...xx..x..x..x.x.x...xx...xxx..xx..x.....xx
//  4 ............................................x...........................
//  5 ............................................x...........................
//  6 ............................................x...........................
//  7 ........................................................................
//
// Continued...
//    1234567812345678123456
//  0 ......................
//  1 ......................
//  2 ......................
//  3 x.............x.x.....
//  4 .x.x..........x.......
//  5 .x.x..........x.......
//  6 ..xxx.x..x..x.x.x..xx.
//  7 x..x..x..x.x.xx.x.x..x
//  0 .x.x..x..x.x..x.x.x..x
//  1 .x.x..x..x.x..x.x.x..x
//  2 .x.x..x.xx.x.xx.x.x..x
//  3 x..xx..x.x..x.x.x..xx.
//  4 ......................
//  5 ......................
//  6 ......................
//  7 ......................
//
//*****************************************************************************
#if defined(ccs)
static const unsigned char g_pucCodeComposerLogo[188] =
{
    //
    // Top Row
    //
    0xe0, 0x10, 0x08, 0x08, 0x08, 0x30, 0x00, 0x80,
    0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0xf8,
    0x00, 0x80, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00,
    0xe0, 0x10, 0x08, 0x08, 0x08, 0x30, 0x00, 0x80,
    0x40, 0x40, 0x80, 0x00, 0xc0, 0x80, 0x40, 0x80,
    0x80, 0x40, 0xc0, 0x00, 0xc0, 0x80, 0x40, 0x80,
    0x00, 0x80, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40,
    0x40, 0x80, 0x00, 0x80, 0x40, 0x40, 0x80, 0x00,
    0xc0, 0x80, 0x40, 0x00, 0x00, 0x70, 0x88, 0x88,

    0x88, 0x30, 0x40, 0xf0, 0x40, 0x00, 0xc0, 0x00,
    0x00, 0xc0, 0x00, 0x80, 0x40, 0x80, 0xf8, 0x00,
    0xc8, 0x00, 0x80, 0x40, 0x40, 0x80,

    //
    // Second Row
    //
    0x03, 0x04, 0x08, 0x08, 0x08, 0x06, 0x00, 0x07,
    0x08, 0x08, 0x07, 0x00, 0x07, 0x08, 0x04, 0x0f,
    0x00, 0x07, 0x09, 0x09, 0x05, 0x00, 0x00, 0x00,
    0x03, 0x04, 0x08, 0x08, 0x08, 0x06, 0x00, 0x07,
    0x08, 0x08, 0x07, 0x00, 0x0f, 0x00, 0x00, 0x0f,
    0x00, 0x00, 0x0f, 0x00, 0x7f, 0x04, 0x08, 0x07,
    0x00, 0x07, 0x08, 0x08, 0x07, 0x00, 0x04, 0x09,
    0x0a, 0x0c, 0x00, 0x07, 0x09, 0x09, 0x05, 0x00,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x08,

    0x08, 0x07, 0x00, 0x0f, 0x08, 0x00, 0x07, 0x08,
    0x04, 0x0f, 0x00, 0x07, 0x08, 0x04, 0x0f, 0x00,
    0x0f, 0x00, 0x07, 0x08, 0x08, 0x07
};
#endif

//*****************************************************************************
//
// A bitmap for the Texas Instruments logo type.  The bitmap is as follows:
//
//    123456781234567812345678123456781234567812345678123456781234567812345678
//  0 .xxxxxxxxxxxx..............................................xxxxxx.......
//  1 .xxxxxxxxxxxxx.............................................xxxxx........
//  2 xx....xxx...xx..............................................xxx.........
//  3 x.....xxx..xxxxxxxxx.xxxxxx.xxxxx...xxx.......xxxxx.........xxx..xxx....
//  4 ......xxx..xxxxxxxxx.xxxxx..xxxxx...xxxx....xxxxxxxx........xxx..xxxx...
//  5 ......xxx...xxx...xxx.xxxx...xx....xxxxx....xxx.xxxx........xxx...xxxx..
//  6 ......xxx...xxx.....x..xxxx.xx.....xxxxx...xxx....xx........xxx...xxxxx.
//  7 ......xxx...xxx....x....xxxxxx.....xx.xxx..xxxxx............xxx...xxxxxx
//  0 ......xxx...xxxxxxxx.....xxxx.....xx..xxx...xxxxxxx.........xxx...xx.xxx
//  1 ......xxx...xxxxxxxx.....xxxx.....xx..xxxx...xxxxxxx........xxx...xx..xx
//  2 ......xxx...xxx....x....xxxxxx...xxxxxxxxx.....xxxxxx.......xxx...xx...x
//  3 ......xxx...xxx.........xx.xxx...xxxxxxxxx.xx.....xxx.......xxx...xx....
//  4 ......xxx...xxx.....xx.xx...xxx..xx....xxxxxx.....xxx.......xxx...xx....
//  5 .....xxxxx..xxxxxxxxx.xxx...xxxxxxx.....xxx.xxxxxxxx.......xxxxx..xx....
//  6 .....xxxxx.xxxxxxxxxxxxxx..xxxxxxxx....xxxxxxxxxxxx........xxxxx.xxxx...
//  7 ........................................................................
//
// Continued...
//    123456781234567812345678123456781234567812345678123456781234567812345678
//  0 ........................................................................
//  1 ........................................................................
//  2 ........................................................................
//  3 ..xxxx..xxxx...xxxxxxxxx.xxxxxxx....xxxxx...xxxxxxxx......xxxxxxxxxxxxx.
//  4 ..xxx..xxxxxxx.xxxxxxxxxxxxxxxxxxxx.xxxxx...xxxxxxxxx....xxxx.xxxxxxxxx.
//  5 ..xx.xxx..xxx.xx.xxx.xxx.xxxxxxxxx..xxx.....xx..xxxx....xxxxx..xxx...xxx
//  6 ..xx.xx.....xxx..xxx...x.xxx...xxxx.xxx.....xx..xxxxx...xxxxx..xxx.....x
//  7 ..xx.xxxx........xxx.....xxx...xxxx.xxx.....xx..xxxxx...xxxxx..xxx...xx.
//  0 x.xx.xxxxxxx.....xxx.....xxx..xxxx..xxx.....xx..xx.xxx.xx.xxx..xxxxxxxx.
//  1 xxxx..xxxxxxxx...xxx.....xxxxxxxx...xxx.....xx..xx.xxx.xx.xxx..xxx...xx.
//  2 xxxx.....xxxxx...xxx.....xxx.xxxx...xxx.....xx..xx.xxxxx..xxx..xxx......
//  3 xxxx.x.....xxx...xxx.....xxx..xxxx..xxx.....xx..xx..xxxx..xxx..xxx......
//  4 xxxx.xx... xxx...xxx.....xxx...xxx..xxx.....xx..xx..xxxx..xxx..xxx.....x
//  5 .xxx.xxxxxxxxx...xxx.....xxx...xxxx..xxxxxxxxx..xx....xx..xxx..xxxxxxxxx
//  6 ..xx..xxxxxx....xxxxx...xxxxx...xxxx..xxxxxxx..xxxx...xx.xxxxxxxxxxxxxxx
//  7 ........................................................................
//
// Continued...
//
//    12345678123456781234567812345678
//  0 ................................
//  1 ................................
//  2 ................................
//  3 xxxx....xxxx.xxxxxxxx...xxxx....
//  4 xxxxx...xxxxxxxxxxxxxx.xxxxxxxx.
//  5 .xxxxx...xx.xx. xxx.xx.xxx..xxx.
//  6 .xxxxx...xx.x...xxx..xxxx.....x.
//  7 .xx.xxx..xx.....xxx....xxxx.....
//  0 .xx.xxxx.xx.....xxx....xxxxxxx..
//  1 .xx..xxxxxx.....xxx.....xxxxxxx.
//  2 .xx...xxxxx.....xxx........xxxxx
//  3 .xx....xxxx.....xxx...xx.....xxx
//  4 .xx.....xxx.....xxx...xxx....xxx
//  5 .xx......xx.....xxx....xxxxxxxx.
//  6 xxxx.....xx....xxxxx...xxxxxxx..
//  7 ................................
//
//*****************************************************************************
static const unsigned char g_pucTILogo[] =
{
    //
    // Top Row (blank columns added to left and right edges)
    //
    0x00,
    0x0c, 0x07, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff,
    0xff, 0x03, 0x03, 0x1b, 0xfe, 0xfe, 0xf8, 0x18,
    0x18, 0x18, 0x38, 0xb8, 0x60, 0x18, 0x38, 0x78,
    0xf8, 0xf8, 0xc8, 0x80, 0xd8, 0xf8, 0x38, 0x18,
    0x18, 0x00, 0x00, 0xe0, 0xf8, 0x78, 0xf8, 0xf0,
    0x80, 0x00, 0x00, 0xc0, 0xf0, 0xf0, 0xb8, 0x98,
    0x38, 0x38, 0x78, 0x70, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x03,
    0x01, 0x18, 0xf8, 0xf8, 0xf0, 0xe0, 0xc0, 0x80,

    0x00, 0x00, 0xf8, 0xf8, 0x18, 0xe8, 0xe0, 0xb0,
    0x98, 0x18, 0x38, 0x38, 0x70, 0x50, 0x60, 0x38,
    0x18, 0xf8, 0xf8, 0xf8, 0x18, 0x38, 0x38, 0x78,
    0x10, 0xf8, 0xf8, 0xf8, 0x38, 0x38, 0x38, 0xf8,
    0xf0, 0xf0, 0xd0, 0x00, 0xf8, 0xf8, 0xf8, 0x18,
    0x18, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x18, 0x18,
    0xf8, 0xf8, 0xf8, 0xf8, 0xd0, 0x00, 0x00, 0x00,
    0xe0, 0xf0, 0xf8, 0xf8, 0xf8, 0x08, 0x18, 0xf8,
    0xf8, 0xf8, 0x18, 0x18, 0x18, 0xb8, 0xb8, 0x60,

    0x18, 0xf8, 0xf8, 0x78, 0xf0, 0xe0, 0x80, 0x00,
    0x18, 0xf8, 0xf8, 0x18, 0x70, 0x38, 0x18, 0x18,
    0xf8, 0xf8, 0xf8, 0x18, 0x38, 0x70, 0x40, 0xf0,
    0xf8, 0xb8, 0x98, 0x18, 0x38, 0x30, 0x70, 0x00,
    0x00,

    //
    // Second row (blank columns added to left and right edges)
    //
    0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0x7f,
    0x7f, 0x60, 0x00, 0x40, 0x7f, 0x7f, 0x7f, 0x63,
    0x63, 0x63, 0x63, 0x67, 0x70, 0x50, 0x60, 0x70,
    0x7c, 0x0f, 0x07, 0x4f, 0x7f, 0x7c, 0x70, 0x60,
    0x60, 0x7c, 0x7f, 0x0f, 0x0c, 0x0c, 0x0f, 0x5f,
    0x7f, 0x7e, 0x70, 0x58, 0x79, 0x63, 0x63, 0x67,
    0x67, 0x67, 0x7f, 0x3e, 0x1c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x7f, 0x7f, 0x7f, 0x60,
    0x00, 0x40, 0x7f, 0x7f, 0x40, 0x01, 0x03, 0x07,

    0x1f, 0x3e, 0x7f, 0x7f, 0x00, 0x39, 0x73, 0x63,
    0x63, 0x67, 0x67, 0x7f, 0x3e, 0x3e, 0x00, 0x00,
    0x40, 0x7f, 0x7f, 0x7f, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x7f, 0x7f, 0x7f, 0x42, 0x06, 0x0f, 0x3f,
    0x7f, 0x79, 0x60, 0x40, 0x1f, 0x3f, 0x7f, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x7f, 0x3f, 0x00, 0x40,
    0x7f, 0x7f, 0x40, 0x07, 0x1f, 0x1f, 0x7c, 0x7f,
    0x03, 0x40, 0x7f, 0x7f, 0x7f, 0x40, 0x40, 0x7f,
    0x7f, 0x7f, 0x61, 0x61, 0x61, 0x63, 0x63, 0x70,

    0x40, 0x7f, 0x7f, 0x40, 0x01, 0x03, 0x07, 0x0f,
    0x0e, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x7f, 0x7f, 0x7f, 0x40, 0x00, 0x00, 0x18, 0x79,
    0x73, 0x63, 0x61, 0x63, 0x63, 0x7f, 0x3e, 0x1c,
    0x00
};

static const unsigned char g_pucTILogo2[] =
{
    //
    // Top Row (blank columns added to left and right edges)
    //
    0x00,
    0x0c, 0x07, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff,
    0xff, 0x03, 0x03, 0x1b, 0xfe, 0xfe, 0xf8, 0x18,
    0x18, 0x18, 0x38, 0xb8, 0x60, 0x18, 0x38, 0x78,
    0xf8, 0xf8, 0xc8, 0x80, 0xd8, 0xf8, 0x38, 0x18,
    0x18, 0x00, 0x00, 0xe0, 0xf8, 0x78, 0xf8, 0xf0,
    0x80, 0x00, 0x00, 0xc0, 0xf0, 0xf0, 0xb8, 0x98,
    0x38, 0x38, 0x78, 0x70, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x03,
    0x01, 0x18, 0xf8, 0xf8, 0xf0, 0xe0, 0xc0, 0x80,

    0x00, 0x00, 0xf8, 0xf8, 0x18, 0xe8, 0xe0, 0xb0,
    0x98, 0x18, 0x38, 0x38, 0x70, 0x50, 0x60, 0x38,
    0x18, 0xf8, 0xf8, 0xf8, 0x18, 0x38, 0x38, 0x78,
    0x10, 0xf8, 0xf8, 0xf8, 0x38, 0x38, 0x38, 0xf8,
    0xf0, 0xf0, 0xd0, 0x00, 0xf8, 0xf8, 0xf8, 0x18,
    0x18, 0x00, 0x00, 0x00, 0xf8, 0xf8, 0x18, 0x18,
    0xf8, 0xf8, 0xf8, 0xf8, 0xd0, 0x00, 0x00, 0x00,
    0xe0, 0xf0, 0xf8, 0xf8, 0xf8, 0x08, 0x18, 0xf8,
    0xf8, 0xf8, 0x18, 0x18, 0x18, 0xb8, 0xb8, 0x60,

    0x18, 0xf8, 0xf8, 0x78, 0xf0, 0xe0, 0x80, 0x00,
    0x18, 0xf8, 0xf8, 0x18, 0x70, 0x38, 0x18, 0x18,
    0xf8, 0xf8, 0xf8, 0x18, 0x38, 0x70, 0x40, 0xf0,
    0xf8, 0xb8, 0x98, 0x18, 0x38, 0x30, 0x70, 0x00,
    0x00,

    //
    // Second row (blank columns added to left and right edges)
    //
    0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0x7f,
    0x7f, 0x60, 0x00, 0x40, 0x7f, 0x7f, 0x7f, 0x63,
    0x63, 0x63, 0x63, 0x67, 0x70, 0x50, 0x60, 0x70,
    0x7c, 0x0f, 0x07, 0x4f, 0x7f, 0x7c, 0x70, 0x60,
    0x60, 0x7c, 0x7f, 0x0f, 0x0c, 0x0c, 0x0f, 0x5f,
    0x7f, 0x7e, 0x70, 0x58, 0x79, 0x63, 0x63, 0x67,
    0x67, 0x67, 0x7f, 0x3e, 0x1c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x7f, 0x7f, 0x7f, 0x60,
    0x00, 0x40, 0x7f, 0x7f, 0x40, 0x01, 0x03, 0x07,

    0x1f, 0x3e, 0x7f, 0x7f, 0x00, 0x39, 0x73, 0x63,
    0x63, 0x67, 0x67, 0x7f, 0x3e, 0x3e, 0x00, 0x00,
    0x40, 0x7f, 0x7f, 0x7f, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x7f, 0x7f, 0x7f, 0x42, 0x06, 0x0f, 0x3f,
    0x7f, 0x79, 0x60, 0x40, 0x1f, 0x3f, 0x7f, 0x60,
    0x60, 0x60, 0x60, 0x60, 0x7f, 0x3f, 0x00, 0x40,
    0x7f, 0x7f, 0x40, 0x07, 0x1f, 0x1f, 0x7c, 0x7f,
    0x03, 0x40, 0x7f, 0x7f, 0x7f, 0x40, 0x40, 0x7f,
    0x7f, 0x7f, 0x61, 0x61, 0x61, 0x63, 0x63, 0x70,

    0x40, 0x7f, 0x7f, 0x40, 0x01, 0x03, 0x07, 0x0f,
    0x0e, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x7f, 0x7f, 0x7f, 0x40, 0x00, 0x00, 0x18, 0x79,
    0x73, 0x63, 0x61, 0x63, 0x63, 0x7f, 0x3e, 0x1c,
    0x00
};



//*****************************************************************************
//
// A set of flags used to track the state of the application.
//
//*****************************************************************************
unsigned long g_ulFlags;

//*****************************************************************************
//
// The current filtered value of the potentiometer.
//
//*****************************************************************************
unsigned long g_ulWheel;

//*****************************************************************************
//
// Storage for a local frame buffer.
//
//*****************************************************************************
unsigned char g_pucFrame[192];

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// The interrupt handler for the ADC interrupt.
//
//*****************************************************************************
void
ADCIntHandler(void)
{
    unsigned long ulData;

    //
    // Clear the ADC interrupt.
    //
    ADCIntClear(ADC0_BASE, 3);

    //
    // Read the data from the ADC.
    //
    ADCSequenceDataGet(ADC0_BASE, 3, &ulData);

    //
    // Add the ADC data to the random number entropy pool.
    //
    RandomAddEntropy(ulData);

    //
    // Pass the ADC data through the low pass filter (with a coefficient of
    // 0.9) to update the position of the potentiometer.
    //
    g_ulWheel = ((g_ulWheel * 58982) + (ulData * 6554)) / 65536;

    //
    // Read the push button.
    //
    ulData = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;

    //
    // See if the push button state doesn't match the debounced push button
    // state.
    //
    if(ulData != HWREGBITW(&g_ulFlags, FLAG_BUTTON))
    {
        //
        // Increment the debounce counter.
        //
        HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW) ^= 1;
        if(!HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW))
        {
            HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_HIGH) = 1;
        }

        //
        // See if the debounce counter has reached three.
        //
        if(HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW) &&
           HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_HIGH))
        {
            //
            // The button has been in the new state for three consecutive
            // samples, so it has been debounced.  Toggle the debounced state
            // of the button.
            //
            HWREGBITW(&g_ulFlags, FLAG_BUTTON) ^= 1;

            //
            // If the button was just pressed, set the flag to indicate that
            // fact.
            //
            if(HWREGBITW(&g_ulFlags, FLAG_BUTTON) == 0)
            {
                HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 1;
            }
        }
    }
    else
    {
        //
        // Since the button state matches the debounced state, reset the
        // debounce counter.
        //
        HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW) = 0;
        HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_HIGH) = 0;
    }

    //
    // Increment the clock count.
    //
    HWREGBITW(&g_ulFlags, FLAG_CLOCK_COUNT_LOW) ^= 1;
    if(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_COUNT_LOW))
    {
        HWREGBITW(&g_ulFlags, FLAG_CLOCK_COUNT_HIGH) ^= 1;
    }

    //
    // If the clock count has wrapped around to zero, then set a flag to
    // indicate that the display needs to be updated.
    //
    if((HWREGBITW(&g_ulFlags, FLAG_CLOCK_COUNT_LOW) == 0) &&
       (HWREGBITW(&g_ulFlags, FLAG_CLOCK_COUNT_HIGH) == 0))
    {
        HWREGBITW(&g_ulFlags, FLAG_UPDATE) = 1;
    }

    //
    // Indicate that a timer interrupt has occurred.
    //
    HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 1;
}

#ifndef gcc
//*****************************************************************************
//
// Displays a logo for a specified amount of time.
//
//*****************************************************************************
static void
DisplayLogo(const unsigned char *pucLogo, unsigned long ulX,
            unsigned long ulWidth, unsigned long ulDelay)
{
    unsigned long ulLoop, ulBit;

    //
    // Loop over the sixteen scan lines of the display, scrolling the logo onto
    // the display from the bottom by one scan line at a time.
    //
    for(ulBit = 1; ulBit <= 16; ulBit++)
    {
        //
        // Clear the local frame buffer.
        //
        for(ulLoop = 0; ulLoop < 192; ulLoop += 4)
        {
            *(unsigned long *)(g_pucFrame + ulLoop) = 0;
        }

        //
        // See if eight scan lines or less should be displayed.
        //
        if(ulBit <= 8)
        {
            //
            // Loop over the columns of the logo.
            //
            for(ulLoop = 0; ulLoop < ulWidth; ulLoop++)
            {
                //
                // Copy the specified number of scan lines from the first row
                // of the logo to the second row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop + 96] = pucLogo[ulLoop] << (8 - ulBit);
            }
        }
        else
        {
            //
            // Loop over the columns of the logo.
            //
            for(ulLoop = 0; ulLoop < ulWidth; ulLoop++)
            {
                //
                // Copy N - 8 of the scan lines from the first row of the logo
                // to the first row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop] = pucLogo[ulLoop] << (16 - ulBit);

                //
                // Copy the remaining scan lines from the first row of the logo
                // and N - 8 scan lines from the second row of the logo to the
                // second row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop + 96] =
                    ((pucLogo[ulLoop] >> (ulBit - 8)) |
                     (pucLogo[ulLoop + ulWidth] << (16 - ulBit)));
            }
        }

        //
        // Display the local frame buffer on the display.
        //
        Display96x16x1ImageDraw(g_pucFrame, 0, 0, 96, 2);

        //
        // Wait for a twentieth of a second.
        //
        for(ulLoop = 0; ulLoop < (CLOCK_RATE / 20); ulLoop++)
        {
            //
            // Wait until a SysTick interrupt has occurred.
            //
            while(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK))
            {
            }

            //
            // Clear the SysTick interrupt flag.
            //
            HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
        }
    }

    //
    // Delay for the specified time while the logo is displayed.
    //
    for(ulLoop = 0; ulLoop < ulDelay; ulLoop++)
    {
        //
        // Wait until a SysTick interrupt has occurred.
        //
        while(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK))
        {
        }

        //
        // Clear the SysTick interrupt flag.
        //
        HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
    }

    //
    // Loop over the sixteen scan lines of the display, scrolling the logo off
    // the display to the top by one scan line at a time.
    //
    for(ulBit = 1; ulBit <= 16; ulBit++)
    {
        //
        // Clear the local frame buffer.
        //
        for(ulLoop = 0; ulLoop < 192; ulLoop += 4)
        {
            *(unsigned long *)(g_pucFrame + ulLoop) = 0;
        }

        //
        // See if more than eight scan lines should be displayed.
        //
        if(ulBit <= 8)
        {
            //
            // Loop over the columns of the logo.
            //
            for(ulLoop = 0; ulLoop < ulWidth; ulLoop++)
            {
                //
                // Copy 8 - N scan lines from the first row of the logo and
                // N scan lines from the second row of the logo to the first
                // row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop] =
                    ((pucLogo[ulLoop] >> ulBit) |
                     (pucLogo[ulLoop + ulWidth] << (8 - ulBit)));

                //
                // Copy 8 - N scan lines from the second row of the logo to the
                // second row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop + 96] = (pucLogo[ulLoop + ulWidth] >>
                                                 ulBit);
            }
        }
        else
        {
            //
            // Loop over the columns of the logo.
            //
            for(ulLoop = 0; ulLoop < ulWidth; ulLoop++)
            {
                //
                // Copy 16 - N scan lines of of the second row of the logo to
                // the first row of the local frame buffer.
                //
                g_pucFrame[ulX + ulLoop] = (pucLogo[ulLoop + ulWidth] >>
                                            (ulBit - 8));
            }
        }

        //
        // Display the local frame buffer on the display.
        //
        Display96x16x1ImageDraw(g_pucFrame, 0, 0, 96, 2);

        //
        // Wait for a twentieth of a second.
        //
        for(ulLoop = 0; ulLoop < (CLOCK_RATE / 20); ulLoop++)
        {
            //
            // Wait until a SysTick interrupt has occurred.
            //
            while(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK))
            {
            }

            //
            // Clear the SysTick interrupt flag.
            //
            HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
        }
    }
}
#endif

//*****************************************************************************
//
// Scrolls a wide image across the display.
//
//*****************************************************************************
static void
ScrollImage(const unsigned char *pucImage, unsigned long ulWidth)
{
    unsigned long ulLoop, ulIdx;
	 
    //
    // Loop over the columns of the image plus the columns of the display,
    // scrolling the image across the dipslay from right to left one column at
    // a time.
    //
	
	 /*for(ulIdx = 0; ulIdx <= 96; ulIdx++)
	{
    Display96x16x1StringDraw("Hello World",ulIdx, 0);
		for(ulLoop = 0; ulLoop < 20000; ulLoop++)
        {
	}
				Display96x16x1Clear();
}*/
	  
    Display96x16x1Init(true);
	  
    for(ulIdx = 1; ulIdx <= (ulWidth + 96); ulIdx++)
    {
		/*	if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
			{
				//Display96x16x1Clear();
				//Display96x16x1DisplayOn();
				//Display96x16x1Init(false);
				//HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 1;
				//Display96x16x1StringDraw("Hello World", 0, 0);
				break;
			}*/
        //
        // Clear the local frame buffer.
        //
			
			
        for(ulLoop = 0; ulLoop < 192; ulLoop += 4)
        {
            *(unsigned long *)(g_pucFrame + ulLoop) = 0;
        }

        //
        // See if the image has reached the left side of the display.
        //
        if(ulIdx <= 60)
        {
            //
            // Copy the first N columns of the image to the right side of the
            // local frame buffer.
            //
            for(ulLoop = 0; ulLoop < ulIdx; ulLoop++)
            {
                g_pucFrame[ulLoop + 96 - ulIdx] = pucImage[ulLoop];
                g_pucFrame[ulLoop + 96 - ulIdx + 96] =
                    pucImage[ulLoop + ulWidth];
							
							
            }
						Display96x16x1ImageDraw(g_pucFrame, 0, 0, 96, 2);
						
        }

        //
        // See if the right side of the image has reached the right side of the
        // display.
        //
        else if(ulIdx < ulWidth)
        {
            //
            // Copy 96 columns from the middle of the image to the local frame
            // buffer.
            //
            for(ulLoop = 0; ulLoop < 96; ulLoop++)
            {
                g_pucFrame[ulLoop] = pucImage[ulLoop + ulIdx - 96];
                g_pucFrame[ulLoop + 96] =
                    pucImage[ulLoop + ulIdx - 96 + ulWidth];
							
							    
            }
        }

        //
        // Otherwise, the right side of the image has already reached the right
        // side of the display.
        //
        else
        {
            //
            // Copy the right N columns of the image to the left side of the
            // local frame buffer.
            //
            for(ulLoop = 0; ulLoop < (ulWidth + 96 - ulIdx); ulLoop++)
            {
                g_pucFrame[ulLoop] = pucImage[ulLoop + ulIdx - 96];
                g_pucFrame[ulLoop + 96] =
                    pucImage[ulLoop + ulIdx - 96 + ulWidth];
            }
        }
        
        //
        // Display the local frame buffer on the display.
        //
        
			/*  if(len>0)
				{					
				Display96x16x1StringDraw(s,0, 0);
					for(ulLoop = 0; ulLoop < 2000000; ulLoop++)
        {
        }
				}
				len--;*/
				
				//for(ulLoop = 0; ulLoop < 96; ulLoop++)
          //  {
						//	Display96x16x1StringDraw(s,ulLoop + 96 - ulIdx, 0);
							
						//}
				
				
			/*	if(ulIdx<=60)
				{
					Display96x16x1StringDraw("Hello World",ulIdx, 0);
					
					for(ulLoop = 0; ulLoop < 2000000; ulLoop++)
					{
					}
			  
				Display96x16x1Clear();
				}
				
				Output_Init();
    Output_Color(1);
    //
    // Initialize the OLED display.
    //
    Display96x16x1Init(true);*/
        
		  
		
				
      
	  
  
        //
        // Wait for a thirtieth of a second.
        //
        for(ulLoop = 0; ulLoop < (CLOCK_RATE / 30); ulLoop++)
        {
            //
            // Wait until a SysTick interrupt has occurred.
            //
            while(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK))
            {
            }

            //
            // Clear the SysTick interrupt flag.
            //
            HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
        }
    }
}

//*****************************************************************************
//
// The main code for the application.  It sets up the peripherals, displays the
// splash screens, and then manages the interaction between the game and the
// screen saver.
//
//*****************************************************************************
int
main(void)
{
	volatile unsigned long ulLoop;
	volatile unsigned long end_pushing=0;
	volatile unsigned long more_pushing=0;
	volatile unsigned long count;
	unsigned long sh;
  unsigned char s[11]= "Hello World";
	unsigned char *s3;
	unsigned char s2[192];
	
	volatile unsigned long button_count=0;
	volatile unsigned long more_push=0;
	volatile unsigned long button_count2;
	volatile unsigned long button_count3;
	volatile unsigned long button_count4;
	volatile unsigned long button_count5;
	
	  //std::string str ("Hello World");
	   //int len= strlen(s);
    //
    // Set the clocking to run at 20MHz from the PLL.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_6MHZ);

    //
    // Enable the peripherals used by the application.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure the ADC to sample the potentiometer when the timer expires.
    // After sampling, the ADC will interrupt the processor; this is used as
    // the heartbeat for the game.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                             ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntEnable(ADC0_BASE, 3);
    IntEnable(INT_ADC0SS3);

    //
    // Configure the first timer to generate a 10 kHz PWM signal for driving
    // the user LED.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_B, (SysCtlClockGet() / 10000) - 1);
    TimerMatchSet(TIMER0_BASE, TIMER_B, 0);
    TimerControlLevel(TIMER0_BASE, TIMER_B, true);
    TimerEnable(TIMER0_BASE, TIMER_B);

    //
    // Configure the second timer to generate triggers to the ADC to sample the
    // potentiometer.
    //
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 120);
    TimerControlStall(TIMER1_BASE, TIMER_A, true);
    TimerControlTrigger(TIMER1_BASE, TIMER_A, true);
    TimerEnable(TIMER1_BASE, TIMER_A);

    //
    // Configure the LED, push button, and UART GPIOs as required.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, PUSH_BUTTON);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, USER_LED);
    GPIOPinWrite(GPIO_PORTC_BASE, USER_LED, 0);

    //
    // Configure the first UART for 115,200, 8-N-1 operation.
    //
 /*   UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);

    //
    // Send a welcome message to the UART.
    //
    UARTCharPut(UART0_BASE, 'L');
    UARTCharPut(UART0_BASE, 'M');
    UARTCharPut(UART0_BASE, '3');
    UARTCharPut(UART0_BASE, 'S');
    UARTCharPut(UART0_BASE, '8');
    UARTCharPut(UART0_BASE, '1');
    UARTCharPut(UART0_BASE, '1');
    UARTCharPut(UART0_BASE, '\r');
    UARTCharPut(UART0_BASE, '\n');
*/
   
				
	  Output_Init();
    Output_Color(1);
    //
    // Initialize the OLED display.
    //
    Display96x16x1Init(true);

   

    //
    // Display the Code Composer Studio logo for five seconds.
    //
				/*
#if defined(ccs)
    DisplayLogo(g_pucCodeComposerLogo, 1, 94, 5 * CLOCK_RATE);
#endif

    //
    // Display the Keil/ARM logo for five seconds.
    //
#if defined(rvmdk) || defined(__ARMCC_VERSION)
    DisplayLogo(g_pucKeilLogo, 10, 76, 5 * CLOCK_RATE);
#endif

    //
    // Display the IAR logo for five seconds.
    //
#if defined(ewarm)
    DisplayLogo(g_pucIarLogo, 2, 92, 5 * CLOCK_RATE);
#endif

    //
    // Display the CodeSourcery logo for five seconds.
    //
#if defined(sourcerygxx)
    DisplayLogo(g_pucCodeSourceryLogo, 6, 83, 5 * CLOCK_RATE);
#endif

    //
    // Display the code_red logo for five seconds.
    //
#if defined(codered)
    DisplayLogo(g_pucCodeRedLogo, 9, 78, 5 * CLOCK_RATE);
#endif
*/

    //
    // Throw away any button presses that may have occurred while the splash
    // screens were being displayed.
    //
    HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS) = 0;
		
		

    //
    // Loop forever.
    //
    while(1)
    {
			  //HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
		  
			 // button_count = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;
			
			 /* while(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
				{
					//smile
					button_count++;
					for(ulLoop = 0; ulLoop < 10000; ulLoop++)
						{
						}
					
					HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;	
					while(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
					{
						//disappointed
					   button_count++;
						 for(ulLoop = 0; ulLoop < 10000; ulLoop++)
							{
							}
						 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
						while(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
						{  //surprising
							  button_count++;
						    for(ulLoop = 0; ulLoop < 10000; ulLoop++)
							   {
							   }
								 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
							  while(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
								{
									//sad
									 button_count++;
									for(ulLoop = 0; ulLoop < 10000; ulLoop++)
									{
									}
									 Display96x16x1ImageDraw(g_pucSadLogo, 40, 0, 96, 2);
								}
							
							
								  Display96x16x1ImageDraw(g_pucSurpriseLogo, 40, 0, 96, 2);
							}
						
						
						
						
							Display96x16x1ImageDraw(g_pucDisappointedLogo, 40, 0, 96, 2);
						}
						
					
			
			
							Display96x16x1ImageDraw(g_pucSmileLogo, 40, 0, 96, 2);
					}
						
					
				
					*/
					
					if(button_count==1)
						 {
							 GPIO_PORTC_DATA_R |= 0x20;
	 Display96x16x1StringDraw("Smile",0, 0);
	 for(ulLoop = 0; ulLoop < 500000; ulLoop++)
						{
						}
	 Display96x16x1Clear();
	 Display96x16x1ImageDraw(g_pucSmileLogo, 40, 0, 96, 2);
						 }
						 
			  else if(button_count==2)
						 {
							 Display96x16x1StringDraw("Disappointed",0, 0);
		GPIO_PORTC_DATA_R ^= 0x20;
		for(ulLoop = 0; ulLoop < 500000; ulLoop++)
        {
        }
		Display96x16x1Clear();
		Display96x16x1ImageDraw(g_pucDisappointedLogo, 40, 0, 96, 2);
		GPIO_PORTC_DATA_R ^= 0x20;
	  
		for(ulLoop = 0; ulLoop < 500000; ulLoop++)
						{
						}
						 }
				else if(button_count==3){
						 
				//blinkpressed=1;
							 Display96x16x1StringDraw("Surprise",0, 0);
	 for(ulLoop = 0; ulLoop < 500000; ulLoop++)
						{
						}
	 Display96x16x1Clear();
	 Display96x16x1ImageDraw(g_pucSurpriseLogo, 40, 0, 96, 2);
						
		while(1){
		GPIO_PORTC_DATA_R |= 0x20;
		for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
        }
		GPIO_PORTC_DATA_R &= ~(0x20);
		//
        for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
        }
			}
						 }
				else if(button_count==4)
						 {
							 GPIO_PORTC_DATA_R &= ~(0x20);
	 Display96x16x1StringDraw("Sad",0, 0);
	 for(ulLoop = 0; ulLoop < 500000; ulLoop++)
						{
						}
	 Display96x16x1Clear();
	 Display96x16x1ImageDraw(g_pucSadLogo, 40, 0, 96, 2);
						 }
				/*else{
							 Display96x16x1StringDraw("Do Nothing",0, 0);
						 }
				 */
					
			   while(!HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS))
				{
					//smile
				/*	if(blinkpressed=1){
						//blibk here
					}*/
					
				}
					
					HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;	
					
			     button_count=1;
				
				/* for(ulLoop = 0; ulLoop < 1000000; ulLoop++)
							{
							}*/
				
				//2nd push button
				  for(ulLoop = 0; ulLoop < (300000*CLOCK_RATE / 4); ulLoop++)
				
			   {
					 end_pushing=1;
					 if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)==1)
					 {
						 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
						 button_count=2;
						 end_pushing=0;
						 break;
					 }
				 }
				 
				/* for(ulLoop = 0; ulLoop < 1000000; ulLoop++)
							{
							}*/
				 
				 //third push button
				 if(end_pushing==0){
				 for(ulLoop = 0; ulLoop < (300000*CLOCK_RATE / 4); ulLoop++)
				
			   {
					 end_pushing=1;
					 if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)==1)
					 {
						 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
						 button_count=3;
						 end_pushing=0;
						 break;
					 }
				 }
			 }
				 
			 //4th push button
				
			 if(end_pushing==0){
				 for(ulLoop = 0; ulLoop < (300000*CLOCK_RATE / 4); ulLoop++)
				
			   {
					 end_pushing=1;
					 if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)==1)
					 {
						 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
						 button_count=4;
						 end_pushing=0;
						 break;
					 }
				 }
			 }
			 
			 //fifth push button
			 
			 if(end_pushing==0){
				 end_pushing=1;
				 for(ulLoop = 0; ulLoop < (300000*CLOCK_RATE / 4); ulLoop++)
				
			   {
					 if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)==1)
					 {
						 button_count=0;
						 end_pushing=0;
						 more_pushing=1;
						 break;
					 }
				 }
			 }
				 
			while(more_pushing==1)
			{
				 for(ulLoop = 0; ulLoop < (300000*CLOCK_RATE / 4); ulLoop++)
				
			   {
				more_pushing=0;
				if(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)==1)
					 {
						 HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)=0;
						 more_pushing=1;
					 }
				 }
			}
			
      if(end_pushing==1){			
			Display96x16x1Clear();
				//blinky=0;
      }
			
		   
			/*  while(HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)){
					  button_count++;
					  
					if(button_count==2){
						Display96x16x1ImageDraw(g_pucSmileLogo, 40, 0, 96, 2);
					}
				
				}
			*/
			  
			
			   //Display96x16x1ImageDraw(g_pucSmileLogo, 40, 0, 96, 2);
			  //Display96x16x1ImageDraw(g_pucSurpriseLogo, 40, 0, 96, 2);
			  //Display96x16x1ImageDraw(g_pucSadLogo, 40, 0, 96, 2);
			   //Display96x16x1ImageDraw(g_pucDisappointedLogo, 40, 0, 96, 2);
			
		/*	for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
			
			
			
			  if(button_count != HWREGBITW(&g_ulFlags, FLAG_BUTTON))
          {*/
						
						
        //
        // Increment the debounce counter.
        //
       /* HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW) ^= 1;
        if(!HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_LOW))
        {
            HWREGBITW(&g_ulFlags, FLAG_DEBOUNCE_HIGH) = 1;
					  Display96x16x1StringDraw("twice",0, 0);
        }
							
							
				
			   else
							Display96x16x1StringDraw("once",0, 0);*/
						
						/* button_count2 = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;
						for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
			       button_count3 = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;
				for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
			       button_count4 = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;
				for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
			       button_count5 = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON) ? 1 : 0;
				for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
			
						
						 if(!button_count2 && !button_count3 && !button_count4 && !button_count5)
						 {
							 Display96x16x1StringDraw("once",0, 0);
						 }
						 
						 else if(button_count2 && !button_count3 && !button_count4 &&!button_count5)
						 {
							 //button_count++;
							 Display96x16x1StringDraw("twice",0, 0);
						 }
						 
						 else if(!button_count2 && button_count3 && !button_count4 && !button_count5)
						 {
							Display96x16x1StringDraw("thrice",0, 0);
						 }
						 
						 else if(!button_count2 && !button_count3 && button_count4 &&!button_count5)
						 {
							 Display96x16x1StringDraw("four",0, 0);
						 }
						 
						 else if(!button_count2 && !button_count3 && !button_count4 &&button_count5)
						 {
							 Display96x16x1StringDraw("five",0, 0);
						 }
						 
						//Display96x16x1StringDraw("Hello World",0, 0);
		      }
					
					*/
					
			/*		for(ulLoop = 0; ulLoop < (CLOCK_RATE / 30); ulLoop++)
        {
            //
            // Wait until a SysTick interrupt has occurred.
            //
            while(!HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK))
            {
            }

            //
            // Clear the SysTick interrupt flag.
            //
            HWREGBITW(&g_ulFlags, FLAG_CLOCK_TICK) = 0;
        }*/
			
			  /*while(!HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)){
					  
				
				}*/
			
			
			 /* Display96x16x1Clear();
			  Display96x16x1StringDraw("Hello World",0, 0);*/
       /* for(ulLoop = 0; ulLoop < 15000000; ulLoop++)
        {
        }
				for(count = 0; count < 96; count++)
        {
					Display96x16x1StringDraw("Hello World",count, 0);
					
						
					for(ulLoop = 0; ulLoop < 100000; ulLoop++)
        {
				}
				Display96x16x1Clear();
			}
				*/
		//	Output_Init();
      //Output_Color(1);
    //
    // Initialize the OLED display.
    //
      //Display96x16x1Init(true);
				
		  //ScrollImage(g_pucSmileLogo, sizeof(g_pucSmileLogo)/2);
		 /* while(!HWREGBITW(&g_ulFlags, FLAG_BUTTON_PRESS)){
				
				}*/
				
				
			
    }
	
}
