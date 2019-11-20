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
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

//#include "FreeRTOS_IO.h"

#include "esp_err.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include <inttypes.h>

#ifdef CONFIG_EXAMPLE_IPV4
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#else
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#endif

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "example";
//static const char *payload = "Message from ESP32 ";
static const char *payload = "OK";

void delay_ms(int ms)
{
	vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void udp_client_task(void *pvParameters)
{
	char rx_buffer[128];
	char addr_str[128];
	int addr_family;
	int ip_protocol;

	int block = 0, nblocks = 1;
	//while (1) {

#ifdef CONFIG_EXAMPLE_IPV4
		struct sockaddr_in dest_addr;
		dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(PORT);
		addr_family = AF_INET;
		ip_protocol = IPPROTO_IP;
		inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
#else // IPV6
		struct sockaddr_in6 dest_addr;
		inet6_aton(HOST_IP_ADDR, &dest_addr.sin6_addr);
		dest_addr.sin6_family = AF_INET6;
		dest_addr.sin6_port = htons(PORT);
		addr_family = AF_INET6;
		ip_protocol = IPPROTO_IPV6;
		inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

		int64_t start = 0, stop; //xTaskGetTickCount( void );
		//char start1 = esp_timer_get_time();
		//printf("%c", start1);
		int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
		if (sock < 0) {
			ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
			goto oops;
		}
		ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

		while (nblocks) {
			nblocks--;

			int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
			if (err < 0) {
				ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
				break;
			}
			//ESP_LOGI(TAG, "Message sent");

			struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
			socklen_t socklen = sizeof(source_addr);
			int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) , 0, (struct sockaddr *)&source_addr, &socklen);

			// Error occurred during receiving
			if (len < 0) {
				ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
				break;
			}
			// Data received
			else {
				if(block==0){
				rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
				ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
				ESP_LOGI(TAG, "%s", rx_buffer);
				sscanf(rx_buffer, "%d", &nblocks);
				printf("Blocks to scan:%d\n", nblocks);
				start =esp_timer_get_time();
				}
				else if(nblocks % 1000 == 0) {
					printf("Block:%d\n", nblocks);
				}
			}

			//vTaskDelay(2000 / portTICK_PERIOD_MS);
			block++;
		}

		if (sock != -1) {
			ESP_LOGE(TAG, "Shutting down socket and restarting...");
			shutdown(sock, 0);
			close(sock);
		}
	//}
		stop =  esp_timer_get_time();
		int64_t elapsed_secs = (stop - start) / 1000 / 1000;
		printf("Time taken: %" PRId64 "\n", elapsed_secs);
oops:
	vTaskDelete(NULL);
}

/*
void do_step3()
{
	puts("do_step3: TODO");
}
*/

void user_loop_task(void* pvp)
{
	while(1) {
		puts("\nMenu:");
		puts(" 3: Speed test");
		puts("Enter step:");
		int c = getc(stdin);
		printf("%c\n", c);
		switch(c) {
			case '3':
				xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
				//do_step3();
				break;
		}
	}

}
esp_err_t config_stdio()
{
	/* Initialize VFS & UART so we can use stdin and stdout */
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	/* Install UART driver for interrupt-driven reads and writes */
	ESP_ERROR_CHECK( uart_driver_install( (uart_port_t)CONFIG_ESP_CONSOLE_UART_NUM,
				256, 0, 0, NULL, 0) );
	/* Tell VFS to use UART driver */
	esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
	esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
	/* Move the caret to the beginning of the next line on '\n' */
	esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);
	return ESP_OK;
}

void app_main()
{
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
	 * Read "Establishing Wi-Fi or Ethernet Connection" section in
	 * examples/protocols/README.md for more information about this function.
	 */
	ESP_ERROR_CHECK(example_connect());

	ESP_ERROR_CHECK(config_stdio());

	//user_loop();

	xTaskCreate(user_loop_task, "user_loop", 4096, NULL, 5, NULL);
	//xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}
