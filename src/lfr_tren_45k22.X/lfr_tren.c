/* 
 * File:   lfr_tren.c
 * Author: ilyas
 *TCRT 5000 sensörlü
 * Created on 31 Temmuz 2019 Çarþamba, 08:50
 */

#include "lfr_tren.h"
#include "tanimlamalar.h"
#include <stdio.h>
#include <string.h>

#include <math.h>

#define STREAM_SERIAL_INPUT dfPlayerSerial
//#include "_input.c"

#define RAND_MAX 9      //rand() fonksiyonu için gerekli  RAND_MAX=MP3_SD_RASTGELE_MAX_SAYI olmalý
#include <stdlib.h>

#include "timer.h"
#include "hc595_spi.c"
#include "dfPlayer.c"
#include "dfPlayer.h"
#include "pid_hesapla.c"

unsigned int32 time_led = 0; //long long int32
unsigned int32 time_aku_sarj_seslendir=0;      //long long, int32
//unsigned int32 time_sens_deger_goster = 0;

unsigned int8 lfr_led = 0;                  // sensorlerin çizgi pozisyonlarýný tutar digital olarak.
unsigned int16 ir_sens_pos_digital = 0;     // ir_sens_pos_ort=0;

signed int16 solMhiz=0;     // *** 17.01.2020 : signed deðer olmalý heasaplama yanlýþ çýkýyor
signed int16 sagMhiz=0;     // PD den gelen deðer uygun þekilde alýnmadýðýndan kontrol uyuugn deðer alamýyor
                            // týrtýl sapýtýyor çizgi takip yapmýyor

unsigned int16 ortHiz=150, ortHiz_max=200;      // soft kalkýþ ve kontrol için deðiþkenler.
BOOLEAN soft_kalkis=FALSE;

int aku_ornek_say=0;
int aku_sarj_seslendir_say=0;
BOOLEAN aku_yuzde_50=FALSE;
BOOLEAN aku_yuzde_25=FALSE;
BOOLEAN aku_dusuk=FALSE;
BOOLEAN aku_sarja_tak=FALSE;

unsigned int32 time_aku_ornek=0;
int ee_jeton_suresi_deger=25;           //2.5 dakika demek.
unsigned int32 oyunSuresiSay=0;

float v_aku = 0.0;
unsigned int16 aku_adc_toplam=0;

//unsigned int16 aku_hareketli_ortalama_dizi[ORNEK_SAYISI];

int hareketli_ort_orn_say = 0;


unsigned int16 ir_sens_adc_ham_deger[SENS_SAYISI];
unsigned int16 ir_sens_ort_deger[SENS_SAYISI];          //kalibre edlmiþ ortalama deðer, ortam ýþýðýnýn etkisinden arýndýrýþný deðer.
unsigned int16 ir_sens_ort_deger_e_on[SENS_SAYISI];
unsigned int16 ir_sens_ort_deger_e_off[SENS_SAYISI];

//unsigned int16 ir_sens_ort_max_deger[SENS_SAYISI];
//unsigned int16 ir_sens_ort_min_deger[SENS_SAYISI];
//
unsigned int16 ir_sens_esik_deger[SENS_SAYISI];
unsigned int16 ir_sens_baslangic_esik_deger[SENS_SAYISI];

char ir_sens_isik_durumu[SENS_SAYISI]={0,0,0,0,0,0,0,0};
unsigned int16 ir_sens_asiri_isik_deger[SENS_SAYISI]={0,0,0,0,0,0,0,0};
unsigned int8 son_isik_durumu[SENS_SAYISI]={0,0,0,0,0,0,0,0};

unsigned int8 ir_sens_sayisal_deger[SENS_SAYISI] = {0, 0, 0, 0, 0, 0, 0, 0};

BOOLEAN sens_yeni_deger = 0;
int sens_emiter_on_off = 0;

