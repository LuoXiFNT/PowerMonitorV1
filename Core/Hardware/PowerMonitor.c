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

#include "PowerMonitor.h"

/**
 * @file PowerMonitor.c
 * @brief Power monitor runtime implementation.
 * @author LuoXiFNT
 * @date 2026-03-25
 * @lastModified 2026-03-25
 */

/** LED software counter used by PM_LED_RUN(). */
uint16_t pm_led_cnt = 0;
/** Debounce countdown counters for each key. */
uint16_t pm_key_cnt[KEY_MAX] = {0};
/** Internal counter for periodic OLED refresh. */
uint16_t pm_oled_refresh_cnt = 0;
/** Internal counter for periodic ADC Sampling. */
uint16_t pm_adc_sample_cnt = 0;

/** Current display page mode. */
PM_DisplayMode_t pm_display_mode = MODE_POWER;
/** Current screen freeze state. */
PM_FreezeState_t pm_freeze_state = SCREEN_UNFREEZE;
/** Current clear-confirm workflow state. */
PM_ClearState_t pm_clear_state = STATE_NORMAL;
/** OLED refresh period in ms. */
PM_RefreshRate_t pm_refresh_rate = REFRESH_100MS;
/** Data structure for storing analog data. */
PM_Analog_Data_t pm_analog_data = {0.0};

/**
 * @brief Toggle run LED at a fixed software period.
 * @param time Toggle period in ms.
 */
void PM_LED_RUN(uint16_t time)
{
  if (++pm_led_cnt >= time)
  {
    pm_led_cnt = 0;
    HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
  }
}

/**
 * @brief Run debounce for one key and report a valid press event.
 * @param key_id Logical key ID.
 * @param GPIOx GPIO port of the key input.
 * @param GPIO_Pin GPIO pin of the key input.
 * @return uint8_t 1 when a debounced press is detected, otherwise 0.
 */
uint8_t PM_Key_Debounce(PM_KeyId_t key_id, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  if (pm_key_cnt[key_id] > 0 && --pm_key_cnt[key_id] == 0)
  {
    return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);
  }
  return 0;
}

/**
 * @brief Debounce timer for a key based on EXTI pin number.
 * @param GPIO_Pin EXTI pin that triggered the key event.
 */
void PM_Key_get(uint16_t GPIO_Pin)
{
  PM_KeyId_t id = (GPIO_Pin == MENU_SW_Pin)  ? KEY_MENU  :
                  (GPIO_Pin == HOLD_SW_Pin)  ? KEY_HOLD  :
                  (GPIO_Pin == CLEAR_SW_Pin) ? KEY_CLEAR :
                  (GPIO_Pin == UNIT_SW_Pin)  ? KEY_UNIT  : KEY_MAX;

  if (id != KEY_MAX && pm_key_cnt[id] == 0)
  {
    pm_key_cnt[id] = DEBOUNCE_TIME;
  }
}

/**
 * @brief Process all keys and update UI state machine.
 */
void PM_Key_Task(void)
{
  if (PM_Key_Debounce(KEY_MENU, MENU_SW_GPIO_Port, MENU_SW_Pin))
  {
    pm_freeze_state = SCREEN_UNFREEZE;
    if (pm_clear_state == STATE_CONFIRM_CLEAR) 
    {
      pm_clear_state = STATE_NORMAL;
    }
    else
    {
      pm_display_mode = (pm_display_mode == MODE_POWER) ? MODE_ENERGY : MODE_POWER;
    }
    OLED_Clear();
  }

  if (PM_Key_Debounce(KEY_HOLD, HOLD_SW_GPIO_Port, HOLD_SW_Pin) && pm_clear_state == STATE_NORMAL)
  {
    pm_freeze_state = (pm_freeze_state == SCREEN_UNFREEZE) ? SCREEN_FREEZE : SCREEN_UNFREEZE;
  }

  if (PM_Key_Debounce(KEY_CLEAR, CLEAR_SW_GPIO_Port, CLEAR_SW_Pin))
  {
    pm_freeze_state = SCREEN_UNFREEZE;
    if (pm_clear_state == STATE_NORMAL)
    {
      pm_clear_state = STATE_CONFIRM_CLEAR;
    }
    else
    {
      pm_analog_data.energy = pm_analog_data.capacity = pm_analog_data.time = 0.0;
      pm_clear_state = STATE_NORMAL;
    }
    OLED_Clear();
  }

  if (PM_Key_Debounce(KEY_UNIT, UNIT_SW_GPIO_Port, UNIT_SW_Pin))
  {
    pm_freeze_state = SCREEN_UNFREEZE;
    pm_refresh_rate = (pm_refresh_rate == REFRESH_10MS)  ? REFRESH_25MS  :
                      (pm_refresh_rate == REFRESH_25MS)  ? REFRESH_50MS  :
                      (pm_refresh_rate == REFRESH_50MS)  ? REFRESH_100MS :
                      (pm_refresh_rate == REFRESH_100MS) ? REFRESH_200MS : REFRESH_10MS;
    OLED_Clear();
  }
}

