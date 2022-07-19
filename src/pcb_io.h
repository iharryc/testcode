#ifndef _pcb_io_h
#define _pcb_io_h


// ATmega8     ==> FUSE bit : LOW(0xA4), HIGH(0xD8)
// ATtiny13    ==> FUSE bit : LOW(0x7A), HIGH(0xFB)
// ATT90can128 ==> FUSE bit : LOW(0xFF), HIGH(0xD9), EXT(0xFD)


// MCU 동작 주파수    
#ifndef F_CPU
  #define F_CPU 16000000UL  // 16MHz
#endif



typedef union {
  unsigned char byte;
  struct {
    unsigned char bit0 :1,
                  bit1 :1,
                  bit2 :1,    
                  bit3 :1,    
                  bit4 :1,
                  bit5 :1,
                  bit6 :1,    
                  bit7 :1;
  };
} bitfiled8_t; 
//  ex) bitfileds_t temp; 
//      temp.byte = 0xff;
//      temp.bit0 = 1;


#define IO_ADDR(addr) 	( *(volatile bitfiled8_t *) addr )
// AT90can128
#define ddra	(IO_ADDR(0x21))
#define ddrb	(IO_ADDR(0x24))
#define ddrc	(IO_ADDR(0x27))
#define ddrd	(IO_ADDR(0x2A))
#define ddre	(IO_ADDR(0x2D))
#define ddrf	(IO_ADDR(0x30))
#define ddrg	(IO_ADDR(0x33))

#define porta	(IO_ADDR(0x22))
#define portb	(IO_ADDR(0x25))
#define portc	(IO_ADDR(0x28))
#define portd	(IO_ADDR(0x2B))
#define porte	(IO_ADDR(0x2E))
#define portf	(IO_ADDR(0x31))
#define portg	(IO_ADDR(0x34))

#define pina	(IO_ADDR(0x20))
#define pinb	(IO_ADDR(0x23))
#define pinc	(IO_ADDR(0x26))
#define pind	(IO_ADDR(0x29))
#define pine	(IO_ADDR(0x2C))
#define pinf	(IO_ADDR(0x2F))
#define ping	(IO_ADDR(0x32))




#define RELAY1 porta.bit0 
#define RELAY2 porta.bit1
#define RELAY3 porta.bit2
#define RELAY4 portg.bit0
#define RELAY5 portg.bit1
#define RELAY6 portg.bit2

#define LED1 porta.bit3
#define LED2 porta.bit4
#define LED3 porta.bit5
#define LED4 porta.bit6


#endif

