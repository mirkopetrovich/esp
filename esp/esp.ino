//#define MEGA

const int tiempo_de_calibracion = 1000;  //[ms]
const int corriente_de_calibracion = 1000; //[mA] 

unsigned long tiempo;
unsigned long tiempo_calibracion;
int segundos, minutos, horas, psegundos;
int ss, mm, hh;
int militar;

//-------------------------------------------------------
//VARIABLES ATOMIZADORES

bool atom_status;
int pin_atom = 7;
unsigned long t_inicio_atom;
unsigned long t_transcurrido_atom;
unsigned long t_fin_atom;
unsigned long t_apagado_atom;
const unsigned long t_dur_on_atom = 40;    //segundos
const unsigned long t_dur_off_atom = 300;  // 5 min

// -------------------------------------------------------
// VARIABLES EXTRACTORES

bool x1a_status, x1b_status, x2a_status, x2b_status, x3a_status, x3b_status;

const int pin_X1A = 26;
const int pin_X1B = 28;
const int pin_X2A = 30;
const int pin_X2B = 32;
const int pin_X3A = 34;
const int pin_X3B = 36;

// ---------------------------------------------------------
// VARIABLES VÁLVULAS

int corriente_0, corriente_1, corriente_2, corriente_3;
int sensor_0_max = 0, sensor_1_max = 0, sensor_2_max = 0, sensor_3_max = 0;
byte mariposa_0 = 0,mariposa_1 = 0, mariposa_2 = 0, mariposa_3 = 0; 

const int pin_M0a = 23;
const int pin_M0b = 25;
const int pin_M1a = 27;
const int pin_M1b = 29;
const int pin_M2a = 31;
const int pin_M2b = 33;
const int pin_M3a = 35;
const int pin_M3b = 37;

// -----------------------------------------------------------

void setup() {

#ifdef MEGA
  pinMode(pin_M0a, OUTPUT);  // VÁLVULAS
  pinMode(pin_M0b, OUTPUT);
  pinMode(pin_M1a, OUTPUT);
  pinMode(pin_M1b, OUTPUT);
  pinMode(pin_M2a, OUTPUT);
  pinMode(pin_M2b, OUTPUT);
  pinMode(pin_M3a, OUTPUT);
  pinMode(pin_M3b, OUTPUT);

  pinMode(pin_X1A, OUTPUT);  // EXTRACTORES
  pinMode(pin_X1B, OUTPUT);
  pinMode(pin_X2A, OUTPUT);
  pinMode(pin_X2B, OUTPUT);
  pinMode(pin_X3A, OUTPUT);
  pinMode(pin_X3B, OUTPUT);

  pinMode(pin_atom, OUTPUT);  // ATOMIZADORES (LOW=apaga)
#endif


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


  delay(1000);

  calibra_M0();
  calibra_M1();
  calibra_M2();
  calibra_M3();

  M_off();
  tiempo_calibracion = millis();
  prende_atomizadores();
}

