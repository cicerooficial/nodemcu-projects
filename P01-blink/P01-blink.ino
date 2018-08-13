void setup() {
  //o parâmetro "16" equivale ao pino D0
  //e poderia ser substituído por "D0", "LED_BUILTIN" ou "BUILTIN_LED"
  pinMode(16, OUTPUT);
}

void loop() {
  digitalWrite(16, LOW);  //liga o LED, que opera invertido
  delay(100);
  digitalWrite(16, HIGH); //desliga o LED, que opera invertido
  delay(100);
}
