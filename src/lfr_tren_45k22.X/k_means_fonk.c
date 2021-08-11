//
#include "tanimlamalar.h"

unsigned int16 k_means_veri_havuzu[SENS_SAYISI * 10];
unsigned int16 max_esik_veri_havuzu[SENS_SAYISI * 10];
unsigned int16 min_esik_veri_havuzu[SENS_SAYISI * 10];
unsigned int16 max_esik_ort_deger = 0;
unsigned int16 min_esik_ort_deger = 0;
unsigned int16 k_means_esik_ort_deger = 0, baslangic_esik_ort_deger = 0;
BYTE k_means_veri_say = 0, k_means_veri_havuzu_index = 0;
BOOLEAN k_means_yeni_veri = 0;
BOOLEAN baslangic_esik_gec = FALSE;

void k_means_veri_havuzu_olustur() {
    int i = 0;

    k_means_veri_say++;

    if (k_means_veri_say <= 10 && !k_means_yeni_veri) {

        for (i = 0; i < SENS_SAYISI; i++) {

            if (ir_sens_ort_deger_e_on[i] == ir_sens_ort_deger_e_off[i]
                ||
                ir_sens_ort_deger_e_on[i]<ir_sens_ort_deger_e_off[i]    ) //Aþýrý ýþýk, Von=Voff
            {

                ir_sens_ort_deger[i] = ir_sens_ort_deger_e_on[i];
                k_means_veri_havuzu[k_means_veri_havuzu_index++] = ir_sens_ort_deger_e_on[i];
                ir_sens_isik_durumu[i] = ASIRI_ISIK; //Sayýsallaþtýrma iþlemine hangi durumda baþlanacaðýný belirler.

            } else if (ir_sens_ort_deger_e_on[i] > ir_sens_ort_deger_e_off[i] + 50) //normal ýþýk, Von>voff
            {
                ir_sens_ort_deger[i] = ir_sens_ort_deger_e_on[i] - ir_sens_ort_deger_e_off[i];
                k_means_veri_havuzu[k_means_veri_havuzu_index++] = ir_sens_ort_deger[i];
                ir_sens_isik_durumu[i] = NORMAL_ISIK; //Sayýsallaþtýrma iþlemine hangi durumda baþlanacaðýný belirler.
            }

            if (k_means_veri_havuzu_index >= (SENS_SAYISI * 10)) break;
        }
    }

    if (k_means_veri_say >= 10 || (k_means_veri_havuzu_index >= (SENS_SAYISI * 10))) {
        k_means_veri_havuzu_index = k_means_veri_say = 0;
        k_means_yeni_veri = TRUE;
    }


}

void k_means_baslangic_esik(unsigned int16 *veri) {
    int x = 0;

    unsigned int16 _max_deger = 0;
    unsigned int16 _min_deger = 1023;

    if (k_means_yeni_veri) {

        k_means_yeni_veri = FALSE;

        for (x = 0; x < SENS_SAYISI * 10; x++) {

            if (veri[x] > _max_deger) _max_deger = veri[x];
            if (veri[x] < _min_deger) _min_deger = veri[x];
        }

        baslangic_esik_ort_deger = (unsigned int16) ((_max_deger + _min_deger) / 2);
        if (baslangic_esik_ort_deger < 100) baslangic_esik_ort_deger = 400;
        
        for (x = 0; x < SENS_SAYISI; x++)
             ir_sens_baslangic_esik_deger[x]= baslangic_esik_ort_deger;
    }


}

void k_means_baslangic_esik_olustur() {

    unsigned int32 time_kal_yap = 0;
    bip_rf_var();
    time_kal_yap=millis();
    while (1) {

        k_means_baslangic_esik(k_means_veri_havuzu);

        if (millis() - time_kal_yap > 5000) {
            break;
        }

    }
    //    fprintf(usbSerial,"K Means Bas Esik:%lu \r\n",baslangic_esik_ort_deger);
    bip_yazildi();

}

