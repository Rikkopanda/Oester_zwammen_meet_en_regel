/*
 *  Auteur : Daan van Oppenraaij
 *  Datum  : 2 - 6 - 2021
 *  Note   : v1.1 - changing the timerRepeat so it actually works
 *           v1.2 - enable the alarms instead of only disabling them.
 *                - scrap enable/disable, and use free to delete(?) timers
 *           v1.3 - using free with timers unit 1 disables timers unit 2 and vice versa for some reason, free timers before starting  
 *           v1.4 - better emergency stop button (no ghosts)
 *           v1.5 - reset instead of emergency stop 
 *           v1.6 - air and humidifier (now with git) v2 will be the final version for air + hum
 *           v1.7 - air and humidifier screens basics are finished 
 *           V1.8 - air and humidifier timers first version (not tested)
 *           
 *          02/06 - air and humidifier
*/

#include <stdint.h>
#include <TFTv2.h>
#include <SeeedTouchScreen.h>
#include <TimeAlarms.h>

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3
#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

#define MAX_PERIODES 10
#define STND_MIST 15
#define STND_LUCHT 10
#define STND_UIT 0
#define STND_HRHL 5
#define SEC 60

#define RELAY_1 22
#define RELAY_2 24
#define RELAY_AIR_IN 26
#define RELAY_AIR_OUT 28

#define ID_SIZE 2

TouchScreen ts = TouchScreen(XP, YP, XM, YM);
int backlight ;

// Make this u_int16
bool scherm;
bool menu;
bool beginn;
bool mist;
bool lucht;
bool uit;
bool herhaal;
bool start;
bool startTimer;
bool startEenmaal;
bool reset;
bool info;

int totaleTijd;
int huidigePeriode;
int invoerPeriodes;
int invoerMist[MAX_PERIODES], invoerLucht[MAX_PERIODES], invoerUit[MAX_PERIODES], invoerHerhaal[MAX_PERIODES];
time_t check;

AlarmID_t id[ID_SIZE];
bool nieuwMist = true;
int humSec = 6,
    fanSec = 10;

void setup() {
  ts = TouchScreen(XP, YP, XM, YM);
  backlight = 7;

  totaleTijd = 0;
  huidigePeriode = 1;
  invoerPeriodes = 2;
  check = 0;
  scherm = false;
  menu = false;
  beginn = false;
  mist = false;
  lucht = false;
  uit = false;
  herhaal = false;
  start = false;
  startTimer = false;
  startEenmaal = true;
  reset = false;
  info = false;
  
  for(int i = 0; i < MAX_PERIODES; i++)
  {
    invoerMist[i] = STND_MIST;
    invoerLucht[i] = STND_LUCHT;
    invoerUit[i] = STND_UIT;
    invoerHerhaal[i] = STND_HRHL;
  }
      
  pinMode(RELAY_AIR_IN, OUTPUT);
  digitalWrite(RELAY_AIR_IN, HIGH);
  pinMode(RELAY_AIR_OUT, OUTPUT);
  digitalWrite(RELAY_AIR_OUT, HIGH);
  pinMode(RELAY_1, OUTPUT);
  digitalWrite(RELAY_1, LOW);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_2, LOW);
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, LOW);
  
  setTime(0,0,0,1,0,0);
  Tft.TFTinit();  // init TFT library
  Tft.setDisplayDirect(DOWN2UP);
}

void loop() {
  //menuScreen();
  Alarm.delay(1);
  Point p = ts.getPoint();
    
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

  if (p.z > __PRESURE && !scherm){
     digitalWrite(backlight, HIGH);
     menu = menuScreen();
     scherm = true;
  }
  //------- -----------------------------------menu-------------------------------------------------------------------------
  else if(p.z > __PRESURE && menu ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 57 && p.x <= 178){
      if(startTimer)
      {
          start = startScreen(invoerPeriodes, invoerMist, invoerLucht, invoerUit, invoerHerhaal);
          invoerStartScreen();
      }
      else
        beginn = beginScreen(invoerPeriodes);
      menu = false;
    } 
    
    else if(p.y >= 140 && p.y <= 320 && p.x >= 179 && p.x <= 239){
      scherm = schermUitScreen();
      menu = false;
    }
    else if(p.y >= 0 && p.y <= 139 && p.x >= 57 && p.x <= 178){
      
      // Set all var to original values
      reset = true;
    }
    else if(p.y >= 0 && p.y <= 139 && p.x >= 179 && p.x <= 239){
      info = humFanScreen();
      menu = false;
    }
  }
  //------------------------------------------apartB-> beginn-------------------------------------------------------------------------
  else if(p.z > __PRESURE && beginn ){  
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      beginn = false;
    }
    // als op verder wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      mist = true;
      beginn = false;
      huidigePeriode = 1;
      mistScreen(invoerMist[huidigePeriode-1], huidigePeriode);
    }
