#include <DallasTemperature.h>
#include <OneWire.h>
#include <HD44780_LCD_PCF8574.h>
#include <Arduino.h>

HD44780LCD pantalla(2, 16, 0x27);        // Crea un espacio para el LCD en la memoria
OneWire onewire(10);                     // Crea un objeto OneWire para el termometro en el pin 10
DallasTemperature termometro(&onewire);  // Crea un objeto de termometro que lee por el OneWire 

// Buffers
char* bufferTemperatura;
char* bufferLuz;
char* bufferNivel;
char* bufferTarea;

// Mensajes del display
char MENSAJE_TEMPERATURA[] = "T:";
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
char SIETE_ESPACIOS[] = "       ";
char TRES_ESPACIOS[] = "   ";
char DOS_ESPACIOS[] = "  ";

// Configuracion LED / LDR
int PIN_LECTURA_LDR = A0;
int PIN_DISPARO_RELAY_LED = 13;
int UMBRAL_ENCENDIDO_LED = 250;
int valorLDR;

// Configuracion termometro
float TEMPERATURA_MAXIMA = 28.0;
float valorTemp;

// Configuracion nivel de agua
int PIN_DISPARO_RELAY_BOMBA_TANQUE = 12;
int PIN_DISPARO_RELAY_BOMBA_PECERA = 11;
int PIN_NIVEL_AGUA_BAJO = 6;
int PIN_NIVEL_AGUA_ALTO = 7;
int TIEMPO_ACTIVACION_BOMBA_MS = 10000;
int valorSensorNivelAlto;
int valorSensorNivelBajo;

// Cada cuando tiempo se verifica el estado de la pecera en ms
int PERIODO_REFRESCO = 5000;


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
 *    T:-56.45
 *  
 *  valor: El valor que se quiere presentar en la pantalla
 */
