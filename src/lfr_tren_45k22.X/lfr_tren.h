/* 
 * File:   lfr_tren.h
 * Author: ilyas
 *
 * Created on 31 Temmuz 2019 �ar�amba, 08:51
 */

#ifndef LFR_TREN_H
#define   LFR_TREN_H

#ifdef   __cplusplus
extern "C" {
#endif
    #include <18F45K22.h>
    //#include <18F4520.h>
    #device adc=10
    #device HIGH_INTS=TRUE 
    #device PASS_STRINGS=IN_RAM  
   // #device ICD=FALSE

    #FUSES NOWDT                    //No Watch Dog Timer
    #FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
    #FUSES INTRC_IO                 //Internal RC Osc, no CLKOUT
    #FUSES PLLEN                  //4X HW PLL disabled, 4X PLL enabled in software
    #FUSES NOPUT                      //Power Up Timer
    #FUSES NOBROWNOUT               //No brownout reset
    #FUSES WDT_SW                   //No Watch Dog Timer, enabled in Software
    #FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
    #FUSES MCLR                   //Master Clear pin used for I/O
    #FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
    #FUSES PROTECT                  //Code protected from reads
    //#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
    #FUSES NOFCMEN
    #FUSES NOHFOFST     
    #FUSES NODEBUG 

    #use delay(internal=64MHz) //

    #use fast_io(d)
   // #use rs232(baud=9600,parity=N,UART2,bits=8,stream=usbSerial)  //uart1
    #use rs232(baud=9600,parity=N,xmit=PIN_D6, rcv=PIN_D7, bits=8,STREAM=usbSerial) 

   // #use rs232(baud=9600,parity=N, xmit=PIN_C6, rcv=PIN_C7,bits=8,DISABLE_INTS,STREAM=dfPlayerSerial)
    #use rs232 (baud=9600, UART1, parity=N, stop=1, ERRORS,STREAM=dfPlayerSerial)
    

    extern unsigned int32 timerIntSay;



#ifdef   __cplusplus
}
#endif

#endif   /* LFR_TREN_H */