//++ knop
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125){
      invoerPeriodes++;
      invoerScreen(invoerPeriodes);
      delay(100);
    }
//-- knop   
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200){
      invoerPeriodes--;
      invoerScreen(invoerPeriodes);
      delay(100);
    }
  }
 //------------------------------------------mist1-------------------------------------------------------------------------
 else if(p.z > __PRESURE && mist ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      mist = false;
    }
    
    // als op verder wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      uit = uitScreen(invoerUit[huidigePeriode-1], huidigePeriode);
      mist = false; 
    }

    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240){
      if(huidigePeriode == 1)
        beginn = beginScreen(invoerPeriodes);
      else
      {
        huidigePeriode--;
        herhaal = herhaalScreen(invoerHerhaal[huidigePeriode-1], invoerMist[huidigePeriode-1], invoerLucht[huidigePeriode-1], invoerUit[huidigePeriode-1], huidigePeriode);
      }
      mist = false; 
    }

    // +1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125){
      invoerMist[huidigePeriode-1]++;
      invoerScreen(invoerMist[huidigePeriode-1]);
      delay(100);
    }

    // -1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200){
      invoerMist[huidigePeriode-1]--;
      invoerScreen(invoerMist[huidigePeriode-1]);
      delay(100);
    }
    
  }
  //------------------------------------------uit-------------------------------------------------------------------------
 else if(p.z > __PRESURE && uit ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      uit = false;
    }
    
    // als op verder wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      lucht = luchtScreen(invoerLucht[huidigePeriode-1], huidigePeriode);
      uit = false; 
    }

    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240){
      mist = mistScreen(invoerMist[huidigePeriode-1], huidigePeriode);
      uit = false; 
    }

    // +1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125){
      invoerUit[huidigePeriode-1]++;
      invoerScreen(invoerUit[huidigePeriode-1]);
      delay(100);
    }

    // -1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200){
      invoerUit[huidigePeriode-1]--;
      invoerScreen(invoerUit[huidigePeriode-1]);
      delay(100);
    }
    
  }
  //------------------------------------------lucht-------------------------------------------------------------------------
 else if(p.z > __PRESURE && lucht ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      lucht = false;
    }
    
    // als op verder wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      herhaal = herhaalScreen(invoerHerhaal[huidigePeriode-1], invoerMist[huidigePeriode-1], invoerLucht[huidigePeriode-1], invoerUit[huidigePeriode-1], huidigePeriode);
      lucht = false; 
    }

    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240){
      uit = uitScreen(invoerUit[huidigePeriode-1], huidigePeriode);
      lucht = false; 
    }

    // +1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125){
      invoerLucht[huidigePeriode-1]++;
      invoerScreen(invoerLucht[huidigePeriode-1]);
      delay(100);
    }

    // -1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200){
      invoerLucht[huidigePeriode-1]--;
      invoerScreen(invoerLucht[huidigePeriode-1]);
      delay(100);
    } 
    
  }
  
   //------------------------------------------herhaal-------------------------------------------------------------------------
 else if(p.z > __PRESURE && herhaal ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      herhaal = false;
    }
    
    // als op verder wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      if(huidigePeriode < invoerPeriodes)
      {
        huidigePeriode++;
        mist = mistScreen(invoerMist[huidigePeriode-1],huidigePeriode);
        herhaal = false;
      }
      else
      {
        start = startScreen(invoerPeriodes, invoerMist, invoerLucht, invoerUit, invoerHerhaal);
        herhaal = false; 
      }
    }

    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240){
      lucht = luchtScreen(invoerLucht[huidigePeriode-1], huidigePeriode);
      herhaal = false; 
    }

    // +1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125){
      invoerHerhaal[huidigePeriode-1]++;
      invoerScreen(invoerHerhaal[huidigePeriode-1]);
      invoerHerhaalScreen(invoerHerhaal[huidigePeriode-1], invoerMist[huidigePeriode-1], invoerLucht[huidigePeriode-1], invoerUit[huidigePeriode-1]);
      delay(100);
    }

    // -1 minuut
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200){
      invoerHerhaal[huidigePeriode-1]--;
      invoerScreen(invoerHerhaal[huidigePeriode-1]);
      invoerHerhaalScreen(invoerHerhaal[huidigePeriode-1], invoerMist[huidigePeriode-1], invoerLucht[huidigePeriode-1], invoerUit[huidigePeriode-1]);
      delay(100);
    }
  }

   //------------------------------------------start-------------------------------------------------------------------------
 else if(p.z > __PRESURE && start ){
    
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56){
      menu = menuScreen();
      start = false;
    }
    
    // als op start wordt gedrukt
    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240){
      startTimer = invoerStartScreen();
    }

    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240){
      if(!startTimer)
      {
        herhaal = herhaalScreen(invoerHerhaal[huidigePeriode-1], invoerMist[huidigePeriode-1], invoerLucht[huidigePeriode-1], invoerUit[huidigePeriode-1], huidigePeriode);
        start = false;
      } 
    }
    
  }

    //------------------------------------------info-------------------------------------------------------------------------
  else if(p.z > __PRESURE && info )
  {  
    if(p.y >= 140 && p.y <= 320 && p.x >= 0 && p.x <= 56)
    {
      menu = menuScreen();
      info = false;
    }

    // +1 uit
    if(p.y >= 85 && p.y <= 145 && p.x >= 65 && p.x <= 125)
      infoUitNumScreen(++fanSec - humSec);
      
    // -1 uit
    if(p.y >= 85 && p.y <= 145 && p.x >= 140 && p.x <= 200 && fanSec - humSec > 0)
      infoUitNumScreen(--fanSec - humSec);

    // +1 aan
    if(p.y >= 260 && p.y <= 320 && p.x >= 65 && p.x <= 125){
      infoAanNumScreen(++humSec);
      
      // fanSec is fan + hum so increment to keep it the same
      fanSec++; 
    }
    
    // -1 uit
    if(p.y >= 260 && p.y <= 320 && p.x >= 140 && p.x <= 200 && humSec > 0){
      infoAanNumScreen(--humSec);
      fanSec--;
    }
      
    // als op terug wordt gedrukt
    if(p.y >= 160 && p.y <= 320 && p.x >= 210 && p.x <= 240)
      nieuwMist = infoAanScreen(); 

    if(p.y >= 0 && p.y <= 160 && p.x >= 210 && p.x <= 240)
      nieuwMist = infoAanUitScreen();
     
  }


  
  //******************************************

  
  
  //startTimers
  if(startTimer && startEenmaal)
  {
    int timer = 0;
    startEenmaal = false;

    // Only used for display 
    for( int k = 0; k < invoerPeriodes; k++)
    {
      for( int l = 0; l < invoerHerhaal[k]; l++)
      {
        totaleTijd += invoerMist[k] + invoerUit[k] + invoerLucht[k];  
      }  
    }
    
    for( int j = 0; j < invoerPeriodes; j++)
    {
      for( int i = 0; i < invoerHerhaal[j]; i++)
      {
       // Start cycle
        if(invoerMist[j]){
          if(nieuwMist){
            if(!timer)
              nieuwMistTimer();                          
            Alarm.timerOnce(timer * SEC, nieuwMistTimer);
          }
          else{
            if(!timer)
              mistTimer();                          
            Alarm.timerOnce(timer * SEC, mistTimer);
          }
        }
         
        timer += invoerMist[j];
        
        if(invoerUit[j])
          Alarm.timerOnce(timer * SEC, uitTimer);  

        timer += invoerUit[j];
        
        if(invoerLucht[j])
          Alarm.timerOnce(timer * SEC, luchtTimer);
          
        timer += invoerLucht[j];      
      }  
    }
  }

  if( Alarm.count() == 255)
    reset = true;
  if(reset){
    setup();
    Alarm.freeAll();   // ensure all Alarms are cleared and available for allocation
  }

  if(info ){
    
  } 
}

 void mistTimer()
 {
   mistAanLuchtUit();
   Alarm.timerRepeat( totaleTijd * SEC, mistAanLuchtUit);
 
 }

 void nieuwMistTimer()
 {
   nieuwMistAanLuchtUit();
   Alarm.timerRepeat( totaleTijd * SEC, nieuwMistAanLuchtUit);
 
 }

 void uitTimer()
 {
   beideUit();
   Alarm.timerRepeat(totaleTijd * SEC, beideUit); 
   
 }

 void luchtTimer()
 {
   luchtAanMistUit();
   Alarm.timerRepeat(totaleTijd * SEC, luchtAanMistUit); 
 }

