#include "tanimlamalar.h"

unsigned int16 tic,o_tic;  // Türev hesabý için hata deðiþim süresini belirler, sayar. 
                          
//signed int16 sonHata;  // 
//float kp=0.8f;
signed int16 sonTurev;
//signed int16 hataDeg;
float kd;

signed int16 pdKontrol(signed int16 err, signed int16 deg_err,signed int16 son_err, float kp,float kd)
{
   
    //kd=kp/8;
    //hata=orta-pos;//orta referans degeridir yani set edilmesi gereken deger.
    signed int16 oransal=(kp*err);
    signed int16 turev=0;
    
    if (err == son_err)  //hata ayný ise deðiþim süresini say.
    {
      tic = tic + 1;
      if(tic > 60000){tic = 60000;}
      //if(tic > o_tic){turev = sonTurev/(tic);}
      //if(tic > o_tic){turev = sonTurev/(tic*0.001f);}
      if(tic > o_tic)  // son hata deðiþim süresi geçildi ise yeni hesaplamayý son türeve göre yap.
      {
         // turev = (sonTurev*100)/tic;  //2.5mS dt *400  1mS dt*1000 10mS dt*100
          turev = (sonTurev*(1000/40))/tic; //40mS
      }
      //if(tic > tic_old){errord = (errord_old*tic_old)/tic;}
    }
    else  // yeni hata oluþtu ise, yeni türev hesapla.
    {
      tic++;
      //printf("%lu\n",tic);
      //printf("%d\n",deg_e);
      //turev=kd*(hataDeg)/(tic);
      //turev=kd*(deg_e)/(tic*0.001f);
      //turev=(kd*deg_err*100)/(tic);//2.5mS dt
      //turev=kd*(hataDeg)/1;
      turev=(kd*deg_err*(1000/40))/(tic); //40mS
      sonTurev= turev;
      o_tic=tic;
      tic=0;
      //printf("%d\n",turev+oransal);
    }  
    
    //turev=kd*(hata-sonHata);
    //sonHata=hata;;
    //sonTurev=turev;
    
    return (signed int16)(oransal+turev);
}

//int1 hepsiSiyahBeyaz(unsigned int16 *sDig){
//    int i=0;
//    int sb=0,digSB=0;
//    
//    for (i = 0; i < 5; i++)
//    {
//        if(sDig[i]==1){
//            digSB|=(1<<i);   //i. inci biti 1 yapar
//        }else if(senDig[i]==1){
//            digSB|=~(1<<i);  //i. inci biti sýfýr yapar
//        }
//    }
//    //printf("%u\n",digSB); 
//    if(digSB==0b11111111){
//       //sb=hepsiSiyah; 
//    }else if(digSB==0b00000000){
//        //sb=hepsiBeyaz;
//    }else {
//         //sb=enAzBiriSB;
//        
//         //if((digSB==0b00000100) || (digSB==0b00001110)) sb=ortaNokta;
////        if((digSB==0b00111111)||(digSB==0b00011111)||(digSB==0b00011111)){
////            sb=sagDoksan;
////            //printf("sagD\n");    
////        }
////         if((digSB==0b11111100)||(digSB==0b11111000)||(digSB==0b11111000)){
////            sb=solDoksan;
////            //printf("solD\n"); 
////        }
//    }
//                            //en az biri siyah veya beyaz
//    return sb;
//}
//
//void solDoksanDon(){
// 
//}
//void sagDoksanDon(){
// 
//}
