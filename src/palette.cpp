/*******************************************************
*                                                      *
* ####   ###  ##    ##### ###### ###### #####     #### *
* ## ## ## ## ##    ##      ##     ##   ##       ##    *
* ####  ##### ##    ####    ##     ##   ####     ##    *
* ##    ## ## ##    ##      ##     ##   ##       ##    *
* ##    ## ## ##### #####   ##     ##   ##### ##  #### *
*                                                      *
*******************************************************/
/****************
* include files *
****************/
#include "graphics.h"

#define RED 0
#define GREEN 1
#define BLUE 2

/************************************************
* function to spread from one colour to another *
************************************************/
void spread_col(int start_col, int end_col, int palette[][3])
{
	float dr, dg, db, red, green, blue;
	int minc, maxc, size, loop;

	/* make sure that start col references the colour that is lowest */
	/* in the colour lookup-table */
	if (start_col > end_col)
	{
		maxc = start_col;
		minc = end_col;
	}
	else
	{
		maxc = end_col;
		minc = start_col;
	}

	/* now find the rate of change in the red, green and blue */
	/* between the start and end colours */
	size = maxc - minc;
	dr = (float)(palette[maxc][RED] - palette[minc][RED]) / (float)size;
	dg = (float)(palette[maxc][GREEN] - palette[minc][GREEN]) / (float)size;
	db = (float)(palette[maxc][BLUE] - palette[minc][BLUE]) / (float)size;

	/* set the initial red, green and blue values to those found */
	/* at the start colour */
	red = (float)palette[minc][RED];
	green = (float)palette[minc][GREEN];
	blue = (float)palette[minc][BLUE];

	for (loop = 1; loop < size; loop++)
	{
		/* now for every colour value after the start colour and up to */
		/* and including the end colour, start to change the RGB values */
		red += dr;
		green += dg;
		blue += db;

		/* set the values in the palette array */
		palette[minc+loop][RED] = (int)red;
		palette[minc+loop][GREEN] = (int)green;
		palette[minc+loop][BLUE] = (int)blue;
	}
}

/***************************************************************************
* function to set the hardware colours in the lookup table to those values *
* found within the palette array                                           *
***************************************************************************/
void set_palette(int palette[][3])
{
	int loop;

	/* this loop runs through the palette array and sets the colours */
	/* accordingly - we assume that there is 256 entries in this table */
	for (loop = 0; loop < 256; loop++)
		setrgbpalette(loop, palette[loop][RED],
					  palette[loop][GREEN],
					  palette[loop][BLUE]);
}

