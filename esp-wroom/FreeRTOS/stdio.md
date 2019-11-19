# FreeRTOS stdio

Via USB

```
#include "esp_err.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"


void user_loop_task(void* pvp)
{
        while(1) {
                puts("Enter step:");
                int c = getc(stdin);
                printf("%c\n", c);
                if(c == '3')
                        puts("found 3");
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
        //ESP_ERROR_CHECK(nvs_flash_init());

        ESP_ERROR_CHECK(config_stdio());
        xTaskCreate(user_loop_task, "user_loop", 4096, NULL, 5, NULL);
}
```

## Links to other sites

* [stdin_out.c](https://github.com/espressif/esp-idf/blob/master/examples/common_components/protocol_examples_common/stdin_out.c)