void mistAanLuchtUit()
{
  digitalWrite(RELAY_1,HIGH);
  digitalWrite(RELAY_2,HIGH);
  digitalWrite(RELAY_AIR_IN,HIGH);
  digitalWrite(RELAY_AIR_OUT,HIGH);
}

void nieuwMistAanLuchtUit(){
  // Free old timers before setting new ones
  freeTimer();
  
  // Repeat the humidifier / humidifier-fan until they both have to be off. 
  nieuwMistAanLuchUitEenmaal();
  id[0] = Alarm.timerRepeat(fanSec,  nieuwMistAanLuchUitEenmaal );
}

void freeTimer(){
  for(int i=0; i < ID_SIZE; i++)
    Alarm.free(id[i]);
}

// Function that turns the humidifier on for X seconds and the humidifier fan on indefinitely.
void nieuwMistAanLuchUitEenmaal(){
  humTijdelijkAan();
  id[1] = Alarm.timerOnce( humSec, humTijdelijkUit);
  digitalWrite(RELAY_AIR_IN,HIGH);
  digitalWrite(RELAY_AIR_OUT,HIGH);
}

void humTijdelijkAan(){
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, LOW);
}

void humTijdelijkUit(){
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, HIGH);
}

void humTijdelijkPauze(){
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
}
  
