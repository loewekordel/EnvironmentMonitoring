# Environment Monitoring
Monitoring home environment using an ESP8266 and sensors as shown in the list below:
- SHT30 (Temperature and humidity)

The project uses Deepsleep to save energy, but it is not fully optimized for low power consumption since it is still connected via USB in my current use case. The Deepsleep is mainly used to avoid temperature and humidity deviations due to heating of the sensor.

Secrets are stored in secrets.h which have to be configured according to your settings.