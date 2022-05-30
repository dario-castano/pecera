#ifndef PEZLCD_H
#define PEZLCD_H
#include <LiquidCrystal.h>
#include <Arduino.h>

class PezLCD {
	private:
		LiquidCrystal* lcd;

		char* temperature = "-999";

		char* tempBuf;
		char* luzBuf;
		char* nivelBuf;
		char* tareaBuf;

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
		const char ERROR_MSG[6] = "ERROR";

		const char LUZ_MSG_ON[3] = "ON";
		const char LUZ_MSG_OFF[4] = "OFF";

		const char NIVEL_MSG_OK[3] = "OK";
		const char NIVEL_MSG_LOW[3] = "LO";
		const char NIVEL_MSG_HIGH[3] = "HI";

		const char FOUR_EMPTY_SPACES[5] = "    ";
		const char THREE_EMPTY_SPACES[4] = "   ";
		const char TWO_EMPTY_SPACES[3] = "  ";

		const char TAREA_NIVEL_AGUA[3] = "10";
		const char TAREA_LLENAR_AGUA[3] = "11";
		const char TAREA_EXTRAER_AGUA[3] = "12";
		const char TAREA_LEER_FOTORESISTOR[3] = "20";
		const char TAREA_ENCENDER_LED[3] = "21";
		const char TAREA_APAGAR_LED[3] = "22";
		const char TAREA_LEER_TERMOMETRO[3] = "30";
		const char ERROR_TERMO_FUERA_RANGO[3] = "99";

	public:
		PezLCD(
				int rs,
				int e,
				int d4,
				int d5,
				int d6,
				int d7);

		~PezLCD();

		void setTemp(int tempValue);
		void setLuz(int luzMode);
		void setNivel(int nivelMode);
		void setTarea(int tarea);

		void print();
		void clear();
		void clear(int campo);
};

#endif
