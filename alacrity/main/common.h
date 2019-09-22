#pragma once


extern const int IPV4_GOTIP_BIT;
extern const int IPV6_GOTIP_BIT;

extern const char *TAG;
//void tcp_client_task(void *pvParameters);
void mqtt_app_main();
int get_hr();

int secs(int n);
#define DELAY_S(n) vTaskDelay(secs(n))
#define DELAY_MIN(n) for(int i=0; i<n; ++i) { DELAY_S(60); }
#define DELAY_MS(n) vTaskDelay(n/portTICK_PERIOD_MS)