void beideUit()
{
  freeTimer();
  digitalWrite(RELAY_1,LOW);
  digitalWrite(RELAY_2,LOW);
  digitalWrite(RELAY_AIR_IN,HIGH);
  digitalWrite(RELAY_AIR_OUT,HIGH);
}
  
void luchtAanMistUit()
{
  freeTimer();
  digitalWrite(RELAY_1,LOW);
  digitalWrite(RELAY_2,LOW);
  digitalWrite(RELAY_AIR_IN,LOW);
  digitalWrite(RELAY_AIR_OUT,LOW);
}
  
//**************************************************************************************SCREENS*********************************************************************************************************

bool menuScreen(void){
  Tft.fillScreen();
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawString("Scherm uit", 202, 302, 2, WHITE);
  Tft.drawRectangle(179,140,60,180,WHITE);
  
  Tft.drawString("Begin", 104, 290, 3, WHITE);
  Tft.drawRectangle(57,140,120,180,WHITE);
  Tft.drawRectangle(57,0,121,139,WHITE);
  
  Tft.drawString("Reset", 104, 124, 3, RED);
  Tft.drawRectangle(57,0,121,139,WHITE);

  //size of loop is the linewidth of the rectangle
  for(int i=1;i<=10;i++){ 
    Tft.drawRectangle(57+i,0+i,121-i*2,139-i*2,RED);
  }
  Tft.drawString("INFO", 202, 102, 2, WHITE);
  Tft.drawRectangle(179,0,60,139,WHITE);
  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;    
}

//bool aparteBesturingScreen(void){
//  Tft.fillScreen();
//  
//  Tft.drawString("MENU", 17, 280, 3, WHITE);
//  Tft.drawRectangle(0,140,56,180,WHITE);
//  Tft.drawString("UNIT 1", 100, 230, 3, WHITE);
//  Tft.drawRectangle(80,70,60,180,WHITE);
//  Tft.drawString("UNIT 2", 180, 230, 3, WHITE);
//  Tft.drawRectangle(160,70,60,180,WHITE);
//  
//  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
//  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
//  return true;
//}

bool beginScreen(int periodes){
  
  Tft.fillScreen();
//  invoerAanString = String(invoerAan);
//  invoerAanString.toCharArray(invoerAanCharArray,4);
  String invoerPeriodesString;
  char invoerPeriodesCharArray[4];

  invoerPeriodesString = String(periodes);
  invoerPeriodesString.toCharArray(invoerPeriodesCharArray,4);
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);

//  Tft.drawString("AAN", 65, 225, 2, WHITE);
  Tft.drawString("Periodes", 120, 300, 2, WHITE);
  Tft.drawRectangle(56,160,154,160,WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
//  Tft.drawCircle(95,275,30, WHITE);
//  Tft.drawCircle(170,275,30, WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
//  Tft.drawRectangle(110,160,40,80,WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerAanCharArray, 120,235,3,WHITE);
  Tft.drawString(invoerPeriodesCharArray, 120,75,3,WHITE);
  
//  Tft.drawTriangle(75,275,105,290,105,260, WHITE);
//  Tft.drawTriangle(190,275,160,290,160,260, WHITE);
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);
 
  
//  Tft.drawString("START", 217, 280, 2, WHITE);
//  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Verder", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
  Tft.drawString("HaagseZwam", 22, 122, 1, GREEN);
  Tft.drawString("Touchscreen", 32, 122, 1, GREEN);
  return true;
}


