
#include "tanimlamalar.h"

int tusBasSay = 0;
int tusBasAyarSay = 0;
int jetonSureSesSay = 9;
unsigned int8 jetonSuresiSay = 20;
int sensIptalSay = 0;
long long time_sure_son_say=0;


void JtnSureAyarSeslendir(unsigned int ses) {
    mp3_stop_100mS();
    mp3_play(ses);
    delay_ms(1000);

}

void rf_islemler() {

    if (input(tus1)) // Kumanda: Çalýþmayý baþlatýr, hýzý arttýrýr. 
                     // Çizgi:Çalýþmayý baþlatýr.
    {
        time_aku_sarj_seslendir=millis();
        output_toggle(LED2_PIN);
        bip_rf_var();
        tusBasSay = 0;
        sensIptalSay = 0;
        mp3_stop_100mS();
        if (rfKontrol) {
            fan_on();
            do {
                solMhiz += 50;
                sagMhiz += 50;
                if (solMhiz >= pwmMaxDeger)solMhiz = pwmMaxDeger;
                if (sagMhiz >= pwmMaxDeger)sagMhiz = pwmMaxDeger;
                ileri_git(solMhiz, sagMhiz);
               if(solMhiz<700) bip_rf_var();
                delay_ms(250);
            } while (input(tus1));

            
            
            ileri_git(solMhiz, sagMhiz);
        } else {
            //cizgi takip iþlemi baþlat
            if (cizgiKontrol) {
                mp3_stop_100mS();
                k_means_baslangic_esik_olustur();
                mp3_set_volume (df_player_ses);
                delay_ms(500);
                mp3_play(ses_getReady);
                delay_ms(3000);
                mp3_stop_100mS();
                mp3_play(ses_miniko);
                delay_ms(1000);
                
                ortHiz=ORT_HIZ_MIN; 
                soft_kalkis=FALSE;
                
                set_millis(0);
                fan_on();
                oyunBitti=FALSE;
                cizgiKontrolBaslat = TRUE;  // oyun baþlat 
                oyunSuresiSay = millis();   // oyun suresini resetle.
            }
        }
    }
    if (input(tus2))        //  Kumanda : Hýzý azaltýr. Çalýþma süresi ayarlama iþleminde süreyi arttýrýr.
                            //  Uzun süre basýlýnca mp3 player sesi ayarlar
    {
        time_aku_sarj_seslendir=millis();
        bip_rf_var();
        mp3_stop_100mS();
        tusBasAyarSay++;
        
        if (tusBasAyarSay > 5 && rfKontrol) {
            tusBasAyarSay=0;
            
            mp3_rastgele();
            
            do{
                  if (input(tus2)){
                      delay_ms(250);
                      df_player_ses+=3;
                      if(df_player_ses>27) df_player_ses=3;
                      mp3_set_volume(df_player_ses);
                      bip_rf_var();
                      delay_ms(100);
                  }
                  
                  if (input(tus6)){
                    write_eeprom(ee_adr_df_ses, df_player_ses);
                    bip_yazildi();
                   
                    break;
                  }
                
            } while(1);         
        }
        
        if (rfKontrol) {

            solMhiz -= 50;
            sagMhiz -= 50;

            if (solMhiz < 50)solMhiz = 0;
            if (sagMhiz < 50)sagMhiz = 0;
            ileri_git(solMhiz, sagMhiz);
            bip_rf_var();
            //fprintf(softSerial,"solMotor:%Lu\r\n",solMhiz);
        }
    }

    if (input(tus3))        // Kumanda :Devamlý basýlýnca sola döndürür.  
                            // Çizgi :Çizgi izleme hýzýný arttýrýr.
    {
        time_aku_sarj_seslendir=millis();
        tusBasSay = 0;
        mp3_stop_100mS();
        if (cizgiKontrolBaslat)
        {
            do{
                solMhiz = 0;
                sagMhiz = 0;
                ortHiz=0;
                
                trenDur();
                
                if (input(tus3)) {
                    delay_ms(250);
                    ortHiz_max+=25;
                    if (ortHiz_max >= pwmMaxDeger)ortHiz_max = pwmMaxDeger;
                    else  bip_rf_var();
                }
 
                if (input(tus6)){
                    write_eeprom(ee_adr_ortHiz_H, make8(ortHiz_max,1));
                    write_eeprom(ee_adr_ortHiz_L, make8(ortHiz_max,0));
                    
                    bip_yazildi();
                    
                    cizgiKontrolBaslat=FALSE;
                    break;
                }
               
            }while(1);
        }
        
        if (rfKontrol) {
            do {
                sola_don(solMhiz, 100); //hýz, sure
            } while (input(tus3));
        }
    }
    if (input(tus4))    // Kumanda : Devamlý basýlýnca saða döndürür. Çalýþma süresi ayarlama iþleminde süreyi azaltýr.
                        // Çizgi : Çizgi izleme hýzýný azaltýr.
    {
        time_aku_sarj_seslendir=millis();
        tusBasSay = 0;
        mp3_stop_100mS();

        if (cizgiKontrolBaslat) 
        {
            do{
                solMhiz = 0;
                sagMhiz = 0;
                ortHiz=0;
                
                trenDur();
                
                if (input(tus4)) {
                    delay_ms(250);
                    ortHiz_max-=25; 
                    if (ortHiz_max <= 200)ortHiz_max = 200;
                    else  bip_rf_var();
                }
                if (input(tus6)){
                    write_eeprom(ee_adr_ortHiz_H, make8(ortHiz_max,1));
                    write_eeprom(ee_adr_ortHiz_L, make8(ortHiz_max,0));
                    
                    bip_yazildi();
                    
                    cizgiKontrolBaslat=FALSE;
                    break;
                }
               
            }while(1);
            
        }
        
        if (rfKontrol) {
            do {
                saga_don(SagMhiz, 100); //hýz, sure
            } while (input(tus4));
        }

    }
    if (input(tus5))    // Kumanda : Durdurur ve uzun süre basýlýnca çizgi izleme moduna geçirir
                        // Çizgi : Durdurur ve uzun süre basýlýnca kumanda ile kontrol moduna geçirir.
    {
       time_aku_sarj_seslendir=millis();
        bip_rf_var();
        if (cizgiKontrolBaslat) {
            if (solMHiz < sagMhiz)sagMHiz = solMhiz;
            else solMHiz = sagMhiz;
        }

        solMhiz = 0;
        sagMhiz = 0;
        trenDur();
        tusBasSay++;
        cizgiKontrolBaslat = FALSE;
        mp3_stop_100mS();
        fan_off();
        if (tusBasSay > 10) {

            if ((rfKontrol)&&(!cizgiKontrol)) {

                rfKontrol = FALSE;
                cizgiKontrol = TRUE;

            } else if ((!rfKontrol)&&(cizgiKontrol)) {

                rfKontrol = TRUE;
                cizgiKontrol = FALSE;

            }
            if (cizgiKontrol) {

                mp3_stop_100mS();
                mp3_set_volume(dfPlayerNSes);
                delay_ms(30);
                mp3_play(ses_cizgiTakip);
                delay_ms(1000);
                //mp3_stop();
            }
            if (rfKontrol) {

                mp3_stop_100mS();
                mp3_set_volume(dfPlayerNSes);
                delay_ms(30);
                mp3_play(ses_kumandaKontrol);
                delay_ms(3000);
                mp3_stop();
                delay_ms(100);
             }

            tusBasSay = 0;

        }

    }

    if (input(tus6))    // Kumanda : Uzun süre basýlýnca çalýþma süresi ayarlama iþlemini çalýþtýrýr.
                        // çizgi ort hiz max deðerini kaydeder
    {
        time_aku_sarj_seslendir=millis();
        bip_rf_var();
        tusBasAyarSay++;
        mp3_stop_100mS();

        if (tusBasAyarSay > 5 && rfKontrol) {

            tusBasAyarSay = 0;
            mp3_set_volume(dfPlayerNSes);
            mp3_stop_100mS();
            mp3_play(ses_sureAyar);
            delay_ms(3000);

            do {
                if (input(tus2)) { // süre ++++
                    delay_ms(100);
                    bip_rf_var();
                    jetonSureSesSay++;
                    jetonSuresiSay += 5;

                    if (jetonSuresiSay > 100)jetonSuresiSay = 100;
                    if (jetonSureSesSay > 25) jetonSureSesSay = 25;
                    // fprintf(softSerial,"\r\n\t ses:%u  \t sure:%u \r\n",jetonSureSesSay,jetonSuresiSay); 
                    JtnSureAyarSeslendir(jetonSureSesSay);
                }
                if (input(tus4)) { // süre ---
                    delay_ms(100);
                    bip_rf_var();
                    jetonSureSesSay--;
                    jetonSuresiSay -= 5;
                    if (jetonSuresiSay < 20)jetonSuresiSay = 20;
                    if (jetonSureSesSay < 9) jetonSureSesSay = 9;
                    //fprintf(softSerial,"\r\n\t ses:%u  \t sure:%u \r\n",jetonSureSesSay,jetonSuresiSay); 
                    JtnSureAyarSeslendir(jetonSureSesSay);
                }
                if (input(tus6)) { // kaydet ve çýk
                    delay_ms(100);
                    bip_rf_var();
                    mp3_stop_100mS();
                    write_eeprom(ee_adr_jtn, jetonSuresiSay); //Configde eeprom yazmaya karþý korumalý olmamalý...
                    delay_ms(100);
                    //jetonSuresi=_mul(jetonSuresiSay,120000LL)/10;
                    //fprintf(softSerial,"eesure:%u R0x00:%u \r\n",jetonSuresiSay,read_eeprom(0x10));
                    ee_jeton_suresi_deger=jetonSuresiSay;
                    mp3_stop_100mS();
                    mp3_play(ses_getReady);
                    bip_yazildi();
                    break;
                }

            } while (1);

        }

    }
}
int sure_son_say = 0;

