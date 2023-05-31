//----LEDS----
#include <FastLED_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h>
//#endif
#define PinLedsTauler 15
#define PinLedsBandera 16
#define numLedsT 40

#define numLedsB 20  //Tots els valors necessaris per a inicialitzar els LEDS

FastLED_NeoPixel<numLedsT, PinLedsTauler, NEO_GRB> pixelsT;
FastLED_NeoPixel<numLedsB, PinLedsBandera, NEO_GRB> pixelsB;

const uint32_t blau = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t verd = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t vermellClar = Adafruit_NeoPixel::Color(240, 57, 77);
const uint32_t marro = Adafruit_NeoPixel::Color(238, 208, 157);
const uint32_t lila = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t taronja = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t vermell = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t groc = Adafruit_NeoPixel::Color(255, 255, 0);
const uint32_t gris = Adafruit_NeoPixel::Color(0, 15, 0);  //Valors de tots els colors
const uint32_t negre = Adafruit_NeoPixel::Color(0, 0, 0);  //Valors de tots els colors
uint32_t color;

//----JUEGO----
int mapaJoc[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int mapaAccionsJug[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //mapa del joc i de les accions del jugador
const int sensorsTaulerI[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int sensorsBanderaI[10] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 };  //sensors de de descobrir les caselles i de col·locar banderes
const int numSensors = 10;
bool taulerGenerat = false;
int valorTaul[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBand[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };  //Llistes que contenen els valors actuals del que detecten els sensors
int valorTaulAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBandAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };  //Llistes que contenen els valors antics del que han detectat els senosrs
uint32_t colorLedsT[10] = { gris, gris, gris, gris, gris, gris, gris, gris, gris, gris };
uint32_t colorLedsB[10] = { gris, gris, gris, gris, gris, gris, gris, gris, gris, gris };  //Es guarda el color de cada secció de la tira de leds
float reiniciaPart = 180000;                                                               // temps màxim sense canvis per a que es reinicii el joc (3 minuts)
bool gatTrobat = false;                                                                    //Bolea que indica si s'ha descobert un gat
bool canviTauler = false;
bool canviBanderes = false;
float ultimCanvi;


void setup() {
  //ESTABLECER LEDS
  pixelsT.begin();
  pixelsB.begin();
  pixelsT.setBrightness(80);
  pixelsB.setBrightness(80);  //S'inicialitzen les dues tires de LEDs i es defineix la seva brillantor
  ultimCanvi = 0;             //S'inicialitza l'ultim cop que s'ha detectat un canvi



  for (int s = 0; s < numSensors; s++)
    pinMode(sensorsTaulerI[s], INPUT_PULLUP);

  for (int s = 0; s < numSensors; s++)
    pinMode(sensorsBanderaI[s], INPUT_PULLUP);
  Generador();  //S'inicialitzen els pins dels sensors i es genera el mapa
}

void loop() {
  if (millis() - ultimCanvi >= reiniciaPart) {  //Si no hi ha hagut cap canvi en la partida durant 3 minuts, es crida la funció perdre i es reinicia la partida
    Perdre();
  }
  if (!taulerGenerat) {  //Per si de cas el tauler no s'ha generat correctament, es torna a cridar al generador
    Generador();
  }



  for (int numPin = 0; numPin < numSensors; numPin++) {               //Per a cada sensor
    valorTaul[numPin] = digitalRead(sensorsTaulerI[numPin]);          //Es guarda el valor actual que retorna el sensor, HIGH o LOW
    if (valorTaulAnt[numPin] != valorTaul[numPin]) {                  //Si el valor és diferent al detectat anteriorment
      if (valorTaul[numPin] == LOW && mapaAccionsJug[numPin] == 0) {  //Si és LOW i aquella casella no te cap bandera col·locada ni s'ha descobert
        switch (mapaJoc[numPin]) {                                    //Depenent del valor del mapejat del joc es realitza alguna de les seguents accions
          case -1:
            color = vermell;
            gatTrobat = true;  //s'ha descobert un gat
            break;
          case 0:
            color = taronja;  //No hi ha cap gat a prop
            break;
          case 1:
            color = blau;  //Hi ha un gat a prop
            break;
          case 2:
            color = verd;  //Hi ha dos gats a prop
            break;
          case 3:
            color = vermellClar;  //Hi ha tres gats a prop
            break;
          case 4:
            color = lila;  //Hi ha quatre gats a prop
            break;
          case 5:
            color = marro;  //Hi ha cinc gats a prop
            break;
          case 6:
            color = groc;  //Bandera
            break;
          case 7:
            color = vermell;  //Gat
            break;
          case 8:
            pixelsT.clear();  //Error
            break;
        }

        colorLedsT[numPin] = color;  //Es guarda el color amb el que es pintara aquella seccio de la tira de LEDs
      }
      //Es mostren els colors
      mapaAccionsJug[numPin] = 2;  //Es guarda que el jugador ha descobert la casella
      ultimCanvi = millis();       //Es guarda el moment de l'ultim canvi
      canviTauler = true;
    }
    valorTaulAnt[numPin] = valorTaul[numPin];  //Es modifica el valor antic del sensor
  }
  if (canviTauler == true) {
    canviTauler = false;
    for (int numPin = 0; numPin < numSensors; numPin++) {
      for (int i = 0; i < 10; i++) {        //Per a tots els sectors de la tira de LEDs
        int led = i * 4;                    //cada sector esta compost per 4 leds
        uint32_t colorLed = colorLedsT[i];  //S'agafa el color del que s'ha de pintar aquella part de la tira de leds
        pixelsT.setPixelColor(led, pixelsT.Color(colorLed));
        pixelsT.setPixelColor(led + 1, pixelsT.Color(colorLed));
        pixelsT.setPixelColor(led + 2, pixelsT.Color(colorLed));
        pixelsT.setPixelColor(led + 3, pixelsT.Color(colorLed));  //Es pinta cada seccio de quatre leds de la tira del tauler del color especificat a la llista
      }
    }
    pixelsT.show();  //Es mostra la llista
  }
  if (gatTrobat == true) {
    gatTrobat = false;
    Perdre();  //Si s'ha trobat un gat, es crida a perdre i es reinicia la partida
  }
  for (int numPin = 0; numPin < numSensors; numPin++) {
    valorBand[numPin] = digitalRead(sensorsBanderaI[numPin]);  //Es llegeix el valor del LED de les banderes
    if (valorBand[numPin] != valorBandAnt[numPin]) {           //Si es diferent al valor llegit anteriorment
      if (valorBand[numPin] == LOW) {                          //Si esta a LOW
        if (mapaAccionsJug[numPin] == 0) {                     //Si en el mapa d'accions del jugador aquella casella esta a 0
          mapaAccionsJug[numPin] = 1;                          //Es guarda que el jugador ha col·locat una bandera
          colorLedsB[numPin] = groc;

        } else if (mapaAccionsJug[numPin] == 1) {  //Si ja hi havia una bandera col·locada
          mapaAccionsJug[numPin] = 0;
          colorLedsB[numPin] = gris;  //Es guarda que el jugador ha tret la bandera i es canvia el color d'aquell sector dels LEDs a gris un altre cop
        }
      }
      ultimCanvi = millis();  //Es guarda el moment de l'ultim canvi
      canviBanderes = true;

      valorBandAnt[numPin] = valorBand[numPin];  //Es guarda el valor actual del sensor com a valor antic
    }
  }
  if (canviBanderes == true) {
    canviBanderes = false;
    for (int i = 0; i < 10; i++) {        //Per a tots els sectors de la tira de LEDs de les banderes
      int led = i * 2;                    //Cada sector esta compost per dos LEDs
      uint32_t colorLed = colorLedsB[i];  //Es pinten del color especificat en aquell lloc de la llista colorLEDs
      pixelsB.setPixelColor(led, pixelsB.Color(colorLed));
      pixelsB.setPixelColor(led + 1, pixelsB.Color(colorLed));
    }
    pixelsB.show();  //Es mostren els LEDs que s'han canviat
  }
  if (Comprovador()) {
    Guanyar();
  }

  delay(60);  //Petit delay de 60 milisegons
}

bool Comprovador() {
  int gatsTrobats = 0;     //Casella amb bandera on hi ha un gat
  int banderesFalses = 0;  //Casella amb bandera on no hi ha cap gat
  for (int v = 0; v < 10; v++) {
    if (mapaAccionsJug[v] == 1 && mapaJoc[v] == -1) {  //Si hi ha un gat en aquella casella i s'ha plantat una bandera incrementa el numero de gats trobats
      gatsTrobats++;
    }
    if (mapaAccionsJug[v] == 1 && mapaJoc[v] != -1) {  //Si no hi ha un gat en aquella casella i s'ha plantat una bandera incrementa el numero de banderes falses
      banderesFalses++;
    }
  }
  if (banderesFalses == 0 && gatsTrobats == 3) {  //Si s'han trobat tots els gats i no hi ha cap bandera falsa retorna true, si no retorna false
    return true;
  } else {
    return false;
  }
}

void Guanyar() {  //Animació de victoria
  delay(1000);
  for (int v = 0; v < 5; v++) {  //durant 5 cops
    for (int t = 0; t < pixelsT.numPixels(); t++) {
      if (t % 4 == 0) {
        pixelsT.setPixelColor(t, pixelsT.Color(negre));
      } else {
        pixelsT.setPixelColor(t, pixelsT.Color(verd));  //Nomes es pinten els LEDs del tauler que el seu numero no es multiple de 4
      }
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, pixelsB.Color(verd));
    }
    pixelsT.show();
    //pixelsB.show();  //es pinten tots els LEDs de color verd i es mostren durant 0,5 segons
    delay(500);
    for (int t = 0; t < pixelsT.numPixels(); t++) {
      if (t % 4 == 0) {
        pixelsT.setPixelColor(t, pixelsT.Color(negre));
      } else {
        pixelsT.setPixelColor(t, pixelsT.Color(blau));  //Nomes es pinten els LEDs del tauler que el seu numero no es multiple de 4
      }
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, pixelsB.Color(blau));
    }
    pixelsT.show();
    pixelsB.show();  //Es pinten tots els LEDs de color blau i es mostren durant 0,5 segons
    delay(500);
  }
  pixelsT.clear();
  pixelsB.clear();
  pixelsT.show();
  pixelsB.show();  //"S'esborren" els colors de tots els LEDs
  delay(5000);
  taulerGenerat = false;
  Generador();  //Després d'un delay de 5 segons, es reinicia la partida
}

