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
	this->tempValue = 0;
	this->luzMode = 0;
	this->nivelMode = 0;
	this->tarea = 0;
}

// Libera los buffer de la memoria
PezLCD::~PezLCD() {
	free(tempBuf);
	free(luzBuf);
	free(nivelBuf);
	free(tareaBuf);
}
