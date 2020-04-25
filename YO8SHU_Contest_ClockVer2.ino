// Ceas pentru concursurile de radioamatori
// Versiune 3.0
// YO8SHU Robert

// Connectare LCD:
//pin SDA to Arduino Analog pin A4
//pin SCL to Arduino Analog pin A5
//pin GND to Arduino GND
//pin VCC to Arduino 5v

//Conectare RTC:
//pin SDA to Arduino Analog pin A4
//pin SCL to Arduino Analog pin A5
//pin GND to Arduino GND
//pin VCC to Arduino 5v

// Conectare Divizor pin A0
// Conectare buton pin A2

//Librarii

#include <Wire.h>                             // include libraria Wire.h
#include <LCD.h>                              // include libraria LCD.h
#include <LiquidCrystal_I2C.h>                // include libraria LiquidCrystal_I2C.h
#define DS1307_I2C_ADDRESS 0x68               // defineste adresa I2C folosita de RTC 0x68
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);   // defineste adresa I2C folosita de LCD 0x27

// Configurare Voltmetru
   int vBat = A0;                                //Pinul folosit pentru citirea voltajului de pe baterie, A0.
   float vout = 0;
   float vin = 0;
   float R1 = 103000;                            // R1 aprox 100K, pentru acuratetea masurarii trebuie introdusa valoarea exacta!
   float R2 = 10000;                             // R2 aprox 10K, pentru acuratetea masurarii trebuie introdusa valoarea exacta!
   int value = 0;

// Configurare activare si temporizare retroiluminare
   const byte BUTTON = A2;                       // Pinul folosit pentru butonul ce prinde retroiluminarea, A2
   unsigned long buttonPushedMillis = 0;         // defineste cand butonul a fost eliberat
   unsigned long ledTurnedOnAt = 0;              // defineste cand ledul sa aprins
   unsigned long turnOnDelay = 250;              // defineste cat se asteapta pana se aprinde ledul
   unsigned long turnOffDelay = 10000;           // defineste cat timp sta aprins ledul
   bool ledReady = true;                         // flag for when button is let go
   bool ledState = false;                        // for LED is on or not.

// Converteste numerele Zecimale in Binare
   byte decToBcd(byte val)
        {return ( (val/10*16) + (val%10) );}

// Converteste numerele Binare in Zecimale
   byte bcdToDec(byte val)
        {return ( (val/16*10) + (val%16) );}

// 1) Setarea datei si orei in modulul RTC
// 2) Porneste ceasul
// 3) regleaza afisarea orei in formatul 24 ore
// introduceti cifre corecte                                     
   void setDateDs1307( byte second,                         //ORA:   // 0-59  
                       byte minute,                                  // 0-59
                       byte hour,                                    // 1-23
                       byte dayOfWeek,                      //DATA:  // 1-7   
                       byte dayOfMonth,                              // 1-28/29/30/31
                       byte month,                                   // 1-12
                       byte year)                                    // 0-99
   {Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write(0);
    Wire.write(decToBcd(second));                                   // 0 to bit 7 starts the clock
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour)); 
    Wire.write(decToBcd(dayOfWeek));
    Wire.write(decToBcd(dayOfMonth));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));
    Wire.write(0x10);                                               // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave
    Wire.endTransmission();}

// Citirea datei si orei din modulul RTC
   void getDateDs1307(byte *second,
   byte *minute,
   byte *hour,
   byte *dayOfWeek,
   byte *dayOfMonth,
   byte *month,
   byte *year)
   
// Reset the register pointer
             {Wire.beginTransmission(DS1307_I2C_ADDRESS);
              Wire.write(0);
              Wire.endTransmission();
              Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
               *second     = bcdToDec(Wire.read() & 0x7f);        // A few of these need masks because certain bits are control bits
               *minute     = bcdToDec(Wire.read());
               *hour       = bcdToDec(Wire.read() & 0x3f);        // Need to change this if 12 hour am/pm
               *dayOfWeek  = bcdToDec(Wire.read());
               *dayOfMonth = bcdToDec(Wire.read());
               *month      = bcdToDec(Wire.read());
               *year       = bcdToDec(Wire.read());}
              
