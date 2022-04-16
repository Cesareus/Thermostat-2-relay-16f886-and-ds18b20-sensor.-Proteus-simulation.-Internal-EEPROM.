#include <16F886.h>
#device adc=8
#fuses INTRC_IO,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT
#fuses NOIESO,NOFCMEN,NOWRT,NODEBUG,NOLVP 
#use delay(clock=8M)
#define use_portb_lcd TRUE