signed int16 hata = 0, son_hata = 0,hata_deg=0 ,kontrol=0;      // iþaretli sayý olmalarý önemli...
float kP=0.0;   //oransal katsayý

BOOLEAN sens_hepsi_beyaz = 0;
BOOLEAN sens_hepsi_siyah = 0;

int1 rfKontrol=TRUE;
int1 cizgiKontrol=FALSE;
int1 cizgiKontrolBaslat=FALSE;
int1 oyunBitti=1;

int time_saniye_say=0;
long time_saniye=0;
long time_ort_hiz=0;

unsigned int16 agirlikliPosGetir(char ortDigital);
void ir_sens_adc_oku(BOOLEAN e_on);
void millis_kurulum();
//void sens_max_min_bul(BOOLEAN anlik, BOOLEAN baslangic);
void sens_led_goster();
//void sens_deger_sayisal();
//void kalibrasyon();
void Timer0_int();
//void sens_cizgi_durum_getir();
//unsigned int16 hareketli_ortalama(int ornek_say, unsigned int16 an_deger, unsigned int16 *moving_dizi);
void bipp(unsigned int16 sure, int sayi, long pin);
void delay_m_us(unsigned int16 n);
//float v_aku_oku_ort();
v_aku_oku(int orn_say);
void aku_sarj_seslendir(float aku_gerilim);
//void debug_ir_sens_dizi(unsigned int16 *ir_dizi);
//void debug_ir_sens(int deger_tip,BOOLEAN volt);
void test_donanim();
//void dizi_set(unsigned int16 *dizi, unsigned int16 deger, int16 uzunluk);

#include "k_means_fonk.c"
#include "rf_islemler.c"

