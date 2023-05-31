//----LEDS----
#include <Adafruit_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h>
//#endif
#define PinLedsTauler 15
#define PinLedsBandera 16
#define numLedsT 40
#define numLedsB 20

const uint32_t blau = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t verd = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t vermellClar = Adafruit_NeoPixel::Color(240, 57, 77);
const uint32_t marro = Adafruit_NeoPixel::Color(238, 208, 157);
const uint32_t lila = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t taronja = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t vermell = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t groc = Adafruit_NeoPixel::Color(255, 255, 0);
const uint32_t grisClar = Adafruit_NeoPixel::Color(100, 100, 100);
uint32_t color;

Adafruit_NeoPixel pixels(numLedsT, PinLedsTauler, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsB(numLedsB, PinLedsBandera, NEO_GRB + NEO_KHZ800);


//----JUEGO----
int mapaJoc[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int mapaAccionsJug[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//int mapa [2][10]= {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
//int mapa_2 [2][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
const int sensorsTaulerI[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int sensorsBanderaI[10] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
const int numSensors = 10;
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
  pixels.setBrightness(200);
  pixelsB.setBrightness(200);
  ultimCanvi = 0;


  for (int s = 0; s < numSensors; s++)
    pinMode(sensorsTaulerI[s], INPUT);

  for (int s = 0; s < numSensors; s++)
    pinMode(sensorsBanderaI[s], INPUT);
  Generador();
}

void loop() {
  color = Adafruit_NeoPixel::Color(0, 0, 0);
  if (millis() - ultimCanvi >= reiniciaPart) {
    Perdre();
  }
  if (!taulerGenerat) {
    Generador();
  }



  for (int numPin = 0; numPin < numSensors; numPin++) {
    valorTaul[numPin] = digitalRead(sensorsTaulerI[numPin]);
    if (valorTaulAnt[numPin] != valorTaul[numPin]) {
      if (valorTaul[numPin] == LOW && mapaAccionsJug[numPin] == 0) {
        switch (mapaJoc[numPin]) {
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
        mapaJoc[numPin] = 2;
        ultimCanvi = millis();
      }

      valorTaulAnt[numPin] = valorTaul[numPin];
    }
    if (gatTrobat == true) {
      gatTrobat = false;
      Perdre();
    }
    valorBand[numPin] = digitalRead(sensorsBanderaI[numPin]);
    if (valorBand[numPin] != valorBandAnt[numPin]) {
      if (valorBand[numPin] == LOW) {
        if (mapaAccionsJug[numPin] == 0) {
          mapaAccionsJug[numPin] = 1;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, groc);
          pixelsB.setPixelColor(numLED + 1, groc);
          pixelsB.show();

        } else if (mapaAccionsJug[numPin] == 1) {
          mapaAccionsJug[numPin] = 0;
          int numLED = numPin * 2;
          pixelsB.setPixelColor(numLED, grisClar);
          pixelsB.setPixelColor(numLED + 1, grisClar);
          pixelsB.show();
        }
        ultimCanvi = millis();
      }
      valorBandAnt[numPin] = valorBand[numPin];
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
    if (mapaAccionsJug[v] == 1 && mapaJoc[v] == -1) {
      gatsTrobats++;
    }
    if (mapaAccionsJug[v] == 1 && mapaJoc[v] != -1) {
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
    for (int t = 0; t < pixels.numPixels(); t++) {
      pixels.setPixelColor(t, verd);
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, verd);
    }
    pixels.show();
    pixelsB.show();
    delay(500);
    for (int t = 0; t < pixels.numPixels(); t++) {
      pixels.setPixelColor(t, blau);
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, blau);
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
  Generador();
}

void Perdre() {
  delay(1000);
  for (int v = 0; v < 5; v++) {
    for (int t = 0; t < pixels.numPixels(); t++) {
      pixels.setPixelColor(t, vermell);
    }
    for (int b = 0; b < pixelsB.numPixels(); b++) {
      pixelsB.setPixelColor(b, vermell);
    }
    pixels.show();
    pixelsB.show();
    delay(500);
    for (int s = 0; s < pixels.numPixels(); s++) {
      pixels.setPixelColor(s, taronja);
    }
    for (int s = 0; s < pixelsB.numPixels(); s++) {
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
  Generador();
}
void Generador() {
  for (int j = 0; j < 10; j++) {
    mapaJoc[j] = 0;
    mapaAccionsJug[j] = 0;
  }

  int numGats = 0;
  while (numGats < 3) {
    int posX = random(0, 10);

    if (mapaJoc[posX] == 0) {
      mapaJoc[posX] = -1;
      numGats++;
    }
  }
  for (int l = 0; l < 10; l++) {
    if (mapaJoc[l] == -1) {
      for (int x = -1; x <= 1; x++) {

        if (x == 0) {
          continue;
        }
        int fila = l + x;
        if (fila >= 0 && fila < 10 && mapaJoc[fila] != -1) {
          mapaJoc[fila]++;
        }
      }
    }
  }
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, grisClar);
  }
  for (int k = 0; k < pixelsB.numPixels(); k++) {
    pixelsB.setPixelColor(k, grisClar);
  }
  pixels.show();
  pixelsB.show();
  taulerGenerat = true;
  ultimCanvi = millis();
}