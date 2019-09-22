#pragma once

extern const char *TAG;

//void raise_mqtt_alarm();
void mqtt_app_main();
//void pub_msg(char* data);
void pub_topic_msg(char* topic, char* msg);
