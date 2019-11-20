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
#include "freertos/queue.h"
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


#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <driver/dac.h>
#include "driver/spi_slave.h"
#include <driver/i2s.h>


#ifdef CONFIG_EXAMPLE_IPV4
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#else
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#endif

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "step5";

static QueueHandle_t q1;
static bool finished;
static unsigned char raw[256];


//static unsigned char snd[1000000];
//static unsigned char snd[1048576];
static unsigned char snd[1048576/8];

static const char *payload = "OK";
void download_snd()
{
	//char rx_buffer[128];
	char addr_str[128];
	int addr_family;
	int ip_protocol;


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

	int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
	if (sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		goto oops;
	}
	ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);


	//int msg;
	for(int block = 0; block<sizeof(snd)/256; ++block) {

		int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (err < 0) {
			ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
			break;
		}
		//ESP_LOGI(TAG, "Message sent");

		//printf("Block:%d\n", block);
		struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
		socklen_t socklen = sizeof(source_addr);
		int len = recvfrom(sock, snd + block*256, 256 , 0, (struct sockaddr *)&source_addr, &socklen);

		// Error occurred during receiving
		if (len < 0) {
			ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
			break;
		}
		// Data received
		else {
			if(block % 1000 == 0) {
				printf("Block:%d\n", block);
			}
		}

	}

	if (sock != -1) {
		ESP_LOGE(TAG, "Shutting down socket and restarting...");
		shutdown(sock, 0);
		close(sock);
	}
	//}
	//stop =  esp_timer_get_time();
	//int64_t elapsed_secs = (stop - start) / 1000 / 1000;
	//printf("Time taken: %" PRId64 "\n", elapsed_secs);
oops:
finished = true;
puts("Finished downloading");
}
void udp_task5(void *pvParameters)
{
	//char rx_buffer[128];
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

	int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
	if (sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		goto oops;
	}
	ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);


	int msg;
	while (nblocks) {
		nblocks--;
		xQueueReceive(q1, &msg, portMAX_DELAY);

		int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (err < 0) {
			ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
			break;
		}
		//ESP_LOGI(TAG, "Message sent");

		struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
		socklen_t socklen = sizeof(source_addr);
		int len = recvfrom(sock, raw, sizeof(raw) , 0, (struct sockaddr *)&source_addr, &socklen);

		// Error occurred during receiving
		if (len < 0) {
			ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
			break;
		}
		// Data received
		else {
			if(block==0){
				raw[len] = 0; // Null-terminate whatever we received and treat like a string
				ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
				ESP_LOGI(TAG, "%s", raw);
				sscanf((char*)raw, "%d", &nblocks);
				printf("Blocks to scan:%d\n", nblocks);
				//start =esp_timer_get_time();
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
	//stop =  esp_timer_get_time();
	//int64_t elapsed_secs = (stop - start) / 1000 / 1000;
	//printf("Time taken: %" PRId64 "\n", elapsed_secs);
oops:
	finished = true;
	vTaskDelete(NULL);
}



//void do_step4(void *pvParameters)
void do_step5()
{
	esp_err_t ret;
	q1 = xQueueCreate(1, sizeof(int));
	assert(q1);

	//xTaskCreate(udp_task5, "udp_task5", 4096, NULL, 5, NULL);
	download_snd();
	static const int i2s_num = 0; // i2s port number

	static const i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
		.sample_rate = 44100,
		.bits_per_sample = 16,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		//.communication_format = I2S_COMM_FORMAT_I2S_MSB,
		//.communication_format = I2S_COMM_FORMAT_I2S_LSB,
		.communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB,
		.intr_alloc_flags = 0, // default interrupt priority
		.dma_buf_count = 8,
		.dma_buf_len = 64,
		.use_apll = false
	};

	ret = i2s_driver_install(i2s_num, &i2s_config, 0, NULL);	
	assert(ret == ESP_OK);

	ret = i2s_set_clk(0, 44100, 16, I2S_CHANNEL_MONO);
	//ret = i2s_set_clk(0, 8000, 16, I2S_CHANNEL_MONO);
	assert(ret == ESP_OK);
	ret = i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
	assert(ret == ESP_OK);



	puts("Hi 1");
	fflush(stdout);
	puts("Data received");
	fflush(stdout);

	size_t nbytes;
	//ret = i2s_write(0, snd , sizeof(snd), &nbytes, portMAX_DELAY);
	for(int i = 0; i<sizeof(snd)/2; ++i)
		ret = i2s_write(0, snd + i*2 , 2, &nbytes, portMAX_DELAY);

	i2s_driver_uninstall(i2s_num);
	return;


	//while(1) {
	int msg;
	if(xQueueSend(q1, &msg, portMAX_DELAY))
		puts("Queue seems to have started OK");
	else
		puts("Initialisation of queue seems bad");

	finished = false;
	while(!finished) { 
		if(!xQueueSend(q1, &msg, portMAX_DELAY))
			puts("!xQueueSend failed");
		ret = i2s_write(0, raw , sizeof(raw), &nbytes, portMAX_DELAY);
		assert(ret == ESP_OK);
		if(nbytes != sizeof(raw))
			puts("Failed to write 256 bytes to I2S");
	}

	/*
	   for(int i = 0 ; i < track_raw_len; ++i) {
	   uint16_t vol = track_raw[i] << 8;
	   ret = i2s_write(0, &vol , 2, &nbytes, portMAX_DELAY);
	   assert(ret == ESP_OK);
	   }
	   */
	puts("Seems to have written OK");
	//}

	i2s_driver_uninstall(i2s_num);
	//vTaskDelete(NULL);
}


