#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>

/** @brief environment data structure*/
typedef struct envData_s
{
  float tCels; /** Temperatur [deg C] */
  float pHum;  /** Humidity [%]*/
} envData_t;

#endif
