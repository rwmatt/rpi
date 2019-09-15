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

/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#if __has_include("/home/mcarter/repos/redact/docs/secret/settings.h")
#include "/home/mcarter/repos/redact/docs/secret/settings.h"
//else
#define EXAMPLE_WIFI_SSID WIFI_SSID
#define EXAMPLE_WIFI_PASS WIFI_PASSWORD
#endif

#define PORT CONFIG_EXAMPLE_PORT

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;


const int led = 19;
const int bzr = 22;


const char *TAG = "alacrity";

char rx_buffer[128];
char tx_buffer[128];

int secs(int n) { return n * 1000 / portTICK_PERIOD_MS;}
#define DELAY_S(n) vTaskDelay(secs(n))
#define DELAY_MIN(n) for(int i=0; i<n; ++i) { DELAY_S(60); }
#define DELAY_MS(n) vTaskDelay(n/portTICK_PERIOD_MS)

static int alarm_activated = 0;
void both_on(int yes)
{
	gpio_set_level(bzr, yes);
	gpio_set_level(led, yes);
}

int do_beep_twice = 0;
static void beep_twice_task(void* pvParameters)
{
	while(1) {
		if(do_beep_twice) {
			for(int j = 0; j<2; ++j) {
				for(int i = 0; i<5; ++i) {
					both_on(1);
					DELAY_MS(100);
					both_on(0);
					DELAY_MS(100);
				}
				DELAY_MIN(1);
			}
			do_beep_twice = 0;
		}
		DELAY_S(1);
	}

	vTaskDelete(NULL);
}

static void remind(void *pvParameters)
{
	uint32_t on_hr = (uint32_t) pvParameters;
	ESP_LOGI(TAG, "remind started for hour %d", on_hr);
	//struct tm tinfo;
	for(;;) {
		//time_t t = mktime(&tinfo);
		//time_t now = time(0);
		//struct tm* localtm = localtime(&now);
		if(get_hr() == on_hr) {
			do_beep_twice =1;
			//DELAY_MIN(2);
			//do_beep_twice =1;
			//while(get_hr() == on_hr) DELAY_MIN(1);
			DELAY_MIN(63);
		}
		DELAY_S(10);
	}
}


static void play_alarm(void *pvParameters)
{
	while(1) {
		if(alarm_activated) {
			alarm_activated = 0;
			for(int i=0; i<20; ++i) {
				both_on(1);
				DELAY_S(4);
				both_on(0);
				DELAY_S(1);
				both_on(1);
				DELAY_S(1);
				both_on(0);
				DELAY_S(1);
			}
		}
		DELAY_S(1);
	}
	vTaskDelete(NULL);
}


int matching(char* str)
{
	if(strncmp(rx_buffer, str, strlen(str)) == 0)
		return 1;
	else
		return 0;
}

void process_rx()
{
	int len = strlen(rx_buffer);

	// chomp line terminators
	while((len>0) && ((rx_buffer[len-1] == '\n') || (rx_buffer[len-1] == '\r')))
		rx_buffer[--len] = 0;

	if(matching("ALON")) {
		alarm_activated = 1;
	} else if(matching("BEEP")) {
		do_beep_twice = 1;
	}
	rx_buffer[0] = 0;

	strcpy(tx_buffer, "OK\n");
				
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        /* enable ipv6 */
        tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
        xEventGroupClearBits(wifi_event_group, IPV6_GOTIP_BIT);
        break;
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
        xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

        char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
        ESP_LOGI(TAG, "IPv6: %s", ip6);
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
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static void wait_for_ip()
{
    uint32_t bits = IPV4_GOTIP_BIT | IPV6_GOTIP_BIT ;

    ESP_LOGI(TAG, "Waiting for AP connection...");
    xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");
}

static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {

#ifdef CONFIG_EXAMPLE_IPV4
        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
        struct sockaddr_in6 destAddr;
        bzero(&destAddr.sin6_addr.un, sizeof(destAddr.sin6_addr.un));
        destAddr.sin6_family = AF_INET6;
        destAddr.sin6_port = htons(PORT);
        addr_family = AF_INET6;
        ip_protocol = IPPROTO_IPV6;
        inet6_ntoa_r(destAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket binded");

        err = listen(listen_sock, 1);
        if (err != 0) {
            ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_in6 sourceAddr; // Large enough for both IPv4 or IPv6
        uint addrLen = sizeof(sourceAddr);
        int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occured during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Connection closed
            else if (len == 0) {
                ESP_LOGI(TAG, "Connection closed");
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (sourceAddr.sin6_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
                } else if (sourceAddr.sin6_family == PF_INET6) {
                    inet6_ntoa_r(sourceAddr.sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);

		process_rx();

                int err = send(sock, tx_buffer, strlen(tx_buffer), 0);
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                    break;
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);

	    // mcarter 13-Sep-2019 err 112 fix. Ostensibly
	    shutdown(listen_sock, 0);
	    close(listen_sock);

	    vTaskDelay(5);
        }
    }
    vTaskDelete(NULL);
}

void app_main()
{
    ESP_ERROR_CHECK( nvs_flash_init() );

    gpio_pad_select_gpio(bzr);
    gpio_set_direction(bzr, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);

    initialise_wifi();
    wait_for_ip();


    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
    xTaskCreate(play_alarm, "play_alarm", 4096, NULL, 5, NULL);
    xTaskCreate(remind, "remind09", 4096, (void*)  9, 5, NULL);
    xTaskCreate(remind, "remind12", 4096, (void*) 12, 5, NULL);
    xTaskCreate(remind, "remind17", 4096, (void*) 17, 5, NULL);
    xTaskCreate(remind, "remind21", 4096, (void*) 21, 5, NULL);
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
    xTaskCreate(beep_twice_task, "beep_twice", 4096, NULL, 5, NULL);
}
