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

#include "esp_err.h"

#include "esp_vfs_dev.h"
#include "driver/uart.h"



#include <inttypes.h>

void delay_ms(int ms)
{
	vTaskDelay(ms / portTICK_PERIOD_MS);
}


void do_step3(void *pvParameters);
void do_step4();
void do_step5();


void user_loop_task(void* pvp)
{
	while(1) {
		puts("\nMenu:");
		puts(" 3: Speed test");
		puts(" 4: hard-coded sound test");
		puts(" 5: wifi sound test");
		puts("Enter step:");
		int c = getc(stdin);
		printf("%c\n", c);
		switch(c) {
			case '3':
				xTaskCreate(do_step3, "step3", 4096, NULL, 5, NULL);
				break;
			case '4':
				do_step4();
				break;
			case '5':
				do_step5();
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

	xTaskCreate(user_loop_task, "user_loop", 4096, NULL, 5, NULL);
}
