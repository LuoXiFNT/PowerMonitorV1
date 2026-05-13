/*
 * Copyright (C) 2026 LuoXiFNT
 * * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

/**
 * @file PowerMonitor.h
 * @brief Public interface for power-monitor measurement, key handling, and display update.
 * @author LuoXiFNT
 * @date 2026-03-25
 * @lastModified 2026-03-25
 */

#include "main.h"
#include "gpio.h"
#include "OLED.h"

/**
 * @brief Logical key IDs used by the debounce and key task logic.
 */
typedef enum {
  /**< Menu key: toggles page or exits clear-confirm state. */
  KEY_MENU = 0,
  /**< Hold key: toggles freeze/unfreeze. */
  KEY_HOLD,
  /**< Clear key: enter/confirm data clear state. */
  KEY_CLEAR,
  /**< Unit key: cycles oled refresh interval. */
  KEY_UNIT,
  /**< The number of keys */
  KEY_MAX
} PM_KeyId_t;

/**
 * @brief Main display pages.
 */
typedef enum {
  /**< Show instantaneous power/current/voltage. */
  MODE_POWER = 0,
  /**< Show accumulated energy/capacity/time. */
  MODE_ENERGY
} PM_DisplayMode_t;

/**
 * @brief Clear workflow state.
 */
typedef enum {
  /**< Normal operation state. */
  STATE_NORMAL = 0,
  /**< Waiting for clear confirmation state. */
  STATE_CONFIRM_CLEAR
} PM_ClearState_t;

/**
 * @brief Display freeze state.
 */
typedef enum {
  /**< Data and UI update are active. */
  SCREEN_UNFREEZE = 0,
  /**< Data presentation is frozen. */
  SCREEN_FREEZE
} PM_FreezeState_t;

/**
 * @brief OLED refresh period in ms.
 */
typedef enum {
  /**< 10 ms refresh interval. */
  REFRESH_10MS = 10,
  /**< 25 ms refresh interval. */
  REFRESH_25MS = 25,
  /**< 50 ms refresh interval. */
  REFRESH_50MS = 50,
  /**< 100 ms refresh interval. */
  REFRESH_100MS = 100,
  /**< 200 ms refresh interval. */
  REFRESH_200MS = 200
} PM_RefreshRate_t;

/**
 * @brief Data structure for storing analog data.
 */
typedef struct
{
  /**< Instantaneous current in A. */
  double current; 
  /**< Instantaneous voltage in V. */
  double voltage; 
  /**< Instantaneous power in W. */
  double power; 
  /**< Accumulated energy in Wh. */
  double energy; 
  /**< Accumulated capacity in mAh. */
  double capacity; 
  /**< Elapsed time in s. */
  double time; 
  /**< Time interval for energy/capacity accumulation in h. */
  double dt_hours; 
} PM_Analog_Data_t;

/** Debounce countdown time for key press validation in ms. */
#define DEBOUNCE_TIME 20
/** Fixed ADC sampling interval in ms. */
#define ADC_SAMPLE_INTERVAL_MS 10
/** ADC reference voltage in V. */
#define ADC_VREF 3.292
/** Upper resistor value of voltage divider in kiloohms. */
#define VOLTAGE_UPPER_RESISTOR 110.0
/** Lower resistor value of voltage divider in kiloohms. */
#define VOLTAGE_LOWER_RESISTOR 10.0
/** Current sense shunt resistor in ohms. */
#define CURRENT_SENSE_RESISTOR 0.002
/** Gain of current-sense amplifier. */
#define CURRENT_SENSE_AMPLIFIER_GAIN 20.0

/**
 * @brief Toggle run LED at a fixed software period.
 * @param time Toggle period in ms.
 */
void PM_LED_RUN(uint16_t time);

/**
 * @brief Run debounce for one key and report a valid press event.
 * @param key_id Logical key ID.
 * @param GPIOx GPIO port of the key input.
 * @param GPIO_Pin GPIO pin of the key input.
 * @return uint8_t 1 when a debounced press is detected, otherwise 0.
 */
uint8_t PM_Key_Debounce(PM_KeyId_t key_id, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Debounce timer for a key based on EXTI pin number.
 * @param GPIO_Pin EXTI pin that triggered the key event.
 */
void PM_Key_get(uint16_t GPIO_Pin);

/**
 * @brief Process all keys and update UI state machine.
 */
void PM_Key_Task(void);

/**
 * @brief Update electrical values from ADC and accumulate energy statistics.
 * @param current_adc_12bit Raw 12-bit ADC data for current channel.
 * @param voltage_adc_12bit Raw 12-bit ADC data for voltage channel.
 */
void PM_ADC_Data_Update(uint16_t current_adc_12bit, uint16_t voltage_adc_12bit);

/**
 * @brief Advance OLED refresh software timer and report refresh event.
 * @return uint8_t 1 when OLED should refresh, otherwise 0.
 */
uint8_t PM_Display_Refresh(void);

/**
 * @brief Render current UI content to OLED and flush display buffer.
 */
void PM_Display_Update(void);

#endif 