/*
 * File:   newmain.c
 * Author: gombe
 *
 * Created on 2017/08/06, 20:24
 */



#include <xc.h>
#include <stdint.h>
#include <cp0defs.h>


#define DMA_TRANSLATION_SIZE 12

#define ONE_LINE_BIT 3632
#define LINE_BUFFER_SIZE (ONE_LINE_BIT/8)

// LINE_BUFFER_SIZE は DMA_TRANSLATION_SIZE　で割り切れなければならない。 
#define NUM_OF_BUFFERS 6 //6,12,...

#define BUFFER_SIZE (NUM_OF_BUFFERS * LINE_BUFFER_SIZE)

uint8_t ch0data[BUFFER_SIZE];
uint8_t ch1data[BUFFER_SIZE];
uint8_t ch2data[BUFFER_SIZE];
uint8_t ch3data[BUFFER_SIZE];
uint8_t ch4data[BUFFER_SIZE];

int drawing_line;

#define LOW_SPEED_SIGNAL 0

uint32_t colordata[]={
  0x0000ff,0x0000ff,//(r,g,b)=(0,0,0)
  0x00f8c5,0x000f37,//(r,g,b)=(128,0,0)
  0x0000ff,0x0000ff,//(r,g,b)=(0,0,0)
  0x000ff0,0x00fc23,//(r,g,b)=(0,64,0)
  0x010ef0,0xa05e5d,//(r,g,b)=(0,128,0)
  0x010ef7,0xfc02f1,//(r,g,b)=(0,192,0)
  0x0000ff,0x0000ff,//(r,g,b)=(0,0,0)
  0x007f88,0x0000e4,//(r,g,b)=(0,0,128)
};

void __SPIConfig(void){
/*spi configulation*/
/* Enable enhanced buffer*/
/* baud rate is PBx / 4 */
  SPI2BRG = LOW_SPEED_SIGNAL;
  SPI3BRG = LOW_SPEED_SIGNAL;
  SPI4BRG = LOW_SPEED_SIGNAL;
  SPI5BRG = LOW_SPEED_SIGNAL;
  SPI6BRG = LOW_SPEED_SIGNAL;
  SPI5CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI4CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI3CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI6CON = 0x18120;
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  SPI2CON = 0x18120;
}


void Video_Init(void){
  __SPIConfig();
  asm("nop");
  asm("nop");
  /*TMR2 enable toggle interval:DMA_TRANSLATION_SIZE[byte] * 8[bits] * (2 * 2)SPIDIV*/
  PR2 = (DMA_TRANSLATION_SIZE<<(3+1+LOW_SPEED_SIGNAL))-1;//SPLLCON
  TMR2 = 0;
    
  /*DMA configulation*/
  DMACON = 0x8000;
  
  /* note :
   *  In the channels natural priority order, channel 0 has the highest priority. 
   */
  
  /*channel 0*/
  DCH0CON=0x12;// channel off, pri 2, no chaining, auto mode
  DCH0ECON=(_TIMER_2_VECTOR << 8)| 0x10;//set trigger :TIMER2 IRQ
  // program the transfer
  DCH0SSA=((uint32_t)ch0data)&0x1FFFFFFF;// transfer source physical address
  DCH0DSA=((uint32_t)(&SPI5BUF))&0x1FFFFFFF;// transfer destination physical address
  DCH0SSIZ=BUFFER_SIZE;// source size 15 bytes
  DCH0DSIZ=1;// destination size 1 bytes  
  DCH0CSIZ=DMA_TRANSLATION_SIZE;// 15 bytes transferred per event
  DCH0INTCLR=0x00ff00ff;// clear existing events, disable all interrupts
  DCH0CONSET=0x80;// turn channel on

  /*channel 1*/
  DCH1CON=0x12;
  DCH1ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH1SSA=((uint32_t)ch1data)&0x1FFFFFFF;
  DCH1DSA=((uint32_t)(&SPI4BUF))&0x1FFFFFFF;
  DCH1SSIZ=BUFFER_SIZE;
  DCH1DSIZ=1;
  DCH1CSIZ=DMA_TRANSLATION_SIZE;
  DCH1INTCLR=0x00ff00ff;
  DCH1CONSET=0x80;

  /*channel 2*/
  DCH2CON=0x12;
  DCH2ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH2SSA=((uint32_t)ch2data)&0x1FFFFFFF;
  DCH2DSA=((uint32_t)(&SPI3BUF))&0x1FFFFFFF;
  DCH2SSIZ=BUFFER_SIZE;
  DCH2DSIZ=1;
  DCH2CSIZ=DMA_TRANSLATION_SIZE;
  DCH2INTCLR=0x00ff00ff;
  DCH2CONSET=0x80;

  /*channel 3 low priority*/
  DCH3CON=0x11;
  DCH3ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH3SSA=((uint32_t)ch3data)&0x1FFFFFFF;
  DCH3DSA=((uint32_t)(&SPI6BUF))&0x1FFFFFFF;
  DCH3SSIZ=BUFFER_SIZE;
  DCH3DSIZ=1;
  DCH3CSIZ=DMA_TRANSLATION_SIZE;
  DCH3INTCLR=0x00ff00ff;
  DCH3CONSET=0x80;

  /*channel 4*/
  DCH4CON=0x11;
  DCH4ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH4SSA=((uint32_t)ch4data)&0x1FFFFFFF;
  DCH4DSA=((uint32_t)(&SPI2BUF))&0x1FFFFFFF;
  DCH4SSIZ=BUFFER_SIZE;
  DCH4DSIZ=1;
  DCH4CSIZ=DMA_TRANSLATION_SIZE;
  DCH4INTCLR=0x00ff00ff;
  DCH4CONSET=0x80;

  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  SPI5BUF = 0xFF;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI4BUF = 0xFF;
  SPI4BUF = 0;
  SPI5BUF = 0;
  SPI3BUF = 0xFF;
  SPI4BUF = 0;
  SPI3BUF = 0;

  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  T2CONSET = 0x8000;//TMR2 ON
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
  SPI5BUF = 0xAA;  SPI4BUF = 0xAA;  SPI3BUF = 0xAA;
}

