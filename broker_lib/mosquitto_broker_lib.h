#ifndef MOSQUITTO_BROKER_LIB_H
#define MOSQUITTO_BROKER_LIB_H

#define BROKER_LIB_ERROR_NONE                  0
#define BROKER_LIB_ERROR_BAD_ARGUMENT          1
#define BROKER_LIB_ERROR_CANT_FORK             2
#define BROKER_LIB_ERROR_DATABASE_OPEN         3
#define BROKER_LIB_ERROR_SECURITY_MODULE_INIT  4
#define BROKER_LIB_ERROR_SECURITY_INIT         5
#define BROKER_LIB_ERROR_SOCKET_OPEN           6
#define BROKER_LIB_ERROR_SOCKET_LISTEN         7
#define BROKER_LIB_ERROR_SOCKET_INVALID        8
#define BROKER_LIB_ERROR_WEBSOCKET_CREATE      9
#define BROKER_LIB_ERROR_PRIVILEGE_DROP       10
#define BROKER_LIB_ERROR_MALLOC               11
#define BROKER_LIB_ERROR_NO_OPEN_PORT         12

int broker_init(void);
int broker_main(int argc, char *argv[], int port, void (*connect_callback)(int port));

int broker_start(int argc, char *argv[], int port, void (*connect_callback)(int port), void (*return_callback)(int return_code));
int broker_stop(void);
int broker_is_running(void);

#endif /* MOSQUITTO_BROKER_LIB_H */