void main() {
    // setup_oscillator(OSC_32MHZ);
    setup_oscillator(OSC_64MHZ);

    setup_dac(DAC_OFF);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    //setup_adc(ADC_OFF);

    setup_ccp1(CCP_PWM); // Configure CCP1 as standard PWM
    setup_ccp2(CCP_PWM); // Configure CCP2 as standard PWM
    setup_ccp3(CCP_OFF);
    setup_ccp4(CCP_OFF);
    setup_ccp5(CCP_OFF);

    setup_timer_1(T1_DISABLED);
    //setup_timer_2(T2_DIV_BY_16, 250, 1);   // 40MHz fosc ile 2.5KHz yaklaþýk(div_16)
    setup_timer_4(T4_DISABLED, 0, 1);
    setup_timer_5(T5_DISABLED);
    setup_timer_6(T6_DISABLED, 0, 1);

    // setup_adc(ADC_CLOCK_DIV_16); 
    setup_adc(ADC_CLOCK_INTERNAL); //  9 bit (max 511) okuyan adc bu þekilde 10 bit oldu.....
    setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3 | sAN4 | sAN5 | sAN6 | sAN7 | sAN8, VSS_VDD);
    /* PWM Hakkýnda :
        setup_timer_2(T2_DIV_BY_16, 124, 1); komut satýrý ile 10 bit PWM
     *  2500Hz için ayarlanmýþ oldu 
     */
    //setup_timer_2(T2_DIV_BY_16, 124, 1);   // Set PWM frequency to 2500Hz(div_16) max Pwm deger 500
    //setup_timer_2(T2_DIV_BY_1, 249, 1);   //20KHz max PWM deger 500

    setup_timer_2(T2_DIV_BY_16, 199, 1);      //5Khz max PWM deger=200 peryot kadar osilaskop sonucu 22.01.20 bu uygulamada olmadý
                                              // 17.01.2020 : Ugulamada  700 deðeri hesaplamaya göre görüldü 22.01.20 evet....
                                              // excell hesabýnda %50 duty deðeri 400 çýkýyor. 5KHz PWM 
    
    //setup_timer_2(T2_DIV_BY_4, 124, 1);     //10HzKhz max PWM deger 460

    sagMotor_hiz(0) ;     //set_pwm1_duty(100);
    solMotor_hiz(0);      // //set_pwm2_duty(100);
   
   // set_tris_d(10111111);

    buzzer_off();
    bip_rf_var();
    fan_off();
    // led1_off(); //kullanýlmamalý
    led2_off();

    millis_kurulum();
    delay_m_us(100);
    
    if(read_eeprom(ee_adr_jtn)!=0xFF)
        ee_jeton_suresi_deger=read_eeprom(ee_adr_jtn);
    else ee_jeton_suresi_deger=25; // 2.5 dak
    
    if(read_eeprom(ee_adr_ortHiz_H)!=0xFF && read_eeprom(ee_adr_ortHiz_L)!=0xFF)
    {
        ortHiz_max=make16(read_eeprom(ee_adr_ortHiz_H), read_eeprom(ee_adr_ortHiz_L));
    }else {
        ortHiz_max=350;
//        ortHiz_max=350;
//        ortHiz_max=450;
    }
    
    if(read_eeprom(ee_adr_df_ses)!=0xFF)
        df_player_ses=read_eeprom(ee_adr_df_ses);
    else df_player_ses=dfPlayerNSes;
    
    mp3_set_volume(df_player_ses);delay_ms(100);
     //fprintf(usbSerial, "Basla...\r\n");
     
     memset(ir_sens_ort_deger, 0, SENS_SAYISI);
    ir_led_off();
    //kalibrasyon();
    k_means_baslangic_esik_olustur();

    srand(MP3_SD_RASTGELE_MAX_SAYI); // everytime this is called the seed will be initialized and the random sequence will be started 
    // time_aku_ornek=millis();
    time_aku_ornek = time_aku_sarj_seslendir=millis();
    lfr_led_goster(lfr_led=0);
    
    ortHiz=ORT_HIZ_MIN;      //150     //max pwm=700
    time_ort_hiz=millis();
    
    while (TRUE) {

//        if(!input(ACIL_DUR_SENS_PIN)){
//            ileri_git(0,0); //tren acil dur
//            mp3_stop_100mS();
//            mp3_play(TREN_DUDUK);
//            delay_ms(1000);
//            mp3_play(TREN_DUDUK);
//            delay_ms(1000);
//        }             
//        else 
        
        ileri_git(solMhiz,sagMhiz);     // *** hýzda devemlýlýðý saðlýyor.
        
        rf_islemler() ;

        if (rfKontrol || oyunBitti) 
        {
            if (input(DF_BUSY_PIN) &&
                (!input(tus1) && !input(tus2) && !input(tus3) && !input(tus4) && !input(tus5) && !input(tus6))
               ) 
            {
                if(solMhiz==0 && sagMhiz ==0)
                        mp3_rastgele();
            }
        }
        
        if(rfKontrol && k_means_yeni_veri) //!rfKontrol)// normal çalýþýrken 
        {
            k_means_max_min_esik();
            sens_cizgi_durum_getir();
            sens_led_goster();
            k_means_yeni_veri = FALSE;
        }
        
        if (k_means_yeni_veri && cizgiKontrolBaslat) {
            
            k_means_max_min_esik();
            sens_cizgi_durum_getir();
            sens_led_goster();
            
            ir_sens_pos_digital = agirlikliPosGetir(DIGITAL_DEGER);
            
           // ir_sens_pos_digital = agirlikliPosGetir(ORTALAMA_DEGER);        
           // fprintf(usbSerial,"ort:%lu \t ht:%ld\r\n",ir_sens_pos_digital,hata);
            
            hata = HATA_REF - ir_sens_pos_digital;
            hata_deg=hata-son_hata;
            
            //ortHiz=200;      //150     //max pwm=700
            
            kP=(float)(((float)ortHiz)/((float)MAX_HATA_DEGERI));// 150/1750=0,08571    0.1143f;
            
            kontrol=pdKontrol(hata, hata_deg,son_hata, kP,(kP/8.0));
            
            fprintf(usbSerial,"pos:%lu\tkont:%ld\r\n",ir_sens_pos_digital,kontrol);

            if (kontrol < 0) //// araba sola kaymýþ durumda sol motora güç ver
            {
                solMhiz = ortHiz;
                sagMhiz = ortHiz + kontrol; // kontrol negatif sayý

                if (sagMHiz < 0)sagMhiz = 0;
                if (solMHiz < 0)solMhiz = 0;
                if (sagMHiz > pwmMaxDeger)sagMhiz = pwmMaxDeger;
                if (solMHiz > pwmMaxDeger)solMhiz = pwmMaxDeger;

            } else if (kontrol > 0) //araba saða kaymýþ sað motora güç ver 
            {
                
                solMhiz = ortHiz - kontrol;
                sagMhiz = ortHiz;

                if (sagMHiz < 0)sagMhiz = 0;
                if (solMHiz < 0)solMhiz = 0;
                if (sagMHiz > pwmMaxDeger)sagMhiz = pwmMaxDeger;
                if (solMHiz > pwmMaxDeger)solMhiz = pwmMaxDeger;
                
            } else if (kontrol == 0) //
            {
                sagMhiz = ortHiz;
                solMhiz = ortHiz;
            }
            if(sens_hepsi_siyah)    //Sekiz þeklinde çizilmiþ pist için...
            {
                sagMhiz = ortHiz;
                solMhiz = ortHiz;
            }
            if(sens_hepsi_beyaz) {
                solMhiz=sagMhiz=0;
            }
            
            ileri_git(solMhiz,sagMhiz); //motorlara yeni hýz bilgisi gonderiliyor....
            
            sureBul(ee_jeton_suresi_deger); //oyun suresi bitiþi kontrol ediliyor...
            
            son_hata = hata;
            k_means_yeni_veri = FALSE;
            
            if (input(DF_BUSY_PIN) && cizgiKontrolBaslat)
            {
                mp3_play(ses_miniko);
            }

            if((millis()-time_ort_hiz>100) && !soft_kalkis)   //soft kalkýþ için hýzýn arttýrýlmasý
            {
                ortHiz+=10;
                
                if(ortHiz>=ortHiz_max){
                    ortHiz=ortHiz_max;
                    soft_kalkis=TRUE;
                }
                time_ort_hiz=millis();
            }
            
            /*
            //fprintf(usbSerial, "t:%Lu\r\n", millis() - time_sens_deger_goster);
            //time_sens_deger_goster = millis();
            */
            
        }//çizgi kontrol

        if(((millis()-time_aku_ornek)>50))//&& !cizgiKontrolBaslat)
        {
            v_aku_oku(aku_ornek_say++);
   
            if(aku_ornek_say>19){
                aku_ornek_say=0;
            }
           
            time_aku_ornek=millis();
        }
        
        if(((millis()-time_aku_sarj_seslendir)>150000LL))// && !cizgiKontrolBaslat) 
            // 5 dakikada bir ve tren duruyorken
        {  
            if((solMhiz==0 && sagMhiz==0) && rfKontrol){
                 aku_sarj_seslendir(v_aku);
            }
               
            
            time_aku_sarj_seslendir=millis();
        }
//        
        //test_donanim();
       
    }//while loop

}//main