void sureSon() {

    if (lfr_led == 0b00011000 || kontrol == 0 || sure_son_say > 10) //tren orta noktayý bulunca duracak.
    {

        solMhiz = 0;
        sagMhiz = 0;
        trenDur();
        
        oyunSuresiSay = millis();
        cizgiKontrolBaslat = FALSE;

        mp3_stop_100mS();
        mp3_play(TREN_DUDUK);
        mp3_stop_100mS();
        mp3_play(OYUN_SON_JENERIK);
        delay_ms(3000);
        
        oyunBitti = TRUE;
        fan_off();
        sure_son_say = 0;
        dfPlayerNormSes();

    } 
    if (millis() - time_sure_son_say > 1000) {
            sure_son_say++;
            ortHiz -= 5;
            time_sure_son_say=millis();
    }
    
    if(ortHiz<100) ortHiz=100;

}//

void sureBul(int ee_deger) {

    switch (ee_deger) {
        case 20:
            if ((millis() - oyunSuresiSay)>(120000LL))//(240000LL)) 
            {
                sureSon();
            }
            break;
        case 25:
            if ((millis() - oyunSuresiSay)>(150000LL))//(300000LL)) 
            {
                sureSon();
            }
            break;
        case 30:
            if ((millis() - oyunSuresiSay)>(180000LL)) //(360000LL)) 
            {
                sureSon();
            }
            break;
        case 35:
            if ((millis() - oyunSuresiSay)>(210000LL))//(420000LL)) 
            {
                sureSon();
            }
            break;
        case 40:
            if ((millis() - oyunSuresiSay)>(240000LL))//(480000LL)) 
            {
                sureSon();
            }
            break;
        case 45:
            if ((millis() - oyunSuresiSay)>(270000LL))//(540000LL)) 
            {
                sureSon();
            }
            break;
        case 50:
            if ((millis() - oyunSuresiSay)>(300000LL))//(600000LL)) 
            {
                sureSon();
            }
            break;
        case 55:
            if ((millis() - oyunSuresiSay)>(330000LL))//(660000LL)) 
            {
                sureSon();
            }
            break;
        case 60:
            if ((millis() - oyunSuresiSay)>(360000LL))//(720000LL)) 
            {
                sureSon();
            }
            break;
        case 65:
            if ((millis() - oyunSuresiSay)>(390000LL))//(780000LL)) 
            {
                sureSon();
            }
            break;
        case 70:
            if ((millis() - oyunSuresiSay)>(420000LL))//(840000LL)) 
            {
                sureSon();
            }
            break;
        case 75:
            if ((millis() - oyunSuresiSay)>(450000LL))//(900000LL)) 
            {
                sureSon();
            }
            break;
        case 80:
            if ((millis() - oyunSuresiSay)>(480000LL))//(960000LL)) 
            {
                sureSon();
            }
            break;
        case 85:
            if ((millis() - oyunSuresiSay)>(510000LL))//(1020000LL)) 
            {
                sureSon();
            }
            break;
        case 90:
            if ((millis() - oyunSuresiSay)>(540000LL))//(1080000LL)) 
            {
                sureSon();
            }
            break;
        case 95:
            if ((millis() - oyunSuresiSay)>(570000LL))//(1140000LL)) 
            {
                sureSon();
            }
            break;
        case 100:
            if ((millis() - oyunSuresiSay)>(600000LL))//(1200000LL)) 
            {
                sureSon();
            }
            break;
    }
    
}


