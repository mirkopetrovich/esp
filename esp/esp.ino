unsigned long tiempo;
int segundos,minutos,horas;
int ss,mm ,hh;
int militar;
int preset = -1;
int multiplicador = 1;

// MOTOR 2





void setup() {

  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(7, LOW);

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

  if ((militar>=5)&&(militar<6)) {
  if (preset!=0) {
   preset_0(); //  ATOMIZADORES
  }
 }

 if ((militar>=10)&&(militar<11)) {
  if (preset!=1) {
   preset_1(); //  abre
  }
 }

  if ((militar>=12)&&(militar<13)) {
  if (preset!=2) {
   preset_2(); // apaga
  }
 }
 
   if ((militar>=15)&&(militar<16)) {
  if (preset!=3) {
   preset_3(); // ciera
  }
 }
 
   if ((militar>=17)&&(militar<18)) {
  if (preset!=4) {
   preset_4(); //apaga
  }
 }
 
  delay(1000);
  print();
 
}

void calibra() {
  Serial.println(" -----------------> CALIBRACION");
  delay (3000);
}

void preset_0() {
  preset=0; 
  Serial.println(" -----------------> ATOMIZADORES");
  digitalWrite(7,HIGH); 
  
  
}


void preset_1() {
  preset=1; 
  Serial.println(" -----------------> PRESET 1");
  digitalWrite(23,HIGH);  //abre
  digitalWrite(25,LOW);
  
}

void preset_2() {
  preset=2; 
  Serial.println(" -----------------> PRESET 2");
 digitalWrite(23,HIGH);  
  digitalWrite(25, HIGH);
}

void preset_3() {
  preset=3; 
  Serial.println(" -----------------> PRESET 3");
  digitalWrite(23,LOW);
  digitalWrite(25, HIGH);
}

void preset_4() {
  preset=4; 
  Serial.println(" -----------------> PRESET 4");
  digitalWrite(23,HIGH);
  digitalWrite(25, HIGH);
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








