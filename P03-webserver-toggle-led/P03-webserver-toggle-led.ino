#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer servidor;

char* id_rede = "nomedarede";
char* senha_rede = "senhadarede";

void setup() {
  pinMode(2, OUTPUT);
  //garante que o LED inicie desligado
  digitalWrite(2, HIGH);
  Serial.begin(115200);

  WiFi.begin(id_rede, senha_rede);

  Serial.print("Conectando à rede ");
  Serial.print(id_rede);
  Serial.print("...");

  //como a função anterior é assíncrona,
  //travamos o código para garantir a conexão
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //é necessário saber qual o endereço ip
  //atribuído ao esp8266 pelo roteador wifi
  Serial.println("");
  Serial.println("Dispositivo conectado.");
  Serial.println(WiFi.localIP());

  //adiciona rotas ao servidor
  servidor.on("/", [](){
    servidor.send(200, "text/plain", "Hello World! :)");
  });

  servidor.on("/alteraLED", [](){
    //altera o estado do LED para o contrário da leitura atual
    //ou seja: se estiver ligado, desliga (e vice-versa)
    digitalWrite(2, !digitalRead(2));
    servidor.send(200, "text/plain", digitalRead(2) ? "LED DESLIGADO!" : "LED LIGADO!");
  });

  servidor.begin();
}

void loop() {
  servidor.handleClient();
}
