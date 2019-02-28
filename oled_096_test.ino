/* rychly test dispele s cidlem dht11
 */

#include "U8glib.h"
#include <Wire.h>
#include "DHT.h"

// nastavení propojovacích pinů OLED
#define D0 13
#define D1 11
#define DC 9
#define CS 8
#define RES 10
#define pinDHT 5
#define typDHT11 DHT11     // DHT 11
DHT mojeDHT(pinDHT, typDHT11);

// inicializace OLED displeje z knihovny U8glib 
U8GLIB_SSD1306_128X64 mujOled(D0, D1, CS, DC, RES);

// proměnná pro uchování času poslední obnovy displeje
long int prepis = 0;

void setup(void) {
  // mujOled.setRot180();
  Serial.begin(9600);
    mojeDHT.begin();
}

void loop(void) {
  // porovnání uloženého a aktuálního času
  // při rozdílu větším než 100 ms se provede
  // obnovení displeje, čas můžeme nastavit dle potřeby
  if (millis()-prepis > 100) {
    // následující skupina příkazů
    // obnoví obsah OLED displeje
    mujOled.firstPage();
    do {
      // funkce vykresli vykreslí žádanou obsah
      vykresli();
    } while( mujOled.nextPage() );
    // uložení posledního času obnovení
    prepis = millis();
  }
  
  // zde je místo pro další příkazy pro Arduino
  
// pomocí funkcí readTemperature a readHumidity načteme
  // do proměnných tep a vlh informace o teplotě a vlhkosti,
  // čtení trvá cca 250 ms
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (isnan(tep) || isnan(vlh)) {
    // při chybném čtení vypiš hlášku
    Serial.println("Chyba při čtení z DHT senzoru!");
  } else {
    // pokud jsou hodnoty v pořádku,
    // vypiš je po sériové lince
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" stupnu Celsia, ");
    Serial.print("vlhkost: "); 
    Serial.print(vlh);
    Serial.println("  %");
  }
  // pauza pro přehlednější výpis
  delay(2000);
  
  // volitelná pauza 10 ms pro demonstraci
  // vykonání dalších příkazů
  //delay(10);
}

// funkce vykresli pro nastavení výpisu informací na OLED
void vykresli(void) {
  // nastavení písma, toto písmo umožní vypsat
  // přibližně 15x4 znaků
  mujOled.setFont(u8g_font_unifont);
  // nastavení pozice výpisu v pixelech
  // souřadnice jsou ve tvaru x, y
  // souřadnice 0, 0 je v levém horní rohu
  // OLED displeje, maximum je 128, 64
  // výpis textu na zadanou souřadnici
  mujOled.setPrintPos(0, 10);
  mujOled.print("QNH:"); //radek pro BMP280
  
  mujOled.setPrintPos(0, 26);
  mujOled.print("TEMP:");
  mujOled.print(mojeDHT.readTemperature(),1);
  mujOled.print(char(176)); 
  mujOled.print("C");
  
  mujOled.setPrintPos(0, 40);
  mujOled.print("HUM:");
  mujOled.print(mojeDHT.readHumidity(),1);
  mujOled.print("%");

  mujOled.setPrintPos(0, 55);
  mujOled.print("TIME:");
  
  mujOled.setPrintPos(40, 55);
  mujOled.print(millis()/1000);
  mujOled.print(" sec");
}
