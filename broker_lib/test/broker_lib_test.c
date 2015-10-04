#include <stdio.h>
#include <unistd.h>

#include "mosquitto_broker_lib.h"

#include <pthread.h>

typedef struct
{
    int argc;
    char **argv;
    void (*connect_callback)(int port);
    void (*return_callback)(int return_code);
} args_t;

void *thread_main(void *obj)
{
    int ret = 0;
    args_t *args = (args_t *)obj;
    int argc = 0;
    char **argv = NULL;
    if (args != NULL)
    {
        argc = args->argc;
        argv = args->argv;
    }
    ret = broker_start(argc, argv, 4776, args->connect_callback, args->return_callback);
    return NULL;
}

void return_callback(int return_code)
{
    printf("return code = %d\n", return_code);
}

void connect_callback(int port)
{
    printf("port = %d\n", port);
}

int main(int argc, char *argv[])
{
    int reply;
    pthread_t thread_id;
    args_t args = {argc, argv, connect_callback, return_callback};

    broker_init();
    printf("broker running: %d\n", broker_is_running());
	reply = pthread_create(&thread_id, NULL, thread_main, &args);
    printf("broker started\n");
    sleep(1);
    printf("broker running: %d\n", broker_is_running());
    sleep(4);
    broker_stop();
    printf("broker stopped\n");
    printf("broker running: %d\n", broker_is_running());
    sleep(4);
    return 0;
}

