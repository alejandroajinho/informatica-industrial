// Definición de las entradas
#define A 10
#define B 9
#define C 8

// Función de inicio
void setup()
{
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

// Bucle
void loop()
{
  // Condicional que cumple BC' + AB
  if(digitalRead(B) == HIGH && digitalRead(C) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (digitalRead(A) == HIGH && digitalRead(B) == HIGH) {
  	digitalWrite(LED_BUILTIN, HIGH);
  } else { // Bloque si no se cumple la condición
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(1000); // Un pequeño delay para ahorro de recursos
}