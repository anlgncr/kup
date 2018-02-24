#ifndef kup_h
#define kup_h

#include "Arduino.h"


class kup
{
  public:
	kup();
	void calis();
	void ledYak(byte, byte, byte);
	void katmanYak(byte);
	void hepsiniYak();
	void temizle();
	void katmanKaydir(char);
	void sutunKaydir(char, char, char);
	void setAnim(char);
	void dizi_kar(char *, char);
	void setParlaklik(unsigned char);
	unsigned char getParlaklik();
	void dur();
	void devam();
	void animOynat();
		
  private:
	void kayan_dizi_kopyala(char*);
	void ledSondur(char, char, char);
	void ledToggle(char, char, char);
	bool yaniyor(char, char, char);
	bool menzilde(char, char, char);
	char bellek[6][6];
    char kaydirma_sayac;
	char kayan_led;
	char kaydirma_yon;
	char rast_led[16];
	void veriGonder();
	void anim1(bool);
	void anim2();
	void anim3();
	void anim4();
	void anim5();
	void anim6();
	void anim7();
	void anim8();
	void anim9();
	void anim10();
	void kayan_anim();
	void anim12();
	void anim15();
	void anim16();
	void animAyarla(char);
	char anim_no = 0;
	bool anim_degisti = true;
	void katmanXYak(char);
	void katmanZYak(char);
	void xKatmanKaydir(char);
	void zKatmanKaydir(char);
	void xKaydir(char, char);
	void zKaydir(char, char);
	unsigned char parlaklik;
	unsigned int anim_hiz;
	unsigned char kayan_dizi[14];
	
};

#endif