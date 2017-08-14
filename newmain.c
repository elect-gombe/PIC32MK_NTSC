/*
 * File:   newmain.c
 * Author: gombe
 *
 * Created on 2017/08/06, 20:24
 */

// PIC32MK1024GPD064 Configuration Bit Settings


// PIC32MK1024GPD064 Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
#pragma config FUSBIDIO2 = OFF          // USB2 USBID Selection (USBID pin is controlled by the port function)
#pragma config FVBUSIO2 = OFF           // USB2 VBUSON Selection bit (VBUSON pin is controlled by the port function)
#pragma config PGL1WAY = ON             // Permission Group Lock One Way Configuration bit (Allow only one reconfiguration)
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO1 = OFF          // USB1 USBID Selection (USBID pin is controlled by the port function)
#pragma config FVBUSIO1 = OFF           // USB2 VBUSON Selection bit (VBUSON pin is controlled by the port function)

// DEVCFG2
#pragma config FPLLIDIV = DIV_1         // System PLL Input Divider (1x Divider)
#pragma config FPLLRNG = RANGE_5_10_MHZ // System PLL Input Range (8-16 MHz Input)
#pragma config FPLLICLK = PLL_FRC       // System PLL Input Clock Selection (FRC is input to the System PLL)
#pragma config FPLLMULT = MUL_50        // System PLL Multiplier (PLL Multiply by 57)
#pragma config FPLLODIV = DIV_32         // System PLL Output Clock Divider (4x Divider)
#pragma config VBATBOREN = OFF          // VBAT BOR Enable (Disable ZPBOR during VBAT Mode)
#pragma config DSBOREN = OFF            // Deep Sleep BOR Enable (Disable ZPBOR during Deep Sleep Mode)
#pragma config DSWDTPS = DSPS32         // Deep Sleep Watchdog Timer Postscaler (1:2^36)
#pragma config DSWDTOSC = LPRC          // Deep Sleep WDT Reference Clock Selection (Select LPRC as DSWDT Reference clock)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer Enable (Disable DSWDT during Deep Sleep Mode)
#pragma config FDSEN = OFF              // Deep Sleep Enable (Enable DSEN bit in DSCON)
#pragma config UPLLEN = OFF             // USB PLL Enable (USB PLL Disabled)

// DEVCFG1
#pragma config FNOSC = FRC              // Oscillator Selection Bits (System PLL)
#pragma config DMTINTV = WIN_127_128    // DMT Count Window Interval (Window/Interval value is 127/128 counter value)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Enable Secondary Oscillator)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch Enabled, FSCM Enabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WDTSPGM = STOP           // Watchdog Timer Stop During Flash Programming (WDT stops during Flash programming)
#pragma config WINDIS = NORMAL          // Watchdog Timer Window Mode (Watchdog Timer is in non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window size is 25%)
#pragma config DMTCNT = DMT31           // Deadman Timer Count Selection (2^31 (2147483648))
#pragma config FDMTEN = OFF             // Deadman Timer Enable (Deadman Timer is disabled)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config TRCEN = ON               // Trace Enable (Trace features in the CPU are enabled)
#pragma config BOOTISA = MIPS32         // Boot ISA Selection (Boot code and Exception code is MIPS32)
#pragma config FSLEEP = OFF             // Flash Sleep Mode (Flash is powered down when the device is in Sleep mode)
#pragma config SMCLR = MCLR_NORM        // Soft Master Clear Enable (MCLR pin generates a normal system Reset)
#pragma config SOSCGAIN = GAIN_2X       // Secondary Oscillator Gain Control bits (2x gain setting)
#pragma config SOSCBOOST = ON           // Secondary Oscillator Boost Kick Start Enable bit (Boost the kick start of the oscillator)
#pragma config POSCGAIN = GAIN_LEVEL_3  // Primary Oscillator Gain Control bits (Gain Level 3 (highest))
#pragma config POSCBOOST = ON           // Primary Oscillator Boost Kick Start Enable bit (Boost the kick start of the oscillator)
#pragma config EJTAGBEN = NORMAL        // EJTAG Boot Enable (Normal EJTAG functionality)

// DEVCP
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// SEQ

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <cp0defs.h>


#define TEST_BUF_SIZE 12
uint8_t transdata[] = {
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
};

#define LOW_SPEED_SIGNAL 1

