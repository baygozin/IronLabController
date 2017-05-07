/*
 * utils.h
 *
 *  Created on: 17 сент. 2016 г.
 *      Author: baygozin
 */

#ifndef APPLICATION_USER_UTILS_H_
#define APPLICATION_USER_UTILS_H_
#include "stm32f1xx_hal.h"

typedef int boolean;
#define TRUE  1
#define FALSE 0

uint8_t Buf[64];
uint32_t time;

struct {
  uint8_t code;
  uint8_t year;
  uint8_t month;
  uint8_t date;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} dateTime;

struct {
  uint8_t code;
  int dataEncoder;
} dataEncoder1, dataEncoder2;

struct {
  uint8_t code;
  int encoderA;
  int encoderB;
  uint32_t time;
} dataEncoders;

struct {
  uint8_t code;
  uint32_t value;
} adc1value;

struct {
  uint8_t code;
  uint8_t state;
} dataBtnState;

int oldA, oldB;


void beep();
void send0();
void sendEncoders();
void lcdEncoders(uint32_t A, uint32_t B);
void resetEncoders();
void resetEncoder1();
void resetEncoder2();
void startMeassure();
void stopMeassure();
void toggleLed();
void clearLCD();
void clearLCDandWait();
void lcd_print(char *s);

#endif /* APPLICATION_USER_UTILS_H_ */
