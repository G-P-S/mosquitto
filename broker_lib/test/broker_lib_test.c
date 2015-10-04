#include <stdio.h>

#include "mosquitto_broker_lib.h"

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x * 1000))
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    int reply;
    int port = 4776;

    broker_init();
    printf("broker running: %d\n", broker_is_running());
    reply = broker_start(argc, argv, &port);
    printf("broker started (status=%d, port=%d)\n", reply, port);
    sleep(1);
    printf("broker running: %d\n", broker_is_running());
    sleep(4);
    broker_stop();
    broker_deinit();
    printf("broker stopped\n");
    printf("broker running: %d\n", broker_is_running());
    return 0;
}

