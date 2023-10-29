#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include "com_m2m.h"
#include "config.h"

bool mqtt_stConnected = false;
static uint8_t wifi_cntrReconnect = 0u;

Ticker mqttReconnectTimer;
AsyncMqttClient mqttClient;
Ticker wifiReconnectTimer;
WiFiEventHandler wifiConnectedHandler;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void wifi_connect();
void wifi_onConnected(const WiFiEventStationModeConnected &event);
void wifi_onGotIp(const WiFiEventStationModeGotIP &event);
void wifi_onDisconnected(const WiFiEventStationModeDisconnected &event);
void mqtt_connect();
void mqtt_onConnected(bool sessionPresent);
void mqtt_onDisconnected(AsyncMqttClientDisconnectReason reason);
void mqtt_onPublish(uint16_t packetId);

/**
 * @brief Wifi initalization
 *
 */
void wifi_init(void)
{
  wifiConnectedHandler = WiFi.onStationModeConnected(wifi_onConnected);
  wifiConnectHandler = WiFi.onStationModeGotIP(wifi_onGotIp);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(wifi_onDisconnected);

  mqttClient.onConnect(mqtt_onConnected);
  mqttClient.onDisconnect(mqtt_onDisconnected);
  mqttClient.onPublish(mqtt_onPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}

/**
 * @brief Connect Wifi
 *
 */
void wifi_connect(void)
{
  LOG("[WiFi] Connecting to Wi-Fi ");
  LOGLN(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

/**
 * @brief Wifi connected callback
 *
 * @param event Wifi Station Mode Connected evend
 */
void wifi_onConnected(const WiFiEventStationModeConnected &event)
{
  LOGLN("[WiFi] Wifi connected");
}

/**
 * @brief Wifi got IP callback
 *
 * @param event WiFi Station Mode Got IP event
 */
void wifi_onGotIp(const WiFiEventStationModeGotIP &event)
{
  LOG("[WiFi] Got IP: ");
  LOGLN(WiFi.localIP());
  mqtt_connect();
}

/**
 * @brief Wifi disconnected callback
 *
 * @param event WiFi Station Mode Disconnected event
 */
void wifi_onDisconnected(const WiFiEventStationModeDisconnected &event)
{
  LOG("[WiFi] Wifi disconnected. Reason: ");
  LOGLN(event.reason);

  switch (event.reason)
  {
  case WIFI_DISCONNECT_REASON_ASSOC_LEAVE:
    break;
  default:
    if (wifi_cntrReconnect < WIFI_MAXRECONATTEMPTS)
    {
      LOGLN("[WiFi] Try to reconnect");
      mqttReconnectTimer.detach(); // ensure not attempting reconnecting to MQTT while reconnecting to Wi-Fi
      wifiReconnectTimer.once(2, wifi_connect);
      wifi_cntrReconnect++;
    }
    else
    {
      LOGLN("[WiFi] Max. number of reconnect attempts reached. Shutdown and try later");
      ESP.deepSleep((TDEEPSLEEP * 10e5), WAKE_RF_DEFAULT); // Sleep for TDEEPSLEEP seconds
    }
    break;
  };
}

/**
 * @brief Connect to MQTT server
 *
 */
void mqtt_connect()
{
  LOGLN("[MQTT] Connecting to MQTT");
  mqttClient.connect();
}

/**
 * @brief MQTT connect callback
 *
 * @param sessionPresent Session alrady present
 */
void mqtt_onConnected(bool sessionPresent)
{

  LOGLN("[MQTT] Connected to MQTT");
  LOG("[MQTT] Session present: ");
  LOGLN(sessionPresent);
  mqtt_stConnected = true;
}

/**
 * @brief MQTT disconnect callback
 *
 * @param reason Disconnect reason
 */
void mqtt_onDisconnected(AsyncMqttClientDisconnectReason reason)
{
  LOGLN("[MQTT] Disconnected from MQTT");

  if (WiFi.isConnected())
  {
    mqttReconnectTimer.once(2, mqtt_connect);
  }
}

/**
 * @brief MQTT publish callback
 *
 * @param packetId Acknowlged packet id
 */
void mqtt_onPublish(uint16_t packetId)
{
  LOGLN("[MQTT] Publish acknowledged");
  LOG("  packetId: ");
  LOGLN(packetId);
}

/**
 * @brief Publish environment data over MQTT
 *
 * @param tCels Temperature [deg C]
 * @param pHum Humidity [%]
 * @return uint16_t Return the packet ID (or 1 if QoS 0) or 0 if failed
 */
uint16_t mqtt_publish(float tCels, float pHum)
{
  char json[150];

  sprintf(json, "{\"tCels\": %.1f, \"pHum\": %.1f}", tCels, pHum);
  return mqttClient.publish(MQTT_TOPIC, 0, true, json);
}