void setup()
    {pinMode(BUTTON, INPUT_PULLUP);           // Setaza butonul ca fiind Pulup
     pinMode(vBat, INPUT);                    // Setaza pinul de masura voltaj baterie ca fiind INPUT
     lcd.setBacklightPin(3,POSITIVE);         // Seteaza pinul 3 din i2c to lcd ca fiind cel ce controleaza iluminatul
     lcd.setBacklight(HIGH);                  // Aprinde iluminatul
     byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     Wire.begin();
 
// Introduceti mai jos data si ora exacta
// Aceasta informatii se introduc doar atunci cand dorim sincronizarea ceasului cu ora excacta
// In functie de calitatea RTC-ului acest lucru nu trebuie efectuat prea des.

             second = 0;                     // aici se introduce secunda exacta
             minute = 12;                    // aici se introduce minutul exact
             hour = 13;                      // aici se introduce ora exacta
             dayOfWeek = 7;                  // aici se introduce numarul zilei din saptamana
             dayOfMonth = 12;                // aici se introduce ziua
             month = 04;                     // aici se introduce luna
             year = 20;                      // aici se introduce anul
 
// Pentru a introduce datele de mai sus in modulul RTC, se decomenteaza urmatoarea linie si apoi se uploadeaza codul in arduino.
// setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
// Dupa ce sa uploadat codul cu ora exacta reglata, se comentaza la loc linia de deasupra si se face upload la cod din nou.
// daca linia raman decomentata, la fiecare pornire a ceasului informatiile de mai sus se reintroduc in
// modulul RTC ceea ce face ca data si ora sa se reseteze la momentul uploadului initial.

// initializare LCD
   lcd.begin(16, 2);                                                         // Stabileste dimensiunile LCD-ului
   lcd.setCursor(0,1);                                                       // Stabileste pozitia cursorului la inceputul primului rand , Stanga sus
   lcd.print(" CONTEST  TIMER ");                                            // Afiseaza textul ditre paranteze
   lcd.setCursor(0,0);                                                       // Stabileste pozitia cursorului la inceputul urmatorului rand.
   lcd.print("YO8SHU/P  Ver. 3");                                            // Afiseaza textul ditre paranteze, puteti modifica textul dupa plac
   delay(5000);                                                              // pauza de 5 secunde sa se poata citi textul
   lcd.clear();}                                                             // sterge ecranul

  void loop()
       {byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
        getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

        BackLight();                                                          //Apeleaza functia backlight
  
//Afisare DATA si ORA
       UTC();                                                                  // Afiseaza ora UTC pe primul rand
            if (second < 20){CFR(); }                                          // Daca secunde < 20 afiseaza pe randul 2 ora CFR
            else if (second < 30){BatVolt(); }                                 // Altfel daca secunde < 30 afiseaza voltajul bateriei pe randul 2                    
            else if (second < 50){ DATA(); }                                   // Altfel daca secunde < 50 afiseaza pe randul doi data 
            else if (second < 60){BatVolt(); }}                                // Altfel daca secunde < 60 afiseaza voltajul bateriei pe randul 2    

//Functie citire si afisare voltaj baterie
void BatVolt()
    {value = analogRead(vBat);                                               // citeste pinul vBat si stocheaza valoarea in value
     vout =(value * 5) / 1024.0;                                             // defineste vout ca fiind value inmulti cu tensiunea de referinta totul impartit la numarul de pasi
     vin = vout / (R2/(R1+R2));                                              // defineste vin ca fiind vout impartit la divizorul de tensiune
       if (vin<6) {                                                          // daca vin mai mic ca 6
          lcd.setCursor(0,1);                                                // stabileste cursorul pe randul 2
          lcd.print("Low bat Recharge");}                                    // afiseaza low bat reincarcati
       else{                                                                 // altfel
          lcd.setCursor(0,1);                                                // cursorul pe randul 2
          lcd.print("Bat. volt. ");                                          // afiseaza: Bat. Volt. 
          lcd.print(vin);                                                    // afiseaza valoarea lui vin
          lcd.print("V");}}                                                  // afiseaza: V

