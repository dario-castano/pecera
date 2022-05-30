#ifndef PEZLCD_H
#define PEZLCD_H
#include <LiquidCrystal.h>
#include <Arduino.h>

class PezLCD {
	private:
		LiquidCrystal* lcd;

		char* temp_buf;
		char* luz_buf;
		char* nivel_buf;
		char* tarea_buf;

		const byte CURSOR_TEMP_COL = 0;
		const byte CURSOR_TEMP_ROW = 0;
		const byte CURSOR_LUZ_COL = 9;
		const byte CURSOR_LUZ_ROW = 0;
		const byte CURSOR_NIVEL_COL = 0;
		const byte CURSOR_NIVEL_ROW = 1;
		const byte CURSOR_TAREA_COL = 9;
		const byte CURSOR_TAREA_ROW = 1;

		const char TEMP_MSG[5] = "T.C:";
		const char LUZ_MSG[5] = "LUZ:";
		const char NIVEL_MSG[7] = "NIVEL:";
		const char TAREA_MSG[6] = "TAREA";

		const char LUZ_MSG_ON[3] = "ON";
		const char LUZ_MSG_OFF[4] = "OFF";

		const char NIVEL_MSG_OK[3] = "OK";
		const char NIVEL_MSG_LOW[3] = "LO";
		const char NIVEL_MSG_HIGH[3] = "HI";

		const char TAREA_NIVEL_AGUA = "10";
		const char TAREA_LLENAR_AGUA = "11";
		const char TAREA_EXTRAER_AGUA = "12";
		const char TAREA_LEER_FOTORESISTOR = "20";
		const char TAREA_ENCENDER_LED = "21";
		const char TAREA_APAGAR_LED = "22";
		const char TAREA_LEER_TERMOMETRO = "30";
		const char ERROR_TERMO_FUERA_RANGO = "99";

};

#endif
