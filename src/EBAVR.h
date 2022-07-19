#ifndef _EBAVR_h
#define _EBAVR_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "pcb_io.h"


// 지정하지 않을시 기본값
#ifndef READPIN
  #define READPIN (~PINB)&0x0F
#endif
// ------------------------- 

extern volatile unsigned int g_uiSysTick;



#define OFF         0
#define ON          1
#define N_OFF       1
#define N_ON        0

#define TRUE        1
#define FALSE       0

#define LOW         0
#define HIGH        1
#define PULLUP      2
#define HIGHZ       3


#define ENABLE      1
#define DISABLE     0
#define TIMER       0
#define COUNTER     1
#define INTERNAL    0
#define EXTERNAL    1
#define IN          0
#define OUT         1
#define UP          0
#define DOWN        1
#define LEFT        2
#define RIGHT       3

#define GET         1
#define PUT         2
#define CLEAR       3
#define CHECK       4
#define PEEP        5

#define EVEN        0
#define ODD         1


#define BAUD2400      24
#define BAUD4800      48
#define BAUD9600      96
#define BAUD19200    192  
#define BAUD38400    384
#define BAUD57600    576
#define BAUD76800    768
#define BAUD115200  1152

#define MODE0       0
#define MODE1       1
#define MODE2       2
#define MODE3       3


#define EDGE_LEVEL  0
#define EDGE_CHANGE 1
#define EDGE_FALL   2
#define EDGE_RISE   3


// CLOCK TIMER
#define CLKTIMER0   0
#define CLKTIMER1   1
#define CLKTIMER2   2
#define CLKTIMER3   3
#define CLKTIMER4   4
#define CLKTIMER5   5
#define CLKTIMER6   6
#define CLKTIMER7   7
#define CLKTIMER8   8
#define CLKTIMER9   9


// same packet count
#define STATE_READY    0x00
#define STATE_START    0x80
#define STATE_END      0xC0



#define mSetBit(Var,Sel) Var |= (1<<Sel) 
#define mClrBit(Var,Sel) Var &= ~(1<<Sel)
#define mToggleBit(Var, Sel) Var ^= (1<<Sel) 
#define mIsSetBit(Var, Sel) (Var & (1<<Sel)) 
#define mIsClrBit(Var, Sel) !(Var & (1<<Sel))
#define mLSB(Var) (1<<Var)
#define mRSB(Var) (1>>var)


// #define LOWORD(Var)  (unsigned char) (Var & 0x00ff)
// #define HIWORD(Var)  (unsigned char) ((Var>>8) & 0x00ff)

#define enable()  asm("SEI")  // __enable_interrupt();
#define disable() asm("CLI")  // __disable_interrupt();
#define wdr()     asm("WDR")  // __watchdog_reset();






/* 
// ATMEAGA8A
#define pind    IO_ADDR(0x10)
#define ddrd    IO_ADDR(0x11)
#define portd   IO_ADDR(0x12)
#define pinc    IO_ADDR(0x13)
#define ddrc    IO_ADDR(0x14)
#define portc   IO_ADDR(0x15)
#define pinb    IO_ADDR(0x16)
#define ddrb    IO_ADDR(0x17)
#define portb   IO_ADDR(0x18)
*/





#define mStartTimer0(byValue) (TCNT0 = byValue, TCCR0A = 0x03) // internal clock/64, ATMEGA8A
#define mStartTimer0(byValue) (TCNT0 = byValue, TCCR0A = 0x03) // internal clock/64 
#define mStopTimer0() (TCCR0 = 0x00)
#define mStartTimer1(byValue) (TCNT1 = byValue, TCCR1B = 0x03) // internal clock/64
#define mStopTimer1() (TCCR1B = 0x00)


void SetWatchDog(unsigned char byAction);
void SetTimeInterrupt(unsigned char byNum, unsigned char bAction);
void SetExInterrupt(unsigned char num, unsigned char bAction, unsigned char type);
void SetSerial(unsigned char bAction, unsigned int uiBaudRate);
void SetADC(unsigned char bAction, unsigned char byNum);
unsigned int ReadADC(void);
unsigned char ReadEEPROM(unsigned int uiAddress);
void WriteEEPROM(unsigned int uiAddress, unsigned char byData);
int PutChar(int ch);







void delay(register unsigned int iValue);
void delay_us(register unsigned int iDelay_us);
void delay_ms(register unsigned int iDelay_ms);
void KeyDebounce(void);
unsigned char ScanKey(void);
void print(char *szData);
signed int Qmem(unsigned char byMode, unsigned char byQdata);
unsigned char SamePacketCount(unsigned char byStreamData);


// void InitMPU(void);
// void InitIO(void);




#endif

