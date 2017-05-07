/*
 * utils.c
 *
 *  Created on: 17 сент. 2016 г.
 *      Author: baygozin
 */
#include "stm32f1xx_hal.h"
#include "utils.h"
#include "hd44780.h"
#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern LCD_PCF8574_HandleTypeDef lcd;
extern UART_HandleTypeDef huart1;

char s[32], l[24];

void send0() {
    dataEncoders.code = 0;
    dataEncoders.encoderA = 0;
    dataEncoders.encoderB = 0;
    dataEncoders.time = 0;
    sprintf(s, "%5i,%6i,%6i,%5i\n",
            (unsigned int) dataEncoders.code,
            (int) dataEncoders.encoderA,
            (int) dataEncoders.encoderB,
            (unsigned int) dataEncoders.time);
    // Вывод в uart
    HAL_UART_Transmit(&huart1, (uint8_t *) &s,  sizeof(s), 100);
}

void sendEncoders()
{
    HAL_TIM_Base_Stop_IT(&htim4);
    time += 1;
    int A = 32767 - htim2.Instance->CNT;
    int B = 32767 - htim3.Instance->CNT;
//    if (A <= 32767) A -= 32767;
//    if (B <= 32767) B -= 32767;
    if ((oldA != A) || (oldB != B)) {
        dataEncoders.code = 1;
        dataEncoders.encoderA = A;
        dataEncoders.encoderB = B;
        dataEncoders.time = time;
        // Форматируем строку вывода
        sprintf(s, "%5d,%6i,%6i,%5i\n",
                (unsigned int) dataEncoders.code,
                (int) dataEncoders.encoderA,
                (int) dataEncoders.encoderB,
                (unsigned int) dataEncoders.time);
        // Вывод в uart
        if (A != 32767 || B != 32767) {
            HAL_UART_Transmit(&huart1, (uint8_t *) &s,  sizeof(s), 100);
        }
        // Вывод на дисплей
        oldA = A;
        oldB = B;
        lcdEncoders(dataEncoders.encoderA, dataEncoders.encoderB);
    }
    HAL_TIM_Base_Start_IT(&htim4);
}

void lcdEncoders(uint32_t A, uint32_t B) {
    sprintf(l, "%6i", (int)A);
    LCD_SetLocation(&lcd, 9, 0);
    LCD_WriteString(&lcd, l);
    sprintf(l, "%6i", (int)B);
    LCD_SetLocation(&lcd, 9, 1);
    LCD_WriteString(&lcd, l);
}

void resetEncoders(){
    HAL_TIM_Base_Stop_IT(&htim4);
    htim2.Instance->CNT = 0x7FFF;
    htim3.Instance->CNT = 0x7FFF;
    dataEncoders.encoderA = 0x7FFF;
    dataEncoders.encoderB = 0x7FFF;
    clearLCD();
    time = 0;
    HAL_TIM_Base_Start_IT(&htim4);
}

void resetEncoder1(){
    htim2.Instance->CNT = 0x7FFF;
    dataEncoders.encoderA = 0x7FFF;
    LCD_SetLocation(&lcd, 9, 0);
    LCD_WriteString(&lcd, "     0");
}

void resetEncoder2(){
    htim3.Instance->CNT = 0x7FFF;
    dataEncoders.encoderB = 0x7FFF;
    LCD_SetLocation(&lcd, 9, 1);
    LCD_WriteString(&lcd, "     0");
}

void startMeassure() {
    HAL_TIM_Encoder_Start_IT(&htim2,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start_IT(&htim3,TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&htim4);
}

void stopMeassure() {
    HAL_TIM_Base_Stop_IT(&htim4);
    HAL_TIM_Encoder_Stop_IT(&htim2,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop_IT(&htim3,TIM_CHANNEL_ALL);
}

void clearLCD() {
    LCD_ClearDisplay(&lcd);
    LCD_SetLocation(&lcd, 0, 0);
    LCD_WriteString(&lcd, "Shift         0");
    LCD_SetLocation(&lcd, 0, 1);
    LCD_WriteString(&lcd, "Power         0");
}

void clearLCDandWait() {
    LCD_ClearDisplay(&lcd);
    LCD_SetLocation(&lcd, 0, 0);
    LCD_WriteString(&lcd, "Welcome Iron lab");
}


void toggleLed()
{
    //HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void lcd_print(char *s)
{
  LCD_ClearDisplay(&lcd);
  LCD_SetLocation(&lcd, 0, 0);
  LCD_WriteString(&lcd, s);
}
