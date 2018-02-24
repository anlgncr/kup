#include "Arduino.h"
#include "kup.h"
#include "Zamanlayici.h"
#include <SPI.h>
#include <EEPROM.h>

Zamanlayici zaman(10);
unsigned char dizi_anim1[] = {1,2,4,8,4,2,1,0,0,0,0,0,0,0};
unsigned char dizi_anim2[] = {9,9,9,9,0,0,0,0,0,0,0,0,0,0};
unsigned char dizi_anim3[] = {9,6,6,9,0,0,0,0,0,0,0,0,0,0};


kup::kup(){
	SPI.begin();
	randomSeed(analogRead(0));
	
	temizle();
	hepsiniYak();
	char i;
	for(i=0;i<16;i++)
		rast_led[i]=i; 
	
	parlaklik = EEPROM.read(0);	
}

void kup::calis(){
	zaman.calis();
	
	if(zaman.tik())
		animOynat();
		
  
  veriGonder();
}

void kup::animOynat(){
	if(anim_degisti)
			animAyarla(anim_no);
		
		switch(anim_no){
			case 0:anim1(false);break;
			case 1:anim1(true);	break;
			case 2:anim2();		break;
			case 3:anim3();		break;
			case 4:anim4();		break;
			case 5:anim5();		break;
			case 6:anim6();		break;
			case 7:anim7();		break;
			case 8:anim8();		break;
			case 9:anim9();		break;
			case 10:anim10();	break;
			case 11:kayan_anim();	break;
			case 12:anim12();	break;
			case 13:kayan_anim(); break;
			case 14:kayan_anim(); break;
			case 15:anim15();break;
			case 16:anim16();break;
		}
}

void kup::setParlaklik(unsigned char _parlaklik){
	unsigned char p = 32;
	switch(_parlaklik){
		case 0: p = 1; 	break;
		case 1: p = 2; 	break;
		case 2: p = 4; 	break;
		case 3: p = 8; 	break;
		case 4: p = 16; break;
		case 5: p = 32; break;
		case 6: p = 64; break;
		case 7: p = 128; break;
		case 8: p = 255;
	}
	EEPROM.write(0, p);
	parlaklik = p;
}

unsigned char kup::getParlaklik(){
	unsigned char p = 5;
	switch(parlaklik){
		case 1: 	p = 0; 	break;
		case 2: 	p = 1; 	break;
		case 4: 	p = 2; 	break;
		case 8: 	p = 3; 	break;
		case 16: 	p = 4; 	break;
		case 32: 	p = 5; 	break;
		case 64: 	p = 6; 	break;
		case 128: 	p = 7; 	break;
		case 255: 	p = 8; 	
	}
	return p;
}

void kup::setAnim(char _anim_no){
	anim_no = _anim_no;
	anim_degisti = true;
}

void kup::animAyarla(char _anim_no){
	temizle();
	switch(_anim_no){
		case 0:	char i;
				for(i=0;i<16;i++) // Rastgele led için 0'dan 15'e kadar diziyi doldur
					rast_led[i]=i; 
				
				katmanYak(4); // En üst katmanı yak
				kaydirma_yon = -1; // Kaydırma yönü aşağı 
				kaydirma_sayac = 0; // Her bir kaydırışı tutan sayaç
				kayan_led = 0; // Kayan sütun
				break;	
		case 1:	katmanYak(1);
				kaydirma_yon = 1;
				kaydirma_sayac = 0;
				kayan_led = 0;
				dizi_kar(rast_led,16);
				break;
		case 2: katmanYak(4);
				kaydirma_sayac = 0;
				kaydirma_yon = -1;
				zaman.zamanAyarla(30);
				break;
		case 3: katmanXYak(1);
				kaydirma_sayac = 0;
				kaydirma_yon = 1;
				zaman.zamanAyarla(30);
				break;
		case 4: katmanZYak(1);
				kaydirma_sayac = 0;
				kaydirma_yon = 1;
				zaman.zamanAyarla(30);
				break;
		case 5: zaman.zamanAyarla(50);
				break;
		case 6: kaydirma_yon = 1;
				zaman.zamanAyarla(300);
				break;
		case 7: zaman.zamanAyarla(1);
				break;
		case 8: kaydirma_sayac = 0;
				zaman.zamanAyarla(200);
				break;
		case 9: kaydirma_sayac = 0;
				zaman.zamanAyarla(100);
				break;
		case 10:kaydirma_sayac = 0;
				zaman.zamanAyarla(20);
				break;
		case 11:zaman.zamanAyarla(70);
				kayan_dizi_kopyala(dizi_anim1);
				break;
		case 12:zaman.zamanAyarla(200);
				kaydirma_sayac = 0;	
				kaydirma_yon = 1;
				break;
		case 13:zaman.zamanAyarla(70);
				kayan_dizi_kopyala(dizi_anim2);
				break;
		case 14:zaman.zamanAyarla(70);
				kayan_dizi_kopyala(dizi_anim3);
				break;
		case 15:zaman.zamanAyarla(100);
				kaydirma_sayac = 0;
				break;
		case 16:zaman.zamanAyarla(75);
				kaydirma_sayac = 0;
				break;
	}
	
	anim_degisti = false;
}

