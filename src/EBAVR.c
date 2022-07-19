//-------------------------------------------------------------------
//  ��  �� : AVR Series �⺻ �Լ�
//  ȭ�ϸ� : EBAVR.C
//  ��  �� : 0.4

//  ������̽� : ATMEGA8 (16MHz)
//  �����Ϸ� : AVR-GCC Ver 7.3.0
//  �����Ϸ� ȯ�� : Windows 10 LTSC
//  �� �� �� : ���ط�(iharryc@naver.com)
//  ���۳�¥ : 2003/12/08(���ط�)
//  ������¥ : 2005/03/20(���ط�) ó������ 6��繫�ǿ���.....
//  ������¥ : 2013/10/05(���ط�) ������ ���������� �繫�ǿ���...
//  ������¥ : 2022/07/05(���ط�) ��ſ� ���� �� �繫�ǿ��� .
//-------------------------------------------------------------------

// flash �޸𸮿� ���� �Ҵ�
// __attribute__((progmem)) static char flStr[10];
// ^^^^^^^^^^^^^^^^^^^^^^^^
// flash �޸� ���� 
//
//
// IO �޸� ����
// #define PORTD ( *(volatile uint8_t *) (0x2B) ) 
//                 ^^^^^^^^^^^^^^^^^^^  ^^^^^^ 
//                �޸� �����������   SFR�ּ� 


// RAM�뷮 ���� MCU���� ���������� ���������� ���� �����ϸ� ���û���� ����� Ŀ���Ѵ�. 
// �����Ϸ� �ɼǿ��� Data CSTACK ����� (0x20+��������) ����
// CSTACK : 0x30,   RSTACK : 16



#include "EBAVR.h"

volatile unsigned int g_uiSysTick = 0;
volatile unsigned char g_byKeyIn, g_bKeyValidFlag;  //Ű�Է� ���� 



//-------------------------------------------------------------------
// Watchdog timer interrupt Declare                     
// action (interrupt enable/disable) : ENABLE, DISABLE  
// WTDON fuse bit ����(Unprogrammed)�Ұ�.���ϸ��׻�ENABLE 
// ���� 3V/5V���� ��1�� watchdog ����                 
// watchdog�� ����Ŭ���� �и��� ���������(1MHz)�ε����ϸ�
// ���ͷ�Ʈ�ʹ� �����ϰ� ���������� ����ȴ�.             
// �ݵ�� ���η������� 1���ֱ�� wdr();�����ؾ߸��¾ȵ� 
// wdr();  // Reset Watchdog timer      
//-------------------------------------------------------------------
void SetWatchDog(unsigned char byAction)
{
 
  if (byAction) 
  {
    WDTCR |= (1<<WDCE) | (1<<WDE);  // ������� ����.
    WDTCR |= (1<<WDE) | (1<<WDP2) | (1<<WDP1);  // 1�� ����
  }  
  else 
  {
    // WDCE, WDE�� ���ÿ� SET�ؾ� WDTCT������� ����
    // ������ ������ �ٷ� WDTCR �����ؾߵ�.
    WDTCR |= (1<<WDCE) | (1<<WDE);  
    WDTCR = 0x00;  // Turn off WDT
  }    

}



//-------------------------------------------------------------------
//    Ÿ�̸� ���ͷ�Ʈ ����
//    byNum(���ǹ�ȣ) : 0,1
//    bAction(���ۼ���) : ENABLE, DISABLE
//
//    Example) SetTimeInterrupt(0, ENABLE);
//             StartTimer0(0xFF);  
// ----------------------------------------------------------------
// ���ֺ�(TCCR)        CLK(1:1)     CLK/8      CLK/64      CLK/256   
// ----------------------------------------------------------------
// ���ļ�(Hz)        8,000,000    1,000,000    125,000      31,250   
// ----------------------------------------------------------------
// ī��Ʈ ��������
// ���� �ð�(uS)         0.125        1.0         8          32      
//-------------------------------------------------------------------
void SetTimeInterrupt(unsigned char byNum, unsigned char bAction)
{
  switch (byNum) 
  {
    case 0:
      TCNT0 = 0x00;   // 8 bit 
      TCCR0A = 0x00;   // StopTimer0();
      if (bAction == ENABLE) mSetBit(TIMSK0,TOIE0);  // enable Time Interrupt0
      else mClrBit(TIMSK0,TOIE0);    // disable Time Interrupt0
      break;

    case 1:
      TCNT1 = 0x0000; // 16 bit 
      TCCR1B = 0x00;  // StopTimer1();      
      if (bAction == ENABLE) mSetBit(TIMSK1,TOIE1);  // enable Time Interrupt1
      else mClrBit(TIMSK1,TOIE1);    // disable Time Interrupt1
      break;
  }
}




