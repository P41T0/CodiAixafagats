int detector=5;
int detector2 = 6;
void setup() {
  Serial.begin(9600);
  pinMode(detector,INPUT_PULLUP);
  pinMode(detector2,INPUT);
}

void loop() {
  int value = HIGH;
  int value2 = HIGH;
  value = digitalRead(detector);
  value2 = digitalRead(detector2);
  int S=1;
   if (value == LOW){
         Serial.print("Detectado obstaculo 1");
         Serial.println(S);
  }
  
  delay (1000);
  }