void kup::kayan_dizi_kopyala(char * dizi){
	kaydirma_sayac = 0;	
	char i;
	for(i=0; i<14; i++)
		kayan_dizi[i] = dizi[i];
}

/* 	Kaydırma sayacı başlangıçta sıfır, yön önceden belirleniyor
	Kaydırma işlemi için en üst katman yakıldı 3 kere aşağı kayıp yukarı çıkacak*/
void kup::anim1(bool karistir){
	
	if(karistir){				//	Eğer sütünlar karışık aşağı yukarı inip çıkacaksa
		if(kaydirma_yon==-1)	//	Sütunlar aşağı kayacaksa
		zaman.zamanAyarla(3);	//	5 ms' de aşağı insinler
		else
		zaman.zamanAyarla(10);	//	Sütunlar yukarı çıkacaksa 10 ms' de çıksınlar
	}else{
		zaman.zamanAyarla(5);	// 	Sütunlar düzenli inip çıkıyorsa 5 ms' de inip çıksınlar
	}

	sutunKaydir(rast_led[kayan_led] /4, rast_led[kayan_led] %4, kaydirma_yon); // Sütunu bir kere kaydır
	kaydirma_sayac ++; 			// 	Her kaydırma için sayacı bir arttır
	
	if(kaydirma_sayac>2){ 		// 	Sütun 3 kere aşağı veya yukarı kaydıysa 
	  kayan_led ++;				// 	Sıradaki sütuna geç
	  kaydirma_sayac = 0;		//	Kaydırma sayacını sıfırla
	}
	if(kayan_led > 15){			// 	Tüm sütunlar kaydıysa 
	  kayan_led = 0;			//	Kayacak sütunu sıfırla başa dön
	  kaydirma_yon *= -1;		//	Kaydırma yönünü değiştir
	  
	  if(karistir){				// 	Eğer sütünlar karışık aşağı yukarı inip çıkacaksa
		dizi_kar(rast_led,16);		//	Sütunları karıştır
		zaman.zamanAyarla(150);	//	0.2 saniye beklet
	  }else{
		zaman.zamanAyarla(20);	//	Eğer düzenli inip çıkıyorsa 0.02 saniye baklet
	  }
	}
}

void kup::anim2(){
	katmanKaydir(kaydirma_yon);
	zaman.zamanAyarla(30);
	kaydirma_sayac++;
	if(kaydirma_sayac>2){
		kaydirma_sayac = 0;
		kaydirma_yon *= -1;
		zaman.zamanAyarla(150);
	}
}

void kup::anim3(){
	xKatmanKaydir(kaydirma_yon);
	zaman.zamanAyarla(30);
	kaydirma_sayac++;
	if(kaydirma_sayac>2){
		kaydirma_sayac = 0;
		kaydirma_yon *= -1;
		zaman.zamanAyarla(150);
	}
}