//-------------------------------------------------------------------
//    External Interrupt Declare                         
//    num (Exteranl interrupt number) : 0,1                
//    action (interrupt enable/disable) : ENABLE, DISABLE  
//    type ( declare interrupt type ) : FALL, RISE, LEVEL  
//-------------------------------------------------------------------
void SetExInterrupt(unsigned char num, unsigned char bAction, unsigned char type)
{
/*  
  switch (num) {
    case 0:

      MCUCR &= 0xFC; // clear two bit
      MCUCR +=type;  // select interrupt type

      mSetBit(GIFR, INTF0); // interrupt0 flag clear
         // ���� :  1�� ����ؾ� ���� flag�� 0�� ��
      if (bAction) mSetBit(GICR, INT0);
      else mClrBit(GICR, INT0);
      break;

    case 1:
      MCUCR &= 0xF3; // clear two bit
      MCUCR += (type<<2);  // select interrupt type 1

      mSetBit(GIFR, INTF1); // interrupt0 flag clear
      // ���� :  1�� ����ؾ� ���� flag�� 0�� ��
      if (bAction) mSetBit(GICR, INT1);
      else mClrBit(GICR, INT1);
      break;

  }
*/
}





//-------------------------------------------------------------------
//   ��  ��   : �ø������ �ʱ�ȭ
//              databit:8bit, stopbit:1bit, noparity
//   ��  ��
//
//   F_CPU     : MPU MASTER CLOCK
//   baudRate : ��żӵ�(2400, 4800, 9600, 19200, 38400)
//   ���ϰ�   : ����.
//-------------------------------------------------------------------
void SetSerial(unsigned char bAction, unsigned int uiBaudRate)
{
/*  
  mSetBit(UCSRA, U2X);  // enable double speed
  //DATASHEET���� BAUDRATE ����
  // UBRRH = ( HIWORD( ((unsigned long int) F_CPU/(uiBaudRate<<3)*100) -1 ) ) & 0x7F;
  //DATASHEET���� BAUDRATE ����
  UBRRL = ( (unsigned long int) F_CPU/(uiBaudRate<<3)*100 )-1;
  UCSRB = 0x98;  // only Receive Interrupt enable
  UCSRC = 0x86;  // mode:Async, databit:8bit, stopbit:1bit, noparity
  if (bAction) { mSetBit(UCSRB, RXEN); mSetBit(UCSRB, TXEN); }
  else  {  mClrBit(UCSRB, RXEN); mClrBit(UCSRB, TXEN); }  
*/
}





//-------------------------------------------------------------------
// ADC interrupt Declare                     
//-------------------------------------------------------------------
void SetADC(unsigned char  bAction, unsigned char byNum)
{
  
  ADMUX |= (byNum & 0x07);  // ADC ä�μ���
  ADMUX |= (1 << REFS0);    // ADC ���������� AVCC�� ����
  // ADMUX |= (1 << ADLAR);    // ��ȯ�� �������ķ� �ڸ��� ����

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 128����
  // ADCSR |= (1 << ADFR);  // ��ŸƮ ��ȣ ���� ���� ������
  // ADCSR |= (1 << ADIE);  // Enable ADC Interrupt 

  if (bAction) mSetBit(ADCSRA, ADEN);  // Enable ADC 
  else mClrBit(ADCSRA, ADEN);  

  // �ݵ�� ADC��ŸƮ�� �Ǹ��������� �����Ұ�
  ADCSRA |= (1 << ADSC);   // Start ADC Conversions 
}



//-------------------------------------------------------------------
// 
//-------------------------------------------------------------------
unsigned int ReadADC(void)
{
  unsigned int uiADC;
  
  while ( mIsClrBit(ADCSRA, ADIF) ); // ADC ������ �Ϸ�ɶ� �������...
  
  uiADC = ADC;
  mClrBit(ADCSRA, ADIF);
  mSetBit(ADCSRA, ADSC);  // Start ADC Conversions
  
  return uiADC;
}




//-------------------------------------------------------------------
//              EEPROM�� ������ �б��Լ�
//-------------------------------------------------------------------
unsigned char ReadEEPROM(unsigned int uiAddress)
{
  while(EECR & (1 << EEWE));   // ���� �۾� ���̸� ����
  EEAR = uiAddress;            // ���� �ּ� ����
  EECR = EECR | (1 << EERE);   // �б� ���
  return EEDR;                 // ������ �б�
}