void fuzzy_pos_led(float *f_dizi) {
    int x = 0, say_siyah = 0, say_beyaz = 0;

    unsigned int8 int_pos; //= (unsigned int8) ((s_pos- floor(s_pos) > 0.5) ? ceil(s_pos) : floor(s_pos));

    for (x = 0; x < 8; x++) {
        if (ir_sens_ort_deger[x] < baslangic_esik_ort_deger) ++say_siyah;
        if (ir_sens_ort_deger[x] > baslangic_esik_ort_deger) ++say_beyaz;
    }

    if (say_siyah >= 8) {
        int_pos = 15;
        sens_hepsi_siyah = 1;
    } else sens_hepsi_siyah = 0;

    if (say_beyaz >= 8) {
        int_pos = 16;
        sens_hepsi_beyaz = 1;
    } else sens_hepsi_beyaz = 0;

    for (x = 0; x < 8; x++) {

        if (f_dizi[x] > 0.5) {
            bit_set(lfr_led, x);
            //  lfr_led_goster(lfr_led);
        } else if (f_dizi[x] < 0.5) {
            bit_clear(lfr_led, x);
            // lfr_led_goster(lfr_led);
        }
    }
    if (sens_hepsi_siyah) {
        lfr_led = 0xFF;
        //lfr_led_goster(lfr_led);
    }
    if (sens_hepsi_beyaz) {
        lfr_led = 0x00;
        // lfr_led_goster(lfr_led);
    }
    lfr_led_goster(lfr_led);

}

void k_means_sens_sayisal() {
    int x = 0;

    for (x = 0; x < SENS_SAYISI; x++) {

        if (ir_sens_ort_deger[x] <= k_means_esik_ort_deger+100)
        //if (ir_sens_ort_deger[x] <=  baslangic_esik_ort_deger)
        {
            ir_sens_sayisal_deger[x] = SIYAH;
        }
      
        if (ir_sens_ort_deger[x] > k_means_esik_ort_deger+100)
        //if (ir_sens_ort_deger[x] > baslangic_esik_ort_deger)
        {
            ir_sens_sayisal_deger[x] = BEYAZ;
        }
        
    }//for...
}

void sens_cizgi_durum_getir(){
    int x=0;
    
    for(x=0;x<SENS_SAYISI;x++)   //Iþýk durum tespiti ve ortalama deger hesaplama   
    {
        if(ir_sens_ort_deger_e_on[x]==ir_sens_ort_deger_e_off[x] //sensor on ve off ölçümleri eþitse
           ||                                                    //veya
           ir_sens_ort_deger_e_on[x]<ir_sens_ort_deger_e_off[x]) // off ölçümü on ölçümünden büyükse
        {
           // ir_sens_ort_deger[x]=0;                              // sensor AÞIRI ýþýk altýnda
            ir_sens_isik_durumu[x]=ASIRI_ISIK;
            
            ir_sens_asiri_isik_deger[x]=ir_sens_ort_deger_e_on[x];  //Siyahta olma aþýrý ýþýk koþulu için Von deger kaydediliyor...
            fprintf(usbSerial,"--> ASIRI ISIK:%u\r\n",x);
        }
        else
        {

            if (son_isik_durumu[x] == ASIRI_ISIK) // ASIRI_ISIK--> NORMAL_ISIK geçiþi
            {
               // ir_sens_baslangic_esik_deger[x] = k_means_esik_ort_deger; // ilk degeri güncelle
                ir_sens_esik_deger[x]=k_means_esik_ort_deger;
               // ir_sens_ort_deger[x] = ir_sens_ort_deger_e_on[x] - ir_sens_ort_deger_e_off[x];
              //  ir_sens_isik_durumu[x] = NORMAL_ISIK;
                
                fprintf(usbSerial, "--> Normal ISIK:%u\r\n",x);
            } 
            if (son_isik_durumu[x] == NORMAL_ISIK) 
            {
                if (ir_sens_ort_deger_e_on[x] > ir_sens_ort_deger_e_off[x] + 100) //100=~500mV
                {
                    ir_sens_ort_deger[x] = ir_sens_ort_deger_e_on[x] - ir_sens_ort_deger_e_off[x];
                    ir_sens_isik_durumu[x] = NORMAL_ISIK;
                   // ir_sens_esik_deger[x] = k_means_esik_ort_deger;//
                    ir_sens_esik_deger[x] = ir_sens_baslangic_esik_deger[x];

                }
            }
        } 
        //*                         *//

        if(ir_sens_isik_durumu[x]==ASIRI_ISIK) ////Von==Voff
        {
            ir_sens_sayisal_deger[x]=BEYAZ;
           // hepsi_beyaz_say++;
        }

        if(ir_sens_isik_durumu[x]==NORMAL_ISIK)
        {
            if(ir_sens_ort_deger[x]> ir_sens_esik_deger[x]) {
                ir_sens_sayisal_deger[x]=BEYAZ;
                 //hepsi_beyaz_say++;
            }
            
            if(ir_sens_ort_deger[x]< ir_sens_esik_deger[x]) //Normal ýþýk SIYAH tespiti
            {
                ir_sens_sayisal_deger[x]=SIYAH;
               // hepsi_siyah_say++;
            }
        }

        if((ir_sens_ort_deger_e_on[x]>ir_sens_ort_deger_e_off[x]) //Aþýrý ýþýk altýnda siyah tespiti
               // &&  (ir_sens_esik_deger[x]<350)) //350:1.7V  //deneysel çalýþmaya göre bulundu dolayýsý ile baðýmlý bir durum
               && (son_isik_durumu[x]==ASIRI_ISIK))                                   // adaptif olamaz.
            
        {
            if((ir_sens_ort_deger_e_on[x]<ir_sens_asiri_isik_deger[x]-100)&& ir_sens_ort_deger[x]>0){
                ir_sens_sayisal_deger[x]=SIYAH;     
               // hepsi_siyah_say++;
            }
        }
        son_isik_durumu[x]= ir_sens_isik_durumu[x];  //deðiþimi algýlmak için

//        //Sayýsal durum incelemesi...
//        if(!ir_sens_sayisal_deger[3] && !ir_sens_sayisal_deger[4]){
//            if(ir_sens_sayisal_deger[0] || ir_sens_sayisal_deger[1] || ir_sens_sayisal_deger[2]){
//                ir_sens_sayisal_deger[5]=ir_sens_sayisal_deger[6]=ir_sens_sayisal_deger[7]=BEYAZ;
//            }
//            if(ir_sens_sayisal_deger[5] || ir_sens_sayisal_deger[6] || ir_sens_sayisal_deger[7]){
//                ir_sens_sayisal_deger[0]=ir_sens_sayisal_deger[1]=ir_sens_sayisal_deger[2]=BEYAZ;
//            }
//        }
//        if(ir_sens_sayisal_deger[3] && ir_sens_sayisal_deger[4]){
//            ir_sens_sayisal_deger[0]=ir_sens_sayisal_deger[1]=ir_sens_sayisal_deger[2]=BEYAZ;
//            ir_sens_sayisal_deger[5]=ir_sens_sayisal_deger[6]=ir_sens_sayisal_deger[7]=BEYAZ;
//        }
//        
//        if(hepsi_beyaz_say>=SENS_SAYISI)
//            sens_hepsi_beyaz=1;
//        else sens_hepsi_beyaz=0;
//        
//        if(hepsi_siyah_say>=SENS_SAYISI)
//            sens_hepsi_siyah=1;
//        else sens_hepsi_siyah=0;
        
    }//for
}//

