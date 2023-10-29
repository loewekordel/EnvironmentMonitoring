#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "secrets.h"

#define TDEEPSLEEP (600) // Time to deepsleep (in seconds):

#define WIFI_MAXRECONATTEMPTS (5)

#define MQTT_HOST IPAddress(192, 168, 8, 100)
#define MQTT_PORT (1883)
#define MQTT_TOPIC "EnvData/Living"

#define THINGSPEAK_FIELD_TEMP (1)
#define THINGSPEAK_FIELD_HUM (2)
// #define THINGSPEAK_FIELD_TEMP (3)
// #define THINGSPEAK_FIELD_HUM  (4)

#endif
