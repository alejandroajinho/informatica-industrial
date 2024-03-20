/*
  En esta práctica se recreará el funcionamiento de una ventana de coche.
  Consta de 3 pulsadores, uno que acciona la marcha de la ventana,
  pasando por paro si se encuentra en movimiento, y otros dos
  simulando finales de carrera.

  A diferencia de la pr´áctica anterior, en está no se tendrá en cuenta el
  tiempo de 7 segundos para detener el movimiento de la ventana

  Autor: Alejandro Sampedro Fernández
  Fecha: Marzo 2024
*/

#define BUTTON 2           // El pin del pulsador que queremos utilizar
#define END_OF_RACE_UP 3   // El pin de uno de los finales de carrera
#define END_OF_RACE_DOWN 4 // El pin del otro final de carrera
#define UP 10              // EL pin del relé que alimenta el motor de subida
#define DOWN 11            // El pin del relé que alimenta el motor de bajada
/*
  Descomentar si se quieren ver algunos datos en la ejecución
  (Imprimir cosas en consola por la página de TinkerCad es muy costoso,
  por lo que el funcionamiento del programa se ve afectado, provocando
  que algunas veces no se detecten correctamente los pulsadores, por
  lo que se recomienda no descomentar esto a la hora de probar el circuito)
*/
// #define DEBUG

/*
  Estructura de datos que guarda el estado de la ventana

  Las estructuras de datos son una forma de organizar los datos,
  el funcionamiento es el mismo que si las variables estuviesen
  definidas de la siguiente forma:

  int isUp;
  int isDown;
  int isMoving;
  ...

  Pero utilizando una estructura de datos, deberemos escribir el nombre
  de la estructura antes de acceder o modificar los datos

  state.isUp;
  state.isDown;
  state.isMoving;
  ...
*/
struct State {
  int isUp;         // Indica si la ventana está arriba
  int isDown;       // Indica si la ventana está abajo
  int isMoving;     // Indica si la ventana se está moviendo
  int currentState; // Indica si la ventana está subiendo o bajando
  int startMoving;  // Indica si la ventana tiene que empezar a moverse
} state;            // <-- Este es el nombre que tiene la structura de datos

void setup() {
  // Inicializamos las variables en sus valores por defecto
  state.isDown = 1;
  state.isUp = 0;
  state.isMoving = 0;
  state.currentState = 10; // 11 bajando, 10 subiendo
  state.startMoving = 0;

  // Se indica si los pines son entradas o salidas
  pinMode(BUTTON, INPUT);
  pinMode(END_OF_RACE_UP, INPUT);
  pinMode(END_OF_RACE_DOWN, INPUT);
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  debug(); // Imprime algunos datos de información en pantalla

  /*
    Al pulsar el boton,
    se indica que la ventana deberia moverse poniendo "startMoving" a uno,
    y se queda en bucle hasta soltar el pulsador,
    pa ra evitar que se cuenten varias pulsaciones
  */
  while (digitalRead(BUTTON)) {
    state.startMoving = 1;
  }

  /*
    Obtiene el estado de los pulsadores que hacen de final de carrera
    (En esta caso no es necesario asegurar de que solo se pulso una vez,
    ya que no nos importa si detecta varias pulsaciones)
  */
  state.isDown = digitalRead(END_OF_RACE_DOWN);
  state.isUp = digitalRead(END_OF_RACE_UP);

  // Si la ventana debe empezar a moverse
  if (state.startMoving) {
    // Se invierte la variable para que no se ejecute esta parte del código en
    // la próxima iteración del bucle
    state.startMoving = !state.startMoving;

    // Devuelve los estados de los finales de carrera a 0
    state.isDown = 0;
    state.isUp = 0;

    // Si la ventana se estaba moviendo se detendrá el movimiento anterior
    if (state.isMoving) {
      digitalWrite(state.currentState, LOW);
      // Se indica que la ventana ha dejado de moverse
      state.isMoving = 0;
    } else {
      // Invierte el estado de la ventana (si está abajo bajará, si está
      // arriba subirá)
      state.currentState = getOpositePin();

      state.isMoving = 1; // Se indica que se está moviendo la ventana
      digitalWrite(state.currentState, HIGH); // Se pone en movimiento
    }
  }

  // En caso de que se esté moviendo y detecte algun final de carrera,
  // detiene la subida/bajada de la ventana
  if (state.isMoving && (state.isDown && state.currentState == 11) ||
      (state.isUp && state.currentState == 10)) {
    digitalWrite(state.currentState, LOW);
    state.isMoving = 0; // Se indica que dejo de moverse
  }

  delay(50);
}

// Esta función invierte el pin seleccionado
int getOpositePin() {
  if (state.currentState == DOWN) {
    return UP;
  } else {
    return DOWN;
  }
}

// Esta función devuelve el nombre de la dirección a la que
// se está moviendo la ventana (parada, subiendo, bajada)
String getDirectionName() {
  if (!state.isMoving) {
    return "PARADO";
  }

  if (state.currentState == DOWN) {
    return "BAJANDO";
  } else {
    return "SUBIENDO";
  }
}

/* Esta función imprime por pantalla algunos datos
 de información de la ejecución (esta función es
 muy costosa y puede verse alterado el funcionamiento
 del programa debido al costo que tiene su ejecución)*/
void debug() {
#ifdef DEBUG
  Serial.println("----------------------------");
  Serial.print("Direccion:");
  Serial.print(" | ");
  Serial.print("FDC ABAJO:");
  Serial.print(" | ");
  Serial.print("FDC ARRIBA:\n");
  Serial.print(getDirectionName());
  Serial.print("     |      ");
  Serial.print(state.isDown);
  Serial.print("     |      ");
  Serial.print(state.isUp);
  Serial.print("\n----------------------------");
#endif
}
