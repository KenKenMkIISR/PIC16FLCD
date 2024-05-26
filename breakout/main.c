// Break out game (analog version) for PIC16F18313 + ILI9341 QVGA LCD (SPI) ball_y K.Tanaka

/*
 * PIC16F18313   - ILI9341(SPI)
 * 1 VDD +3.3V
 * 2 RA5(ANALOG)
 * 3 RA4         - RESET
 * 4 MCLR(RA3/START)
 * 5 RA2         - DC
 * 6 RA1(ICSPCLK)- SCK
 * 7 RA0(ICSBDAT)- SDI(MOSI)
 * 8 VSS (GND)
 *                 CS -> GND
 *                 SDO(MISO) -> (NC)
 */

#include "mcc_generated_files/mcc.h"
#include "ili9341driver.h"
#include "graphlib.h"

#define COLOR0 0x0000
#define COLOR1 0x001f
#define COLOR2 0xf800
#define COLOR3 0xf81f
#define COLOR4 0x07e0
#define COLOR5 0x07ff
#define COLOR6 0xffe0
#define COLOR7 0xffff

uint8_t ball_x;  //ball's x position (center)
int16_t ball_y;  //ball's y position (center)
int8_t  ball_vx; //ball's x speed
int16_t ball_vy; //ball's y speed
uint8_t padx; //pad's x position (left side)
uint8_t b1,b2,b3; //block bits (1:block exist)
uint8_t num; //number of blocks

#define TOP_Y 8
#define WALL 8 // wall width
#define BOTTOM_Y 309
#define PAD_Y (BOTTOM_Y-16) //pad's y position
#define BALLSIZE 8 // ball's size (radius)
#define PADSIZE 40 // pad width
#define BLOCKY1 (TOP_Y+50)
#define BLOCKY2 (TOP_Y+66)
#define BLOCKY3 (TOP_Y+82)

volatile uint16_t delaycounter=0;
// called by Timer0 interrupt service routine
void one_ms_interrupt(void){
	delaycounter++;
}
void delayms(uint16_t t){
	while(delaycounter<t)
		asm("SLEEP"); // Go to Idle mode
	delaycounter=0;
}

uint8_t random()
{
	static uint8_t r;
	// r=r*5+1;
	r=r+r+r+r+r;
	r++;
	return r;
}

// Draw ball and pad
void putballpad(void){
	g_circlefill((uint16_t)ball_x,ball_y,BALLSIZE-1,COLOR7);
	g_boxfill(padx,PAD_Y,padx+PADSIZE-1,PAD_Y+4,COLOR5);
}
// Erase ball and pad
void eraseballpad(void){
	g_circlefill((uint16_t)ball_x,ball_y,BALLSIZE-1,COLOR0);
	g_boxfill(padx,PAD_Y,padx+PADSIZE-1,PAD_Y+4,COLOR0);
}

// Draw a block
void putblock(uint8_t x,int y,uint16_t c){
	g_boxfill((int)x,y,(int)(x+30),y+14,c);
}

