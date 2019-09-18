/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "common.h"

#include "/home/mcarter/repos/redact/docs/secret/settings.h"

#define PORT 3333
#define HOST_IP_ADDR "192.168.0.32"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
#ifdef CONFIG_EXAMPLE_IPV6
const int IPV6_GOTIP_BIT = BIT1;
#endif

static const char *TAG = "alarm";

int secs(int n) { return n * 1000 / portTICK_PERIOD_MS;}
#define DELAY_S(n) vTaskDelay(secs(n))
#define DELAY_MIN(n) for(int i=0; i<n; ++i) { DELAY_S(60); }
#define DELAY_MS(n) vTaskDelay(n/portTICK_PERIOD_MS)


const int btn = 18;

static void send_message(char* msg);

void _button_task()
{
	ESP_LOGI(TAG, "_button_task() called");
	send_message("ALON\n");
	raise_mqtt_alarm();
	while(gpio_get_level(btn) == 0) DELAY_MS(30);
}

static void button_task(void *pvParameters)
{
	for(;;) {
		if(gpio_get_level(btn) == 0)
			_button_task();
		DELAY_MS(10);
	}
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
#ifdef CONFIG_EXAMPLE_IPV6
        /* enable ipv6 */
        tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
#endif
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
#ifdef CONFIG_EXAMPLE_IPV6
        xEventGroupClearBits(wifi_event_group, IPV6_GOTIP_BIT);
#endif
        break;
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
#ifdef CONFIG_EXAMPLE_IPV6
        xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

        char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
        ESP_LOGI(TAG, "IPv6: %s", ip6);
#endif
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void wait_for_ip()
{
#ifdef CONFIG_EXAMPLE_IPV6
    uint32_t bits = IPV4_GOTIP_BIT | IPV6_GOTIP_BIT;
#else
    uint32_t bits = IPV4_GOTIP_BIT;
#endif

    ESP_LOGI(TAG, "Waiting for AP connection...");
    xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");
}

static void send_message(char* msg)
{
	char rx_buffer[128];
	char addr_str[128];
	int addr_family;
	int ip_protocol;


#ifdef CONFIG_EXAMPLE_IPV4
	struct sockaddr_in destAddr;
	destAddr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(PORT);
	addr_family = AF_INET;
	ip_protocol = IPPROTO_IP;
	inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
	struct sockaddr_in6 destAddr;
	inet6_aton(HOST_IP_ADDR, &destAddr.sin6_addr);
	destAddr.sin6_family = AF_INET6;
	destAddr.sin6_port = htons(PORT);
	addr_family = AF_INET6;
	ip_protocol = IPPROTO_IPV6;
	inet6_ntoa_r(destAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

	int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
	if (sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		goto finis;
	}
	ESP_LOGI(TAG, "Socket created");

	int err = connect(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
	if (err != 0) {
		ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
		goto finis;
	}
	ESP_LOGI(TAG, "Successfully connected");

	err = send(sock, msg, strlen(msg), 0);
	if (err < 0) {
		ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
		goto finis;
	}

	int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
	// Error occured during receiving
	if (len < 0) {
		ESP_LOGE(TAG, "recv failed: errno %d", errno);
		goto finis;
	}
	// Data received
	else {
		rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
		ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
		ESP_LOGI(TAG, "%s", rx_buffer);
	}

finis:
	if (sock != -1) {
		ESP_LOGI(TAG, "Shutting down socket");
		shutdown(sock, 0);
		close(sock);
	}
    
}

void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );

    gpio_pad_select_gpio(btn);
    gpio_set_direction(btn, GPIO_MODE_INPUT);
    gpio_set_pull_mode(btn, GPIO_PULLUP_ONLY);

    initialise_wifi();
    wait_for_ip();
    mqtt_app_main();

    xTaskCreate(button_task, "button_task", 4096, NULL, 5, NULL);
}
