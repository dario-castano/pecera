#include <DallasTemperature.h>
#include <OneWire.h>
#include <HD44780_LCD_PCF8574.h>

HD44780LCD pantalla(2, 16, 0x27); // Crea un espacio para el LCD en la memoria

// Buffers
char* bufferTemperatura;
char* bufferLuz;
char* bufferNivel;
char* bufferTarea;

// Mensajes del display
char MENSAJE_TEMPERATURA[] = "T.C:";
char MENSAJE_LUZ[] = "LUZ:";
char MENSAJE_LUZ_ON[] = "ON ";
char MENSAJE_LUZ_OFF[] = "OFF";
char MENSAJE_NIVEL[] = "NIVEL:";
char MENSAJE_NIVEL_ALTO[] = "HI";
char MENSAJE_NIVEL_BAJO[] = "LO";
char MENSAJE_NIVEL_OK[] = "OK";
char MENSAJE_TAREA[] = "TAREA";
char MENSAJE_ERROR[] = "ERROR";

// Rellenos del display (actuan como borrador)
char CUATRO_ESPACIOS[] = "    ";
char TRES_ESPACIOS[] = "   ";
char DOS_ESPACIOS[] = "  ";

// Configuracion LED / LDR
int PIN_LECTURA_LDR = A0;
int PIN_DISPARO_RELAY_LED = 13;
int UMBRAL_ENCENDIDO_LED = 250;

int valorLDR;




// SECCION: LCD ********************************************************************************* //

// Configura lo que necesita el LCD para arrancar
void iniciarLCD()
{
  pantalla.PCF8574_LCDInit(LCDCursorTypeOn);
  pantalla.PCF8574_LCDClearScreen();
  pantalla.PCF8574_LCDBackLightSet(true);
}

/* imprimir - Imprime en la pantalla un mensaje
 *  
 *  buffer: El espacio temporal en la memoria que guarda el valor a mostar
 *  mensaje: El titulo que muestra el display por ejemplo: "T.C:", "LUZ:", etc...
 *  relleno: Es un espacio en blanco que se usa para borrar algo que habia escrito anteriormente
 *  inicio: Desde que columna se escribe el mensaje
 *  offset: Desde que posicion debo empezar a escribir para no dañar el mensaje 
 *  linea: Desde que fila se escribe el mensaje
 */
void imprimir(
  char* buffer,
  char mensaje[],
  char relleno[],
  byte inicio,
  byte offset,
  LCDLineNumber_e linea
) 
{
  pantalla.PCF8574_LCDGOTO(linea, inicio);
  pantalla.PCF8574_LCDSendString(mensaje);
  pantalla.PCF8574_LCDGOTO(linea, offset);
  pantalla.PCF8574_LCDSendString(relleno);
  pantalla.PCF8574_LCDGOTO(linea, offset);
  pantalla.PCF8574_LCDSendString(buffer);
}

/* imprimirTemperatura - Imprime la temperatura en la pantalla
 *  
 *  Ejemplo:
 *    T.C:-56
 *  
 *  valor: El valor que se quiere presentar en la pantalla
 */
void imprimirTemperatura(int valor) 
{
  itoa(valor, bufferTemperatura, 10);
  imprimir(bufferTemperatura, MENSAJE_TEMPERATURA, CUATRO_ESPACIOS, 0, 4, LCDLineNumberOne);
}

/* imprimirEstadoLuz - Imprime en que estado esta la iluminacion de la pecera en la pantalla
 *  
 *  Ejemplo:
 *    LUZ:OFF
 *  
 *  estado: El valor que se quiere presentar en la pantalla
 *    0 -> Luz apagada (OFF)
 *    1 -> Luz encendida (ON)
 */
void imprimirEstadoLuz(int estado)
{
  switch (estado) {
    case 0:
      strcpy(bufferLuz, MENSAJE_LUZ_OFF);
      break;
    case 1:
      strcpy(bufferLuz, MENSAJE_LUZ_ON);
      break;
    }
  imprimir(bufferLuz, MENSAJE_LUZ, TRES_ESPACIOS, 9, 13, LCDLineNumberOne);  
}

