//----LEDS----
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PINTauler 15
#define PINBandera 16
#define numPixelsT 40
#define numPixelsB 20
Adafruit_NeoPixel pixels(numPixelsT, PINTauler, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsB(numPixelsB, PINBandera, NEO_GRB + NEO_KHZ800);
uint32_t blau, verd, vermellClar, marro, lila, taronja, vermell, groc, grisClar;
uint32_t color;

//----JUEGO----
int mapa[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int mapa_2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//int mapa [2][10]= {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
//int mapa_2 [2][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
const int sensorI[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int banderaI[10] = { 22, 23, 24, 25, 26, 27, 28, 29, 30 };
bool taulerGenerat = false;
int valorTaul[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBand[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorTaulAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
int valorBandAnt[10] = { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH };
float reiniciaPart = 180000;
bool gatTrobat = false;
float ultimCanvi;

void setup() {
  //ESTABLECER LEDS
  pixels.begin();
  pixelsB.begin();
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
  grisClar = pixels.Color(100, 100, 100);
  ultimCanvi = 0;


  for (int s = 0; s < sizeof(sensorI); s++)
    pinMode(sensorI[s], INPUT);

  for (int s = 0; s < sizeof(banderaI); s++)
    pinMode(banderaI[s], INPUT);
  generador();

  pixels.clear();
  pixelsB.clear();
}

void loop() {
  color = 0;
  if (millis() - ultimCanvi >= reiniciaPart) {
    Perdre();
  }
  if (!taulerGenerat) {
    generador();
  }



  for (int numPin = 0; numPin < 10; numPin++) {
    valorTaul[numPin] = digitalRead(sensorI[numPin]);
    if (valorTaulAnt[numPin] != valorTaul[numPin]) {
      if (valorTaul[numPin] == LOW && mapa_2[numPin] == 0) {
        switch (mapa[numPin]) {
          case -1:
            color = vermell;
            gatTrobat = true;
            break;
          case 0:
            color = taronja;
            break;
          case 1:
            color = blau;
            break;
          case 2:
            color = verd;
            break;
          case 3:
            color = vermellClar;
            break;
          case 4:
            color = marro;
            break;
          case 5:
            color = lila;
            break;
          case 6:
            pixels.clear();
            break;
        }

        int numLED = numPin * 4;
        pixels.setPixelColor(numLED, color);
        pixels.setPixelColor(numLED + 1, color);
        pixels.setPixelColor(numLED + 2, color);
        pixels.setPixelColor(numLED + 3, color);
        pixels.show();
        mapa[numPin] = 2;
        ultimCanvi = millis();
      }
      valorTaulAnt[numPin] = valorTaul[numPin];
    }

    valorBand[numPin] = digitalRead(banderaI[numPin]);
    if (valorBand[numPin] != valorBandAnt[numPin]) {
      if (valorBand[numPin] == LOW) {
        if (mapa_2[numPin] == 0) {
          mapa_2[numPin] = 1;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, groc);
          pixelsB.setPixelColor(numLED + 1, groc);
          pixelsB.show();

        } else if (mapa_2[numPin] == 1) {
          mapa_2[numPin] = 0;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, grisClar);
          pixelsB.setPixelColor(numLED + 1, grisClar);
          pixelsB.show();
        }
        ultimCanvi = millis();
      }
      valorBandAnt[numPin] = valorBand[numPin];
    }
    if (gatTrobat == true) {
      gatTrobat = false;
      Perdre();
    }

    if (Comprovador())
      Guanyar();
  }

  delay(60);
}

bool Comprovador() {
  //Bombas maximas = 3.
  int gatsTrobats = 0;
  int banderesFalses = 0;
  for (int v = 0; v < 10; v++) {
    if (mapa_2[v] == 1 && mapa[v] == -1) {
      gatsTrobats++;
    }
    if (mapa_2[v] == 1 && mapa[v] != -1) {
      banderesFalses++;
    }
  }
  if (banderesFalses == 0 && gatsTrobats == 3) {
    return true;
  } else {
    return false;
  }
}

void Guanyar() {
  delay(1000);
  for (int v = 0; v < 5; v++) {
    for (int s = 0; s <= numPixelsT; s++) {
      pixels.setPixelColor(s, verd);
      pixelsB.setPixelColor(s, verd);
      pixels.show();
      pixelsB.show();
    }
    delay(500);
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
  taulerGenerat = false;
  generador();
}

void Perdre() {
  delay(1000);
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
  generador();
}
void generador() {
  for (int i = 0; i < numPixelsT; i++) {
    pixels.setPixelColor(i, grisClar);
  }
  pixels.show();
  for (int k = 0; k < numPixelsB; k++) {
    pixelsB.setPixelColor(k, grisClar);
  }
  pixelsB.show();
  for (int j = 0; j < 10; j++) {
    mapa[j] = 0;
    mapa_2[j] = 0;
  }

  int numGats = 0;
  while (numGats < 3) {
    int posX = random(10);

    if (mapa[posX] == 0) {
      mapa[posX] = -1;
      numGats++;
    }
  }
  for (int l = 0; l < 10; l++) {
    if (mapa[l] == -1) {
      for (int x = -1; x <= 1; x++) {

        if (x == 0) {
          continue;
        }
        int fila = l + x;
        if (fila >= 0 && fila < 10 && mapa[fila] != -1) {
          mapa[fila]++;
        }
      }
    }
  }
  taulerGenerat = true;
  ultimCanvi = millis();
}