#include <main.h>
#include <internal_eeprom.c>
#include <lcd.c>
#include <1wire.c>
#include <ds1820.c>
#define CALOR PIN_C5
#define FRIO PIN_C4
#define BLIGHT PIN_A1 //PORTA RA1
#define SET PIN_A2 //PORTA RA2
#define MAS PIN_A3 //PORTA RA3
#define MENOS PIN_A4 //PORTA RA4
#define BLLCD PIN_C3 //luz backlight
#use fast_io(A)   // Configuración rápida del puerto A
#use fast_io(B)  // Configuración rápida del puerto B
#use fast_io(C) // Configuración rápida del puerto C

int8 RA=21,RC=19,var,var1,counter1;
int counter=0;

void config (void){
var=RC;
var1=RA;
output_high(BLLCD);
printf(lcd_putc"\f");
   lcd_gotoxy(1,1);
   printf(lcd_putc,"CONFIGURACION");
   lcd_gotoxy(1,2);
   printf(lcd_putc,"PRESIONE SET");
   lcd_gotoxy(1,2);
   delay_ms(2000);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"PARA GUARDAR   ");
   lcd_gotoxy(1,2);
   printf(lcd_putc,"LOS VALORES    ");
   delay_ms(2000);
   printf(lcd_putc"\f");
   
while(true){
 lcd_gotoxy(1,1);
  printf(lcd_putc,"Temp limite");
  if(RC<100){
  lcd_gotoxy(1,2);
  printf(lcd_putc,"Menor T:%d ",var);}
  else{
  lcd_gotoxy(1,2);
  printf(lcd_putc,"Menor T:%d",var);}
if(!input(MAS)){
while(!input(MAS)){}
var++;
}
if(!input(MENOS)){
while(!input(MENOS)){}
var--;
}
if(!input(SET))
   {
   while(!input(SET)){}
   WRITE_EEPROM(0,var);
   printf(lcd_putc"\f");
   lcd_gotoxy(1,1);
   printf(lcd_putc,"    Grabado     ");
   delay_ms(1000);
   break;
   }}
printf(lcd_putc"\f");  

while(true){
 lcd_gotoxy(1,1);
  printf(lcd_putc,"Temp limite");
  if(RC<100){
  lcd_gotoxy(1,2);
  printf(lcd_putc,"Mayor T:%d ",var1);}
  else{
  lcd_gotoxy(1,2);
  printf(lcd_putc,"Mayor T:%d",var1);}
  
if(!input(MAS)){
while(!input(MAS)){}
var1++;}
if(!input(MENOS)){
while(!input(MENOS)){}
var1--;}
if(!input(SET))
   {
   while(!input(SET)){}
   WRITE_EEPROM(1,var1);
   printf(lcd_putc"\f");
   lcd_gotoxy(1,1);
   printf(lcd_putc,"    Grabado     ");
   delay_ms(1000);
   break;
}}
printf(lcd_putc"\f");
counter=0;
counter1=10;
WRITE_EEPROM(3,counter1);
return;
}
// ************** Programa Principal ************** //
void main ()
{
//******* Variables *******//
   float temo;
       
   set_tris_a(0x1F);
   set_tris_b(0x00);
   set_tris_c(0x00);
   output_C (0x00);
     
   setup_oscillator(OSC_8MHZ);      
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(SPI_DISABLED); 
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1); 
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1); 
   setup_comparator(NC_NC_NC_NC); 
   setup_ccp1(CCP_OFF); 
        
   lcd_init ();// Inicializa la libreria para manejar el LCD.
   printf ( LCD_PUTC, "\f" );
   counter1=READ_EEPROM(3);
if(counter1==10){
   RA=READ_EEPROM(1);
   RC=READ_EEPROM(0);}

   while (true) //Bucle infinito.
   {
while(input(SET)){

lcd_gotoxy(3,2);
printf(lcd_putc,"RA:");
lcd_gotoxy(9,2);
printf(lcd_putc,"- RC:");
temo = ds1820_read();
lcd_gotoxy(1,1);
printf(lcd_putc,"   TEMP:%2.1FoC",temo);
if(RA<10){
lcd_gotoxy(6,2);
printf(lcd_putc,"%d ",RA);}
else{
lcd_gotoxy(6,2);
printf(lcd_putc,"%d",RA);}
if(RC<10){
lcd_gotoxy(14,2);
printf(lcd_putc,"%d ",RC);}
else{
lcd_gotoxy(14,2);
printf(lcd_putc,"%d",RC);}
counter++;

if(counter >= 25){
output_low(BLLCD);}

if(!input(BLIGHT)){
while(!input(BLIGHT)){}
output_high(BLLCD);
counter=0;
}

if(RA>RC && RA>0 && RC>0){
if(temo<=RC)
output_high(CALOR);
if(temo>=RA-1)
output_low(CALOR);

if(temo>=RA)
output_high(FRIO);
if(temo<=RC+1)
output_low(FRIO);

counter1=10;
}

else{
printf(lcd_putc,"\f");
lcd_gotoxy(1,1);
printf(lcd_putc," ERROR    Tm>=TM o");
lcd_gotoxy(1,2);
printf(lcd_putc,"  Tm o TM <= 0 ");
delay_ms(1500);
printf(lcd_putc,"\f");
lcd_gotoxy(1,1);
printf(lcd_putc,"Configure");
lcd_gotoxy(1,2);
printf(lcd_putc,"Nuevamente");
delay_ms(1500);
printf(lcd_putc"\f");
config();
}
}
printf(lcd_putc"\f");
config();
if(counter1==10){
   RA=READ_EEPROM(1);
   RC=READ_EEPROM(0);}
counter=0;
}
}
// Fin del loop infinito.

