//----LEDS----
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PINTablero        15
#define PINBandera  16
#define NUMPIXELS 40
#define NUMPIXELSB 20
Adafruit_NeoPixel pixels(NUMPIXELS, PINTablero, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsB(NUMPIXELSB, PINBandera, NEO_GRB + NEO_KHZ800);
uint32_t azul, verde, rojo_claro, marron, morado, naranja, rojo, amarillo, gris;
uint32_t color;

//----JUEGO----
int mapa[10] = {0,0,0,0,0,0,0,0,0,0};
int mapa_2[10] = {0,0,0,0,0,0,0,0,0,0};// 1-> Bandera Plantada, 2-> Casilla descubierta
//int mapa [2][10]= {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};//MAPA TABLERO
//int mapa_2 [2][10] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};//MAPA BANDERAS - 1 = bandera plantada, 2 = casilla descubierta
const int sensorI[10]= {2,3,4,5,6,7,8,9,10,11};
const int banderaI[10] = {22,23,24,25,26,27,28,29,30};
bool generado = false;

void setup() {
  //ESTABLECER LEDS
  pixels.begin();
  pixelsB.begin();
  azul = pixels.Color(0,0,255);
  verde = pixels.Color(0,255,0);
  rojo_claro = pixels.Color(240,57,77);
  marron = pixels.Color(238, 208, 157);
  morado = pixels.Color(255,128,0);
  naranja = pixels.Color(255,128,0);
  rojo = pixels.Color(255,0,0);
  amarillo = pixels.Color(255,255,0);
  gris = pixels.Color(155,155,155);


  //ESTABLECER PINES ENTRADA
  for(int s = 0; s < sizeof(sensorI); s++)
    pinMode(sensorI[s], INPUT);  //definir pin como entrada

  for(int s = 0; s < sizeof(banderaI); s++)
    pinMode(banderaI[s], INPUT);  //definir pin como entrada
  generador();

  pixels.clear();
  pixelsB.clear();
}

void loop() {
  color = "";
  if(!generado){
    generador();
    for(int s = 0; s < NUMPIXELSB; s++){
      pixelsB.setPixelColor(s, amarillo);
      pixelsB.show();
    }
  }
  
  int valueI[10] = {0,0,0,0,0,0,0,0,0,0};
  int vbanderaI[10] = {0,0,0,0,0,0,0,0,0,0};
  //SENSORES DE TABLERO
  for(int s = 0; s < 10; s++){// Escalera Izquierda
    valueI[s] = digitalRead(sensorI[s]);
    if (valueI[s] == HIGH && mapa_2[s] == 0) {
        switch (mapa[s]) {
          case -1:
          color = rojo;
            Perder();
            break;
          case 0: //Sin minas cerca
            color = naranja;
          case 1:
            color = azul;
            break;
          case 2:
            color = verde;
            break;
          case 3:
            color = rojo_claro;
            break;
          case 4:
            color = marron;
            break;
          case 5:
            color = morado;
            break;
          case 6: //Bandera plantada
            color = amarillo;
            break;
          case 7://Mina encontrada
            color = rojo;
            break;
          case 8: //Borrar todos los leds;
            pixels.clear();
            break;
        }
      
      int a = s*4;
      if(s == 0)
        a = 4;
      pixels.setPixelColor(a, color);
      pixels.setPixelColor(a-1, color);
      pixels.setPixelColor(a-2, color);
      pixels.setPixelColor(a-3, color);
      pixels.show();
      mapa[s] = 2;
    }
    vbanderaI[s] = digitalRead(banderaI[s]);
    if(vbanderaI[s] == HIGH){
      if(mapa_2[s] == 0){
        mapa_2[s] == 1;
        int a = s*4;
        if(s == 0)
          a = 4;
        pixels.setPixelColor(a-4, amarillo);
      }

    }
    if(Comprobador())
        Ganar();
    delay(50);
  }
}

bool Comprobador(){
  //Bombas maximas = 3.
  int bombasEncontradas = 0;
  int banderasFalsas = 0;
    for(int v = 0; v < 10; v++){
        if(mapa_2[v] == 1 && mapa[v] == -1){//Si el usuario ha plantado una bandera, el contador suma
          bombasEncontradas++;
        }
        if(mapa_2[v] == 1 && mapa[v] != -1){
          banderasFalsas++;
        }
    }    
   if(banderasFalsas == 0 && bombasEncontradas == 3){
      return true;      
    }else {
      return false;
    }
}
void Ganar(){
  //Animacion de ganar
  pixels.clear();
  pixelsB.clear();
  //Animacion de PERDER
  for(int v = 0; v < 5; v++){
    for(int s = 0; s < NUMPIXELS; s++){
      pixels.setPixelColor(s, verde);
      pixels.show();
    }
    delay(500);
    for(int s = 0; s < NUMPIXELS; s++){
      pixels.setPixelColor(s, azul);
      pixels.show();
    }
  }
  pixels.clear();
  delay(5000);
  generado = false;
  generador();
}

void Perder(){
  pixels.clear();
  pixelsB.clear();
  //Animacion de PERDER
  for(int v = 0; v < 5; v++){
    for(int s = 0; s < NUMPIXELS; s++){
      pixels.setPixelColor(s, rojo);
      pixels.show();
    }
    delay(500);
    for(int s = 0; s < NUMPIXELS; s++){
      pixels.setPixelColor(s, naranja);
      pixels.show();
    }
  }
  pixels.clear();
  delay(5000);
  generado = false;
  generador();
}
void generador() {
    for (int j = 0; j < 10; ++j) {
        mapa[j] = 0;
        mapa_2[j] = 0;
    }
  
  int numBombs = 0;
  while (numBombs < 3) {
      int posY = random(12);
	
      if (mapa[posY] == 0) {
          mapa[posY] = -1;
          numBombs++;
        
      }
  }
    for (int j = 0; j < 12; j++) {
      if (mapa[j] == -1) {
        // La casilla contiene una bomba. Incrementar el valor de las casillas adyacentes.
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) {
              // Saltar la casilla actual.
              continue;
            }
            int columna = j + y;
            if (columna >= 0 && columna < 12 && mapa[columna] != -1) {
              // Incrementar el valor de la casilla adyacente.
              mapa[columna]++;
            }
          }
        }
      }
  }
  generado = true;
}