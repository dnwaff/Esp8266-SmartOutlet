#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define USE_WIFI_MODE		STATIONAP_MODE
#define SSID		"belkin.38a"
#define PASSWORD	"8ed92m42"
#define WIFI_AP_NAME		"ESP8266"
#define WIFI_AP_PASSWORD	"00000000"
#define TCPSERVERIP		"192.168.2.4"
#define TCPSERVERPORT		5555
#define PLATFORM_DEBUG		true
#define timer_time       5000
#define CLIENT_SSL_ENABLE
//#define LWIP_DEBUG		true

//#include "driver/ssd1306.h"
#include "driver/i2c.h"
#include "driver/i2c_oled.h"
#include "mem.h"


#endif


