unsigned long tiempo;
int segundos,minutos,horas,psegundos;
int ss,mm ,hh;
int militar;
int preset = -1;
int multiplicador = 1;


//-------------------------------------------------------
//VARIABLES ATOMIZADORES

bool atom_status;
int pin_atom = 7;
unsigned long t_inicio_atom;
unsigned long t_transcurrido_atom;
unsigned long t_fin_atom;
unsigned long t_apagado_atom;
unsigned long t_dur_on_atom = 40; //segundos
unsigned long t_dur_off_atom = 300; // 5 min

// -------------------------------------------------------
// VARIABLES EXTRACTORES

bool x1a_status,x1b_status,x2a_status,x2b_status,x3a_status,x3b_status;

int pin_X1A = 26;
int pin_X1B = 28;
int pin_X2A = 30;
int pin_X2B = 32;
int pin_X3A = 34;
int pin_X3B = 36;

// ---------------------------------------------------------
// VARIABLES VÁLVULAS

int sensor_0,sensor_1,sensor_2,sensor_3;

int pin_M0a = 23;
int pin_M0b = 25;
int pin_M1a = 27;
int pin_M1b = 29;
int pin_M2a = 31;
int pin_M2b = 33;
int pin_M3a = 35;
int pin_M3b = 37;

// -----------------------------------------------------------

void setup() {

  pinMode(pin_M0a, OUTPUT); // VÁLVULAS
  pinMode(pin_M0b, OUTPUT);
  pinMode(pin_M1a, OUTPUT);
  pinMode(pin_M1b, OUTPUT);
  pinMode(pin_M2a, OUTPUT);
  pinMode(pin_M2b, OUTPUT);
  pinMode(pin_M3a, OUTPUT);
  pinMode(pin_M3b, OUTPUT);

  pinMode(pin_X1A,OUTPUT); // EXTRACTORES
  pinMode(pin_X1B,OUTPUT);
  pinMode(pin_X2A,OUTPUT);
  pinMode(pin_X2B,OUTPUT);
  pinMode(pin_X3A,OUTPUT);
  pinMode(pin_X3B,OUTPUT);

  pinMode(pin_atom,OUTPUT); // ATOMIZADORES (LOW=apaga)
 
 Serial.begin(9600);

  delay(1000);

  Serial.println("        __.....__");
  Serial.println("     .'\" _  o    \"`.");
  Serial.println("   .' O (_)     () o`.");
  Serial.println("  .           O       .");
  Serial.println(" . ()   o__...__    O  .");
  Serial.println(". _.--\"       \"\"\"--._ .");
  Serial.println(":\"                     \";");
  Serial.println(" `-.__    :   :    __.-'");
  Serial.println("      \"\"\"-:   :-\"\"\"");
  Serial.println("         J     L");
  Serial.println("         :     :");
  Serial.println("        J       L");
  Serial.println("        :       :");
  Serial.println("        `._____.' ");


 delay (1000);

 calibra();
 M_off();
 prende_atomizadores();

}

void loop() {

que_hora_es(); 
lee_sensores();
check_atomizadores();

if ((segundos>=10)&&(segundos<11)) {
  if (!x3b_status) X3B_on();
 }

 if ((segundos>=20)&&(segundos<21)) {
  if (x3b_status) X3B_off();
  if (!x3a_status) X3A_on();
 }

 if ((segundos>=30)&&(segundos<31)) {
  if (x3a_status) X3A_off();
  if (!x2b_status) X2B_on();
 }

 if ((segundos>=50)&&(segundos<51)) {
  if (x2b_status) X2B_off();
  if (!x2a_status) X2A_on();
 }

 if ((segundos>=55)&&(segundos<56)) {
  if (x2a_status) X2A_off();
  if (!x1b_status) X1B_on();
 }

 if ((segundos>=60)&&(segundos<61)) {
  if (x1b_status) X1B_off();
  if (!x1a_status) X1A_on();
 }

  if ((segundos>=65)&&(segundos<66)) {
  if (x1a_status) X1A_off();
 }







/*
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
 }*/
 
  delay(50);
   
  if (psegundos!=segundos) {
  print();
  psegundos = segundos;
  }
 
}