////////

unsigned int16 agirlikliPosGetir(char ortDigital) {
#define AGIRLIK_KATSAYI 500
    //çarpma yada bölme iþleminde uygun  deðiþken tipi belirlenmezse  inerrupt larýn deisable edildiðine dair uyarý verir.....
    unsigned int8 x;
    unsigned int32 topAgirlik_int = 0;
    unsigned int16 toplam_int = 0;

    for (x = 0; x < SENS_SAYISI; x++) {

        if (ortDigital == ORTALAMA_DEGER) {

            topAgirlik_int += _mul(_mul((unsigned int16) ir_sens_ort_deger[x], x), AGIRLIK_KATSAYI);
            toplam_int += ir_sens_ort_deger[x];
        }

        if (ortDigital == DIGITAL_DEGER) {
            if (bit_test(lfr_led, x)) {
                topAgirlik_int += 1 * (x * AGIRLIK_KATSAYI);
                toplam_int++;
            }

            //topAgirlik+=_mul(_mul(ir_sens_sayisal_deger[x],x),AGIRLIK_KATSAYI);
        }
        //topAgirlik+=(sensorDeger[.x])*(x*500);
        //toplam+= sensorDeger[x];

    }
    return (unsigned int16) (topAgirlik_int / toplam_int);

    //fprintf(usbSerial,"A:%Lu\tT:%Lu\t pos=%Lu \r\n",topAgirlik,toplam,sonDeger);


}