// Functie aprindere backlight
void BackLight()
    {unsigned long currentMillis = millis();                                          // deineste surrentMillis ca find ora actuala
      if (digitalRead(BUTTON) == LOW) {                                               
          buttonPushedMillis = currentMillis;                                         // inoieste milis cand butonul a fost apasat
          ledReady = true; }
      if (ledReady) {                                                                 // this is typical millis code here:
      if ((unsigned long)(currentMillis - buttonPushedMillis) >= turnOnDelay) {       // okay, enough time has passed since the button was let go.
         lcd.setBacklight(HIGH);                                                      // Backlight on setup our next "state"
         ledState = true;                                                             // save when the LED turned on
         ledTurnedOnAt = currentMillis;                                               // wait for next button press
         ledReady = false;}}
      if (ledState) {                                                                 // okay, led on, check for now long  
      if ((unsigned long)(currentMillis - ledTurnedOnAt) >= turnOffDelay) {
          ledState = false;
          lcd.setBacklight(LOW);}}}                                                    // Backlight off               

// functie afisare ora UTC
void UTC()
    {byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 

//Afiseaza ORA UTC 
     lcd.setCursor(0,0);
     lcd.print("  UTC ");
         if (hour<10){lcd.print("0");}   
     lcd.print(hour, DEC);
     lcd.print(":");
         if (minute<10){lcd.print("0");}
     lcd.print(minute, DEC);
     lcd.print(":");
         if (second<10){lcd.print("0");}
     lcd.print(second, DEC);
     lcd.print("  ");}

// Functie afisare ora CFR
void CFR()
    {byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

//Afiseaza ORA CFR
     lcd.setCursor(0,1);
     lcd.print("  CFR ");
         if (hour+3<10){lcd.print("0");}                               // se inlocuieste +3 cu +2 daca se foloseste ora de iarna
         if (hour+3>24){lcd.print (hour-21,DEC);}                      // se inlocuieste +3 cu +2 daca se foloseste ora de iarna
         else {lcd.print(hour+3, DEC);}                                // se inlocuieste +3 cu +2 daca se foloseste ora de iarna
     lcd.print(":");
         if (minute<10){lcd.print("0");}
     lcd.print(minute, DEC);
     lcd.print(":");
         if (second<10){lcd.print("0");}
     lcd.print(second, DEC);
     lcd.print("  ");}

// Functie afisare DATA
void DATA()
    {byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
     getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

//Afiseaza DATA
     lcd.setCursor(0,1);
     lcd.print(" ");
         switch(dayOfWeek){                           //AFISEAZA in loc de cifra, numele zilei
          case 1: lcd.print("Lun"); break;
          case 2: lcd.print("Mar"); break;
          case 3: lcd.print("Mie"); break;
          case 4: lcd.print("Joi"); break;
          case 5: lcd.print("Vin"); break;
          case 6: lcd.print("Sam"); break;
          case 7: lcd.print("Dum"); break; }  
     lcd.print(" ");
         if (dayOfMonth<10){lcd.print("0");}           //Daca ZIUA mai mica ca 10 afiseaza "0" inainte
     lcd.print(dayOfMonth, DEC);                       //Print ZIUA
     lcd.print(".");
         if (month<10) {lcd.print("0");}               //Daca LUNA mai mica ca 10 afiseaza "0" inainte
     lcd.print(month, DEC);                            //Print LUNA
     lcd.print(".20");
     lcd.print(year, DEC);                             //Print ANUL
     lcd.print(" ");}
