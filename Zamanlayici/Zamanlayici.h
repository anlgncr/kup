/*
  Zaman kontrol Anýl Gençer 
*/
#ifndef Zamanlayici_h
#define Zamanlayici_h

#include "Arduino.h"

class Zamanlayici
{
  public:
    Zamanlayici(unsigned long);
	void calis();
	boolean tik(void);
	void zamanAyarla(unsigned long);
	void dur();
	void devam();
  private:
    unsigned long _aralik;
	boolean aktif;
	unsigned long sonZaman;
	bool _devam = true;
};

#endif