//----LEDS----
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PINTauler 15                                                              //pins on es connecten els leds del tauler de joc
#define PINBandera 16                                                             //pins on es connecten els LEDs de les banderes
#define numPixelsT 40                                                             //Número de LEDs que estan al tauler
#define numPixelsB 20                                                             //Número de LEDs que estan al tauler dels gats
Adafruit_NeoPixel pixels(numPixelsT, PINTauler, NEO_GRB + NEO_KHZ800);            //Numero de pixels del tauler de joc, pin del tauler, model dels LEDS
Adafruit_NeoPixel pixelsB(numPixelsB, PINBandera, NEO_GRB + NEO_KHZ800);          //Numero de pixels del tauler dels gats, pin del tauler dels gats, model dels LEDS
uint32_t blau, verd, vermellClar, marro, lila, taronja, vermell, groc, grisClar;  //tots els colors dels que es poden pintar els LEDs
uint32_t color;                                                                   //El color

//----JUEGO----
int mapa[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    //Tauler de joc
int mapa_2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //0->casella sense descobrir 1-> Bandera Plantada, 2-> Casilla descoberta
//int mapa [2][10]= {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};//MAPA del tauler 2
//int mapa_2 [2][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};//MAPA BANDERAS del segon tauler - 1 = bandera plantada, 2 = casilla descubierta
const int sensorI[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };       //Pins on estan connectats els sensors de distancia encarregats de detectar si es descobreix una casella
const int banderaI[10] = { 22, 23, 24, 25, 26, 27, 28, 29, 30 };  //Pins on estan connectats els sensors encarregats de detectar si es planta una bandera
bool taulerGenerat = false;
int valorTaul[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBand[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorTaulAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBandAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
float reiniciaPart = 180000;
bool gat = false;
float ultimCanvi;

void setup() {
  //ESTABLECER LEDS
  pixels.begin();   //s'inicien els LEDs del tauler de joc
  pixelsB.begin();  //s'inicien els LEDs de les banderes
  pixels.setBrightness(255);
  pixelsB.setBrightness(255);
  pixels.show();
  pixelsB.show();
  blau = pixels.Color(0, 0, 255);
  verd = pixels.Color(0, 255, 0);
  vermellClar = pixels.Color(240, 57, 77);
  marro = pixels.Color(238, 208, 157);
  lila = pixels.Color(255, 128, 0);
  taronja = pixels.Color(255, 128, 0);
  vermell = pixels.Color(255, 0, 0);
  groc = pixels.Color(255, 255, 0);
  grisClar = pixels.Color(100, 100, 100);  //Els diferents colors dels quals es pinten els LEDs
  ultimCanvi = 0;


  for (int s = 0; s < sizeof(sensorI); s++)
    pinMode(sensorI[s], INPUT);  //es defineixen com a entrada tots els pins dels sensors que detecten si es descobreix una casella

  for (int s = 0; s < sizeof(banderaI); s++)
    pinMode(banderaI[s], INPUT);  //es defineixen com a entrada tots els pins dels sensors encarregats de plantar una bandera
  generador();                    //es crida al generador

  pixels.clear();
  pixelsB.clear();  //s'apaguen tots els LEDs
}

void loop() {
  color = "";
  if (millis() - ultimCanvi >= reiniciaPart) {
    Perdre();
  }
  if (!taulerGenerat) {  //bolea que detecta si s'ha generat el tauler de joc
    generador();
  }


  //SENSORES DEL TAULER i el valor del que detecten
  for (int numPin = 0; numPin < 10; numPin++) {        // Per cada pin on es trobi el sensor
    valorTaul[numPin] = digitalRead(sensorI[numPin]);  //Es guarda el valor del sensor ubicat en la posicio s
    if (valorTaulAnt[numPin] != valorTaul[numPin]) {
      if (valorTaul[numPin] == LOW && mapa_2[numPin] == 0) {  //Si el valor s és alt i no s'ha descobert la casella
        switch (mapa[numPin]) {
          case -1:
            color = vermell;  //si s'ha detectat un gat es pinta de color vermell la casella i es perd la partida
            gat = true;
            break;
          case 0:
            color = taronja;  //si no hi ha mines a prop es pinta de color taronja
          case 1:
            color = blau;  //si hi ha un gat a prop es pinta de color blau
            break;
          case 2:
            color = verd;  // si hi ha dos gats a prop, es pinta de color verd
            break;
          case 3:
            color = vermellClar;  // si hi ha 3 gats a prop, es pinta de color vermell clar
            break;
          case 4:
            color = marro;  // si hi ha 4 gats a prop, es pinta de color marró
            break;
          case 5:
            color = lila;  //si hi ha 5 gats a prop, es pinta de color lila
            break;
          case 6:  //Borrar todos los leds;
            pixels.clear();
            break;
        }

        int numLED = numPin * 4;
        pixels.setPixelColor(numLED, color);
        pixels.setPixelColor(numLED + 1, color);
        pixels.setPixelColor(numLED + 2, color);
        pixels.setPixelColor(numLED + 3, color);  //Els LEDs que es canviaran de color
        pixels.show();
        if (gat == true) {
          Perdre();
        }
        mapa[numPin] = 2;
        ultimCanvi = millis();
      }
      valorTaulAnt[numPin] = valorTaul[numPin];
    }

    valorBand[numPin] = digitalRead(banderaI[numPin]);
    if (valorBand[numPin] != valorBandAnt[numPin]) {
      if (valorBand[numPin] == LOW) {
        if (mapa_2[numPin] == 0) {
          mapa_2[numPin] == 1;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, groc);
          pixelsB.setPixelColor(numLED + 1, groc);
          pixelsB.show();

        } else if (mapa_2[numPin] == 1) {
          mapa_2[numPin] == 0;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, grisClar);
          pixelsB.setPixelColor(numLED + 1, grisClar);
          pixelsB.show();
        }
        ultimCanvi = millis();
      }
      valorBandAnt[numPin] = valorBand[numPin];
    }
    if (Comprobador())
      Guanyar();
  }

  delay(50);  //si el comprovador esta a cert, es guanya la partida, si no aquesta continua
}

bool Comprobador() {
  //Bombas maximas = 3.
  int bombasEncontradas = 0;
  int banderasFalsas = 0;
  for (int v = 0; v < 10; v++) {
    if (mapa_2[v] == 1 && mapa[v] == -1) {  //Si l'usuari posa una bandera on hi ha un gat
      bombasEncontradas++;
    }
    if (mapa_2[v] == 1 && mapa[v] != -1) {  //si hi ha banderes on no hi ha gats
      banderasFalsas++;
    }
  }
  if (banderasFalsas == 0 && bombasEncontradas == 3) {  //si nomès hi ha banderes on es localitzen les mines
    return true;
  } else {
    return false;
  }
}

void Guanyar() {
  delay(1000);
  //Animacio de guanyar
  for (int v = 0; v < 5; v++) {
    for (int s = 0; s <= numPixelsT; s++) {
      pixels.setPixelColor(s, verd);
      pixelsB.setPixelColor(s, verd);
      pixels.show();
      pixelsB.show();
    }
    delay(500);
    //animacio de guanyar
    for (int s = 0; s <= numPixelsT; s++) {
      pixels.setPixelColor(s, blau);
      pixelsB.setPixelColor(s, blau);
      pixels.show();
      pixelsB.show();
    }
  }
  pixels.clear();
  pixelsB.clear();
  pixels.show();
  pixelsB.show();
  delay(5000);
  //es torna a generar un mapa nou al cap de 5 segons
  taulerGenerat = false;
  generador();
}

void Perdre() {
  delay(1000);
  //Animacion de PERDER
  for (int v = 0; v < 5; v++) {
    for (int s = 0; s < numPixelsT; s++) {
      pixels.setPixelColor(s, vermell);
      pixelsB.setPixelColor(s, vermell);
    }
    pixels.show();
    pixelsB.show();
    delay(500);
    for (int s = 0; s < numPixelsT; s++) {
      pixels.setPixelColor(s, taronja);
      pixelsB.setPixelColor(s, taronja);
    }
    pixels.show();
    pixelsB.show();
  }
  pixels.clear();
  pixelsB.clear();
  pixels.show();
  pixelsB.show();
  delay(5000);
  taulerGenerat = false;
  //es torna a generar un altre mapa al cap de 5 segons
  generador();
}
void generador() {
  //generador del mapa
  for (int i = 0; i < numPixelsT; i++) {
    pixels.setPixelColor(i, grisClar);
  }
  pixels.show();
  for (int k = 0; k < numPixelsB; k++) {
    pixelsB.setPixelColor(k, grisClar);
  }
  pixelsB.show();
  for (int j = 0; j < 10; ++j) {
    mapa[j] = 0;
    mapa_2[j] = 0;
  }

  int numGats = 0;
  while (numGats < 3) {
    int posX = random(10);  //posa banderes en un valor entre 0 i 10

    if (mapa[posX] == 0) {  //si en aquella casella no hi ha cap gat
      mapa[posX] = -1;
      numGats++;  //suma el numero de banderes i col·loca un gat en aquell lloc
    }
  }
  for (int j = 0; j < 10; j++) {
    if (mapa[j] == -1) {
      // La casilla contiene una bomba. Incrementar el valor de las casillas adyacentes.
      for (int x = -1; x <= 1; x++) {

        if (x == 0) {
          // Saltar la casilla actual.
          continue;
        }
        int fila = j + x;
        if (fila >= 0 && fila < 10 && mapa[fila] != -1) {
          // Incrementar el valor de la casilla adyacente.
          mapa[fila]++;
        }
      }
    }
  }
  taulerGenerat = true;
}