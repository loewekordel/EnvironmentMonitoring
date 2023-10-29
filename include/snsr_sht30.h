#ifndef _SNSR_SHT30_H_
#define _SNSR_SHT30_H_

#include <WEMOS_SHT3X.h>

/**
 * @brief Get data from SHT30 sensor
 *
 * @param tCels Temperature [deg C]
 * @param pHum Humidity [%]
 * @return byte 0 on success, else 1
 */
extern byte sht30_getData(float *tCels, float *pHum);

#endif
