
//bit_set(var, bit)


#define load PIN_B5
#define data PIN_B1
#define clock PIN_B4

#define CLK_DELAY 1     //uS olarak


void hc595_spi_yaz(unsigned int16 veri,unsigned char yaz_bit)//MSB first gönderir
{                                                    
      int x=0;
      output_low(load);         //load=0; 
      for(x=0;x<yaz_bit;++x)                           
      {
        if (bit_test(veri,((yaz_bit-1)-x))) 
             output_high(data);         //data=1; 
        else  output_low(data);             //data=0;   
         output_low(clock);         //clock=0;
         output_high(clock);        //clock=1;
      }                                      
      output_high(load);         //load=1;            
}//Sonu

void hc595_spi_24bit_yaz(unsigned int8 veriH,unsigned int8 veriM,unsigned int8 veriL)//MSB first 
{                                                    
      int x=0;
      
      output_low(load);         //load=0; 
      
      for(x=0;x<8;++x)                           
      {
        if (bit_test(veriH,((7)-x))) //8-1=7
             output_high(data);         //data=1; 
        else  output_low(data);             //data=0;   
         output_low(clock);         //clock=0;
         delay_us(CLK_DELAY);
         output_high(clock);        //clock=1;
         delay_us(CLK_DELAY);
      } 
      
      for(x=0;x<8;++x)                           
      {
        if (bit_test(veriM,((7)-x))) 
             output_high(data);         //data=1; 
        else  output_low(data);             //data=0;   
         output_low(clock);         //clock=0;
         delay_us(CLK_DELAY);
         output_high(clock);        //clock=1;
         delay_us(CLK_DELAY);
      } 
      
      for(x=0;x<8;++x)                           
      {
        if (bit_test(veriL,((7)-x))) 
             output_high(data);         //data=1; 
        else  output_low(data);             //data=0;   
         output_low(clock);         //clock=0;
         delay_us(CLK_DELAY);
         output_high(clock);        //clock=1;
         delay_us(CLK_DELAY);
      }
      output_high(load);         //load=1;    
     
      output_toggle(PIN_A4);
}//Sonu