/* imprimirEstadoNivel - Imprime en que estado esta el nivel del agua de la pecera en la pantalla
 *  
 *  Ejemplo:
 *    NIVEL:OK
 *  
 *  estado: El valor que se quiere presentar en la pantalla
 *    -1 -> Nivel bajo (LO)
 *     0 -> Nivel bueno (OK)
 *     1 -> Nivel alto (HI)
 */
void imprimirEstadoNivel(int estado)
{
  switch(estado) {
    case -1:
      strcpy(bufferNivel, MENSAJE_NIVEL_BAJO);
      break;
    case 0 :
      strcpy(bufferNivel, MENSAJE_NIVEL_OK);
      break;
    case 1 :
      strcpy(bufferNivel, MENSAJE_NIVEL_ALTO);
      break;
    }
  imprimir(bufferNivel, MENSAJE_NIVEL, DOS_ESPACIOS, 0, 6, LCDLineNumberTwo);
}

/* imprimirEvento - Imprime el valor de un evento en la pantalla
 *  
 *  Un evento puede ser una TAREA (Un trabajo que esta ejecutando la pecera) o 
 *  un ERROR (Una situacion que impide la operacion de la pecera). Codigos del
 *  10 al 89 son TAREAS, y de 90 a 99 ERRORES.
 *  
 *  Ejemplos:
 *    TAREA23
 *    ERROR99
 *  
 *  codigo: El valor del evento que se quiere presentar en la pantalla
 */
void imprimirEvento(int codigo)
{
  itoa(codigo, bufferTarea, 10); // El 10 es porque la conversion de numero a cadena es en base 10

  if ((codigo >= 90) && (codigo <= 99)) {
    imprimir(bufferTarea, MENSAJE_ERROR, DOS_ESPACIOS, 9, 14, LCDLineNumberTwo);
  } else {
    imprimir(bufferTarea, MENSAJE_TAREA, DOS_ESPACIOS, 9, 14, LCDLineNumberTwo);
  }

}

// FIN SECCION: LCD ***************************************************************************** //

// SECCION: LDR ********************************************************************************* //
void encenderLED()
{
  if (digitalRead(PIN_DISPARO_RELAY_LED) == HIGH) {
    imprimirEvento(21);
    digitalWrite(PIN_DISPARO_RELAY_LED, LOW);
  }
  imprimirEstadoLuz(1);
}

void apagarLED()
{
  if (digitalRead(PIN_DISPARO_RELAY_LED) == LOW) {
    imprimirEvento(22);
    digitalWrite(PIN_DISPARO_RELAY_LED, HIGH);
  }
  imprimirEstadoLuz(0);
}


void ajustarIluminacion() 
{
  imprimirEvento(20);
  valorLDR = analogRead(PIN_LECTURA_LDR);
  Serial.print("Lectura LDR: ");
  Serial.println(valorLDR);

  if (valorLDR < 200) {
    encenderLED();
  } else{
    apagarLED();
  }
}


// FIN SECCION: LDR ***************************************************************************** //

void setup()
{
  bufferTemperatura = (char*)malloc(sizeof(char) * 5);
  bufferLuz = (char*)malloc(sizeof(char) * 4);
  bufferNivel = (char*)malloc(sizeof(char) * 3);
  bufferTarea = (char*)malloc(sizeof(char) * 3);
  iniciarLCD();

  pinMode(PIN_DISPARO_RELAY_LED, OUTPUT);
  digitalWrite(PIN_DISPARO_RELAY_LED, HIGH);
  pantalla.PCF8574_LCDClearScreen();
  Serial.begin(9600);
  Serial.println("INICIANDO PECERA!");
  delay(3000);
}

void loop()
{
  ajustarIluminacion();
  delay(5000);
}
