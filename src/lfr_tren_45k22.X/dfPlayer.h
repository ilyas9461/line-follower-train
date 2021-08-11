/* 
 * File:   dfPlayer.h
 * Author: ilyas
 *
 * Created on 28 Þubat 2018 Çarþamba, 16:11
 */

/*
 *	name:				DFPlayer_Mini_Mp3
 *	version:			1.0
 *	Author:				lisper <lisper.li@dfrobot.com>
 *	Date:				2014-05-22
 *	official website:		http://www.dfrobot.com
 *	Products page:			http://www.dfrobot.com/index.php?route=product/product&product_id=1121#.U5Z_RYbUN8E
 *	Description:			mp3 library for DFPlayer mini board
 *					        note: mp3 file must put into mp3 folder in your tf card
 */

#ifndef DFPLAYER_H
#define	DFPLAYER_H

#ifdef	__cplusplus
extern "C" {
#endif
    typedef unsigned char mbyte;
    
    #define startByte 0x7E
    #define endByte 0xEF
    #define versionByte 0xFF
    #define dataLength 0x06
    #define infoReq 0x01        //0x01
    #define isDebug false
    #define dfPlayerMaxSes 25 // dec30:0x1E
    #define dfPlayerNSes 20 //20

    #define DF_BUSY_PIN  PIN_C5 //PIN_B3

    #define  MP3_SD_RASTGELE_BASLANGIC 30
    #define  MP3_SD_RASTGELE_MAX_SAYI 9

    #define dfPplayerMaxSes() mp3_set_volume (dfPlayerMaxSes);delay_ms(100)//48
    #define dfPlayerNormSes() mp3_set_volume(dfPlayerNSes);delay_ms(100)  //
    #define dfPlayerMinSes() mp3_set_volume(1);delay_ms(100)

    unsigned int8 df_player_ses=15;

    void sendCommand(mbyte Command, mbyte Param1, mbyte Param2);
    void mp3_set_volume (char vol);
    void mp3_set_equalizer(char eq);
    void mp3_play_physical(unsigned int16 fileNum);
    void mp3_get_error ();
    void mp3_single_loop (unsigned int16 fileNum) ;
    void mp3_enableLoopAll();
    void mp3_disbleLoopAll();
    void mp3_getData(*dizi);
    void mp3_DataSeriGonder(*dizi,char len);
    void mp3_stop();
    void mp3_reset();
    void mp3_baslangic();


#ifdef	__cplusplus
}
#endif

#endif	/* DFPLAYER_H */

