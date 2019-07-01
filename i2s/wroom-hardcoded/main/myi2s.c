#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <driver/dac.h>
#include "driver/spi_slave.h"
#include <driver/i2s.h>

#include "track.h"

/*
 * Put your speaker into D26, not D25
 */


typedef uint8_t byte;


void app_main()
{
	esp_err_t ret;


	static const int i2s_num = 0; // i2s port number

	static const i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
		//.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
		.sample_rate = 8000,
		.bits_per_sample = 16,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = I2S_COMM_FORMAT_I2S_MSB,
		.intr_alloc_flags = 0, // default interrupt priority
		.dma_buf_count = 8,
		.dma_buf_len = 64,
		.use_apll = false
			//.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1
	};

	ret = i2s_driver_install(i2s_num, &i2s_config, 0, NULL);	
	assert(ret == ESP_OK);
	ret = i2s_set_clk(0, 8000, 16, I2S_CHANNEL_MONO);
	assert(ret == ESP_OK);
	ret = i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN);
	assert(ret == ESP_OK);


	puts("Hi 1");
	fflush(stdout);



	puts("Data received");
	fflush(stdout);


	size_t nbytes;
	//size_t nblocks = track_raw_len /512;
	//for(int blk = 0 ; blk < nblocks; ++blk) {
	while(1) {
		for(int i = 0 ; i < track_raw_len; ++i) {
			uint16_t vol = track_raw[i] << 8;
			//ret = i2s_write(0, track_raw + 512*blk , 512, &nbytes, portMAX_DELAY);
			ret = i2s_write(0, &vol , 2, &nbytes, portMAX_DELAY);
			//printf("bytes written: %zu\n", nbytes);
			assert(ret == ESP_OK);
		}
		puts("Seems to have written OK");
	}

	for(;;) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}


