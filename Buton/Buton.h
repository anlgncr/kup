/*
  Buton kontrol Anıl Gençer çalışması için loop fonksiyonu içerisinde calis() fonksiyonu çağırılmalı
*/
#ifndef Buton_h
#define Buton_h

#include "Arduino.h"

class Buton
{
  public:
    Buton(int pin);
    boolean basildi();
	boolean cekildi();
	boolean basili();
	void calis();
	void bitir();
    boolean asagida=false;
	boolean yukarida=true;
	boolean buton=false;
	boolean a=false;
	boolean b=false;
	boolean birKere=true;
    unsigned long cekilenzaman=0;
	unsigned long basilanzaman=0;
	int aralik=200;
  private:
    int _pin;
};

#endif