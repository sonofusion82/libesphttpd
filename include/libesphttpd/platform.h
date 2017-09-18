#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef FREERTOS
#include <libesphttpd/esp.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

//#include "esp_timer.h"
typedef struct RtosConnType RtosConnType;
typedef RtosConnType* ConnTypePtr;
#ifdef ESP32
typedef TimerHandle_t HttpdPlatTimerHandle;
#else
typedef xTimerHandle HttpdPlatTimerHandle;
#endif

#ifndef ESP32 //esp32 does not need this because it can map flash into D-port memory
#define httpd_printf(fmt, ...) do {	\
	static const char flash_str[] ICACHE_RODATA_ATTR STORE_ATTR = fmt;	\
	printf(flash_str, ##__VA_ARGS__);	\
	} while(0)
#else
#define httpd_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif
#ifdef ESP32
#define ICACHE_FLASH_ATTR
#endif

#elif defined(linux)

#include <stdbool.h>
typedef struct RtosConnType RtosConnType;
typedef RtosConnType* ConnTypePtr;

#define httpd_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define vTaskDelay(milliseconds) usleep((milliseconds) * 1000)
#define portTICK_RATE_MS 1

typedef struct
{
	timer_t timer;
	int timerPeriodMS;
	bool autoReload;
	void (*callback)(void* arg);
	void* callbackArg;
} HttpdPlatTimer;

typedef HttpdPlatTimer* HttpdPlatTimerHandle;

#define ICACHE_FLASH_ATTR
#define ICACHE_RODATA_ATTR

#else // no-os, map to os-specific versions that have to be defined

#define printf(...) os_printf(__VA_ARGS__)
#define sprintf(str, ...) os_sprintf(str, __VA_ARGS__)
#define strcpy(a, b) os_strcpy(a, b)
#define strncpy(a, b, c) os_strncpy(a, b, c)
#define strcmp(a, b) os_strcmp(a, b)
#define strncmp(a, b, c) os_strncmp(a, b, c)
#define malloc(x) os_malloc(x)
#define free(x) os_free(x)
#define memset(x, a, b) os_memset(x, a, b)
#define memcpy(x, a, b) os_memcpy(x, a, b)
#define strcat(a, b) os_strcat(a, b)
#define strstr(a, b) os_strstr(a, b)
#define strlen(a) os_strlen(a)
#define memcmp(a, b, c) os_memcmp(a, b, c)
typedef struct espconn* ConnTypePtr;
typedef struct HttpdPlatTimer HttpdPlatTimer;
typedef *HttpdPlatTimer HttpdPlatTimerHandle;
#define httpd_printf(format, ...) os_printf(format, ##__VA_ARGS__)
#endif



#endif
