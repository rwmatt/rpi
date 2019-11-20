# FreeRTOS timing

## elapsed time

```
#include <inttypes.h> // for PRId64

int64_t start = esp_timer_get_timer(); // in microseconds
...
int64_t stop = esp_timer_get_timer();
int64_t elapsed_seconds = (stop -  start)/ 1000 / 1000;
printf("Seconds elapsed: %" PRId64 "\n", elapsed_seconds();
```
