#include <EEPROM.h>
#include <Buton.h>
#include <kup.h>
#include <Zamanlayici.h>

Buton buton_sol(6);
Buton buton_sag(5);

kup led_kup;
Zamanlayici timer(2000);

char rast_anim[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
char anim_sayisi=16;
unsigned char parlaklik = 0;
bool otomatik_gecis = false;
bool karistir = false;

void setup() {
  DDRB = DDRB | 0x03;
  DDRD = DDRD | 0x80;
  randomSeed(analogRead(0));
  otomatik_gecis = EEPROM.read(1);
  parlaklik = led_kup.getParlaklik();
  
  led_kup.dizi_kar(rast_anim,anim_sayisi+1);
  led_kup.setAnim(rast_anim[0]);
  
//led_kup.setAnim(16);
/*Serial.begin(9600);
char i;
for(i=0;i<7;i++)
  Serial.print(rast_anim[i],DEC); */
}


char animasyon=0;
bool parlaklik_ayari = false;

void loop() {

  buton_sol.calis();
  buton_sag.calis();
   
    if(buton_sol.basili()){
      if(parlaklik_ayari){
        parlaklik_ayari = false;
        led_kup.devam();
      }
      else{
        parlaklik_ayari = true;
        led_kup.dur();
      }
    }

     if(!otomatik_gecis && !parlaklik_ayari){
     if(buton_sag.basildi())
        siradakiAnim(1); 
     else if(buton_sol.basildi())
        siradakiAnim(0);
     }

    if(parlaklik_ayari){
      if(buton_sag.cekildi()){
       parlaklik= (parlaklik+1)>8?8:parlaklik+1;
       led_kup.setParlaklik(parlaklik);
      }
      else if(buton_sol.cekildi()){
        parlaklik= (parlaklik-1)<0?0:parlaklik-1;
        led_kup.setParlaklik(parlaklik); 
      }
    }
    
  if(buton_sag.basili() && !parlaklik_ayari){
      otomatik_gecis = !otomatik_gecis;
      EEPROM.write(1, otomatik_gecis);  
    buton_sag.bitir();
  }

        
  if(otomatik_gecis)
    timer.calis();
   
    
  if(timer.tik() && otomatik_gecis){
    siradakiAnim(1);    
    timer.zamanAyarla(random(3000, 6000));
  }

  
  led_kup.calis(); // Bu gerekli ÃƒÆ’Ã‚Â¶n iÃƒâ€¦Ã…Â¸lemler yapÃƒâ€�Ã‚Â±ldÃƒâ€�Ã‚Â±ktan sonra calÃƒâ€�Ã‚Â±Ãƒâ€¦Ã…Â¸tÃƒâ€�Ã‚Â±rÃƒâ€�Ã‚Â±lacak
}



void siradakiAnim(char yon){
  if(yon)
   animasyon = (animasyon+1)>anim_sayisi?0:animasyon+1;
  else
   animasyon = (animasyon-1)<0?anim_sayisi:animasyon-1;
   
   led_kup.setAnim(rast_anim[animasyon]);
  
  if(animasyon>anim_sayisi-1 && otomatik_gecis)
    led_kup.dizi_kar(rast_anim,anim_sayisi+1);
}






