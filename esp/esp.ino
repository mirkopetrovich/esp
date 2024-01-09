unsigned long tiempo;
int segundos;
int minutos;
int horas;
int multiplicador = 1;

void setup() {
 Serial.begin(9600);

}

void loop() {

 tiempo = millis()*multiplicador;
 segundos = tiempo/1000;
 minutos = segundos/60;
 horas = minutos/60;
 //segundos = (tiempo/1000)%60;
 if (horas<10) Serial.print("0");
 Serial.print(horas);
 Serial.print(":");
 if (minutos%60<10) Serial.print("0");
 Serial.print(minutos%60);
 Serial.print(":");
 if (segundos%60<10) Serial.print("0");
 Serial.println(segundos%60);
 delay(1000);

}