void ir_sens_adc_oku(BOOLEAN e_on) {
    unsigned int16 ir_adc = 0;
    int i = 0;

    for (i = 0; i < 8; i++) {
        set_adc_channel(i);
        delay_us(20);
        ir_adc = read_adc();
        ir_sens_adc_ham_deger[i] = ir_adc;
    }

    if (e_on == 1) {

        ir_sens_ort_deger_e_on[0] = ir_sens_adc_ham_deger[0];
        ir_sens_ort_deger_e_on[1] = ir_sens_adc_ham_deger[1];
        ir_sens_ort_deger_e_on[2] = ir_sens_adc_ham_deger[2];
        ir_sens_ort_deger_e_on[3] = ir_sens_adc_ham_deger[3];
        ir_sens_ort_deger_e_on[4] = ir_sens_adc_ham_deger[4];
        ir_sens_ort_deger_e_on[5] = ir_sens_adc_ham_deger[5];
        ir_sens_ort_deger_e_on[6] = ir_sens_adc_ham_deger[6];
        ir_sens_ort_deger_e_on[7] = ir_sens_adc_ham_deger[7];
    }
    if (e_on == 0) {
        ir_sens_ort_deger_e_off[0] = ir_sens_adc_ham_deger[0];
        ir_sens_ort_deger_e_off[1] = ir_sens_adc_ham_deger[1];
        ir_sens_ort_deger_e_off[2] = ir_sens_adc_ham_deger[2];
        ir_sens_ort_deger_e_off[3] = ir_sens_adc_ham_deger[3];
        ir_sens_ort_deger_e_off[4] = ir_sens_adc_ham_deger[4];
        ir_sens_ort_deger_e_off[5] = ir_sens_adc_ham_deger[5];
        ir_sens_ort_deger_e_off[6] = ir_sens_adc_ham_deger[6];
        ir_sens_ort_deger_e_off[7] = ir_sens_adc_ham_deger[7];
    }


    hareketli_ort_orn_say++;

}

void millis_kurulum() {

    setup_timer_0(T0_INTERNAl | T0_8_BIT | T0_DIV_64);    //32 idi 
    set_timer0(0x05); //1000uS 64MHZ  T0_DIV_64 ile 1mS  
                      //0x05, T0_8_BIT | T0_DIV_64 -> 500uS son hesapta, kontrol edilecek. uygulamada 
                      // 17.01.2020 : T0_8_BIT | T0_DIV_64 500uS çýkýyor.
        
    /* set_timer0(0xC0);            //1000uS 16MHZ

    //    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    //    set_timer1(0xFB1D);   //64285 1000uS div_8 ile kesme   
    //    setup_timer_3(T3_INTERNAL|T3_DIV_BY_8);
    //    set_timer3(0xFB1D);
    //    clear_interrupt(INT_TIMER3);
    //    enable_interrupts(INT_TIMER3); 
    //    disable_interrupts(INT_TIMER3);
    //    enable_interrupts(PERIPH);-
    //    enable_interrupts(INT_TIMER1);
     */

    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);

}

