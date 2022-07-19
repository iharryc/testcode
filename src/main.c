#include "EBAVR.h"


  void InitIO(void)
  {

    DDRA = 0xFF;
    DDRG = 0xFF;

    ddre.bit6 = OUT;


  }






int main(void)
{

  disable();  // interrupt disable  

    // InitMPU();
  InitIO();
  SetTimeInterrupt(0,ENABLE);
  mStartTimer0(6);
  
  // enable();   // interrupt enable    
 
//  IO_OUT0 = ON;   
// IO_OUT1 = OFF;  while(1) wdr(); 

  enable();
  

  while(1)
  {
  
    // _delay_ms(1000);

    if ( (g_uiSysTick % 1000) == 0 )
    { 
      LED1 = ~LED1;
    }

    // _delay_ms(1000);
    // RELAY1 = OFF;
    // LED1 = OFF;
 
  }

  return 0;

}


// 커밋1


