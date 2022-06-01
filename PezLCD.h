#ifndef PEZLCD_H
#define PEZLCD_H
#include <LiquidCrystal.h>
#include <Arduino.h>

#define CURSOR_TEMP_COL 0
#define	CURSOR_TEMP_ROW 0
#define CURSOR_LUZ_COL 9
#define CURSOR_LUZ_ROW 0
#define CURSOR_NIVEL_COL 0
#define CURSOR_NIVEL_ROW 1
#define CURSOR_TAREA_COL 9
#define CURSOR_TAREA_ROW 1

#define TEMP_MSG "T.C:"
#define LUZ_MSG "LUZ:"
#define NIVEL_MSG "NIVEL:"
#define TAREA_MSG "TAREA"
#define ERROR_MSG "ERROR"

#define LUZ_MSG_ON "ON"
#define LUZ_MSG_OFF "OFF"

#define NIVEL_MSG_OK "OK"
#define NIVEL_MSG_LOW "LO"
#define NIVEL_MSG_HIGH "HI"

#define FOUR_EMPTY_SPACES "    "
#define THREE_EMPTY_SPACES "   "
#define TWO_EMPTY_SPACES  "  "

#define TAREA_NIVEL_AGUA "10"
#define TAREA_LLENAR_AGUA "11"
#define TAREA_EXTRAER_AGUA "12"
#define TAREA_LEER_FOTORESISTOR "20"
#define TAREA_ENCENDER_LED "21"
#define TAREA_APAGAR_LED "22"
#define TAREA_LEER_TERMOMETRO "30"
#define ERROR_TERMO_FUERA_RANGO "99"

class PezLCD {
	private:
		LiquidCrystal* lcd;
		char* temperature;
		char* tempBuf;
		char* luzBuf;
		char* nivelBuf;
		char* tareaBuf;

	public:
		PezLCD(
				byte rs,
				byte e,
				byte d4,
				byte d5,
				byte d6,
				byte d7);

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
