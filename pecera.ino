#include "Arduino.h"
#include "PezLCD.h"

PezLCD lcd;
const byte LCD_PIN_RS = 12;
const byte LCD_PIN_E = 11;
const byte LCD_PIN_D4 = 5;
const byte LCD_PIN_D5 = 4;
const byte LCD_PIN_D6 = 3;
const byte LCD_PIN_D7 = 2;

void setup()
{
	lcd = PezLCD(
			LCD_PIN_RS,
			LCD_PIN_E,
			LCD_PIN_D4,
			LCD_PIN_D5,
			LCD_PIN_D6,
			LCD_PIN_D7);

}

void loop()
{
	lcd.setTemp(20);
	lcd.setLuz(0);
	lcd.setNivel(0);
	lcd.setTarea(99);
	lcd.print();

	delay(2000);
	lcd.clear();

	lcd.setTemp(76);
	lcd.setLuz(1);
	lcd.setNivel(-1);
	lcd.setTarea(20);
	lcd.print();

	delay(2000);

	lcd.setTemp(23);
	lcd.setLuz(1);
	lcd.setNivel(0);
	lcd.setTarea(10);
	lcd.print();

	delay(2000);
}