void calibra() {
  Serial.println("-----------------> CALIBRACION");
  delay (3000);
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
 Serial.print(militar);
  Serial.print("------>");
 Serial.print(segundos);
 if (atom_status) Serial.print("    |ATOM");
 else Serial.print("|    ");


if (x1a_status) Serial.print("|X1A");
 else Serial.print("|   ");

if (x1b_status) Serial.print("|X1B");
 else Serial.print("|   ");

if (x2a_status) Serial.print("|X2A");
 else Serial.print("|   ");

if (x2b_status) Serial.print("|X2B");
 else Serial.print("|   ");

if (x3a_status) Serial.print("|X3A");
 else Serial.print("|   ");

if (x3b_status) Serial.print("|X3B");
 else Serial.print("|   ");

Serial.print("|");
Serial.print(sensor_0);
Serial.print("|");
Serial.print(sensor_1);
Serial.print("|");
Serial.print(sensor_2);
Serial.print("|");
Serial.print(sensor_3);
Serial.println("|");

 
 
}

void X1A_on() {
x1a_status = 1;
Serial.println("-----------------> EXTRACTOR_1A_ON");
}

void X1A_off() {
x1a_status = 0;
Serial.println("-----------------> EXTRACTOR_1A_OFF");
}

void X1B_on() {
x1b_status = 1;
Serial.println("-----------------> EXTRACTOR_1B_ON");
}

void X1B_off() {
x1b_status = 0;
Serial.println("-----------------> EXTRACTOR_1B_OFF");
}

void X2A_on() {
x2a_status = 1;
Serial.println("-----------------> EXTRACTOR_2A_ON");
}

void X2A_off() {
x2a_status = 0;
Serial.println("-----------------> EXTRACTOR_2A_OFF");
}

void X2B_on() {
x2b_status = 1;
Serial.println("-----------------> EXTRACTOR_2B_ON");
}

void X2B_off() {
x2b_status = 0;
Serial.println("-----------------> EXTRACTOR_2B_OFF");
}

void X3A_on() {
x3a_status = 1;
Serial.println("-----------------> EXTRACTOR_3A_ON");
}

void X3A_off() {
x3a_status = 0;
Serial.println("-----------------> EXTRACTOR_3A_OFF");
}

void X3B_on() {
x3b_status = 1;
Serial.println("-----------------> EXTRACTOR_3B_ON");
}

void X3B_off() {
x3b_status = 0;
Serial.println("-----------------> EXTRACTOR_3B_OFF");
}


void prende_atomizadores() {

t_inicio_atom = millis();
digitalWrite(pin_atom,HIGH);
atom_status = 1;
Serial.println("-----------------> ATOMIZADORES_ON");

}

void apaga_atomizadores() {

t_fin_atom = millis();
digitalWrite(pin_atom,LOW);
atom_status = 0;
Serial.println("-----------------> ATOMIZADORES_OFF");

}

void check_atomizadores() {

unsigned long ahora = millis();
 if (atom_status) {                           
  t_transcurrido_atom = ahora-t_inicio_atom;  
  if (t_transcurrido_atom>=t_dur_on_atom*1000) apaga_atomizadores();
  }
  else {
  t_apagado_atom = ahora-t_fin_atom;
  if (t_apagado_atom>=t_dur_off_atom*1000) prende_atomizadores();
  }
}

void que_hora_es() {

tiempo = millis()*multiplicador;
 segundos = tiempo/1000;
 minutos = segundos/60;
 hh = minutos/60;
 mm = minutos%60; 
 ss = segundos%60;
 militar = ss+(mm*100)+(hh*10000);

}

void lee_sensores() {

sensor_0 = analogRead(A0);
sensor_1 = analogRead(A1);
sensor_2 = analogRead(A2);
sensor_3 = analogRead(A3);

}

/*void calibra() {

  Serial.println(" -----------------> cierra_M0");
  digitalWrite(pin_M0a,LOW);  //cierra
  digitalWrite(pin_M0b,HIGH);


}*/

void M0_off () {

Serial.println("-----------------> M0_OFF");
digitalWrite(pin_M0a, HIGH);
digitalWrite(pin_M0a, HIGH);

}

void M1_off () {

Serial.println("-----------------> M1_OFF");
digitalWrite(pin_M1a, HIGH);
digitalWrite(pin_M1a, HIGH);

}

void M2_off () {

Serial.println("-----------------> M2_OFF");
digitalWrite(pin_M2a, HIGH);
digitalWrite(pin_M2a, HIGH);

}

void M3_off () {

Serial.println("-----------------> M3_OFF");
digitalWrite(pin_M3a, HIGH);
digitalWrite(pin_M3a, HIGH);

}

void M_off() {
  M0_off();
  M1_off();
  M2_off();
  M3_off();
}