/***************************************************************************
* function to create the palette needed by the virtual world program       *
***************************************************************************/
void create_palette(void)
{
	/* first create the array to store the colour values */
	int palette[256][3];

	/* we need to set all the colours in the array */
	/* the array Is spllt lnto 16 blocks of 16 entries each */
	/* each block contaxns a range of shades for a colour that an object */
	/* can be asslgned to */

	/* block 0 - black */
	/* start colour */
	palette[0][RED] = 0;
	palette[0][GREEN] = 0;
	palette[0][BLUE] = 0;
	/* end colour */
	palette[15][RED] = 128;
	palette[15][GREEN] = 128;
	palette[15][BLUE] = 128;
	/* spread between them */
	spread_col(0, 15, palette);

	/* block 1 - blue */
	/* start colour */
	palette[16][RED] = 0;
	palette[16][GREEN] = 0;
	palette[16][BLUE] = 64;
	/* 2nd start colour */
	palette[27][RED] = 0;
	palette[27][GREEN] = 0;
	palette[27][BLUE] = 192;
	/* end colour */
	palette[31][RED] = 192;
	palette[31][GREEN] = 192;
	palette[31][BLUE] = 255;
	/* spread between them */
	spread_col(16, 27, palette);
	spread_col(27, 31, palette);

	/* block 2 = green */
	/* start colour */
	palette[32][RED] = 0;
	palette[32][GREEN] = 64;
	palette[32][BLUE] = 0;
	/* 2nd start colour */
	palette[43][RED] = 0;
	palette[43][GREEN] = 192;
	palette[43][BLUE] = 0;
	/* end colour */
	palette[47][RED] = 192;
	palette[47][GREEN] = 255;
	palette[47][BLUE] = 192;
	/* spread between them */
	spread_col(32, 43, palette);
	spread_col(43, 47, palette);

	/* block 3 = cyan */
	/* start colour */
	palette[48][RED] = 0;
	palette[48][GREEN] = 64;
	palette[48][BLUE] = 64;
	/* 2nd start colour */
	palette[59][RED] = 0;
	palette[59][GREEN] = 192;
	palette[59][BLUE] = 192;
	/* end colour */
	palette[63][RED] = 192;
	palette[63][GREEN] = 255;
	palette[63][BLUE] = 255;
	/* spread between them */
	spread_col(48, 59, palette);
	spread_col(59, 63, palette);

	/* block 4 = red */
	/* start colour */
	palette[64][RED] = 64;
	palette[64][GREEN] = 0;
	palette[64][BLUE] = 0;
	/* 2nd start colour */
	palette[75][RED] = 192;
	palette[75][GREEN] = 0;
	palette[75][BLUE] = 0;
	/* end colour */
	palette[79][RED] = 255;
	palette[79][GREEN] = 192;
	palette[79][BLUE] = 192;
	/* spread between them */
	spread_col(64, 75, palette);
	spread_col(75, 79, palette);

	/* block 5 = magenta */
	/* start colour */
	palette[80][RED] = 64;
	palette[80][GREEN] = 0;
	palette[80][BLUE] = 64;
	/* 2nd start colour */
	palette[91][RED] = 192;
	palette[91][GREEN] = 0;
	palette[91][BLUE] = 192;
	/* end colour */
	palette[95][RED] = 255;
	palette[95][GREEN] = 192;
	palette[95][BLUE] = 255;
	/* spread between them */
	spread_col(80, 91, palette);
	spread_col(91, 95, palette);

	/* block 6 = brown */
	/* start colour */
	palette[96][RED] = 64;
	palette[96][GREEN] = 32;
	palette[96][BLUE] = 0;
	/* 2nd start colour */
	palette[107][RED] = 160;
	palette[107][GREEN] = 96;
	palette[107][BLUE] = 0;
	/* end colour */
	palette[111][RED] = 255;
	palette[111][GREEN] = 192;
	palette[111][BLUE] = 160;
	/* spread between them */
	spread_col(96, 107, palette);
	spread_col(107, 111, palette);

	/* block 7 = light grey */
	/* start colour */
	palette[112][RED] = 96;
	palette[112][GREEN] = 96;
	palette[112][BLUE] = 96;
	/* end colour */
	palette[127][RED] = 224;
	palette[127][GREEN] = 224;
	palette[127][BLUE] = 224;
	/* spread between them */
	spread_col(112, 127, palette);

	/* block 8 = dark grey */
	/* start colour */
	palette[128][RED] = 64;
	palette[128][GREEN] = 64;
	palette[128][BLUE] = 64;
	/* end colour */
	palette[143][RED] = 160;
	palette[143][GREEN] = 160;
	palette[143][BLUE] = 160;
	/* spread between them */
	spread_col(128, 143, palette);

	/* block 9 = light blue */
	/* start colour */
	palette[144][RED] = 64;
	palette[144][GREEN] = 64;
	palette[144][BLUE] = 128;
	/* 2nd start colour */
	palette[151][RED] = 128;
	palette[151][GREEN] = 128;
	palette[151][BLUE] = 192;
	/* end colour */
	palette[159][RED] = 224;
	palette[159][GREEN] = 224;
	palette[159][BLUE] = 255;
	/* spread between them */
	spread_col(144, 151, palette);
	spread_col(151, 159, palette);

	/* block 10 = light green */
	/* start colour */
	palette[160][RED] = 64;
	palette[160][GREEN] = 128;
	palette[160][BLUE] = 64;
	/* 2nd start colour */
	palette[167][RED] = 128;
	palette[167][GREEN] = 192;
	palette[167][BLUE] = 128;
	/* end colour */
	palette[175][RED] = 224;
	palette[175][GREEN] = 255;
	palette[175][BLUE] = 224;
	/* spread between them */
	spread_col(160, 167, palette);
	spread_col(167, 175, palette);

	/* block 11 = light cyan */
	/* start colour */
	palette[176][RED] = 64;
	palette[176][GREEN] = 128;
	palette[176][BLUE] = 128;
	/* 2nd start colour */
	palette[183][RED] = 128;
	palette[183][GREEN] = 192;
	palette[183][BLUE] = 192;
	/* end colour */
	palette[191][RED] = 224;
	palette[191][GREEN] = 255;
	palette[191][BLUE] = 255;
	/* spread between them */
	spread_col(176, 183, palette);
	spread_col(183, 191, palette);

	/* block 12 = light red */
	/* start colour */
	palette[192][RED] = 128;
	palette[192][GREEN] = 64;
	palette[192][BLUE] = 64;
	/* 2nd start colour */
	palette[199][RED] = 192;
	palette[199][GREEN] = 128;
	palette[199][BLUE] = 128;
	/* end colour */
	palette[207][RED] = 255;
	palette[207][GREEN] = 224;
	palette[207][BLUE] = 224;
	/* spread between them */
	spread_col(192, 199, palette);
	spread_col(199, 207, palette);

	/* block 13 = light magenta */
	/* start colour */
	palette[208][RED] = 128;
	palette[208][GREEN] = 64;
	palette[208][BLUE] = 128;
	/* 2nd start colour */
	palette[215][RED] = 192;
	palette[215][GREEN] = 128;
	palette[215][BLUE] = 192;
	/* end colour */
	palette[223][RED] = 255;
	palette[223][GREEN] = 224;
	palette[223][BLUE] = 255;
	/* spread between them */
	spread_col(208, 215, palette);
	spread_col(215, 223, palette);

	/* block 14 = yellow */
	/* start colour */
	palette[224][RED] = 64;
	palette[224][GREEN] = 64;
	palette[224][BLUE] = 0;
	/* 2nd start colour */
	palette[231][RED] = 128;
	palette[231][GREEN] = 128;
	palette[231][BLUE] = 0;
	/* end colour */
	palette[239][RED] = 255;
	palette[239][GREEN] = 255;
	palette[239][BLUE] = 192;
	/* spread between them */
	spread_col(224, 231, palette);
	spread_col(231, 239, palette);

	/* block 15 = white */
	/* start colour */
	palette[240][RED] = 160;
	palette[240][GREEN] = 160;
	palette[240][BLUE] = 160;
	/* end colour */
	palette[255][RED] = 255;
	palette[255][GREEN] = 255;
	palette[255][BLUE] = 255;
	/* spread between them */
	spread_col(240, 255, palette);

	/* now we set the hardware colour lookup table to the values found */
	/* in the palette array */
	set_palette(palette);
}

/****************************************************************************
* function that simply displays the contents of the colour look up table on *
* a graphics display capable of showing all 256 colours (use something like *
* a vesa bgi driver or other svga type)                                     *
****************************************************************************/
void check_palette(void)
{
	int loop1, loop2;
	int dx, dy, colour;

	/* set the rate of change horizontally
	   and vertically acrOsS the screen */
	dx = getmaxx() / 16;
	dy = getmaxy() / 16;

	/* set initial colour to zero */
	colour = 0;
	for (loop1 = 0; loop1 < 16; loop1++)
		for (loop2 = 0; loop2 < 16; loop2++)
		{
			/* draw a coloured box */
			setcolor(colour++);
			bar(loop2*dx, loop1*dy, loop2*dx+dx, loop1*dy+dy);
		}
}