int calc_buffer_pointer(void){
  return DCH0SPTR / LINE_BUFFER_SIZE;
}

void main(void) {
  __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));

  CHECONbits.PFMWS = 2;
  //    CHECONbits.PREFEN = 0x1;

  /*remap pins*/
  RPB10R=0x0A; //RPB10:SCK5
  RPB11R=0x0E; //RPB11:SDO3
  RPB12R=0x04; //PRB12:SDO2
  RPB13R=0x11; //RPB13:SDO6
  RPB14R=0x0F; //RPB14:SDO4
  RPB15R=0x10; //RPB15:SDO5

  TRISBCLR = 1<<9;

  {
    volatile int i;
    for(i=0;i<100000;i++);
  }
  while(!(PB3DIV & (0x1<<11)));
  SYSKEY = 0x0;             // ensure OSCCON is locked
  SYSKEY = 0xAA996655;      // Write Key1 to SYSKEY
  SYSKEY = 0x556699AA;      // Write Key2 to SYSKEY
  // OSCCON is now unlocked //PB2DIV
  // make the desired change
  PB2DIV = 0x8000;
  PB3DIV = 0x8000;
  PB6DIVSET = 0x7F;
  // Relock the SYSKEY
  SYSKEY = 0x0;

  int i=0;
  ch0data[i] = 0xAA;
  ch1data[i] = 0xAA;
  ch2data[i] = 0xAA;
  //0xAD,0xF8,0xAD,0xF8,0x70

  for(i=1;i<BUFFER_SIZE;i++){
    ch0data[i] = i/5000%3==0?0xFF:0;
    ch1data[i] = i/5000%3==1?0xFF:00;
    ch2data[i] = i/5000%3==2?0xFF:0;
  }

  Video_Init();  
  
  testapp();  
  
  while(1);
  return;
}



/* VSYNC 
 * 
 *  2                                        +-------+
 *                                           |       |
 *  0  --------------------------------------+       +--
 *     |<           420 3/8                 >|<33 5/8>|
 */

void VSYNC(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  int i;
  for(i=0;i<420;i++){
    ch0data[i] = 0;
    ch1data[i] = 0;
    ch2data[i] = 0;
  }
  ch0data[i] = 0;
  ch1data[i] = 0xFF>>(8-3);
  ch2data[i] = 0;
  i++;
  for(;i<421+33;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
}

/*  HSYNC and colorburst or so     
 *                                        +------------------+
 *                                +       |                  |         +
 *                                |       |   < NO IMAGE >   |         |
 *                -   -   -       |       |                  |         |
 *  +       +----/ \ / \ / \ /----+       +------------------+         +----+
 *  |       |       -   -   -                                               |
 *  +-------+                                                               +---
 *
 *  |<HSYNC>|    |<Colorburst>|   |<          Image Data              >|    |
 *  |33 5/8 |4 3/8|     18                        320
 * 
 *  |-33 5/8>| 38>|    56    >|74>|              394                  >|454 >|     
 */

void HSYNC_colorburst(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  int i;
  /*HSYNC*/
  for(i=0;i<33;i++){
    ch0data[i] = 0;
    ch1data[i] = 0;
    ch2data[i] = 0;
  }
  ch0data[i] = 0;
  ch1data[i] = 0xFF>>(5);
  ch2data[i] = 0;
  for(;i<38;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
  /*colorburst*/
  for(;i<56;i++){
    ch0data[i] = 0xC1;
    ch1data[i] = 0x7C;
    ch2data[i] = 0;
    i++;
    ch0data[i] = 0x81;
    ch1data[i] = 0;
    ch2data[i] = 0;
  }
  for(;i<454;i++){
    ch0data[i] = 0;
    ch1data[i] = 0xFF;
    ch2data[i] = 0;
  }
}

/*
  HSYNC COUNT ... 10
  ELSE  COUNT ... 252
  SUM 262
 */


// パルス幅定数
#define V_NTSC		262					// 262本/画面
#define V_SYNC		10					// 垂直同期本数

void VTask(uint8_t *ch0data,uint8_t *ch1data,uint8_t *ch2data){
  if(drawing_line < V_SYNC){
    VSYNC(ch0data,ch1data,ch2data);
  }else{
    HSYNC_colorburst(ch0data, ch1data, ch2data);
    if(drawing_line > 30&&drawing_line < 200){
      int i=100;
      for(;i<350;i++){
	uint32_t d;
	d = colordata[(i>>4)&0x7];
	ch0data[i] = d;
	ch1data[i] = d>>8;
	ch2data[i] = d>>16;
	i++;
	d = colordata[1+((i>>4)&0x7)];
	ch0data[i] = d;
	ch1data[i] = d>>8;
	ch2data[i] = d>>16;
      }
    }
  }
  drawing_line++;
  if(drawing_line==V_NTSC)drawing_line = 0;
}

void testapp(void){
  int i;
  while(1){
    for(i=0;i<NUM_OF_BUFFERS;i++){
      while(calc_buffer_pointer()!=(i+5)%NUM_OF_BUFFERS)LATBINV = 1<<9;
      if(46 < drawing_line&&drawing_line < 250) {drawing_line++;continue;}
      VTask(ch0data+LINE_BUFFER_SIZE*i, ch1data+LINE_BUFFER_SIZE*i, ch2data+LINE_BUFFER_SIZE*i);
    }
  }
  while(1);
}
