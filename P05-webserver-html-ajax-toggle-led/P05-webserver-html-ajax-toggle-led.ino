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
char pagina_web[] PROGMEM = R"=====(
<html>
  <head>
    <meta charset="UTF-8">
    <title>IoT with NodeMCU/ESP2866</title>
    <link href='https://fonts.googleapis.com/css?family=Share+Tech+Mono' rel='stylesheet'> 
    <style>
      body {background-color:#efefef;font-family:'Share Tech Mono', monospace;}
      button {font-family:'Share Tech Mono',monospace;font-size:14px;border:none;font-weight:800;text-align:center;letter-spacing:-2px;}
      #box {position:absolute;top:50%;left:50%;max-width:50%;transform:translate(-50%,-60%);-webkit-transform:translate(-50%,-60%);-ms-transform:translate(-50%,-60%);}
      #toggle-led {width:100%;background-color:#EB9532;color:#fff;padding:10px 20px;}
      #led-status-box {position:absolute;top:20px;right:20px;font-size:14px;font-weight:800;background-color:#EB9532;color:#fff;padding:10px 20px;letter-spacing:-2px;}
    </style>
  </head>
  <body>
    <p id='led-status-box'>LED STATUS: <span id='led-status'>--</span></p>
    <div id='box'>
      <h1>Hi, this is an interface to control NodeMCU and we're using AJAX. :)</h1>
      <button id='toggle-led' onclick="enviaRequisicao('toggle-led')">TOGGLE LED</button>
    </div>
  </body>
  <script>
    function enviaRequisicao(url) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('led-status').innerHTML = this.responseText;
        }
      } 
      xhttp.open('GET', url, true);
      xhttp.send();
    }
    document.addEventListener('DOMContentLoaded', enviaRequisicao("led-status"), false);
  </script>
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
    //a variável "pagina_web" está armazenada na memória flash
    servidor.send_P(200, "text/html", pagina_web);
  });
  servidor.on("/led-status", LEDStatus);
  servidor.on("/toggle-led", alternaLED);

  servidor.begin();
}

void loop() {
  servidor.handleClient();
}

void alternaLED() {
  digitalWrite(2, !digitalRead(2));
  LEDStatus();
}

void LEDStatus() {
  servidor.send(200, "text/plain", digitalRead(2) ? "OFF" : "ON");
}