#INT_TIMER0 //HIGH                    //Timer0 Kesmesi

void Timer0_int() {
    // set_timer0(0x43);            //1000uS 16Mhz
    // set_timer0(0x82);            //1000uS 32MHZ
    
    set_timer0(0x05); //1000uS 64MHZ  // 0x05:500uSson hesapta kokontorl edilecek. uygulamada 
    
    // set_timer0(0xC0);            //1000uS 16MHZ
    // output_toggle(clock);

    if (sens_emiter_on_off == 0) {
        ir_led_off(); // Anotlara +5V ver ve 1mS sonra on dðeri oku
    }
    if (sens_emiter_on_off == 1) {
        ir_sens_adc_oku(EMITTER_OFF); // off deðeri oku
    }
    if (sens_emiter_on_off == 2) {
        ir_led_on();
    }
    if (sens_emiter_on_off == 3) {

        ir_sens_adc_oku(EMITTER_ON); // on deðeri oku
        sens_yeni_deger = 1;

        //sens_cizgi_durum_getir();       // sensör çizgi durum güncelle  
        k_means_veri_havuzu_olustur();

        // ir_led_off();                       //Anotlar dan +5V kes ve 1mS sonra aff deðeri oku...
    }

    sens_emiter_on_off++;
    if (sens_emiter_on_off > 3) sens_emiter_on_off = 0;

    timerIntSay++;

    clear_interrupt(INT_TIMER0);
    
}//

void sens_led_goster() {

    int i, say_siyah = 0, say_beyaz = 0;

    for (i = 0; i < SENS_SAYISI; i++) {

        if (ir_sens_ort_deger[i] < baslangic_esik_ort_deger) ++say_siyah; 
        if (ir_sens_ort_deger[i] > baslangic_esik_ort_deger) ++say_beyaz;

        if (ir_sens_sayisal_deger[i] == 1) {
            bit_set(lfr_led, i);
           // lfr_led_goster(lfr_led);
        }
        if (ir_sens_sayisal_deger[i] == 0) {

            bit_clear(lfr_led, i);
            // lfr_led_goster(lfr_led);
        }
    }
    
    if (say_siyah >= 8) {
        sens_hepsi_siyah = 1;
    } else sens_hepsi_siyah = 0;

    if (say_beyaz >= 8 ) {
        sens_hepsi_beyaz = 1;
    } else sens_hepsi_beyaz = 0;

    if (sens_hepsi_siyah) {
        lfr_led = 0xFF;
       // lfr_led_goster(lfr_led);
    }
    if (sens_hepsi_beyaz) {
        lfr_led = 0x00;
        //lfr_led_goster(lfr_led);
    }
    
    lfr_led_goster(lfr_led);
    
}//

//unsigned int16 hareketli_ortalama(int ornek_say, unsigned int16 an_deger, unsigned int16 *moving_dizi) {
//    int x = 0;
//    unsigned int16 ort = 0, toplam = 0;
//    ;
//    int payda = 0;
//
//    if (ornek_sayisi < ORNEK_SAYISI) {
//        moving_dizi[ornek_say] = an_deger; // filtre ilk degerlerde pasif
//        ort = an_deger;
//    } else { // filtre aktif.
//
//        for (x = 0; x < ORNEK_SAYISI; x++) {
//
//            if (moving_dizi[x] != 0) payda++; // sýfýr sayýsýna göre payda deðerini belrile
//            toplam += moving_dizi[x];
//        }
//        if (payda == 0) return an_deger;
//
//        ort = toplam / payda;
//
//        for (x = 1; x < ORNEK_SAYISI - 1; x++) // en eski deðeri çýkar ortalamaya yeni deðeri ekle.
//            moving_dizi[x - 1] = moving_dizi[x];
//
//        moving_dizi[ORNEK_SAYISI - 1] = an_deger;
//    }
//    return ort;
//}

