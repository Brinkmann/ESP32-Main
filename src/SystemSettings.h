#ifndef SYSTEM_SETTINGS_H
#define SYSTEM_SETTINGS_H

#ifndef USB_SERIAL_PORT_BAUD_RATE 
#define USB_SERIAL_PORT_BAUD_RATE 115200
#endif
 
#ifndef ESP32_CLOCK_SPEED_WIFI_BLE
#define ESP32_CLOCK_SPEED_WIFI_BLE 80
#endif

#ifndef DEVICE_WIFI_MANAGER_SSID 
#define DEVICE_WIFI_MANAGER_SSID "FEQ-HUB-WiFi-Mgr"
#endif

#ifndef DEVICE_WIFI_MANAGER_PWRD 
#define DEVICE_WIFI_MANAGER_PWRD "6140667814-FEQ"
#endif
/**
 * @note GPIO 38 Used in production attached to actual strip.
 *       GPIO 48 Used for debugging GPIO-RGP-LED on board.
 * 
 */
#ifndef RBG_LED_STRIP_DATA_PIN_PRIMARY
#define RGB_LED_STRIP_DATA_PIN_PRIMARY 38 
#endif

#ifndef RGB_LED_STRIP_NUMBER_OF_LEDS 
#define RGB_LED_STRIP_NUMBER_OF_LEDS 60
#endif
 
#endif