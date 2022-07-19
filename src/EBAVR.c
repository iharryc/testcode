//-------------------------------------------------------------------
//  제  목 : AVR Series 기본 함수
//  화일명 : EBAVR.C
//  버  젼 : 0.4

//  사용디바이스 : ATMEGA8 (16MHz)
//  컴파일러 : AVR-GCC Ver 7.3.0
//  컴파일러 환경 : Windows 10 LTSC
//  제 작 자 : 김해룡(iharryc@naver.com)
//  시작날짜 : 2003/12/08(김해룡)
//  수정날짜 : 2005/03/20(김해룡) 처음갖는 6평사무실에서.....
//  수정날짜 : 2013/10/05(김해룡) 산정동 세현디지털 사무실에서...
//  수정날짜 : 2022/07/05(김해룡) 즐거운 직장 빈센 사무실에서 .
//-------------------------------------------------------------------

// flash 메모리에 변수 할당
// __attribute__((progmem)) static char flStr[10];
// ^^^^^^^^^^^^^^^^^^^^^^^^
// flash 메모리 지정 
//
//
// IO 메모리 지정
// #define PORTD ( *(volatile uint8_t *) (0x2B) ) 
//                 ^^^^^^^^^^^^^^^^^^^  ^^^^^^ 
//                메모리 절대번지지정   SFR주소 


// RAM용량 적은 MCU사용시 전역변수와 정적변수를 많이 설정하면 스택사이즈가 충분히 커야한다. 
// 컴파일러 옵션에서 Data CSTACK 사이즈를 (0x20+전역변수) 설정
// CSTACK : 0x30,   RSTACK : 16



#include "EBAVR.h"

volatile unsigned int g_uiSysTick = 0;
volatile unsigned char g_byKeyIn, g_bKeyValidFlag;  //키입력 변수 



//-------------------------------------------------------------------
// Watchdog timer interrupt Declare                     
// action (interrupt enable/disable) : ENABLE, DISABLE  
// WTDON fuse bit 해제(Unprogrammed)할것.안하면항상ENABLE 
// 전원 3V/5V사용시 약1초 watchdog 설정                 
// watchdog은 메인클럭과 분리된 내장발진기(1MHz)로동작하며
// 인터럽트와는 무관하게 독자적으로 실행된다.             
// 반드시 메인루프에서 1초주기로 wdr();실행해야리셋안됨 
// wdr();  // Reset Watchdog timer      
//-------------------------------------------------------------------
void SetWatchDog(unsigned char byAction)
{
 
  if (byAction) 
  {
    WDTCR |= (1<<WDCE) | (1<<WDE);  // 설정모드 진입.
    WDTCR |= (1<<WDE) | (1<<WDP2) | (1<<WDP1);  // 1초 설정
  }  
  else 
  {
    // WDCE, WDE를 동시에 SET해야 WDTCT변경모드로 진입
    // 변경모드 진입후 바로 WDTCR 설정해야됨.
    WDTCR |= (1<<WDCE) | (1<<WDE);  
    WDTCR = 0x00;  // Turn off WDT
  }    

}



//-------------------------------------------------------------------
//    타이머 인터럽트 정의
//    byNum(정의번호) : 0,1
//    bAction(동작설정) : ENABLE, DISABLE
//
//    Example) SetTimeInterrupt(0, ENABLE);
//             StartTimer0(0xFF);  
// ----------------------------------------------------------------
// 분주비(TCCR)        CLK(1:1)     CLK/8      CLK/64      CLK/256   
// ----------------------------------------------------------------
// 주파수(Hz)        8,000,000    1,000,000    125,000      31,250   
// ----------------------------------------------------------------
// 카운트 레지스터
// 증가 시간(uS)         0.125        1.0         8          32      
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
         // 주의 :  1을 기록해야 실제 flag가 0이 됨
      if (bAction) mSetBit(GICR, INT0);
      else mClrBit(GICR, INT0);
      break;

    case 1:
      MCUCR &= 0xF3; // clear two bit
      MCUCR += (type<<2);  // select interrupt type 1

      mSetBit(GIFR, INTF1); // interrupt0 flag clear
      // 주의 :  1을 기록해야 실제 flag가 0이 됨
      if (bAction) mSetBit(GICR, INT1);
      else mClrBit(GICR, INT1);
      break;

  }
*/
}