void loop() {

  que_hora_es();
  lee_sensores();
  check_atomizadores();

  if ((segundos >= 10) && (segundos < 11)) {
    if (!x3b_status) X3B_on();
  }

  if ((segundos >= 20) && (segundos < 21)) {
    if (x3b_status) X3B_off();
    if (!x3a_status) X3A_on();
  }

  if ((segundos >= 30) && (segundos < 31)) {
    if (x3a_status) X3A_off();
    if (!x2b_status) X2B_on();
  }

  if ((segundos >= 50) && (segundos < 51)) {
    if (x2b_status) X2B_off();
    if (!x2a_status) X2A_on();
  }

  if ((segundos >= 55) && (segundos < 56)) {
    if (x2a_status) X2A_off();
    if (!x1b_status) X1B_on();
  }

  if ((segundos >= 60) && (segundos < 61)) {
    if (x1b_status) X1B_off();
    if (!x1a_status) X1A_on();
  }

  if ((segundos >= 65) && (segundos < 66)) {
    if (x1a_status) X1A_off();
  }


if ((segundos >=15) && (segundos< 16)) {
  if (mariposa_0!=2) M0_abre();
}

if ((segundos >=20) && (segundos<21)) {
  if (mariposa_0!=0) M0_off();
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

  if (psegundos != segundos) {
    print();
    psegundos = segundos;
  }
}

void calibra_M0() {
  Serial.println("------------------------------> CALIBRANDO_M0");

  bool ov=false; 

  M0_cierra();
  unsigned long t_in_cal= millis();                          // tiempo en que se inicia la calibracion

  // loop mientras tiempo transcurrido(tiempo presente-tiempo que inicia) menor a tiempo de de calibracion y no supera el voltaje de calibracion 
  while(((millis()-t_in_cal)<tiempo_de_calibracion)&&!ov) {
  int sensor = analogRead(A0);                           // lee voltaje
  float corriente = abs((sensor*(10.0/1023.0)-5.)*1000.);  // transforma a 0 - 5000mA

  if (corriente > corriente_de_calibracion) {
    M0_off();
    ov = true;
  }

  if (corriente > sensor_0_max) sensor_0_max = corriente;

  }
  M0_off();

}

void calibra_M1() {
  Serial.println("------------------------------> CALIBRANDO_M1");

  bool ov = false; 

  M1_cierra();
  unsigned long t_in_cal= millis();                          // tiempo en que se inicia la calibracion

  // loop mientras tiempo transcurrido(tiempo presente-tiempo que inicia) menor a tiempo de de calibracion y no supera el voltaje de calibracion 
  while(((millis()-t_in_cal)<tiempo_de_calibracion)&&!ov) {
  int sensor = analogRead(A1);                           // lee voltaje
  float corriente = abs((sensor*(10.0/1023.0)-5.)*1000.);  // transforma a 0 - 5000mA

  if (corriente > corriente_de_calibracion) {
    M1_off();
    ov = true;
  }

  if (corriente > sensor_1_max) sensor_1_max = corriente;
  }
  M1_off();

}

void calibra_M2() {
  Serial.println("------------------------------> CALIBRANDO_M2");

  bool ov = false; 

  M2_cierra();
  unsigned long t_in_cal= millis();                          // tiempo en que se inicia la calibracion

  // loop mientras tiempo transcurrido(tiempo presente-tiempo que inicia) menor a tiempo de de calibracion y no supera el voltaje de calibracion 
  while(((millis()-t_in_cal)<tiempo_de_calibracion)&&!ov) {
  int sensor = analogRead(A2);                           // lee voltaje
  float corriente = abs((sensor*(10.0/1023.0)-5.)*1000.);  // transforma a 0 - 5000mA

  if (corriente > corriente_de_calibracion) {
    M2_off();
    ov = true;
  }

  if (corriente > sensor_2_max) sensor_2_max = corriente;
  }
  M2_off();

}

void calibra_M3() {
  Serial.println("------------------------------> CALIBRANDO_M3");

  bool ov = false; 

  M3_cierra();
  unsigned long t_in_cal= millis();                          // tiempo en que se inicia la calibracion

  // loop mientras tiempo transcurrido(tiempo presente-tiempo que inicia) menor a tiempo de de calibracion y no supera el voltaje de calibracion 
  while(((millis()-t_in_cal)<tiempo_de_calibracion)&&!ov) {
  int sensor = analogRead(A3);                           // lee voltaje
  float corriente = abs((sensor*(10.0/1023.0)-5.)*1000.);  // transforma a 0 - 5000mA

  if (corriente > corriente_de_calibracion) {
    M3_off();
    ov = true;
  }

  if (corriente > sensor_3_max) sensor_3_max = corriente;
  }
  M3_off();

}


void print() {
  if (hh < 10) Serial.print("0");
  Serial.print(hh);
  Serial.print(":");
  if (mm < 10) Serial.print("0");
  Serial.print(mm);
  Serial.print(":");
  if (ss % 60 < 10) Serial.print("0");
  Serial.print(ss % 60);
  //Serial.print("---->");
  //Serial.print(militar);
  Serial.print("---->");
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




Serial.print(" |V0 ");
switch(mariposa_0) {
  case 0:
  Serial.print(" ");
  break;
  case 1:
  Serial.print("X ");
  break;
  case 2:
  Serial.print("O ");
  break;
}

Serial.print(corriente_0);
Serial.print("mA(");
Serial.print(sensor_0_max);
Serial.print(")");

Serial.print("|V1 ");
switch(mariposa_1) {
  case 0:
  Serial.print(" ");
  break;
  case 1:
  Serial.print("X ");
  break;
  case 2:
  Serial.print("O ");
  break;
}

Serial.print(corriente_1);
Serial.print("mA(");
Serial.print(sensor_1_max);
Serial.print(")");

Serial.print("|V2 ");
switch(mariposa_2) {
  case 0:
  Serial.print(" ");
  break;
  case 1:
  Serial.print("X ");
  break;
  case 2:
  Serial.print("O ");
  break;
}

Serial.print(corriente_2);
Serial.print("mA(");
Serial.print(sensor_2_max);
Serial.print(")");

Serial.print("|V3 ");
switch(mariposa_3) {
  case 0:
  Serial.print("");
  break;
  case 1:
  Serial.print("X ");
  break;
  case 2:
  Serial.print("O ");
  break;
}

Serial.print(corriente_3);
Serial.print("mA(");
Serial.print(sensor_3_max);
Serial.print(")");


Serial.println("|");


}

void X1A_on() {
  x1a_status = 1;
  Serial.println("------------------------------> EXTRACTOR_1A_ON");
}

void X1A_off() {
  x1a_status = 0;
  Serial.println("------------------------------> EXTRACTOR_1A_OFF");
}

void X1B_on() {
  x1b_status = 1;
  Serial.println("------------------------------> EXTRACTOR_1B_ON");
}

void X1B_off() {
  x1b_status = 0;
  Serial.println("------------------------------> EXTRACTOR_1B_OFF");
}

void X2A_on() {
  x2a_status = 1;
  Serial.println("------------------------------> EXTRACTOR_2A_ON");
}

void X2A_off() {
  x2a_status = 0;
  Serial.println("------------------------------> EXTRACTOR_2A_OFF");
}

void X2B_on() {
  x2b_status = 1;
  Serial.println("------------------------------> EXTRACTOR_2B_ON");
}

void X2B_off() {
  x2b_status = 0;
  Serial.println("------------------------------> EXTRACTOR_2B_OFF");
}

void X3A_on() {
  x3a_status = 1;
  Serial.println("------------------------------> EXTRACTOR_3A_ON");
}

void X3A_off() {
  x3a_status = 0;
  Serial.println("------------------------------> EXTRACTOR_3A_OFF");
}

void X3B_on() {
  x3b_status = 1;
  Serial.println("------------------------------> EXTRACTOR_3B_ON");
}

void X3B_off() {
  x3b_status = 0;
  Serial.println("------------------------------> EXTRACTOR_3B_OFF");
}


void prende_atomizadores() {

  t_inicio_atom = millis();
  digitalWrite(pin_atom, HIGH);
  atom_status = 1;
  Serial.println("------------------------------> ATOMIZADORES_ON");
}

void apaga_atomizadores() {

  t_fin_atom = millis();
  digitalWrite(pin_atom, LOW);
  atom_status = 0;
  Serial.println("------------------------------> ATOMIZADORES_OFF");
}

void check_atomizadores() {

  unsigned long ahora = millis();
  if (atom_status) {
    t_transcurrido_atom = ahora - t_inicio_atom;
    if (t_transcurrido_atom >= t_dur_on_atom * 1000) apaga_atomizadores();
  } else {
    t_apagado_atom = ahora - t_fin_atom;
    if (t_apagado_atom >= t_dur_off_atom * 1000) prende_atomizadores();
  }
}

void que_hora_es() {

  tiempo = millis()-tiempo_calibracion;
  segundos = tiempo / 1000;
  minutos = segundos / 60;
  hh = minutos / 60;
  mm = minutos % 60;
  ss = segundos % 60;
  militar = ss + (mm * 100) + (hh * 10000);
}

void lee_sensores() {

  int sensor_0 = analogRead(A0);
  corriente_0 = abs((sensor_0*(10.0/1023.0)-5.)*1000.);
  int sensor_1 = analogRead(A1);
  corriente_1 = abs((sensor_1*(10.0/1023.0)-5.)*1000.);
  int sensor_2 = analogRead(A2);
  corriente_2 = abs((sensor_2*(10.0/1023.0)-5.)*1000.);
  int sensor_3 = analogRead(A3);
  corriente_3 = abs((sensor_3*(10.0/1023.0)-5.)*1000.);
}

/*void calibra() {

  Serial.println(" -----------------> cierra_M0");
  digitalWrite(pin_M0a,LOW);  //cierra
  digitalWrite(pin_M0b,HIGH);


}*/

void M0_off() {
  mariposa_0 = 0;
  Serial.println("------------------------------> M0_OFF");
  digitalWrite(pin_M0a, HIGH);
  digitalWrite(pin_M0a, HIGH);
}

void M1_off() {
  mariposa_1 = 0;
  Serial.println("------------------------------> M1_OFF");
  digitalWrite(pin_M1a, HIGH);
  digitalWrite(pin_M1a, HIGH);
}

void M2_off() {
  mariposa_2 = 0;
  Serial.println("------------------------------> M2_OFF");
  digitalWrite(pin_M2a, HIGH);
  digitalWrite(pin_M2a, HIGH);
}

void M3_off() {
  mariposa_3 = 0;
  Serial.println("------------------------------> M3_OFF");
  digitalWrite(pin_M3a, HIGH);
  digitalWrite(pin_M3a, HIGH);
}

void M0_cierra() {
  mariposa_0 = 1;
  Serial.println("------------------------------> M0_CIERRA");
  digitalWrite(pin_M0a, LOW);
  digitalWrite(pin_M0a, HIGH);
}

void M1_cierra() {
  mariposa_1 = 1;
  Serial.println("------------------------------> M1_CIERRA");
  digitalWrite(pin_M1a, LOW);
  digitalWrite(pin_M1a, HIGH);
}

void M2_cierra() {
  mariposa_2 = 1;
  Serial.println("------------------------------> M2_CIERRA");
  digitalWrite(pin_M2a, LOW);
  digitalWrite(pin_M2a, HIGH);
}

void M3_cierra() {
  mariposa_3 = 1;
  Serial.println("------------------------------> M3_CIERRA");
  digitalWrite(pin_M3a, LOW);
  digitalWrite(pin_M3a, HIGH);
}

void M0_abre() {
  mariposa_0 = 2;
  Serial.println("------------------------------> M0_ABRE");
  digitalWrite(pin_M0a, HIGH);
  digitalWrite(pin_M0a, LOW);
}

void M1_abre() {
  mariposa_1 = 2;
  Serial.println("------------------------------> M1_ABRE");
  digitalWrite(pin_M1a, HIGH);
  digitalWrite(pin_M1a, LOW);
}

void M2_abre() {
  mariposa_2 = 2;
  Serial.println("------------------------------> M2_ABRE");
  digitalWrite(pin_M2a, HIGH);
  digitalWrite(pin_M2a, LOW);
}

void M3_abre() {
  mariposa_3 = 2;
  Serial.println("------------------------------> M3_ABRE");
  digitalWrite(pin_M3a, HIGH);
  digitalWrite(pin_M3a, LOW);
}


void M_off() {
  M0_off();
  M1_off();
  M2_off();
  M3_off();
}
