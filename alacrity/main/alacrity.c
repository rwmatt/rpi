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
//#include "freertos/event_groups.h"
#include "esp_system.h"
//#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "common.h"
#include "networkmc.h"

const volatile int led = 19;
const volatile int bzr = 22;


/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

#define HOST_IP_ADDR "192.168.0.17"
//#ifdef CONFIG_EXAMPLE_IPV4
//#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
//#else
//#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
//#endif

//#define PORT CONFIG_EXAMPLE_PORT
#define PORT 1762

/* FreeRTOS event group to signal when we are connected & ready to make a request */
//static EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;

const char *TAG = "alacrity";
static const char *payload = "Message from ESP32 ";
//static const char *payload = "";

static volatile int alarm_activated = 0;

int secs(int n) { return n * 1000 / portTICK_PERIOD_MS;}

void both_on(int yes)
{
        gpio_set_level(bzr, yes);
        gpio_set_level(led, yes);
}

void beepn(int n)
{
        for(int i = 0; i<n; ++i) {
                both_on(1);
                DELAY_MS(100);
                both_on(0);
                DELAY_MS(100);
        }
}



int get_hr()
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	time_t rawtime = time(NULL);
	struct tm* tm = localtime(&rawtime);
	return tm->tm_hour;
}

void set_datetime(char* str)
{

	int  y, m , d, dow, hr, min, sec, dummy;
	sscanf(str, "%d %d %d %d %d %d %d %d", &y, &m , &d, &dow, &hr, &min, &sec, &dummy);
	struct tm tm;
	tm.tm_year = y-1900;
	tm.tm_mon = m-1;
	tm.tm_mday = d;
	tm.tm_hour = hr;
	tm.tm_min = min;
	tm.tm_sec = sec;
	ESP_LOGI(TAG, "Setting time: %s", asctime(&tm));

	time_t t = mktime(&tm);
	struct timeval now = { .tv_sec = t };
	settimeofday(&now, NULL);

}

void tcp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {

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
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = connect(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Successfully connected");

        while (1) {
		 // /* don't think we need send
            int err = send(sock, payload, strlen(payload), 0);
            if (err < 0) {
                ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                break;
            }
	    // */

            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occured during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);
		set_datetime(rx_buffer);
            }

            //vTaskDelay(2000 / portTICK_PERIOD_MS);
	    DELAY_MIN(30);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}


static void play_alarm(void *pvParameters)
{
        while(1) {
                if(alarm_activated) {
                        alarm_activated = 0;
                        both_on(1);
                        DELAY_S(10);
                        both_on(0);
                        /*
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
                        */
                }
                DELAY_S(1);
        }
        vTaskDelete(NULL);
}


static volatile int do_beep_twice = 0;
static void beep_twice_task(void* pvParameters)
{
        while(1) {
                if(do_beep_twice) {
                        for(int j = 0; j<2; ++j) {
                                beepn(15);
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


#define BOOT  GPIO_NUM_0 // boot button GPIO
int is_boot_pressed()
{
        return 0 == gpio_get_level(BOOT);
}

void boot_btn_task( void* pvParameters)
{
        //gpio_pad_select_gpio(boot);
        gpio_set_direction(BOOT, GPIO_MODE_INPUT);
        gpio_set_pull_mode(BOOT, GPIO_PULLUP_ONLY);
        for(;;) {
                if(is_boot_pressed()) {
                        gpio_set_level(bzr, 1);
                        while(is_boot_pressed()) DELAY_MS(20);
                        gpio_set_level(bzr, 0);
                }
                DELAY_MS(20);
                        
        }
}

void boot_beep_task(void* pv)
{
        // Let everyone know we've just started
        beepn(2);
        vTaskDelete(NULL);
}




void app_main()
{
        gpio_pad_select_gpio(bzr);
        gpio_set_direction(bzr, GPIO_MODE_OUTPUT);
        gpio_pad_select_gpio(led);
        gpio_set_direction(led, GPIO_MODE_OUTPUT);

        xTaskCreate(boot_beep_task, "boot_beep_task", 1024, NULL, 5, NULL);

        ESP_ERROR_CHECK( nvs_flash_init() );
        initialise_wifi();
        wait_for_ip();


        //xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
        xTaskCreate(play_alarm, "play_alarm", 4096, NULL, 5, NULL);
        xTaskCreate(remind, "remind09", 4096, (void*)  9, 5, NULL);
        xTaskCreate(remind, "remind12", 4096, (void*) 12, 5, NULL);
        xTaskCreate(remind, "remind17", 4096, (void*) 17, 5, NULL);
        xTaskCreate(remind, "remind21", 4096, (void*) 21, 5, NULL);
        xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
        xTaskCreate(beep_twice_task, "beep_twice", 4096, NULL, 5, NULL);
        xTaskCreate(boot_btn_task, "boot_btn_task", 1024, NULL, 5, NULL);
}

