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



#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <driver/dac.h>
#include "driver/spi_slave.h"
#include <driver/i2s.h>


#include "common.h"



//static const char *TAG = "step5";

static QueueHandle_t q1;

static unsigned char buf1[1024];
static unsigned char buf2[1024];



void fetch5(void *pvParameters)
{
	uint8_t* buf;
	static int idx = 0;
	puts("fetch5 started");
	while(1) {
		puts("fetch5 inner loop");
		assert(q1);
		xQueueReceive(q1, &buf, portMAX_DELAY);
		printf("fetch5 values %p %p %p\n", buf, buf1, buf2);
		puts("... buf info");
		bzero(buf, 512);
		if(idx+512 > track_raw_len) idx = 0;
		memcpy(buf, track_raw + idx, 512);
		idx += 512;
		for(int i = 512; i>0; i--) {
			buf[i*2-1] = buf[i-1];
			buf[i*2-2] = 0;
		}

	}
}

//void do_step4(void *pvParameters)
void do_step5()
{
	puts("Doing step 5");
	esp_err_t ret;
	q1 = xQueueCreate(5, sizeof(unsigned char *));
	assert(q1);

	xTaskCreate(fetch5, "fetch5", 4096, NULL, 5, NULL);
	static const int i2s_num = 0; // i2s port number

	static const i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
		.sample_rate = 8000,
		.bits_per_sample = 16,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = I2S_COMM_FORMAT_I2S_MSB,
		.intr_alloc_flags = 0, // default interrupt priority
		.dma_buf_count = 8,
		.dma_buf_len = 64,
		.use_apll = false
	};

	ret = i2s_driver_install(i2s_num, &i2s_config, 0, NULL);	
	assert(ret == ESP_OK);

	ret = i2s_set_clk(0, 8000, 16, I2S_CHANNEL_MONO);
	assert(ret == ESP_OK);
	ret = i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
	assert(ret == ESP_OK);

	size_t nbytes;
	unsigned char *pc = buf1;
	xQueueSend(q1, &pc, portMAX_DELAY);
	while(1) {
		pc = buf2;
		xQueueSend(q1, &pc, 0);
		ret = i2s_write(0, buf1 , sizeof(buf1), &nbytes, portMAX_DELAY);
		pc = buf1;
		xQueueSend(q1, &pc, 0);
		ret = i2s_write(0, buf2 , sizeof(buf2), &nbytes, portMAX_DELAY);
	}
	//i2s_driver_uninstall(i2s_num);
}