bool mistScreen(int mist, int periode){
  Tft.fillScreen();
//  invoerAanString = String(invoerAan);
//  invoerAanString.toCharArray(invoerAanCharArray,4);
  String invoerMistString;
  String periodeString;
  char invoerMistCharArray[4];
  char periodeCharArray[2];
  
  invoerMistString = String(mist);
  periodeString = String(periode);
  invoerMistString.toCharArray(invoerMistCharArray,4);
  periodeString.toCharArray(periodeCharArray,2);
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawString("Periode", 20, 150, 2, WHITE);
  Tft.drawString(periodeCharArray, 20, 20, 2, WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
//  Tft.drawString("AAN", 65, 225, 2, WHITE);
  Tft.drawString("Mist aan", 100, 310, 2, WHITE);
  Tft.drawString("Lucht uit", 150, 310, 2, WHITE);
  Tft.drawRectangle(56,160,154,160,WHITE);
//  Tft.drawString("Mist", 65, 80, 2, WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
//  Tft.drawCircle(95,275,30, WHITE);
//  Tft.drawCircle(170,275,30, WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
//  Tft.drawRectangle(110,160,40,80,WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerAanCharArray, 120,235,3,WHITE);
  Tft.drawString(invoerMistCharArray, 120,75,3,WHITE);
  
//  Tft.drawTriangle(75,275,105,290,105,260, WHITE);
//  Tft.drawTriangle(190,275,160,290,160,260, WHITE);
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);
 
  
  Tft.drawString("Terug", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Verder", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
//  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
//  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

bool luchtScreen(int lucht, int periode){
  Tft.fillScreen();
//  invoerAanString = String(invoerAan);
//  invoerAanString.toCharArray(invoerAanCharArray,4);
  String invoerLuchtString;
  String periodeString;
  char invoerLuchtCharArray[4];
  char periodeCharArray[2];
  
  invoerLuchtString = String(lucht);
  periodeString = String(periode);
  invoerLuchtString.toCharArray(invoerLuchtCharArray,4);
  periodeString.toCharArray(periodeCharArray,2);
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawString("Periode", 20, 150, 2, WHITE);
  Tft.drawString(periodeCharArray, 20, 20, 2, WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
//  Tft.drawString("AAN", 65, 225, 2, WHITE);
  Tft.drawString("Mist uit", 100, 310, 2, WHITE);
  Tft.drawString("Lucht aan", 150, 310, 2, WHITE);
  Tft.drawRectangle(56,160,154,160,WHITE);
//  Tft.drawString("Mist", 65, 80, 2, WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
//  Tft.drawCircle(95,275,30, WHITE);
//  Tft.drawCircle(170,275,30, WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
//  Tft.drawRectangle(110,160,40,80,WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerAanCharArray, 120,235,3,WHITE);
  Tft.drawString(invoerLuchtCharArray, 120,75,3,WHITE);
  
//  Tft.drawTriangle(75,275,105,290,105,260, WHITE);
//  Tft.drawTriangle(190,275,160,290,160,260, WHITE);
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);
 
  
  Tft.drawString("Terug", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Verder", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
//  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
//  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

bool uitScreen(int uit, int periode){
  Tft.fillScreen();
//  invoerAanString = String(invoerAan);
//  invoerAanString.toCharArray(invoerAanCharArray,4);
  String invoerUitString;
  String periodeString;
  char invoerUitCharArray[4];
  char periodeCharArray[2];
  
  invoerUitString = String(uit);
  periodeString = String(periode);
  invoerUitString.toCharArray(invoerUitCharArray,4);
  periodeString.toCharArray(periodeCharArray,2);
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawString("Periode", 20, 150, 2, WHITE);
  Tft.drawString(periodeCharArray, 20, 20, 2, WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
//  Tft.drawString("AAN", 65, 225, 2, WHITE);
  Tft.drawString("Mist uit", 100, 310, 2, WHITE);
  Tft.drawString("Lucht uit", 150, 310, 2, WHITE);
  Tft.drawRectangle(56,160,154,160,WHITE);
//  Tft.drawString("Uit", 65, 80, 2, WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
//  Tft.drawCircle(95,275,30, WHITE);
//  Tft.drawCircle(170,275,30, WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
//  Tft.drawRectangle(110,160,40,80,WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerAanCharArray, 120,235,3,WHITE);
  Tft.drawString(invoerUitCharArray, 120,75,3,WHITE);
  
//  Tft.drawTriangle(75,275,105,290,105,260, WHITE);
//  Tft.drawTriangle(190,275,160,290,160,260, WHITE);
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);
 
  
  Tft.drawString("Terug", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Verder", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
//  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
//  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

bool herhaalScreen(int herhaal, int mist, int lucht, int uit, int periode){
  Tft.fillScreen();
//  invoerAanString = String(invoerAan);
//  invoerAanString.toCharArray(invoerAanCharArray,4);

  String minutenString;
  String urenString;
  String invoerHerhaalString;
  String periodeString;
  char minutenCharArray[3];
  char urenCharArray[3];
  char invoerHerhaalCharArray[4];
  char periodeCharArray[2];

  int minuten = herhaal * (mist + lucht + uit) % 60;
  int uren = herhaal * (mist + lucht + uit) / 60;

  if(minuten < 10)
    minutenString = "0" + String(minuten);
  else
    minutenString = String(minuten);
  if(uren < 10)
    urenString = "0" + String(uren);
  else
    urenString = String(uren);
  
//  urenString = String(uren);
  invoerHerhaalString = String(herhaal);
  periodeString = String(periode);
  minutenString.toCharArray(minutenCharArray,3);
  urenString.toCharArray(urenCharArray,3);
  invoerHerhaalString.toCharArray(invoerHerhaalCharArray,4);
  periodeString.toCharArray(periodeCharArray,2);
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawString("Periode", 20, 150, 2, WHITE);
  Tft.drawString(periodeCharArray, 20, 20, 2, WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
//  Tft.drawString("AAN", 65, 225, 2, WHITE);
  Tft.drawString("Herhalen", 65, 310, 2, WHITE);
  Tft.drawRectangle(56,160,32,160,WHITE);
  Tft.drawString("Tijd", 105 , 310, 2, WHITE);
  Tft.drawRectangle(56,160,154,160,WHITE);
//  Tft.drawString("Mist", 65, 80, 2, WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
//  Tft.drawCircle(95,275,30, WHITE);
//  Tft.drawCircle(170,275,30, WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
//  Tft.drawRectangle(110,160,40,80,WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerAanCharArray, 120,235,3,WHITE);
  Tft.drawString(invoerHerhaalCharArray, 120,75,3,WHITE);

  Tft.drawString("Uur:", 145, 310, 2, WHITE);
  Tft.drawString("Min:", 145, 230, 2, WHITE);
  
  Tft.drawString(minutenCharArray, 180,222,3,WHITE);
  Tft.drawString(urenCharArray, 180,302,3,WHITE);
//  Tft.drawRectangle(170,180,40,120,WHITE);
  Tft.drawRectangle(170,240,40,80,WHITE);
  Tft.drawRectangle(170,160,40,80,WHITE);
  
//  Tft.drawTriangle(75,275,105,290,105,260, WHITE);
//  Tft.drawTriangle(190,275,160,290,160,260, WHITE);
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);
 
  
  Tft.drawString("Terug", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Verder", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
//  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
//  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

bool startScreen(int periode, int invoerMist[],int invoerLucht[],int invoerUit[],int invoerHerhaal[]){
  Tft.fillScreen();
  int minuten, uren, minutenTotaal = 0, urenTotaal = 0 ;
  String tijdString;
  String minutenString;
  String urenString;
  String nummerString;
  
  char tijdCharArray[6];  
  char minutenCharArray[3];
  char urenCharArray[3];
  char nummerCharArray[2];
    
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
  Tft.drawString("1", 64, 311, 2, WHITE);
  for( int j = 0; j <= 1; j++)
  {
    for( int i = 56; i <= 143; i = i + 29)
    {
       Tft.drawRectangle(i,j*160,29,160,WHITE);
       Tft.drawRectangle(i,130,29,30,WHITE);
       Tft.drawRectangle(i,290,29,30,WHITE);
    }
  }
  for( int k = 0; k < periode; k++)
  {
    
    minuten = invoerHerhaal[k] * (invoerMist[k] + invoerLucht[k] + invoerUit[k]) % 60;
    uren = invoerHerhaal[k] * (invoerMist[k] + invoerLucht[k] + invoerUit[k]) / 60;

    nummerString = String(k+1);
    if(minuten < 10)
      minutenString = "0" + String(minuten);
    else
      minutenString = String(minuten);
    if(uren < 10)
      urenString = "0" + String(uren);
    else
      urenString = String(uren);
  
    tijdString = urenString + ":" + minutenString;
    nummerString.toCharArray(nummerCharArray,2);
    tijdString.toCharArray(tijdCharArray,6);

    if(k < 4)
    {
      Tft.drawString(nummerCharArray, 64 + k*29, 311, 2, WHITE);
      Tft.drawString(tijdCharArray, 64 + k*29, 265, 2, WHITE);  
    }
    else
    {
      Tft.drawString(nummerCharArray, 64 + (k-4)*29, 151, 2, WHITE);
      Tft.drawString(tijdCharArray, 64 + (k-4)*29, 105, 2, WHITE);
    }

    minutenTotaal = minutenTotaal + minuten;
    urenTotaal = urenTotaal + uren; 
    
  }

  urenTotaal = urenTotaal + minutenTotaal / 60;
  minutenTotaal = minutenTotaal % 60;

  
  if(minutenTotaal < 10)
    minutenString = "0" + String(minutenTotaal);
  else
    minutenString = String(minutenTotaal);
  if(urenTotaal < 10)
    urenString = "0" + String(urenTotaal);
  else
    urenString = String(urenTotaal);
  
//  minutenString = String(minutenTotaal);
//  urenString = String(urenTotaal);
  minutenString.toCharArray(minutenCharArray,3);
  urenString.toCharArray(urenCharArray,3);  
  Tft.drawString(minutenCharArray, 180,62,3,WHITE);
  Tft.drawString(urenCharArray, 180,142,3,WHITE);
  Tft.drawString("Totaal", 180,310,3,WHITE);
  Tft.drawRectangle(172,80,38,80,WHITE);
  Tft.drawRectangle(172,0,38,80,WHITE);
  Tft.drawRectangle(172,160,38,160,WHITE);
    
  Tft.drawString("Terug", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);
  Tft.drawString("Start", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

bool humFanScreen(){
  Tft.fillScreen();
    
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,160,56,160,WHITE);
  Tft.drawRectangle(0,0,56,160,WHITE);
  
  Tft.drawRectangle(56,160,154,160,WHITE);
  Tft.drawRectangle(56,0,154,160,WHITE);
  Tft.drawCircle(95,115,30, WHITE);
  Tft.drawCircle(170,115,30, WHITE);
  Tft.drawRectangle(110,0,40,80,WHITE);
//  Tft.drawString(invoerUitCharArray, 120,75,3,WHITE);
  
  Tft.drawTriangle(75,115,105,130,105,100, WHITE);
  Tft.drawTriangle(190,115,160,130,160,100, WHITE);


  Tft.drawCircle(95,280,30, WHITE);
  Tft.drawCircle(170,280,30, WHITE);
  Tft.drawRectangle(110,160,40,80,WHITE);
//  Tft.drawString(invoerUitCharArray, 120,75,3,WHITE);
  
  Tft.drawTriangle(75,280,105,295,105,265, WHITE);
  Tft.drawTriangle(190,280,160,295,160,265, WHITE);

  Tft.drawString("aan", 75, 220, 2, WHITE);
  Tft.drawString("uit", 75, 50, 2, WHITE);
    
  Tft.drawString("Aan", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);

  Tft.fillScreen(211,238,1,159,WHITE);
  Tft.drawString("Aan-Uit", 217, 120, 2, BLACK);
  Tft.drawRectangle(210,0,30,160,WHITE);
  
  infoAanNumScreen(humSec);
  infoUitNumScreen(fanSec-humSec);
  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;
}

void infoAanNumScreen(int num){
  char invoerCharArray[4];
  String invoerString = String(num);
  invoerString.toCharArray(invoerCharArray,4);
  Tft.fillScreen(111,149,161,238,BLACK);
  Tft.drawString(invoerCharArray, 120,235,3,WHITE);
  
  delay(100);
}


void infoUitNumScreen(int num){
  char invoerCharArray[4];
  String invoerString = String(num);
  invoerString.toCharArray(invoerCharArray,4);
  Tft.fillScreen(111,149,1,79,BLACK);
  Tft.drawString(invoerCharArray, 120,75,3,WHITE);
  delay(100);
}

bool infoAanScreen(){

  Tft.fillScreen(211,238,161,319,WHITE);
  Tft.drawString("Aan", 217, 280, 2, BLACK);
  Tft.drawRectangle(210,160,30,160,WHITE);

  Tft.fillScreen(211,238,1,159,BLACK);
  Tft.drawString("Aan-Uit", 217, 120, 2, WHITE);
  Tft.drawRectangle(210,0,30,160,WHITE);
  return false;
}

bool infoAanUitScreen(){

  Tft.fillScreen(211,238,161,319,BLACK);
  Tft.drawString("Aan", 217, 280, 2, WHITE);
  Tft.drawRectangle(210,160,30,160,WHITE);

  Tft.fillScreen(211,238,1,159,WHITE);
  Tft.drawString("Aan-Uit", 217, 120, 2, BLACK);
  Tft.drawRectangle(210,0,30,160,BLACK);
  
  return true;
}

void invoerScreen(int invoer){
  char invoerCharArray[4];
  String invoerString = String(invoer);
  invoerString.toCharArray(invoerCharArray,4);
  Tft.fillScreen(111,149,1,79,BLACK);
  Tft.drawString(invoerCharArray, 120,75,3,WHITE);

}

void invoerHerhaalScreen(int herhaal, int mist, int lucht, int uit){
  
  String minutenString;
  String urenString;
  char minutenCharArray[3];
  char urenCharArray[3];
  
  int minuten = herhaal * (mist + lucht + uit) % 60;
  int uren = herhaal * (mist + lucht + uit) / 60;

  if(minuten < 10)
    minutenString = "0" + String(minuten);
  else
    minutenString = String(minuten);
  if(uren < 10)
    urenString = "0" + String(uren);
  else
    urenString = String(uren);
  
//  minutenString = String(minuten);
//  urenString = String(uren);
  minutenString.toCharArray(minutenCharArray,3);
  urenString.toCharArray(urenCharArray,3);
  Tft.fillScreen(171,209,241,318,BLACK);
  Tft.fillScreen(171,209,161,239,BLACK);
//  Tft.drawString(minutenCharArray, 180,220,3,WHITE);
//  Tft.drawString(urenCharArray, 180,310,3,WHITE);
  
  Tft.drawString(minutenCharArray, 180,222,3,WHITE);
  Tft.drawString(urenCharArray, 180,302,3,WHITE);
  
//  Tft.drawRectangle(170,180,40,120,WHITE);
//  Tft.drawRectangle(170,240,40,80,WHITE);
//  Tft.drawRectangle(170,160,40,80,WHITE);
  
//  return true;
}

bool invoerStartScreen(void){
  Tft.fillScreen(211,238,1,159,WHITE);
  Tft.drawString("Start", 217, 120, 2, BLACK);
//  Tft.fillScreen(211,238,161,318,WHITE);
//  Tft.drawString("START", 217, 280, 2, BLACK);
  return true;
}



bool schermUitScreen(void){
  digitalWrite(backlight,LOW);
  Tft.fillScreen();
  return false;
}

bool infoScreen(void){
  Tft.fillScreen();
  
  Tft.drawString("MENU", 17, 280, 3, WHITE);
  Tft.drawRectangle(0,140,56,180,WHITE);
  Tft.drawString("Systeem", 80, 315, 2, WHITE);
  Tft.drawRectangle(57,180,60,140,WHITE);



  

  Tft.drawString("Unit 1", 141, 315, 2, WHITE);
  Tft.drawRectangle(118,180,60,140,WHITE);
  Tft.drawString("Unit 2", 202, 315, 2, WHITE);
  Tft.drawRectangle(179,180,60,140,WHITE);
  Tft.drawRectangle(57,0,60,179,WHITE);
  Tft.drawRectangle(118,0,60,179,WHITE);
  Tft.drawRectangle(179,0,60,179,WHITE);
  Tft.drawString("HaagseZwam", 12, 102, 1, GREEN);
  Tft.drawString("Touchscreen", 22, 102, 1, GREEN);
  return true;    
}

void updateTimeScreen(time_t sys){
//  if (st1 != 0)
//    u1 = sys - st1;
//  else
//    u1 = 0;
//  if (st2 != 0)
//    u2 = sys - st2;
//  else
//    u2 = 0;
//
  String sysStr;
  char sysChAr[12];
    
  sysStr = String(day(sys)-1)+":"+String(hour(sys))+":"+String( minute(sys))+ ":"+String(second(sys));
//  u1Str = String(day(u1)-1)+":"+String(hour(u1))+":"+String( minute(u1))+ ":"+String(second(u1));
//  u2Str = String(day(u2)-1)+":"+String(hour(u2))+":"+String( minute(u2))+ ":"+String(second(u2)); 
  sysStr.toCharArray(sysChAr,12); 
//  u1Str.toCharArray(u1ChAr,12); 
//  u2Str.toCharArray(u2ChAr,12);
  Tft.fillScreen(70,100,175,2,BLACK);
  Tft.drawString(sysChAr, 80, 175, 2, WHITE);
}
