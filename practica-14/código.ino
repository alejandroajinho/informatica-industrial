// Definición del pulsdor, led amarillo y relé
#define PULSADOR 10 
#define LED_AMARILLO 12
#define RELE 11

// Función de inicio del programa
void setup()
{
  // Indicamos que pines serán entradas y cuales salidas
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(PULSADOR, INPUT);
}

// Bucle del programa
void loop()
{
  // Condicional que comprueba si el pulsador está pulsado o no
  if (digitalRead(PULSADOR) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  	delay(10000);
 	digitalWrite(LED_BUILTIN, LOW);
  	delay(4000);
 	digitalWrite(LED_AMARILLO, HIGH);
	digitalWrite(RELE, HIGH);
 	delay(10000);
 	digitalWrite(RELE, LOW);
  	digitalWrite(LED_AMARILLO, LOW);
  	delay(4000);
  } else { // Si el pulsador está pulsado, pone el resto de pines en LOW
    digitalWrite(RELE, LOW);
    digitalWrite(LED_AMARILLO, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}