/**
 * @brief Update electrical values from ADC and accumulate energy statistics.
 * @param current_adc_12bit Raw 12-bit ADC data for current channel.
 * @param voltage_adc_12bit Raw 12-bit ADC data for voltage channel.
 */
void PM_ADC_Data_Update(uint16_t current_adc_12bit, uint16_t voltage_adc_12bit)
{
  if (++pm_adc_sample_cnt >= ADC_SAMPLE_INTERVAL_MS)
    {
      pm_adc_sample_cnt = 0;

      pm_analog_data.current = (current_adc_12bit / 4096.0 * ADC_VREF - ADC_VREF / 2.0) / CURRENT_SENSE_AMPLIFIER_GAIN / CURRENT_SENSE_RESISTOR;
      pm_analog_data.voltage = voltage_adc_12bit / 4096.0 * ADC_VREF * (VOLTAGE_UPPER_RESISTOR + VOLTAGE_LOWER_RESISTOR) / VOLTAGE_LOWER_RESISTOR;
      pm_analog_data.power = pm_analog_data.current * pm_analog_data.voltage;

      pm_analog_data.dt_hours = (double)ADC_SAMPLE_INTERVAL_MS / 3600.0 / 1000.0;
      pm_analog_data.energy += pm_analog_data.power * pm_analog_data.dt_hours;
      pm_analog_data.capacity += pm_analog_data.current * 1000.0 * pm_analog_data.dt_hours;
      pm_analog_data.time += (double)ADC_SAMPLE_INTERVAL_MS / 1000.0;
    }
  
}

/**
 * @brief Advance OLED refresh software timer and report refresh event.
 * @return uint8_t 1 when OLED should refresh, otherwise 0.
 */
uint8_t PM_Display_Refresh(void)
{
  if (++pm_oled_refresh_cnt >= pm_refresh_rate)
  {
    pm_oled_refresh_cnt = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief Render current UI content to OLED and flush display buffer.
 */
void PM_Display_Update(void)
{
  if (pm_clear_state == STATE_CONFIRM_CLEAR)
  {
    OLED_ShowString(8, 24, "Confirm CLEAR?", OLED_8X16);
  }
  else if (pm_clear_state == STATE_NORMAL && pm_freeze_state == SCREEN_UNFREEZE)
  {
    OLED_Printf(0, 0, OLED_8X16, "PMS V1.0 %3dms  ", pm_refresh_rate);
    
    switch (pm_display_mode)
    {
      case MODE_POWER:
        OLED_Printf(0, 16, OLED_8X16, "P:%10.6f W  ", pm_analog_data.power);
        OLED_Printf(0, 32, OLED_8X16, "I:%10.6f A  ", pm_analog_data.current);
        OLED_Printf(0, 48, OLED_8X16, "V:%10.6f V  ", pm_analog_data.voltage);
        break;
      case MODE_ENERGY:
        OLED_Printf(0, 16, OLED_8X16, "E:%10.4f Wh ", pm_analog_data.energy);
        OLED_Printf(0, 32, OLED_8X16, "Q:%10.4f mAh", pm_analog_data.capacity);
        OLED_Printf(0, 48, OLED_8X16, "T:%10.2f s  ", pm_analog_data.time);
        break;
    }
  }
  OLED_Update();
}

