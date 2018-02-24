/*
  Buton kontrol Anıl Gençer
*/

#include "Arduino.h"
#include "Buton.h"

Buton::Buton(int pin){
  pinMode(pin, INPUT);
  _pin = pin;
}

void Buton::calis()
{
	buton = digitalRead(_pin); 
	
	if(buton){
		basilanzaman=millis();//basili olduğu sürece zamani eşitle ki çektikten 40ms sonra çalışsın
		if(yukarida && (millis()-cekilenzaman)>40){
			a=true;//basildi
			aralik=1000;
			yukarida=false;
			asagida=true;//butona basildi, aşağıda, temas var
		}
	}	
	else{
		cekilenzaman=millis();//çekili olduğu sürece zamanı eşitle ki bastıktan 50ms sonra çalışsın
		if(asagida && (millis()-basilanzaman)>40){
			b=true;
			yukarida=true;
			asagida=false;
			birKere = true;
		}
	}
}

void Buton::bitir(){
	a = false;
	b = false;	
}

boolean Buton::basildi(){
	if(a){
		a=false;
		return true;
	}
	return false;
}

boolean Buton::cekildi(){
	if(b){
		b=false;
		cekilenzaman=millis();
		return true;
	}
	return false;
}

boolean Buton::basili()
{
	if(!buton){
	  return false;
	}
	
	if(birKere){
		if((millis()-cekilenzaman)>aralik){
			/*if(aralik>=600){
				aralik-=200;
			}
			else if(aralik<=400 && aralik>200){
				aralik-=50;
			}
			else if(aralik<=200 && aralik>150){
				aralik-=25;
			}*/
			
			//cekilenzaman=millis();
			birKere = false;
			return true;
		}
	}
return false;
}