void SPIConfig(void){
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
  SPI4CON = 0x18120;
  asm volatile("nop");
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

void main(void) {
  __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));
  
  CHECONbits.PFMWS = 3;//PB2DIV
  //    CHECONbits.PREFEN = 0x1;

  /*remap pins*/
  RPB11R=0x0E; //RPB11:SDO3
  RPB12R=0x04; //PRB12:SDO2
  RPB13R=0x11; //RPB13:SDO6
  RPB14R=0x0F; //RPB14:SDO4
  RPB15R=0x10; //RPB15:SDO5

  SYSKEY = 0x0;             // ensure OSCCON is locked
  SYSKEY = 0xAA996655;      // Write Key1 to SYSKEY
  SYSKEY = 0x556699AA;      // Write Key2 to SYSKEY

  // OSCCON is now unlocked
  // make the desired change
  OSCCONbits.FRCDIV = 2;
  PB2DIVCLR = 0x7F;
  PB3DIVCLR = 0x7F;
  PB6DIVSET = 0x7F;
  // Relock the SYSKEY
  SYSKEY = 0x0;

  SPIConfig();
  
  /*TMR2 enable toggle interval:TEST_BUF_SIZE[byte] * 8[bits] * (2 * 2)SPIDIV*/
  PR2 = (TEST_BUF_SIZE<<(3+1+LOW_SPEED_SIGNAL))-1;
  TMR2 = PR2-1;
    
  /*DMA configulation*/
  DMACON = 0x8000;
  
  /* note :
   *  In the channels natural priority order, channel 0 has the highest priority. 
   */

  /*channel 0*/
  DCH0CON=0x12;// channel off, pri 2, no chaining, auto mode
  DCH0ECON=(_TIMER_2_VECTOR << 8)| 0x10;//set trigger :TIMER2 IRQ
  // program the transfer
  DCH0SSA=((uint32_t)transdata)&0x1FFFFFFF;// transfer source physical address
  DCH0DSA=((uint32_t)(&SPI5BUF))&0x1FFFFFFF;// transfer destination physical address
  DCH0SSIZ=TEST_BUF_SIZE;// source size 15 bytes
  DCH0DSIZ=1;// destination size 1 bytes  
  DCH0CSIZ=TEST_BUF_SIZE;// 15 bytes transferred per event
  DCH0INTCLR=0x00ff00ff;// clear existing events, disable all interrupts
  DCH0CONSET=0x80;// turn channel on

  /*channel 1*/
  DCH1CON=0x12;
  DCH1ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH1SSA=((uint32_t)transdata)&0x1FFFFFFF;
  DCH1DSA=((uint32_t)(&SPI4BUF))&0x1FFFFFFF;
  DCH1SSIZ=TEST_BUF_SIZE;
  DCH1DSIZ=1;
  DCH1CSIZ=TEST_BUF_SIZE;
  DCH1INTCLR=0x00ff00ff;
  DCH1CONSET=0x80;

  /*channel 2*/
  DCH2CON=0x12;
  DCH2ECON=(_TIMER_2_VECTOR << 8)| 0x10;  
  DCH2SSA=((uint32_t)transdata)&0x1FFFFFFF;
  DCH2DSA=((uint32_t)(&SPI3BUF))&0x1FFFFFFF;
  DCH2SSIZ=TEST_BUF_SIZE;
  DCH2DSIZ=1;
  DCH2CSIZ=TEST_BUF_SIZE;
  DCH2INTCLR=0x00ff00ff;
  DCH2CONSET=0x80;

  /*channel 3*/
  DCH3CON=0x12;
  DCH3ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH3SSA=((uint32_t)transdata)&0x1FFFFFFF;
  DCH3DSA=((uint32_t)(&SPI6BUF))&0x1FFFFFFF;
  DCH3SSIZ=TEST_BUF_SIZE;
  DCH3DSIZ=1;
  DCH3CSIZ=TEST_BUF_SIZE;
  DCH3INTCLR=0x00ff00ff;
  DCH3CONSET=0x80;

  /*channel 4*/
  DCH4CON=0x12;
  DCH4ECON=(_TIMER_2_VECTOR << 8)| 0x10;
  DCH4SSA=((uint32_t)transdata)&0x1FFFFFFF;
  DCH4DSA=((uint32_t)(&SPI2BUF))&0x1FFFFFFF;
  DCH4SSIZ=TEST_BUF_SIZE;
  DCH4DSIZ=1;
  DCH4CSIZ=TEST_BUF_SIZE;
  DCH4INTCLR=0x00ff00ff;
  DCH4CONSET=0x80;

  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI5BUF = 0;
  SPI5BUF = 0;
  
  SPI4BUF = 0;
  SPI4BUF = 0;
  SPI4BUF = 0;
  SPI4BUF = 0;
  SPI4BUF = 0;
  SPI4BUF = 0;
  
  SPI3BUF = 0;
  SPI3BUF = 0;
  SPI3BUF = 0;
  asm volatile("add $zero,$zero,$zero");
  asm volatile("add $zero,$zero,$zero");
  asm volatile("add $zero,$zero,$zero");
  asm volatile("add $zero,$zero,$zero");

  SPI6BUF = 0;
  SPI6BUF = 0;
  SPI6BUF = 0;
  SPI6BUF = 0;
  SPI3BUF = 0;
  SPI4BUF = 0;

  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI2BUF = 0;
  SPI6BUF = 0;
  SPI6BUF = 0;
  SPI6BUF = 0;
  SPI3BUF = 0;
  SPI3BUF = 0;
  SPI3BUF = 0;

  SPI5BUF = 0;  
  SPI4BUF = 0;
  SPI3BUF = 0;
  SPI6BUF = 0;
  SPI2BUF = 0;

  SPI5BUF = 0;  
  SPI4BUF = 0;
  SPI3BUF = 0;
  SPI6BUF = 0;
  SPI2BUF = 0;



  /* DCH4ECONSET = 0x80; */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* DCH3ECONSET = 0x80; */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* DCH2ECONSET = 0x80; */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* DCH1ECONSET = 0x80; */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* asm("nop"); */
  /* DCH0ECONSET = 0x80;//force transfer */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  /* asm volatile("nop"); */
  
  T2CONSET = 0x8000;//TMR2 ON
  
  
  
  while(1);

  return;
}