//-------------------------------------------------------------------
//              EEPROM�� ������ ���� �Լ�
//-------------------------------------------------------------------
void WriteEEPROM(unsigned int uiAddress, unsigned char byData)
{
  while(EECR & (1 << EEWE));   // ���� �۾� ���̸� ����
  EEAR = uiAddress;            // �� �ּ� ����
  EEDR = byData;               // �� ������
  EECR = EECR | (1 << EEMWE);  // EEWE�� �غ��ض�
  EECR = EECR | (1 << EEWE);   // ���� ���
}  
  



//-------------------------------------------------------------------
// printf()�Լ����� �⺻�����ġ�� ����ϴ� �Լ�
//-------------------------------------------------------------------
int PutChar(int ch)
{

  UDR0 = (unsigned char) (ch & 0x00FF);  // USART Buffer
  while( mIsClrBit(UCSR0A,UDRE0) );  // UDRE( Transmitte UART Data Register Empty)

  return ch;
}



/*
static int uart_putchar(char c, FILE *stream) 
{
  if (c == '\n') uart_putchar('\r', stream);
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}
*/




//-------------------------------------------------------------------
//   ��  ��   : ����ð� ����
//   ��  ��
//   iValue   : �����ð� (1 to 65535)
//              delay time = (delay value * 6CLOCK) + 4CLOCK
//
//              F_CPU         1CLK        4CLK       6CLK
//             ----------------------------------------------
//               1MhZ      1uS          4uS        6uS
//          3.6864MhZ      0.2712uS     1.0848uS   1.6272uS
//               8MhZ      0.125uS      0.5uS      0.75uS
//              16MhZ      0.0625uS     0.25uS     0.375uS
//
//-------------------------------------------------------------------
void delay(register unsigned int iValue)
{
  while(iValue--);
}


//-------------------------------------------------------------------
// _delay_us(100);
// _delay_ms(100);
//-------------------------------------------------------------------




//-------------------------------------------------------------------
// Time���ͷ�Ʈ���� 10ms���� ȣ��Ǵ� �Լ�
//  50ms �̻� key�Է� ����, 1sec �̻� Repeat �Է�
//-------------------------------------------------------------------
void KeyDebounce(void)
{

  static unsigned char sbySamePinCount, sbyOldPin;
  unsigned char byPin;
  

  byPin = READPIN;  // #define READPIN (~PIND)&0x0F;
  

  if (byPin == sbyOldPin)  // ����Ű �ΰ�?
  {
    sbySamePinCount++;
    if (sbySamePinCount > 5)  // 5ȸ ��������(50ms�� ����)
    {
      sbySamePinCount = 0;   // typematic key time
      g_byKeyIn = byPin;
      g_bKeyValidFlag = TRUE;      
    }      
  }
  else   // Ű�� ��ȭ ������(debounce����)
  {    
    sbySamePinCount = 0;
    sbyOldPin = byPin;    
    g_bKeyValidFlag = FALSE;    
  }
}




//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
unsigned char ScanKey(void)
{

  if (g_bKeyValidFlag) return g_byKeyIn;  
  return 0xFF; // ��ٿ����
}






//-------------------------------------------------------------------
//  �ڵ�޸� �Ǵ� SRAM�޸��� �� ���.
//-------------------------------------------------------------------
// ���� : ���α׷� �ڵ忡 �ִ� ���ڿ� ����ϱ�
// printf("aaaaa") �̷��� ����ϸ� ���� �Լ�������
// "aaaaa"��Ʈ���� �����Ϸ��� ������ SRAM�� �޸𸮸� �Ҵ��ϱ�
// ������ ������ ���ڿ� ����� ������ �޸𸮺��������� �����ü� �ִ�.
// �׷��� ���α׷� �ڵ念������ ��ġ�� ���ڿ��� ��Ī�ϸ� SRAM ������
// �Ҵ����� �ʴ´�.
// ��뿹�� :
// #include <pgmspace.h>
// #inlcude <stdio.h>
//    __flash static char flStr[][100] = {
//     "\nSTART FK2004AU.....\n", "\nEnd of Wizerd\n"
//   };
//
//   print(&flStr[0][0]);
//-------------------------------------------------------------------
void print(char *szData)
{
  while(*szData) putchar(*szData++);
}





