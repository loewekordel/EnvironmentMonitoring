#ifndef _COM_M2M_H_
#define _COM_M2M_H_

extern bool mqtt_stConnected;

/**
 * @brief Wifi initialization
 *
 */
extern void wifi_init(void);

/**
 * @brief Connect Wifi
 *
 */
extern void wifi_connect(void);

/**
 * @brief Publish environment data over MQTT
 *
 * @param tCels Temperature [deg C]
 * @param pHum Humidity [%]
 * @return uint16_t Return the packet ID (or 1 if QoS 0) or 0 if failed
 */
uint16_t mqtt_publish(float tCels, float pHum);

#endif