void kup::anim4(){
	zKatmanKaydir(kaydirma_yon);
	zaman.zamanAyarla(30);
	kaydirma_sayac++;
	if(kaydirma_sayac>2){
		kaydirma_sayac = 0;
		kaydirma_yon *= -1;
		zaman.zamanAyarla(150);
		if(kaydirma_yon == -1)
			katmanZYak(4);
		else
			katmanZYak(1);
	}
}

void kup::anim5(){
    if (random(10) < 6) {
      bellek[5][1] = random(16);
      bellek[5][2] = random(16);
      bellek[5][3] = random(16);
      bellek[5][4] = random(16);
	}
	katmanKaydir(-1);
}

void kup::anim6(){
	temizle();
	if(kaydirma_yon == -1){
		katmanYak(4);
		katmanYak(1);
	}
	else{
		katmanYak(2);
		katmanYak(3);
	}	
	kaydirma_yon *= -1;
}

void kup::anim7(){
	ledToggle(random(1,5),random(1,5),random(1,5));
}

void kup::anim8(){
	temizle();
	switch(kaydirma_sayac){
		case 0:katmanXYak(1);	break;
		case 1:katmanZYak(1);	break;
		case 2:katmanXYak(4);	break;
		case 3:katmanZYak(4);	break;
	}
	kaydirma_sayac ++;
	kaydirma_sayac %=4;
}

void kup::anim9(){
	char i;	
	temizle();
	zaman.zamanAyarla(75);
	switch(kaydirma_sayac){
		case 0:	katmanXYak(1);	break;
		case 1:	for(i=1; i<5; i++)	//	Çapraz yak
					bellek[i][i] = 0x0f;
		break;
		case 2:	katmanYak(1);	break;
		case 3:	for(i=1; i<5; i++)	//	Ters Çapraz yak
					bellek[i][5-i] = 0x0f;
		break;
		case 4:	katmanXYak(4);	break;
		case 5:	for(i=1; i<5; i++)	//	Çapraz yak
					bellek[i][i] = 0x0f;
		break;
		case 6: katmanYak(4);	break;
		case 7: for(i=1; i<5; i++)	//	Ters Çapraz yak
					bellek[i][5-i] = 0x0f;
		break;
	}
	
	if(kaydirma_sayac == 0 || kaydirma_sayac == 2 || kaydirma_sayac == 4 || kaydirma_sayac == 6)
		zaman.zamanAyarla(250);
	
	kaydirma_sayac ++;
	kaydirma_sayac %=8;
	
}

void kup::anim10(){
	
	switch(kaydirma_sayac){
		case 0:	ledYak(random(1,5), random(1,5), random(1,5));
				zaman.zamanAyarla(20);
		break;
		case 1:	temizle();
				zaman.zamanAyarla(100);
	}
	
	kaydirma_sayac ++;
	kaydirma_sayac %=2;
}


void kup::kayan_anim(){	
	char y;
	for(y=0; y<4; y++)
		if(1 & (kayan_dizi[kaydirma_sayac]>>y))
			bellek[y+1][0] = 0x08;
		else	
			bellek[y+1][0] = 0;
		
	zKaydir(1, 1);
	xKaydir(1, -1);
	zKaydir(4, -1);
	xKaydir(4, 1);
	
	kaydirma_sayac++;
	kaydirma_sayac%=13;
}

void kup::anim12(){
	temizle();
	if(kaydirma_yon == -1){
		ledYak(2,2,2);
		ledYak(2,2,3);
		ledYak(2,3,2);
		ledYak(2,3,3);
		ledYak(3,2,2);
		ledYak(3,2,3);
		ledYak(3,3,2);
		ledYak(3,3,3);
	}
	else{
		ledYak(1,1,1);
		ledYak(1,1,4);
		ledYak(4,1,1);
		ledYak(4,1,4);
		ledYak(1,4,1);
		ledYak(1,4,4);
		ledYak(4,4,1);
		ledYak(4,4,4);
	}	
	kaydirma_yon *= -1;
}