//-------------------------------------------------------------------
//   ��  ��   : Queue �޸� ���
//   ��  ��
//   mode : Queue �޸𸮿� save �� load
//           (PUT: save, GET: load)
//   return : ������ ������ �� ����� ������, ������ -1�� ����
//   Q�޸� ���� ���� : ȯ��ť�޸𸮴� �˰���� �޸𸮸� �ϳ���
//   ������������ ��� �ϹǷ� ���� ���ϴ� �޸� �纸�� 1�� �����Ѵ�.
//-------------------------------------------------------------------
signed int Qmem(unsigned char byMode, unsigned char byQdata)
#define QSIZE 10
{

  static unsigned char  byQhead = 0, byQtail = 0;
  static unsigned char byQbuf[QSIZE];
  register signed char byResult = 0;

  switch(byMode) {
    case GET:
      if (byQtail == byQhead) byResult = -2; // underflow
      else {
        byResult = byQbuf[byQtail];
        byQtail++;
        byQtail %= QSIZE;
      }
      break;

    case PUT:
      if ((byQhead + 1) % QSIZE == byQtail) byResult = -1;  // overflow
      else {
        byQbuf[byQhead] = byQdata;
        byQhead++;
        byQhead %= QSIZE;
        byResult = 1;  // successfull
      }
      break;

    case CLEAR:
      byQhead = byQtail = 0;
      break;

    case CHECK:
      byResult = byQhead - byQtail;
      if (byResult < 0) byResult += QSIZE;
      break;

    case PEEP:
      if (byQtail == byQhead) byResult = -2; // underflow
      else byResult = byQbuf[byQtail];
      break;
  }

  return byResult;
}





//-------------------------------------------------------------------
//    �ڵ�� ���� �Ǿ����� �Լ���ġ�� �����Դϴ�. 
//-------------------------------------------------------------------
unsigned char SamePacketCount(unsigned char byStreamData)
{
  __attribute__((progmem)) const static char rom_aPacketCMD[8]  = 
  {
    //              XXXX X000
    //   ILL  ------+||| |
    //   BREAK  -----+|| |
    //   REAR --------+| |
    //   WINKER_L -----+ |
    //   WINKER_R -------+        
    
    0x08, 0x18, 0x10,       // start code
          0x18, 0x10,       // Module ID Count
          0x10, 0x18, 0x08  // end code
  };
// mode change(ex: Moudle ID 3)
// 0x08, 0x18,0x10, 0x18,0x10, 0x18,0x10, 0x18, 0x08 

  static unsigned char byStateFlag = STATE_READY, aStreamBuf[3], byIDcount;
  //
  //                  byStateFlag
  //                  XXX0 0000
  //   Start Flag ----+||- ----
  //   End Flag -------+|   |
  //   Reserved ------------+
  
  aStreamBuf[0] = aStreamBuf[1];
  aStreamBuf[1] = aStreamBuf[2];
  aStreamBuf[2] = byStreamData;

  switch (byStateFlag)
  {
  case STATE_READY:
    byIDcount  = 0;
    if ( (aStreamBuf[0] == rom_aPacketCMD[0]) && 
         (aStreamBuf[1] == rom_aPacketCMD[1]) && 
         (aStreamBuf[2] == rom_aPacketCMD[2]) ) byStateFlag = STATE_START;    
    else break;

    
  case STATE_START:
    if ( (aStreamBuf[1] == rom_aPacketCMD[3]) && 
         (aStreamBuf[2] == rom_aPacketCMD[4]) ) byIDcount ++;
    else 
      if ( !( (aStreamBuf[1] == rom_aPacketCMD[4]) && 
              (aStreamBuf[2] == rom_aPacketCMD[3]) ) ) byStateFlag = STATE_READY;

    if ( (aStreamBuf[0] == rom_aPacketCMD[5]) && 
         (aStreamBuf[1] == rom_aPacketCMD[6]) && 
         (aStreamBuf[2] == rom_aPacketCMD[7]) ) byStateFlag = STATE_END; 
    else break;


  case STATE_END:

    byStateFlag = STATE_READY;    
    return byIDcount ;
//    break;
  }
  return 0;
}









// ���� C ������ �켱���� () [] -> . *
/*
void InitMPU(void)
{

    SetWatchDog(ENABLE);  // 1���ֱ�
    SetSerial(DISABLE, BAUD115200);
    SetExInterrupt(0, DISABLE, EDGE_FALL);
    SetExInterrupt(1, DISABLE, EDGE_LEVEL);
    SetTimeInterrupt(0, ENABLE);
    SetTimeInterrupt(1, ENABLE);
    SetADC(DISABLE, 7);  // channel 7
    
    mStartTimer0(0xFF);            
    mStartTimer1(0xFFFF);            
    // wdr();  // Reset Watchdog timer              

}
*/