int k_means_max_min_esik() {
    int x = 0, _max_index = 0, _min_index = 0, yineleme = 0;
    unsigned int32 _maxtop = 0, _mintop = 0;
    unsigned int16 son_deger = 0;

    max_esik_ort_deger = 0;
    min_esik_ort_deger = 1023;

    memset(max_esik_veri_havuzu, 0, SENS_SAYISI * 10);
    memset(min_esik_veri_havuzu, 0, SENS_SAYISI * 10);

    do {
        max_esik_ort_deger = 0;
        min_esik_ort_deger = 1023;

        for (x = 0; x < SENS_SAYISI * 10; x++) {

            if (!baslangic_esik_gec) {
                if (k_means_veri_havuzu[x] > baslangic_esik_ort_deger) {
                    max_esik_veri_havuzu[_max_index++] = k_means_veri_havuzu[x];
                }
            }
            if (baslangic_esik_gec) {
                if (k_means_veri_havuzu[x] > k_means_esik_ort_deger) {
                    max_esik_veri_havuzu[_max_index++] = k_means_veri_havuzu[x];

                }
            }

            if (!baslangic_esik_gec) {
                if (k_means_veri_havuzu[x] < baslangic_esik_ort_deger) {
                    min_esik_veri_havuzu[_min_index++] = k_means_veri_havuzu[x];
                }
            }
            if (baslangic_esik_gec) {
                if (k_means_veri_havuzu[x] < k_means_esik_ort_deger) {
                    min_esik_veri_havuzu[_min_index++] = k_means_veri_havuzu[x];
                }
            }

            // fprintf(usbSerial,"%lu,",k_means_veri_havuzu[x]);
            //  fprintf(usbSerial,"%lu,",max_esik_veri_havuzu[_max_index]);
            // fprintf(usbSerial,"%lu,",k_means_veri_havuzu[x]);

        }//for

        // fprintf(usbSerial,"\r\n");

        baslangic_esik_gec = TRUE;

        for (x = 0; x < _max_index; x++) _maxtop += max_esik_veri_havuzu[x];
        for (x = 0; x < _min_index; x++) _mintop += min_esik_veri_havuzu[x];

        max_esik_ort_deger = (unsigned int16) (_maxtop / _max_index);
        min_esik_ort_deger = (unsigned int16) (_mintop / _min_index);
        k_means_esik_ort_deger = (unsigned int16) ((max_esik_ort_deger + min_esik_ort_deger) / 2);

        // fprintf(usbSerial,"indexMm:%Lu,%Lu \t %u,%u\r\n",_maxtop,_mintop,_max_index,_min_index);

        _max_index = _min_index = 0;
        _maxtop = _mintop = 0;

        // memset(max_esik_veri_havuzu,0,SENS_SAYISI*10);
        // memset(min_esik_veri_havuzu,0,SENS_SAYISI*10);

        yineleme++;

        //        fprintf(usbSerial,"K Means: max:%lu\tmin:%lu\tesik=%lu\tyineleme=%u\r\n",
        //                                    max_esik_ort_deger,min_esik_ort_deger,k_means_esik_ort_deger,yineleme);

        if ((k_means_esik_ort_deger == son_deger) || yineleme > 10) {
            k_means_yeni_veri = TRUE;
            break;
        }

        son_deger = k_means_esik_ort_deger;

    } while (!k_means_yeni_veri);

//        fprintf(usbSerial,"K Means: max:%lu\tmin:%lu\tesik=%lu\tyineleme=%u\r\n",
//                        max_esik_ort_deger,min_esik_ort_deger,k_means_esik_ort_deger,yineleme);

    // k_means_sens_sayisal();


    //    disable_interrupts(INT_TIMER0);
    //    do {
    //
    //        for (x = 0; x < SENS_SAYISI; x++) {
    //            fprintf(usbSerial, "%lu,", ir_sens_ort_deger[x]);
    //        }
    //
    //        fprintf(usbSerial, "%lu,%lu,\r", max_esik_ort_deger, min_esik_ort_deger);
    //        delay_ms(100);  //100 ugun deger oldu
    //
    //    } while (!rx_pos);
    //    //seri_pos_led(seri_pos);
    //    rx_pos=0;
    //    enable_interrupts(INT_TIMER0);

    // Fuzzy degerlendirme 

 //   disable_interrupts(INT_TIMER0);
//    
//    dizi_setle_coef1(fis_gMFI0Coeff1, min_esik_ort_deger, max_esik_ort_deger);
//    dizi_setle_coef2(fis_gMFI0Coeff2, min_esik_ort_deger, max_esik_ort_deger);
//
//
//    for (x = 0; x < SENS_SAYISI; x++) {
//
//        g_fisOutput[0] = 0.0;
//        g_fisInput[0] = ir_sens_ort_deger[x];
//
//        fis_evaluate();
//
//        ir_sens_fuzzy_deger[x] = g_fisOutput[0];
//        //fprintf(usbSerial, "%f|", ir_sens_fuzzy_deger[x]);
//    }
    // fprintf(usbSerial, "\r\n");

    // fuzzy_pos_led(ir_sens_fuzzy_deger);

 //   enable_interrupts(INT_TIMER0);
    return yineleme;

}


