#include <avr/io.h>
#include <avr/interrupt.h>
#include "EBAVR.h"


ISR(INT0_vect) { }
ISR(INT1_vect) { }
ISR(INT2_vect) { }
ISR(INT3_vect) { }
ISR(INT4_vect) { }
ISR(INT5_vect) { }
ISR(INT6_vect) { }
ISR(INT7_vect) { }


//-------------------------------------------------------------------
//   기  능   : 타이머0 오버플러워 인터럽트
//
//  1ms 만들기 예제(FOSC=8MHz)
//  타임인터럽트 주기 = 1/발진주파수*분주비(64)*TCNT카운트수(0xFF+1-TCNT)
//                      1/8000000*64=8us, 8us*125=1ms
//                      TCNT = 256-125 = 131
// LED밝기조절에 있어 PWM주파수가 100Hz는 되어야 깜빡거림 없다
// PWM주파수는계산방법 (인터럽트주기 100us, 분해능 100일때)
// PWM주파수는= 인터럽트주기*분해능 = 100us * 100 = 10ms = 100Hz  
//-------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
  // 타임 인터럽트에 의해 1ms 마다 실행
  cli();  // disable interrupt 

  g_uiSysTick++;
  if (g_uiSysTick >= 50000) g_uiSysTick = 0;

  mStartTimer0(25);     // FOSC 16MHz일때 100us 주기(256-231=25, 25*4us=100us)
  // mStartTimer0(244);   // FOSC  8MHz일때 100us 주기(256-244=12, 12*8us=96us)

  // mStartTimer1(64286);   // 10ms 주기

  sei();  // enable interrupt

}


ISR(TIMER1_OVF_vect) { }
ISR(TIMER2_OVF_vect) { }
ISR(TIMER3_OVF_vect) { }


ISR(USART0_RX_vect) { }
ISR(USART1_RX_vect) { }
ISR(CANIT_vect) { }