void kup::anim15(){
	char i;	
	temizle();

	switch(kaydirma_sayac){
		case 0:	katmanXYak(2);	
				katmanXYak(3);
				break;
		
		case 1:	for(i=1; i<5; i++)	//	Çapraz yak
					bellek[i][i] = 0x0f;
				break;
				
		case 2:	katmanYak(2);
				katmanYak(3);		
				break;
				
		case 3:	for(i=1; i<5; i++)	//	Ters Çapraz yak
					bellek[i][5-i] = 0x0f;
		break;
	}
	
	kaydirma_sayac ++;
	kaydirma_sayac %=4;	
}

void kup::anim16(){
	char x,y;	
	temizle();

	switch(kaydirma_sayac){
		case 0:	katmanXYak(2);	
				katmanXYak(3);
				break;
		
		case 1:	for(y=1; y<5; y++)
					for(x=1; x<5; x++)	//	Çapraz yak
						bellek[y][x] = 1<<(x-1);
				break;
				
		case 2:	katmanZYak(2);
				katmanZYak(3);		
				break;
				
		case 3:	for(y=1; y<5; y++)
					for(x=4; x>0; x--)	//	Çapraz yak
						bellek[y][x] = 1<<(4-x);
				break;
		break;
	}
	
	kaydirma_sayac ++;
	kaydirma_sayac %=4;	
}

void kup::dur(){
	zaman.dur();
	hepsiniYak();
}

void kup::devam(){
	zaman.devam();
	anim_degisti = true;
}

void kup::veriGonder(){
	char i, j;
	unsigned char veri = 0;

  for (i = 0; i < 4; i++) {
    PORTD |= 0x80; // REGISTER çıkışı inaktif
    for (j = 0; j < 4; j++) {
      veri = i << 6; // Katman bitlerini 11000000 6 sola kaydır
      veri |= j << 4; // Register bitlerini 00110000 4 sola kaydır
      veri |= bellek[i+1][j+1]; // 00001111 veriyi ekle

      SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
      PORTB &= ~0x03; // Register(0,1,2,3) ve Shift Reg Storage -> LOW
      SPI.transfer(veri);
      PORTB |= 0x01; // Shift Reg Storage ->HIGH
      PORTB |= 0x02; // REGISTER(0,1,2,3) clock -> HIGH
      SPI.endTransaction();
    }
    PORTD &= ~0x80; // REGISTER çıkışı aktif
    delayMicroseconds(parlaklik);
  }
  PORTD |= 0x80; // REGISTER çıkışı inaktif
}


void kup::katmanXYak(char x){
	char i;
	for(i=0; i<6; i++)
		bellek[i][x] = 0x0f;
}

void kup::xKatmanKaydir(char yon){
	char i,j;
	if (yon == -1){ 			//<--- Sola kaydır
		for(i=0; i<6; i++) 	//Sanal olarak 6 katman var (2 buffer)
			for(j=0; j<5; j++)
				bellek[i][j] = bellek[i][j+1];
			
		for(i=0; i<6; i++)
			bellek[i][5] = 0;
	}
	else if(yon == 1){			//---> Sağa kaydir
		for(i=0; i<6; i++) 	//Sanal olarak 6 katman var (2 buffer)
			for(j=5; j>0; j--)
				bellek[i][j] = bellek[i][j-1];
			
		for(i=0; i<6; i++)
			bellek[i][0] = 0;
	}
}

void kup::katmanZYak(char z){
	z-=1; // Diğerleri gibi bu da 1 den baslasın diye 
	char i,j;
	for(i=0; i<6; i++)
		for(j=0; j<6; j++)
			bellek[i][j] |= 1<<z; 
}

void kup::zKatmanKaydir(char yon){
	char i,j;
	if(yon == -1){ 		// İleri kaydır
		for(i=0; i<6; i++)
			for(j=0; j<6; j++)
				bellek[i][j] = ((bellek[i][j]>>1)<0)?0:bellek[i][j]>>1;
	}
	else if(yon == 1){	// Geri kaydır
		for(i=0; i<6; i++)
			for(j=0; j<6; j++)
				bellek[i][j] = ((bellek[i][j]<<1)>0x0f)?0:bellek[i][j]<<1;
	}
}

