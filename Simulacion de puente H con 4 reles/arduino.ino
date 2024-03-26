/**
 * SIMULACIÓN DE UNA PUENTE H UTILIZANDO 4 RELÉS
 *
 * En esta práctica se simulará el funcionamiento del integrado L293D
 * utilizando 4 relés, cuyas entradas son dirigidas por por 4
 * microinterruptores. Además, se controlarán las revoluciones del motor
 * mediante un potenciometro idemendiente a la placa Arduino.
 *
 * Por: Alejandro Sampedro Fernádnez
 * Fecha: Marzo 2024
 */

// Se definen los pines de entrada
#define S1 8
#define S2 9
#define S3 10
#define S4 11

// Se definen los pines de salida
#define I1 4
#define I2 5
#define I3 6
#define I4 7

/**
 * Es importante aclarar que las sal´idas se verán invertidas
 * respecto a las entradas, es decir, al pulsar el primer
 * pulsador, se activará el último relé.
 *
 * ------------------------
 * | Pulsador 1 | Relé 4 |
 * | Pulsador 2 | Relé 3 |
 * | Pulsador 3 | Relé 2 |
 * | Pulsador 4 | Relé 1 |
 * ------------------------
 */

// Se define la longitud del array en el que almacenaremos los datos
#define ArrayLength 4

int data[] = {0, 0, 0, 0}; // Creación del array donde se guardan los datos
int inputPins[] = {
    I1, I2, I3, I4}; //  Creación del array con las posiciones de las entradas
int outputPins[] = {S1, S2, S3,
                    S4}; // Creación del array con las posiciones de las salidas

void setup() {
  // Se indica que pines son salidas y cuales entradas
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);

  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(I3, INPUT);
  pinMode(I4, INPUT);
}

void loop() {
  // Primer bucle, que lee el valor de las entradas
  for (int i = 0; i < ArrayLength; i++) {
    /**
     * Se asigna el valor negado de cada entrada a su correspondiente
     posición
     * en el array de datos. La decisión de negar el valor se debe a que las
     * entradas están en lógica negativa, es decir, un 0 para cuando están
     * activas, y un 1 para cuando están inactivas.
     */
    data[i] = !digitalRead(inputPins[i]);
  }

  // Se revisa que no exista la posibilidad de que ocurra un corto
  if (!(data[0] && data[1]) && !(data[2] && data[3])) {
    // Segundo bucle, que escribe los valores en las salidas
    for (int i = 0; i < ArrayLength; i++) {
      // Se asigna el valor a cada una de las salidas
      digitalWrite(outputPins[i], data[i]);
    }
  }

  // Se deja un delay de 30 segundos para evitar un consumo excesivo
  delay(30);
}