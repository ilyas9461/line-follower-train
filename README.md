<h1 align="center">Çizgi İzleyen Tren / Tırtıl</h1>

<p  align="center">
    <img src="./img/tren1.png" width="30%" height="30%" border: 5px solid #555 >

</p>

## İçerik

- [Giriş](#Giriş)
- [Çalışma Şekli](#Çalışma-Şekli)
- [Kullanılan Teknolojiler](#Kullanılan-Teknolojiler)
- [İletişim](#İletişim)

## Giriş



 Alış veriş merkezlerinde veya kapalı bir alanda yere çizilen çigileri takip ederek vagonlara binen çocuklara gezdiren otonom bir araçtır. Sensör sisteminden dolayı iç mekan kullanımına daha uygundur. Personele verilen kumanda ile çeşitli fonksiyonları uzaktan kontrol edilebilir.  
 

## Çalışma Şekli

Mikrodenetleyici tabanlı gömülü sistem mimarisine uygun olarak tasarlamış bir elektronik devre tarafından kontrolü gerçekleştirilir. Sensörlerden gelen konum bilgisine göre mikrodenetleyici PID kontrol sistemi ile motorlara verilecek güç miktarını hesaplar ve motorlara gönderir. İki adet dc motor kullanılmıştır. Çizgiden çıkma durumlarında araç ototmatik olarak durur. 

<br>
<p  align="center">
    <img src="./pcb/cizgi_izleyen_tren_pcb.png" width="80%" height="75%" border: 5px solid #555 >
</p>
<p  align="center"> Ana kart.</p>

PCB üzerinde bulunan 8 adt led ile sensörlerin durumları gözlenebilir. 
Araç iki adet 12V 'luk akü ile beslenmektedir. Çalışma gerilimi 24 V 'tur. SD karta yüklenmiş müzik parçalarının çalarak gezintiyi daha eğlenceli hale getirir.

<br>
<p  align="center">
    <img src="./pcb/cizgi_izleyen_tren_sens_pcb.png" width="80%" height="75%" border: 5px solid #555 >
</p>
<p  align="center"> Sensör devresi.</p>



## Kullanılan Teknolojiler

```bash
- PIC18F45K22 mikrodenetleyici.
- CCS PIC C Derleyici
- Çizgi algılama
- PID motor kontrolü.
- Sensör verilerin filtrelenmesi.
- PCB devre şeması çizme ve oluşturma.

```

## Örnek çalışma videosu :

[![](https://camo.githubusercontent.com/241d4106ff5edca2ee25e04dcf4546fad9d20b626f7a10990307e8f83e95459f/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f796f75747562652d2532334646303030302e7376673f267374796c653d666f722d7468652d6261646765266c6f676f3d796f7574756265266c6f676f436f6c6f723d7768697465253232)](https://youtu.be/0rQU3r0k7vE)

## İletişim

- GitHub [@your-ilyas9461](https://github.com/ilyas9461)
- Linkedin [@your-linkedin](https://www.linkedin.com/in/ilyas-yağcioğlu-6a6b17217)

### *** Not : 
Devre endüstriyel olarak üretimde olduğu için PCB devre şemaları paylaşılmamıştır.
