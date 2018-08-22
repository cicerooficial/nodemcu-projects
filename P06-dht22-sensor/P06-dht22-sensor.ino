#include <DHT.h>

//o parâmetro "5" equivale ao pino D1
DHT sensor (5, DHT22);
float temperatura;
float umidade;

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  temperatura = sensor.readTemperature();
  umidade = sensor.readHumidity();

  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("Umidade:     ");
  Serial.println(umidade);
  Serial.println("");
  
  delay(2000);
}