///önceki fonksiyonlar
//void k_means_veri_havuzu_olustur() {
//    int i = 0;
//
//    k_means_veri_say++;
//
//    if (k_means_veri_say <= 10 && !k_means_yeni_veri) {
//
//        for (i = 0; i < SENS_SAYISI; i++) {
//
//            if (ir_sens_ort_deger_e_on[i] == ir_sens_ort_deger_e_off[i]) //Aþýrý ýþýk, Von=Voff
//            {
//
//                ir_sens_ort_deger[i] = ir_sens_ort_deger_e_on[i];
//                k_means_veri_havuzu[k_means_veri_havuzu_index++] = ir_sens_ort_deger_e_on[i];
//                ir_sens_isik_durumu[i] = ASIRI_ISIK;
//
//            } else if (ir_sens_ort_deger_e_on[i] > ir_sens_ort_deger_e_off[i] + 100) //normal ýþýk, Von>voff
//            {
//                ir_sens_ort_deger[i] = ir_sens_ort_deger_e_on[i] - ir_sens_ort_deger_e_off[i];
//                k_means_veri_havuzu[k_means_veri_havuzu_index++] = ir_sens_ort_deger[i];
//                ir_sens_isik_durumu[i] = NORMAL_ISIK;
//            }
//
//            if (k_means_veri_havuzu_index >= (SENS_SAYISI * 10)) break;
//        }
//    }
//
//    if (k_means_veri_say >= 10 || (k_means_veri_havuzu_index >= (SENS_SAYISI * 10))) {
//        k_means_veri_havuzu_index = k_means_veri_say = 0;
//        k_means_yeni_veri = TRUE;
//    }
//
//
//}
//