void kup::sutunKaydir(char x, char z, char yon){
  char i;
  z = 1<<z;
  if(yon == -1){ //asagi kaydir
    for(i=0; i<5; i++){
      bellek[i][x+1] |= bellek[i+1][x+1] & z;
      bellek[i+1][x+1] &= ~z;
    }
  }
  else if(yon == 1){ // yukarı kaydır
     for(i=5; i>0; i--){
      bellek[i][x+1] |= bellek[i-1][x+1] & z;
      bellek[i-1][x+1] &= ~z;
    }
  }
}

void kup::ledYak(byte x, byte y, byte z) {
	z-=1;
	bellek[y][x] |= 1 << z;
}

void kup::ledSondur(char x, char y, char z){
	z-=1;
	/*if(!menzilde(x,y,z))
		return;*/
	
	bellek[y][x]&=~(1<<z);
}

void kup::ledToggle(char x, char y, char z){
	if(yaniyor(x,y,z))
		ledSondur(x,y,z);
	else
		ledYak(x,y,z);
}

bool kup::yaniyor(char x, char y, char z){
	/*if(!menzilde(x,y,z))
		return false;*/
	z-=1;
	return bellek[y][x]&(1<<z);
}

bool kup::menzilde(char x, char y, char z){
	z-=1;
	return (x<6 && x>-1 && y<6 && y>-1 && z<4 && z>-1)?true:false;
}

void kup::katmanYak(byte k){
  byte i;
  for(i=0; i<6; i++)
    bellek[k][i] = 0x0f;
}

void kup::hepsiniYak() {
  byte i, j;
  for (i = 1; i < 5; i++)
    for (j = 1; j < 5; j++)
      bellek[i][j] = 0x0f;
}

void kup::temizle() {
  byte i, j;
  for (i = 0; i < 6; i++)
    for (j = 0; j < 6; j++)
      bellek[i][j] = 0;
}

void kup::katmanKaydir(char yon) {
  char i, j;

  if (yon == -1) { //asagi kaydir
    for (i = 0; i < 5; i++)
      for (j = 0; j < 6; j++)
        bellek[i][j] = bellek[i + 1][j];
         
    for (i = 0; i < 6; i++)
    bellek[5][i] = 0;
  }
  else if (yon == 1) { // yukarı kaydır
    for (i = 5; i > 0; i--)
      for (j = 0; j < 6; j++)
        bellek[i][j] = bellek[i - 1][j];
        
    for (i = 0; i < 6; i++)
    bellek[0][i] = 0;
  }
}

void kup::dizi_kar(char *dizi, char boyut){
  char i, temp, rast;
  for(i=0; i<boyut; i++){
    rast = random(boyut);
    temp = dizi[i];
    dizi[i] = dizi[rast];
    dizi[rast] = temp;
  } 
}

void kup::xKaydir(char z, char yon){
	z -=1;
	char x,y;
	if(yon == 1){
		for(y=1; y<5; y++) // Sağa kaydır
			for(x=4; x>0; x--){
				if(bellek[y][x-1] & (1<<z)) // Her katmandaki bitler teker teker kaydırlıyor
					bellek[y][x] |= 1<<z;
				else
					bellek[y][x] &= ~(1<<z);
			}
	}
	else if(yon == -1){
		for(y=1; y<5; y++)	//Sola kaydır
			for(x=1; x<4; x++)
				if(bellek[y][x+1] & (1<<z))
					bellek[y][x] |= 1<<z;
				else
					bellek[y][x] &= ~(1<<z);
	}
}

void kup::zKaydir(char x, char yon){
	char y;
	if(yon == 1){
		for(y=0; y<5; y++)	// Saga kaydır
			bellek[y][x] = (((bellek[y][x]<<1)&0x07) | (bellek[y][x]&0x08)) & 0x0f;
	}
	else if(yon == -1){
		for(y=0; y<5; y++)	// Sola kaydır
			bellek[y][x] = (bellek[y][x]>>1);
	}
}


