void test_donanim() {
    
    int8 led_say = 0;
    
    if (millis() - time_led > 250) {

        if (led_say >= 7) {

            led_say = 0;
            lfr_led = 0;

            //bip_rf_var();
            output_toggle(FAN_PIN);
            // output_toggle(IR_LED_ANOT_PIN);

            //fprintf(usbSerial,"Vaku(24V)=%f V\r\n",v_aku);
            //v_aku_oku();

            //debug_ir_sens(ORT_DEGER, GERILIM_VAR);
        }


        if (led_say == 2) {
            output_toggle(LED2_PIN);
        }

        if (led_say == 6) {
            //output_toggle(LED1_PIN);
        }

        led_say++;
        // bit_set(lfr_led,led_say);
        //lfr_led_goster(lfr_led);

        time_led = millis();
    }
}

void bipp(unsigned int16 sure, int sayi, long pin) {
    int a = 0;
    for (a = 0; a < sayi; a++) {

        output_high(pin);
        delay_ms(sure / 2);
        output_low(pin);
        delay_ms(sure / 2);
    }
}//

void delay_m_us(unsigned int16 n) {

    for (; n != 0; n--)
        delay_us(1000);
}



v_aku_oku(int orn_say)
{
    set_adc_channel(AN_CHANNEL_AKU); //0,1,2,....16..
    delay_us(50);
    aku_adc_toplam+= read_adc();
    delay_us(50);
    
    if(orn_say>=19){
       //aku_adc_toplam=aku_adc_toplam>>4;
        aku_adc_toplam=(unsigned int16 )(aku_adc_toplam/20);
        //v_aku_deger=(float)(aku_adc_toplam*(5.0/1024.0)*6.0); //  Vadc=Vaku*(R1/(R1+R2)--> Vaku=Vadc*((R1+R2)/R1)), (R1+R2)/R1=(10+50)Kohm/10Kohm=6
        v_aku=(float)(((float)(aku_adc_toplam))*(0.004792948)); // ADC çözünürlek deðeri 5/1024 yerine ölçü aleti ile yapýlan ölçümler sonucu 
                                                                // escellde belirlenen ortalama deðer alýndý. (muhtemel fark direnc toleranslarý))
        v_aku*=6.0;                                             // ***
        
       // fprintf(usbSerial,"aku_adc:%Lu\r\n",aku_adc_toplam);
       // fprintf(usbSerial,"Vaku(24V)=%f V\r\n",v_aku);
        aku_adc_toplam=0;

    }
    
}

#define AKU_SARJ_SES_DELAY 2000