//-------------------------------------------------------------------
//   기  능   : 시리얼통신 초기화
//              databit:8bit, stopbit:1bit, noparity
//   인  자
//
//   F_CPU     : MPU MASTER CLOCK
//   baudRate : 통신속도(2400, 4800, 9600, 19200, 38400)
//   리턴값   : 없음.
//-------------------------------------------------------------------
void SetSerial(unsigned char bAction, unsigned int uiBaudRate)
{
/*  
  mSetBit(UCSRA, U2X);  // enable double speed
  //DATASHEET상위 BAUDRATE 계산식
  // UBRRH = ( HIWORD( ((unsigned long int) F_CPU/(uiBaudRate<<3)*100) -1 ) ) & 0x7F;
  //DATASHEET하위 BAUDRATE 계산식
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
  
  ADMUX |= (byNum & 0x07);  // ADC 채널선택
  ADMUX |= (1 << REFS0);    // ADC 기준전압을 AVCC로 설정
  // ADMUX |= (1 << ADLAR);    // 변환값 왼쪽정렬로 자리수 맞춤

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 128분주
  // ADCSR |= (1 << ADFR);  // 스타트 신호 없이 연속 컨버팅
  // ADCSR |= (1 << ADIE);  // Enable ADC Interrupt 

  if (bAction) mSetBit(ADCSRA, ADEN);  // Enable ADC 
  else mClrBit(ADCSRA, ADEN);  

  // 반드시 ADC스타트는 맨마지막으로 설정할것
  ADCSRA |= (1 << ADSC);   // Start ADC Conversions 
}



//-------------------------------------------------------------------
// 
//-------------------------------------------------------------------
unsigned int ReadADC(void)
{
  unsigned int uiADC;
  
  while ( mIsClrBit(ADCSRA, ADIF) ); // ADC 컨버전 완료될때 까지대기...
  
  uiADC = ADC;
  mClrBit(ADCSRA, ADIF);
  mSetBit(ADCSRA, ADSC);  // Start ADC Conversions
  
  return uiADC;
}




//-------------------------------------------------------------------
//              EEPROM에 데이터 읽기함수
//-------------------------------------------------------------------
unsigned char ReadEEPROM(unsigned int uiAddress)
{
  while(EECR & (1 << EEWE));   // 쓰기 작업 중이면 루프
  EEAR = uiAddress;            // 읽을 주소 설정
  EECR = EECR | (1 << EERE);   // 읽기 명령
  return EEDR;                 // 데이터 읽기
}



//-------------------------------------------------------------------
//              EEPROM에 데이터 쓰기 함수
//-------------------------------------------------------------------
void WriteEEPROM(unsigned int uiAddress, unsigned char byData)
{
  while(EECR & (1 << EEWE));   // 쓰기 작업 중이면 루프
  EEAR = uiAddress;            // 쓸 주소 설정
  EEDR = byData;               // 쓸 데이터
  EECR = EECR | (1 << EEMWE);  // EEWE할 준비해라
  EECR = EECR | (1 << EEWE);   // 쓰기 명령
}  
  



//-------------------------------------------------------------------
// printf()함수에서 기본출력장치로 사용하는 함수
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
//   기  능   : 실행시간 지연
//   인  자
//   iValue   : 지연시간 (1 to 65535)
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
// Time인터럽트에서 10ms마다 호출되는 함수
//  50ms 이상 key입력 인정, 1sec 이상 Repeat 입력
//-------------------------------------------------------------------
void KeyDebounce(void)
{

  static unsigned char sbySamePinCount, sbyOldPin;
  unsigned char byPin;
  

  byPin = READPIN;  // #define READPIN (~PIND)&0x0F;
  

  if (byPin == sbyOldPin)  // 동일키 인가?
  {
    sbySamePinCount++;
    if (sbySamePinCount > 5)  // 5회 눌림상태(50ms가 적당)
    {
      sbySamePinCount = 0;   // typematic key time
      g_byKeyIn = byPin;
      g_bKeyValidFlag = TRUE;      
    }      
  }
  else   // 키값 변화 있을시(debounce포함)
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
  return 0xFF; // 디바운스상태
}






//-------------------------------------------------------------------
//  코드메모리 또는 SRAM메모리의 값 출력.
//-------------------------------------------------------------------
// 참고 : 프로그램 코드에 있는 문자열 출력하기
// printf("aaaaa") 이렇게 사용하면 사용된 함수내에서
// "aaaaa"스트링을 컴파일러가 변수로 SRAM에 메모리를 할당하기
// 때문에 과도한 문자열 사용은 마이컴 메모리부족현상을 가져올수 있다.
// 그래서 프로그램 코드영역에서 위치한 문자열을 지칭하면 SRAM 변수를
// 할당하지 않는다.
// 사용예제 :
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
//   기  능   : Queue 메모리 운용
//   인  자
//   mode : Queue 메모리에 save 및 load
//           (PUT: save, GET: load)
//   return : 저장할 데이터 및 저장된 데이터, 에러시 -1를 리턴
//   Q메모리 사용상 주의 : 환형큐메모리는 알고리즘상 메모리를 하나를
//   여유간격으로 사용 하므로 실제 원하는 메모리 양보다 1더 설정한다.
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
//    코드는 검증 되었으나 함수위치가 에매함니다. 
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









// 참고 C 연산자 우선순위 () [] -> . *
/*
void InitMPU(void)
{

    SetWatchDog(ENABLE);  // 1초주기
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

