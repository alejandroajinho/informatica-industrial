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
#define LDR A0             // El pin del LDR
#define ACTIVATION 5       // El pin que activa los motores
#define MIN_LDR_VALUE 6
#define MAX_LDR_VALUE 679 // Es el valor máximo que puede alcanzar el LDR
// Distintos niveles de luz que puede recivir el LDR (para el 100% se usa el
// MAX_LDR_VALUE). Es importante castear los valores como "long int" ya que
// el valor a la hora de hacer los calculos es demasiado grande y no entra
// dentro de un "int"
#define LEVEL0 MIN_LDR_VALUE + (long int)MAX_LDR_VALUE * 5 / 100
#define LEVEL1 MIN_LDR_VALUE + (long int)MAX_LDR_VALUE * 20 / 100
#define LEVEL2 MIN_LDR_VALUE + (long int)MAX_LDR_VALUE * 40 / 100
#define LEVEL3 MIN_LDR_VALUE + (long int)MAX_LDR_VALUE * 60 / 100
#define LEVEL4 MIN_LDR_VALUE + (long int)MAX_LDR_VALUE * 80 / 100

#define MAX_SPEED 255 // La velocidad máxima que puede alcanzar el motor
// Las distintas velocidades que puede tener el motor. Al igual que con
// los valores anteriores, es necesario castear el numero como "long int"
#define SPEED0 (long int)MAX_SPEED * 5 / 100
#define SPEED1 (long int)MAX_SPEED * 20 / 100
#define SPEED2 (long int)MAX_SPEED * 40 / 100
#define SPEED3 (long int)MAX_SPEED * 60 / 100
#define SPEED4 (long int)MAX_SPEED * 80 / 100

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
  int ldrValue;     // Indica el valor de lectura del LDR
  int speed;        // Indica la valocidad a la que tiene que moverse el toldo
  bool needsToUpdate;
} state; // <-- Este es el nombre que tiene la structura de datos

void setup() {
  // Inicializamos las variables en sus valores por defecto
  state.isDown = 1;
  state.isUp = 0;
  state.isMoving = 0;
  state.currentState = 10; // 11 bajando, 10 subiendo
  state.startMoving = 0;
  state.ldrValue = 0;
  state.speed = 0;
  state.needsToUpdate = false;

  // Se indica si los pines son entradas o salidas
  pinMode(BUTTON, INPUT);
  pinMode(END_OF_RACE_UP, INPUT);
  pinMode(END_OF_RACE_DOWN, INPUT);
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
  pinMode(ACTIVATION, OUTPUT);

  Serial.begin(9600);

  Serial.println(SPEED0);
  Serial.println(SPEED1);
  Serial.println(SPEED2);
  Serial.println(SPEED3);
  Serial.println(SPEED4);
  Serial.println(MAX_SPEED);
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

  loadSpeed(); // Obtenemos la velocidad a la que debe ir el toldo

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

  delay(10);
}

// Esta función invierte el pin seleccionado
int getOpositePin() {
  if (state.currentState == DOWN)
    return UP;

  return DOWN;
}

// Esta función devuelve el nombre de la dirección a la que
// se está moviendo la ventana (parada, subiendo, bajada)
String getDirectionName() {
  if (!state.isMoving)
    return "PARADO";

  if (state.currentState == DOWN)
    return "BAJANDO";

  return "SUBIENDO";
}

/*
  Esta función lee el valor de entrada del LDR
  y lo traduce en una de las 5 valocidades a las
  que funciona el toldo, luego activa la salida
  POWER con en valor de la velocidad
*/
int loadSpeed() {
  state.ldrValue = analogRead(LDR);

  if (state.ldrValue <= LEVEL0) {
    state.needsToUpdate = !(state.speed == SPEED0);
    state.speed = SPEED0;
  } else if (state.ldrValue <= LEVEL1) {
    state.needsToUpdate = !(state.speed == SPEED1);
    state.speed = SPEED1;
  } else if (state.ldrValue <= LEVEL2) {
    state.needsToUpdate = !(state.speed == SPEED2);
    state.speed = SPEED2;
  } else if (state.ldrValue <= LEVEL3) {
    state.needsToUpdate = !(state.speed == SPEED3);
    state.speed = SPEED3;
  } else if (state.ldrValue <= LEVEL4) {
    state.needsToUpdate = !(state.speed == SPEED4);
    state.speed = SPEED4;
  } else {
    state.needsToUpdate = !(state.speed == MAX_SPEED);
    state.speed = MAX_SPEED;
  }
  
  analogWrite(ACTIVATION, state.speed);
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