// Erase a block
void eraseblock(uint8_t x,int y){
	g_boxfill((int)x,y,(int)(x+30),y+14,COLOR0);
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
	TMR0_SetInterruptHandler(one_ms_interrupt);

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

	CPUDOZE=0x80; // Idle Enable

	LCD_Init();

	uint8_t x,d;
	uint16_t y;

	while(1){
		// Initialize the stage
		LCD_Clear(COLOR0);
		b1=0x7f;
		b2=0x7f;
		b3=0x7f;
		num=21;
		// draw wall
		g_boxfill(0         ,TOP_Y-WALL,WALL-1      ,BOTTOM_Y+10,COLOR7);
		g_boxfill(X_RES-WALL,TOP_Y-WALL,X_RES-1     ,BOTTOM_Y+10,COLOR7);
		g_boxfill(WALL      ,TOP_Y-WALL,X_RES-WALL-1,TOP_Y-1    ,COLOR7);
		//draw blocks
		for(uint8_t i=0;i<32*7;i+=32){
			putblock(i+WALL,BLOCKY1,COLOR2);
			putblock(i+WALL,BLOCKY2,COLOR4);
			putblock(i+WALL,BLOCKY3,COLOR6);
		}
		// wait for start button pressed
		while(!(~PORTA & 0x08)){
			random();
			delayms(17);
		}
		// inside of a stage loop
		while(num){
			padx=ADRESH; // analog read (upper 8bit)
			if(padx<WALL) padx=WALL;
			else if(padx+PADSIZE>X_RES-WALL) padx=X_RES-WALL-PADSIZE;
			ball_x=padx+PADSIZE/2;
			ball_y=PAD_Y-BALLSIZE;

			//ball_vx=(random()&3)*3-4;
			ball_vx=random()&3;
			ball_vx=ball_vx+ball_vx+ball_vx;
			ball_vx-=4;

			if(ball_vx==2 || ball_vx==-1) ball_vy=-6;
			else if(ball_vx==-4) ball_vy=-5;
			else ball_vy=-4;

			putballpad();

			// game's main loop
			while(1){
				putballpad();
				delayms(17);
				eraseballpad();
				//move pad
				padx=ADRESH; // analog read (upper 8bit)
				if(padx<WALL) padx=WALL;
				else if(padx+PADSIZE>X_RES-WALL) padx=X_RES-WALL-PADSIZE;
				// move ball
				ball_x+=(uint8_t)ball_vx;
				ball_y+=ball_vy;

				// Check the ball hits walls, blocks and the pad, or missed

				if(ball_vy>0){
					y=ball_y+BALLSIZE; // ball's lower end
					if(y>=BOTTOM_Y){
						// missed
						putballpad(); // show ball and pad again
						delayms(2000);
						eraseballpad();
						break; // return to a stage loop
					}
					if(y>PAD_Y && ball_x+4>=padx && ball_x-4<padx+PADSIZE){
						// hit by pad
						ball_y=PAD_Y-BALLSIZE;
						ball_vy=-ball_vy;
						if(random()&1){
							// change ball's speed
							if(ball_vx>=4){
								ball_vx-=random()&3;
							}
							else if(ball_vx<=-4){
								ball_vx+=random()&3;
							}
							else if(ball_vx>0){
								ball_vx+=random()&2;
							}
							else{
								ball_vx-=random()&2;
							}
							if(ball_vx>=5 || ball_vx<=-5) ball_vy=-4;
							else if(ball_vx>=3 || ball_vx<=-3) ball_vy=-5;
							else ball_vy=-6;
						}
					}
				}
				else{
					y=ball_y-BALLSIZE; // ball's upper end
					if(y<=TOP_Y){
						// hit top wall
						ball_y=TOP_Y+BALLSIZE;
						ball_vy=-ball_vy;
					}
				}
				// check the ball hits blocks from upper or lower side
				if(ball_vx<0) x=ball_x+4;
				else x=ball_x-4;

				// d:the bit number of block calculated from ball's x position
				// x:left end position of the block calculated from ball's x position
				// The width of block should be 32 dots
				// (Round off the lower 5 bits of ball's x position)
				d=1<<(x>>5);
				x=(x & 0xe0)+WALL;
				if(y>=BLOCKY1){
					if(y<BLOCKY1+16){
						if(b1 & d){
							b1&=~d;
							ball_vy=-ball_vy;
							eraseblock(x,BLOCKY1);
							num--;
						}
					}
					else if(y<BLOCKY2+16){
						if(b2 & d){
							b2&=~d;
							ball_vy=-ball_vy;
							eraseblock(x,BLOCKY2);
							num--;
						}
					}
					else if(y<BLOCKY3+16){
						if(b3 & d){
							b3&=~d;
							ball_vy=-ball_vy;
							eraseblock(x,BLOCKY3);
							num--;
						}
					}
				}

				// check the ball hits blocks from left or right side
				if(ball_vx<0) x=ball_x-BALLSIZE-WALL;
				else x=ball_x+BALLSIZE-WALL;
				d=1<<(x>>5);
				x=(x&0xffe0)+WALL;
				if(ball_vy>0) y=ball_y-4;
				else y=ball_y+4;
				if(y>=BLOCKY1){
					if(y<BLOCKY1+16){
						if(b1 & d){
							b1&=~d;
							ball_vx=-ball_vx;
							eraseblock(x,BLOCKY1);
							num--;
						}
					}
					else if(y<BLOCKY2+16){
						if(b2 & d){
							b2&=~d;
							ball_vx=-ball_vx;
							eraseblock(x,BLOCKY2);
							num--;
						}
					}
					else if(y<BLOCKY3+16){
						if(b3 & d){
							b3&=~d;
							ball_vx=-ball_vx;
							eraseblock(x,BLOCKY3);
							num--;
						}
					}
				}
				if(ball_x-BALLSIZE<=WALL){
					// hit left wall
					ball_x=BALLSIZE+WALL;
					ball_vx=-ball_vx;
				}
				else if(ball_x+BALLSIZE>=X_RES-WALL){
					// hit right wall
					ball_x=X_RES-WALL-BALLSIZE;
					ball_vx=-ball_vx;
				}
				if(num==0){
					// broke all blocks
					delayms(1000);
					break; // go to new stage
				}
			}
		}
	}
}