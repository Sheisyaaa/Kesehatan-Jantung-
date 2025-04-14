#ifndef CONFIG_H
#define CONFIG_H

// #define WIFI_SSID "wifi_ssid"
// #define WIFI_PASSWORD "wifi_password"

#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_TOPIC "esp32/data"

// Definisi pin untuk Touchscreen
#define TP_SDA 9
#define TP_SCL 46
#define TP_INT_PIN 8
#define TP_RST_PIN 3

// Resolusi Layar
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 170
#define TFT_BL 38
#define GFX_BL DF_GFX_BL

#endif
