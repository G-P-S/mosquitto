#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "mosquitto_broker_lib.h"

extern int broker_main(int argc, char *argv[], int port, void (*connect_callback)(int port));
extern int broker_quit;

static pthread_mutex_t broker_lib_mutex;
static pthread_cond_t  broker_lib_cond;
static int broker_lib_status;

int broker_init(void)
{
    broker_quit = 1;
    pthread_mutex_init(&broker_lib_mutex, NULL);
    pthread_cond_init(&broker_lib_cond, NULL);
    return 0;
}

int broker_deinit(void)
{
    broker_quit = 1;
    pthread_mutex_destroy(&broker_lib_mutex);
    pthread_cond_destroy(&broker_lib_cond);
    return 0;
}

void connect_callback(int port)
{
    broker_lib_status = BROKER_LIB_ERROR_NONE;
    pthread_cond_signal(&broker_lib_cond);
    pthread_mutex_unlock(&broker_lib_mutex);
}

typedef struct
{
    int    argc;
    char **argv;
    int   *port;
} args_t;

void *thread_main(void *obj)
{
    int ret = 0;
    int port_search = 1;
    int port_search_count = 0;
    args_t *args = (args_t *)obj;

    if (args == NULL)
    {
        broker_lib_status = BROKER_LIB_ERROR_BAD_ARGUMENT;
        pthread_cond_signal(&broker_lib_cond);
        pthread_mutex_unlock(&broker_lib_mutex);
        return NULL;
    }

    broker_quit = 0;

    while (port_search == 1 && port_search_count++ <= 100)
    {
        ret = broker_main(args->argc, args->argv, *args->port, connect_callback);
        if (ret == BROKER_LIB_ERROR_SOCKET_OPEN ||
            ret == BROKER_LIB_ERROR_SOCKET_LISTEN ||
            ret == BROKER_LIB_ERROR_SOCKET_INVALID)
        {
            (*args->port)++;
        }
        else
        {
            port_search = 0;
        }
    }
    if (port_search_count >= 100)
        ret = BROKER_LIB_ERROR_NO_OPEN_PORT;


    return NULL;
}

int broker_start(int argc, char *argv[], int *default_port)
{
    int r;
    pthread_t broker_thread;
    args_t args = {argc, argv, default_port};

    broker_lib_status = BROKER_LIB_ERROR_THREADING;

    pthread_mutex_lock(&broker_lib_mutex);
    pthread_create(&broker_thread, NULL, thread_main, &args);
    pthread_cond_wait(&broker_lib_cond, &broker_lib_mutex);
    pthread_mutex_unlock(&broker_lib_mutex);

    return broker_lib_status;
}

int broker_stop(void)
{
    broker_quit = 1;
    return 0;
}

int broker_is_running(void)
{
    return (broker_quit == 0);
}

