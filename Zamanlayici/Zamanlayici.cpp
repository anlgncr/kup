/*
  Zaman kontrol Anýl Gençer
*/

#include "Arduino.h"
#include "Zamanlayici.h"

Zamanlayici::Zamanlayici(unsigned long aralik)
{
  _aralik = aralik;
  aktif=false;
  sonZaman=aralik;
}

void Zamanlayici::calis()
{
	if(millis()-sonZaman>=_aralik){
		sonZaman=millis();
		aktif=true;
	}
}

boolean Zamanlayici::tik()
{
	if(!_devam)
		return false;
	
	if(aktif){
		aktif=false;
		return true;
	}
	return false;
}

void Zamanlayici::zamanAyarla(unsigned long aralik)
{
	sonZaman=millis();
	_aralik=aralik;
}

void Zamanlayici::dur(){
	_devam = false;
}

void Zamanlayici::devam(){
	_devam = true;
}