void aku_sarj_seslendir(float aku_gerilim){

    int x=0;
    //Vaku > 12,60      %100        25,2
    //     12,60-12,40  %75-50      25,2-24,80
    //     12,20-12,00  %50-25      24,40-24,00
    //     12,00-11,70  %25-00      24,00-23,40
    //      <11,70      Derin þarj
    // Bu ideal deðerlere göre mevcut durumdaki akülerin 
    // gerilimlerine göre seviye belirlemesi yapýldý
    
    if(aku_gerilim>24.00){
        aku_sarj_seslendir_say=0;
        aku_yuzde_25=FALSE;
        aku_yuzde_50=FALSE;
        aku_dusuk=FALSE;
        aku_sarja_tak=FALSE;
    }

    if ((aku_gerilim < 24.00 && aku_gerilim >= 23.00) && !aku_yuzde_50) {
        bip_hata();
        delay_ms(750);
        
        if (aku_sarj_seslendir_say < 3) {
            for (x = 0; x < 3; x++) {
                mp3_stop_100mS();
                mp3_play(SES_AKU_SARJ_50);
                delay_ms(AKU_SARJ_SES_DELAY);
            }
            
        }
        
        aku_sarj_seslendir_say++;
        if (aku_sarj_seslendir_say >= 3){
            aku_sarj_seslendir_say = 0;
            aku_yuzde_50=TRUE;
        }
    }

    if ((aku_gerilim < 23.00 && aku_gerilim >= 22.50) && !aku_yuzde_25) {
        bip_hata();
        delay_ms(750);

        if (aku_sarj_seslendir_say < 3) {
            for (x = 0; x < 3; x++) {
                mp3_stop_100mS();
                mp3_play(SES_AKU_SARJ_25);
                delay_ms(AKU_SARJ_SES_DELAY);
            }
        }
        
        aku_sarj_seslendir_say++;
        if (aku_sarj_seslendir_say >= 3){
            aku_sarj_seslendir_say = 0;
            aku_yuzde_25=TRUE;
        }
    }

    if ((aku_gerilim < 22.50 && aku_gerilim >= 22.00) && !aku_dusuk) {
        bip_hata();
        delay_ms(750);
        if (aku_sarj_seslendir_say < 3) {
            for (x = 0; x < 3; x++) {
                mp3_stop_100mS();
                mp3_play(SES_AKU_SARJ_DUSUK);
                delay_ms(AKU_SARJ_SES_DELAY);
            }
        }
        
        aku_sarj_seslendir_say++;
        if (aku_sarj_seslendir_say >= 3){
            aku_sarj_seslendir_say = 0;
            aku_dusuk = TRUE;
        }
    }

    if (aku_gerilim < 21.90 && !aku_sarja_tak) {

        bip_hata();
        delay_ms(750);
        
        if (aku_sarj_seslendir_say < 3) {
            for (x = 0; x < 3; x++) {
                mp3_stop_100mS();
                mp3_play(SES_AKU_SARJ_TAK);
                delay_ms(AKU_SARJ_SES_DELAY);
            }
        }

        aku_sarj_seslendir_say++;
        if (aku_sarj_seslendir_say >= 3){
            aku_sarj_seslendir_say = 0;
            aku_sarja_tak = TRUE;
        }
    }

}//



//        if (millis() - time_sens_deger_goster > 500) {
//
//            //debug_ir_sens(ORT_DEGER, GERILIM_VAR);
//            //debug_ir_sens(HAM_DEGER, GERILIM_VAR);
//            //             debug_ir_sens_dizi(ir_sens_ort_deger_e_on);
//            //             debug_ir_sens_dizi(ir_sens_ort_deger_e_off);
//            // debug_ir_sens_dizi(ir_sens_ort_deger);
//            // debug_ir_sens_dizi(ir_sens_baslangic_esik_deger);
//            //  debug_ir_sens_dizi(ir_sens_sayisal_deger);
//
//            // fprintf(usbSerial,"\r\n");
//
//            //  debug_ir_sens(MAX_DEGER, GERILIM_VAR); 
//            //  debug_ir_sens(MIN_DEGER, GERILIM_VAR);
//
//            //max min deðerleri yeni ortam bilgisi için resetle
//            //  dizi_set(ir_sens_ort_max_deger,0,SENS_SAYISI);
//            // dizi_set(ir_sens_ort_min_deger,1023,SENS_SAYISI);
//
//            //  fprintf(usbSerial,"pos=%u \r\n",lfr_led);
//            // fprintf(usbSerial,"posDig=%Lu\thata=%Ld\r\n",ir_sens_pos_digital,hata);
//
//            // fprintf(usbSerial,"fisPOS:%f", g_fisOutput[0]);
//            // sens_deger_sayisal();
//            time_sens_deger_goster = millis();
//        }

//        if (sens_yeni_deger) {
//            //pozisyon bul PID algoritam çalýþtýr 
//            //ir_sens_pos_ort=agirlikliPosGetir(ORTALAMA_DEGER);
//            
//
//            // sens_led_goster();
//
//            sens_yeni_deger = 0;
//
//        }