void Perdre() {  //Animacio de perdre
  delay(1000);
  for (int v = 0; v < 5; v++) {
    for (int t = 0; t < pixelsT.numPixels(); t++) {
      if (t % 4 == 0) {
        pixelsT.setPixelColor(t, pixelsT.Color(negre));
      } else {
        pixelsT.setPixelColor(t, pixelsT.Color(vermell));  //Nomes es pinten els LEDs del tauler que el seu numero no es multiple de 4
      }
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, pixelsB.Color(vermell));
    }
    pixelsT.show();
    pixelsB.show();  //Es pinten tots els LEDs de color vermell i es mostren durant 0,5 segons
    delay(500);
    for (int t = 0; t < pixelsT.numPixels(); t++) {
      if (t % 4 == 0) {
        pixelsT.setPixelColor(t, pixelsT.Color(negre));
      } else {
        pixelsT.setPixelColor(t, pixelsT.Color(taronja));  //Nomes es pinten els LEDs del tauler que el seu numero no es multiple de 4
      }
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, pixelsB.Color(taronja));
    }
    pixelsT.show();
    pixelsB.show();  //Es pinten tots els LEDs de color taronja i es mostren durant 0,5 segons
    delay(500);
  }
  pixelsT.clear();
  pixelsB.clear();
  pixelsT.show();
  pixelsB.show();  //"S'esborren" els valors de tots els LEDs i es mostren
  delay(5000);
  taulerGenerat = false;
  Generador();  //Al cap de 5 segons, es reinicia la partida
}
void Generador() {
  for (int j = 0; j < 10; j++) {
    mapaJoc[j] = 0;
    mapaAccionsJug[j] = 0;  //Es posen tots els valors dos mapes, el del joc i el de les accions del jugador, a 0
  }
  randomSeed(analogRead(A0));  //Cada cop es fa un randomSeed que rep el valor que rep el pin A0, al qual no hi ha connectat res, per a que el Random sigui diferent a cada partida i la generació dels gats varii
  int numGats = 0;
  while (numGats < 3) {        //Mentre el nombre de gats sigui més baix que 3
    int posX = random(0, 10);  //Es genera un gat en una posicio aleatoria entre 0 i 10

    if (mapaJoc[posX] == 0) {
      mapaJoc[posX] = -1;
      numGats++;  //Si en aquella posicio no hi ha cap gat, s'hi col·loca un gat i incrementa el numero de gats
    }
  }
  for (int l = 0; l < 10; l++) {  //En tot el nombre de caselles
    if (mapaJoc[l] == -1) {       //Si en aquella casella del mapa de joc s'hi troba un gat
      for (int x = -1; x <= 1; x++) {

        if (x == 0) {  //Si és la propia casella es salta
          continue;
        }
        int fila = l + x;                                     //fila = numero de casella + valor anterior o seguent al d'un gat
        if (fila >= 0 && fila < 10 && mapaJoc[fila] != -1) {  //Si la posicio de la casella es mes gran que 0 i mes petita que 10, i no s'hi troba un gat
          mapaJoc[fila]++;                                    //incrementa el valor de la casella
        }
      }
    }
  }
  for (int i = 0; i < 40; i++) {
    if (i < 10) {
      colorLedsT[i] = gris;
    }
    if (i % 2 == 0) {
      pixelsT.setPixelColor(i, negre);
    } else {
      pixelsT.setPixelColor(i, gris);  //Per als LEDs del tauler de joc, nomes es pinten de color gris els que el seu numero no es multiple de 2
    }
  }
  for (int k = 0; k < 20; k++) {
    if (k < 10) {
      colorLedsB[k] = gris;
    }
    pixelsB.setPixelColor(k, gris);  //Es canvia el color de tots els LEDs a gris un altre cop
  }
  pixelsT.show();
  pixelsB.show();         //Pinta tots els LEDs de color gris
  taulerGenerat = true;   //Bolea que confirma que s'ha generat el tauler
  ultimCanvi = millis();  //Es guarda el moment que s'ha realitzat l'ultim canvi
}