void imprimirTemperatura(float valor) 
{
  dtostrf(valor, -7, 2, bufferTemperatura); // convierte de float a texto
  imprimir(bufferTemperatura, MENSAJE_TEMPERATURA, SIETE_ESPACIOS, 0, 2, LCDLineNumberOne);
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

// SECCION: LDR / LED *************************************************************************** //

/* encenderLED - Enciende el led y envia
 * 
 */
void encenderLED()
{
  if (digitalRead(PIN_DISPARO_RELAY_LED) == HIGH) {
    imprimirEvento(21);
    Serial.println("Encendiendo LED...");
    digitalWrite(PIN_DISPARO_RELAY_LED, LOW);
  }
  imprimirEstadoLuz(1);
  Serial.println("Iluminacion encendida");
}

void apagarLED()
{
  if (digitalRead(PIN_DISPARO_RELAY_LED) == LOW) {
    imprimirEvento(22);
    Serial.println("Apagando LED...");
    digitalWrite(PIN_DISPARO_RELAY_LED, HIGH);
  }
  imprimirEstadoLuz(0);
  Serial.println("Iluminacion apagada");
}

/* ajustarIluminacion - Verifica el nivel de iluminacion de la pecera
 *  
 *  En el caso de que el valor LDR este debajo del umbral se activa el relay 
 * 
 */
void ajustarIluminacion() 
{
  imprimirEvento(20);
  valorLDR = analogRead(PIN_LECTURA_LDR);
  Serial.print("Lectura LDR: ");
  Serial.println(valorLDR);

  if (valorLDR < UMBRAL_ENCENDIDO_LED) {
    encenderLED();
  } else{
    apagarLED();
  }
}

// FIN SECCION: LDR / LED ************************************************************************ //

// SECCION: NIVEL DE AGUA ************************************************************************ //

void agregarAgua()
{
  imprimirEvento(11);
  Serial.println("Agregando agua...");
  digitalWrite(PIN_DISPARO_RELAY_BOMBA_TANQUE, LOW);
  delay(TIEMPO_ACTIVACION_BOMBA_MS);
  digitalWrite(PIN_DISPARO_RELAY_BOMBA_TANQUE, HIGH);
}

void extraerAgua()
{
  imprimirEvento(12);
  Serial.println("Extrayendo agua...");
  digitalWrite(PIN_DISPARO_RELAY_BOMBA_PECERA, LOW);
  delay(TIEMPO_ACTIVACION_BOMBA_MS);
  digitalWrite(PIN_DISPARO_RELAY_BOMBA_PECERA, HIGH);
}

void verificarNivelAgua()
{
  imprimirEvento(10);
  Serial.println("Verificando nivel de agua");

  valorSensorNivelAlto = digitalRead(PIN_NIVEL_AGUA_ALTO);
  Serial.print("Lectura nivel de agua ALTO: ");
  Serial.println(valorSensorNivelAlto);
  valorSensorNivelBajo = digitalRead(PIN_NIVEL_AGUA_BAJO);
  Serial.print("Lectura nivel de agua BAJO: ");
  Serial.println(valorSensorNivelBajo);

  if ((valorSensorNivelAlto == HIGH) && (valorSensorNivelBajo == HIGH)) {
    imprimirEvento(98);
    Serial.println("ERROR: Ambos sensores de nivel estan emitiendo valor ALTO");
  } 
  else if ((valorSensorNivelAlto == HIGH) && (valorSensorNivelBajo == LOW)) {
    imprimirEstadoNivel(1);
    while (valorSensorNivelAlto == HIGH) {
      extraerAgua();
      valorSensorNivelAlto = digitalRead(PIN_NIVEL_AGUA_ALTO);
      Serial.print("Lectura nivel de agua ALTO: ");
      Serial.println(valorSensorNivelAlto);
      valorSensorNivelBajo = digitalRead(PIN_NIVEL_AGUA_BAJO);
      Serial.print("Lectura nivel de agua BAJO: ");
      Serial.println(valorSensorNivelBajo);
    }
  }
  else if ((valorSensorNivelAlto == LOW) && (valorSensorNivelBajo == HIGH)) {
    imprimirEstadoNivel(-1);
    while (valorSensorNivelBajo == HIGH) {
      agregarAgua();
      valorSensorNivelAlto = digitalRead(PIN_NIVEL_AGUA_ALTO);
      Serial.print("Lectura nivel de agua ALTO: ");
      Serial.println(valorSensorNivelAlto);
      valorSensorNivelBajo = digitalRead(PIN_NIVEL_AGUA_BAJO);
      Serial.print("Lectura nivel de agua BAJO: ");
      Serial.println(valorSensorNivelBajo);
    }
  }
  imprimirEstadoNivel(0);
}

// FIN SECCION: NIVEL DE AGUA ******************************************************************** //

// SECCION: TERMOMETRO *************************************************************************** //

void nivelarTemperatura () {
    imprimirEvento(30);
    termometro.requestTemperatures();
    valorTemp = termometro.getTempCByIndex(0);
    Serial.print("Temperatura de la pecera: ");
    Serial.println(valorTemp);
    imprimirTemperatura(valorTemp);

    while (valorTemp > TEMPERATURA_MAXIMA) {
      extraerAgua();
      agregarAgua();
      termometro.requestTemperatures();
      valorTemp = termometro.getTempCByIndex(0);
    }
    
  } 

// FIN SECCION: TERMOMETRO *********************************************************************** //

void setup()
{
  bufferTemperatura = (char*)malloc(sizeof(char) * 5);
  bufferLuz = (char*)malloc(sizeof(char) * 4);
  bufferNivel = (char*)malloc(sizeof(char) * 3);
  bufferTarea = (char*)malloc(sizeof(char) * 3);
  iniciarLCD();
  termometro.begin();

  pinMode(PIN_DISPARO_RELAY_LED, OUTPUT);
  pinMode(PIN_NIVEL_AGUA_ALTO, INPUT);
  pinMode(PIN_NIVEL_AGUA_BAJO, INPUT);
  digitalWrite(PIN_DISPARO_RELAY_LED, HIGH);
  pantalla.PCF8574_LCDClearScreen();
  Serial.begin(9600);
  Serial.println("****INICIANDO PECERA!****");
  delay(3000);
}

void loop()
{
  ajustarIluminacion();
  nivelarTemperatura();
  verificarNivelAgua();
  delay(PERIODO_REFRESCO);
}
