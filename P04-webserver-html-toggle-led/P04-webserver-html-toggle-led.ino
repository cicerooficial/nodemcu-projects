#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer servidor;

char* id_rede = "nomedarede";
char* senha_rede = "senhadarede";

//a keyword "PROGMEM" indica que a variável abaixo
//deve ser armazenada na memória flash do NodeMCU
//em vez da memória RAM, que possui tamanho limitado.
//a string abaixo está escrita como raw string literal,
//o que permite escrever o conteúdo em diferentes linhas.
char paginaweb[] PROGMEM = R"=====(
<html>
  <head>
  </head>
  <body>
    <form action='alteraLED'>
      <button>ALTERAR LED</button>
    </form>
  </body>
</html>
)=====";

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
    //a função "send_P" foi utilizada porque
    //a variável "paginaweb" está armazenada na memória flash
    servidor.send_P(200, "text/html", paginaweb);
  });

  servidor.on("/alteraLED", [](){
    //altera o estado do LED para o contrário da leitura atual
    //ou seja: se estiver ligado, desliga (e vice-versa)
    digitalWrite(2, !digitalRead(2));
    servidor.send_P(200, "text/html", paginaweb);
  });

  servidor.begin();
}

void loop() {
  servidor.handleClient();
}
