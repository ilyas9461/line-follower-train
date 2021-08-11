/* 
 * File:   tanimlamalar.h
 * Author: ilyas
 *
 * Created on 31 Temmuz 2019 Çarþamba, 09:02
 */

#ifndef TANIMLAMALAR_H
#define	TANIMLAMALAR_H

#ifdef	__cplusplus
extern "C" {
#endif


    #define buzzerPIN PIN_C0

    #define FAN_PIN PIN_C4
    #define ACIL_DUR_SENS_PIN PIN_B3    //PIN_A7  LED1
    #define LED2_PIN PIN_A6
    #define IR_LED_ANOT_PIN PIN_C3

    #define fan_on() output_high(FAN_PIN);
    #define fan_off() output_low(FAN_PIN); 

    #define led1_on() output_high(LED1_PIN);
    #define led1_off() output_low(LED1_PIN);

    #define led2_on() output_high(LED2_PIN);
    #define led2_off() output_low(LED2_PIN);
    
    #define ir_led_on() output_low(IR_LED_ANOT_PIN);
    #define ir_led_off() output_high(IR_LED_ANOT_PIN);

    #define buzzer_off()  output_low(buzzerPIN)
    #define bip_yazildi() bipp(10,20,buzzerPIN) //sure,sayi,pin
    #define bip_rf_var() bipp(50,2,buzzerPIN)  //sure,sayi,pin   
    #define bip_kurulum() bipp(25,2,buzzerPIN)  //sure,sayi,pin
    #define bip_hata() bipp(3,100,buzzerPIN)
    #define bip_tus() bipp(5,2,buzzerPIN)  //sure,sayi,pin

    #define lfr_led_goster(sens)  hc595_spi_yaz(sens,8);

    #define pwmMaxDeger 700 //200     //5Khz sinyal
    
    #define ORT_HIZ_MIN 100 // soft kalkýþ için deðerler...
    #define ORT_HIZ_MAX 600    //350 

    #define solMotor_hiz(v) set_pwm2_duty(v);
    #define sagMotor_hiz(v) set_pwm1_duty(v);
    #define solMotorDur()  set_pwm2_duty(0); 
    #define sagMotorDur()  set_pwm1_duty(0); 

    #define trenDur() solMotorDur();sagMotorDur()
    #define ileri_git(v_sol,v_sag) solMotor_hiz(v_sol);sagMotor_hiz(v_sag)
    #define sola_don(hiz, sure) solMotor_hiz((int16)(hiz/10));sagMotor_hiz(hiz);delay_ms(sure)
    #define saga_don(hiz,sure) solMotor_hiz(hiz);sagMotor_hiz((int16)(hiz/10));delay_ms(sure)

    #define AN_CHANNEL_AKU 8
    #define ORNEK_SAYISI 8
    #define SENS_SAYISI 8

    #define HAM_DEGER 0
    #define ORT_DEGER 1
    #define MAX_DEGER 2
    #define MIN_DEGER 3

    #define MAX_HATA_DEGERI 1750

    #define GERILIM_VAR 1
    #define GERILIM_YOK 0

    #define EMITTER_ON 1
    #define EMITTER_OFF 0
    
    #define BEYAZ 0x00
    #define SIYAH 0x01

    #define ASIRI_ISIK  'A'
    #define NORMAL_ISIK 'N'
    #define DUSUK_ISIK 'D'
    #define KARANLIK_ISIK 'K'
    
    #define ORTALAMA_DEGER 'O'
    #define DIGITAL_DEGER   'B'

    #define HATA_REF    1750

    #define ee_adr_jtn 0x10
    #define ee_adr_ortHiz_H 0x00  //high   //16 bitlik sayý
    #define ee_adr_ortHiz_L 0x01  //low
    #define ee_adr_df_ses   0x02

    #define mp3_stop_100mS() mp3_stop();delay_ms(100)

    //433MHz alýcý baðlantý pinleri
    #define tus1 PIN_D0
    #define tus2 PIN_D1
    #define tus3 PIN_D2
    #define tus4 PIN_D3
    #define tus5 PIN_D4
    #define tus6 PIN_D5
    #define tus_vt  PIN_B0

    #define ses_miniko          1
    #define ses_demo            2
    #define ses_getReady        3
    #define ses_remix           4
    #define ses_trenDuduk       5
    #define ses_sureAyar        6
    #define ses_kumandaKontrol  7
    #define ses_cizgiTakip      8
    #define TREN_DUDUK          5
    #define OYUN_SON_JENERIK    2

    #define SES_AKU_SARJ_50     26
    #define SES_AKU_SARJ_25     27
    #define SES_AKU_SARJ_DUSUK  28
    #define SES_AKU_SARJ_TAK    29


#ifdef	__cplusplus
}
#endif

#endif	/* TANIMLAMALAR_H */

