/*
 * PezLCD.cpp
 *
 *  Created on: May 30, 2022
 *      Author: Un pescado
 */
#include "PezLCD.h"
#include <LiquidCrystal.h>
#include <Arduino.h>

// Constructor del objeto
PezLCD::PezLCD(LiquidCrystal* lcd) {
	this->lcd = lcd;
	this->tempBuf = (char*)malloc(sizeof(char) * 9);
	this->luzBuf = (char*)malloc(sizeof(char) * 8);
	this->nivelBuf = (char*)malloc(sizeof(char) * 9);
	this->tareaBuf = (char*)malloc(sizeof(char) * 8);
}

// Libera los buffer de la memoria
PezLCD::~PezLCD() {
	free(tempBuf);
	free(luzBuf);
	free(nivelBuf);
	free(tareaBuf);
}

// Convierte el valor entero de la temperatura a cadena de caracteres
void PezLCD::setTemp(int tempValue) {
	strcpy(tempBuf, TEMP_MSG);
	strcat(tempBuf, itoa(tempValue, temperature, 10));
}

/*
 * Modos de mensajes de iluminacion:
 * 0 -> OFF
 * 1 -> ON
 */
void PezLCD::setLuz(int luzMode) {
	strcpy(luzBuf, LUZ_MSG);
	switch (luzMode) {
		case 0:
			strcat(luzBuf, LUZ_MSG_OFF);
			break;
		case 1:
			strcat(luzBuf, LUZ_MSG_ON);
			break;
	}
}

/*
 * Modos de nivel de agua
 * -1 -> Nivel bajo (LOW)
 *  0 -> Nivel en rango OK
 *  1 -> Nivel Alto (HIGH)
 */
void PezLCD::setNivel(int nivelMode) {
	strcpy(nivelBuf, NIVEL_MSG);
	switch (nivelMode) {
		case -1:
			strcat(nivelBuf, NIVEL_MSG_LOW);
			break;
		case 0:
			strcat(nivelBuf, NIVEL_MSG_OK);
			break;
		case 1:
			strcat(nivelBuf, NIVEL_MSG_HIGH);
			break;
	}
}

/*
 * Las tareas entre nivel 90 y 99 representan error
 * por lo tanto en estos valores cambia el mensaje de
 * TAREAXX a ERRORXX (XX son dos numeros de 0 a 9)
 */
void PezLCD::setTarea(int tarea) {
	if (tarea >= 90 && tarea <= 99) {
		strcpy(tareaBuf, ERROR_MSG);
	} else {
		strcpy(tareaBuf, TAREA_MSG);
	}

	switch (tarea) {
		case 10:
			strcat(tareaBuf, TAREA_NIVEL_AGUA);
			break;
		case 11:
			strcat(tareaBuf, TAREA_LLENAR_AGUA);
			break;
		case 12:
			strcat(tareaBuf, TAREA_EXTRAER_AGUA);
			break;
		case 20:
			strcat(tareaBuf, TAREA_LEER_FOTORESISTOR);
			break;
		case 21:
			strcat(tareaBuf, TAREA_ENCENDER_LED);
			break;
		case 22:
			strcat(tareaBuf, TAREA_APAGAR_LED);
			break;
		case 30:
			strcat(tareaBuf, TAREA_LEER_TERMOMETRO);
			break;
		case 99:
			strcat(tareaBuf, ERROR_TERMO_FUERA_RANGO);
			break;
	}
}

/*
 * Limpia los valores de los campos:
 * 1 -> temperatura
 * 2 -> luz
 * 3 -> nivel
 * 4 -> tarea/error
 */
void PezLCD::clear(int campo) {
	switch (campo) {
		case 1:
			lcd->setCursor(CURSOR_NIVEL_COL + 5, CURSOR_NIVEL_ROW);
			lcd->print(FOUR_EMPTY_SPACES);
			break;
		case 2:
			lcd->setCursor(CURSOR_LUZ_COL + 5, CURSOR_LUZ_ROW);
			lcd->print(THREE_EMPTY_SPACES);
			break;
		case 3:
			lcd->setCursor(CURSOR_NIVEL_COL + 7, CURSOR_NIVEL_ROW);
			lcd->print(TWO_EMPTY_SPACES);
			break;
		case 4:
			lcd->setCursor(CURSOR_TAREA_COL + 6, CURSOR_TAREA_ROW);
			lcd->print(TWO_EMPTY_SPACES);
			break;
	}
}

void PezLCD::clear() {
	clear(1);
	clear(2);
	clear(3);
	clear(4);
}

void PezLCD::print() {
	lcd->setCursor(CURSOR_TEMP_COL, CURSOR_TEMP_ROW);
	lcd->print(tempBuf);
	lcd->setCursor(CURSOR_LUZ_COL, CURSOR_LUZ_ROW);
	lcd->print(luzBuf);
	lcd->setCursor(CURSOR_NIVEL_COL, CURSOR_NIVEL_ROW);
	lcd->print(nivelBuf);
	lcd->setCursor(CURSOR_TAREA_COL, CURSOR_TAREA_ROW);
	lcd->print(tareaBuf);
}
