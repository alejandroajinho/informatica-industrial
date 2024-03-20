#define BUTTON 2  // El pin del pulsador que queremos utilizar
#define UP 10     // EL pin del relé que alimenta el motor de subida
#define DOWN 11   // El pin del relé que alimenta el motor de bajada
#define TIME 7000 // El tiempo que queremos que esté subiendo/bajando la ventana

int isMoving;     // Indica si la ventana esta subiendo/bajando
int currentState; // Indica si la ventana está subiendo o bajando
int startMoving;  // Indica si la ventana tiene que empezar a bajar o a subir
int startTime;    // Guarda el tiempo en el que empezo a moverse la ventana

void setup() {
  // Inicializamos las variables en sus valores por defecto
  isMoving = 0;
  startMoving = 0;
  currentState = DOWN;
  startTime = millis();
  
  // Indicamos si los pines serán entradas o salidas
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  // Al pulsar el boton, se indica que la ventana deberia moverse poniendo
  // "startMoving" a uno, y se queda en bucle hasta soltar el pulsador, para
  // evitar que se cuenten varias pulsaciones
  while (digitalRead(BUTTON)) {
    startMoving = 1;
  }

  // Si la ventana debe empezar a moverse
  if (startMoving) {
    // Se invierte la variable para que no se ejecute esta parte del código en
    // la próxima iteración del bucle
    startMoving = !startMoving;
    // Invierte el estado de la ventana (si está abajo bajará, si está
    // arriba subirá)
    currentState = getOpositePin(currentState);

    // Si la ventana se estaba moviendo se detendrá el movimiento anterior
    if (isMoving) {
      digitalWrite(getOpositePin(currentState), LOW);
    }

    // Se cambia el valor del tiempo de referencia al actual
    startTime = millis();
    isMoving = 1; // Se indica que se está moviendo la ventana
    digitalWrite(currentState, HIGH); // Se pone en movimiento
  }

  // Si la ventana se está moviendo y pasaron más de 7 segundos se detiene el
  // movimiento, y se invierte el valor de isMoving
  if (isMoving && (startTime + TIME) < millis()) {
    digitalWrite(currentState, LOW);
    isMoving = !isMoving;
  }
}

// Está función invierte el pin seleccionado
int getOpositePin(int pin) {
  if (pin == DOWN) {
    return UP;
  } else {
    return DOWN;
  }
}
