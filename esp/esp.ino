unsigned long tiempo;
int segundos,minutos,horas;
int ss,mm ,hh;
int militar;
int preset;
int multiplicador = 10;

void setup() {

 Serial.begin(9600);

 delay (2000);

 calibra();

}

void loop() {

 tiempo = millis()*multiplicador;
 segundos = tiempo/1000;
 minutos = segundos/60;
 hh = minutos/60;
 mm = minutos%60; 
 ss = segundos%60;
 militar = ss+(mm*100)+(hh*10000);

 if ((militar>=100)&&(militar<120)) {
  if (preset!=1) {
   preset_1();
  }
 }

  if ((militar>=150)&&(militar<160)) {
  if (preset!=2) {
   preset_2();
  }
 }
 
  delay(1000);
  print();
 
}

void calibra() {
  Serial.println(" -----------------> CALIBRACION");
  delay (3000);
}

void preset_1() {
  preset=1; 
  Serial.println(" -----------------> PRESET 1");
}

void preset_2() {
  preset=2; 
  Serial.println(" -----------------> PRESET 2");
}

void print() {
 if (hh<10) Serial.print("0");
 Serial.print(hh);
 Serial.print(":");
 if (mm<10) Serial.print("0");
 Serial.print(mm);
 Serial.print(":");
 if (ss%60<10) Serial.print("0");
 Serial.print(ss%60);
 Serial.print("------>");
 Serial.println(militar);
 
}




