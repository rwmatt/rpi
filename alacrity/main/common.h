#pragma once


extern const int IPV4_GOTIP_BIT;
extern const int IPV6_GOTIP_BIT;

extern const char *TAG;
void tcp_client_task(void *pvParameters);
int get_hr();
