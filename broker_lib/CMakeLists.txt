option(WITH_THREADING "Include client library threading support?" ON)
if (${WITH_THREADING} STREQUAL ON)
	add_definitions("-DWITH_THREADING")
	if (WIN32)
		set (PTHREAD_LIBRARIES C:\\pthreads\\Pre-built.2\\lib\\x64\\pthreadVC2.lib)
		set (PTHREAD_INCLUDE_DIR C:\\pthreads\\Pre-built.2\\include)
	else (WIN32)
		set (PTHREAD_LIBRARIES pthread)
		set (PTHREAD_INCLUDE_DIR "")
	endif (WIN32)
else (${WITH_THREADING} STREQUAL ON)
	set (PTHREAD_LIBRARIES "")
	set (PTHREAD_INCLUDE_DIR "")
endif (${WITH_THREADING} STREQUAL ON)

include_directories(${mosquitto_SOURCE_DIR}
    ${mosquitto_SOURCE_DIR}/src
    ${mosquitto_SOURCE_DIR}/lib
    ${mosquitto_SOURCE_DIR}/broker_lib
    ${STDBOOL_H_PATH} ${STDINT_H_PATH}
    ${OPENSSL_INCLUDE_DIR} ${PTHREAD_INCLUDE_DIR})

link_directories(${mosquitto_SOURCE_DIR}/src)

add_library(libmosquitto_broker STATIC
	../src/conf.c
	../src/context.c
	../src/database.c
	../src/lib_load.h
	../src/logging.c
	../src/loop.c
	../lib/memory_mosq.c
    ../lib/memory_mosq.h
	mosquitto_broker_lib.c
	../src/mosquitto_broker.h
	../src/net.c
	../lib/net_mosq.c
    ../lib/net_mosq.h
	../src/persist.c
    ../src/persist.h
	../src/read_handle.c
    ../src/read_handle_client.c
    ../src/read_handle_server.c
	../lib/read_handle_shared.c
    ../lib/read_handle.h
	../src/subs.c
	../src/security.c
    ../src/security_default.c
	../lib/send_client_mosq.c
    ../lib/send_mosq.h
	../lib/send_mosq.c
    ../lib/send_mosq.h
	../src/send_server.c
	../src/sys_tree.c
	../lib/time_mosq.c
	../lib/tls_mosq.c
	../lib/util_mosq.c
    ../lib/util_mosq.h
	../src/websockets.c
	../lib/will_mosq.c
    ../lib/will_mosq.h)

set (LIBRARIES ${OPENSSL_LIBRARIES} ${PTHREAD_LIBRARIES})

if (UNIX AND NOT APPLE)
	set (LIBRARIES ${LIBRARIES} rt)
endif (UNIX AND NOT APPLE)

if (WIN32)
	set (LIBRARIES ${LIBRARIES} ws2_32)
endif (WIN32)

if (${WITH_SRV} STREQUAL ON)
	# Simple detect c-ares
	find_path(ARES_HEADER ares.h)
	if (ARES_HEADER)
		add_definitions("-DWITH_SRV")
		set (LIBRARIES ${LIBRARIES} cares)
	else (ARES_HEADER)
		message(WARNING "c-ares library not found.")
	endif (ARES_HEADER)
endif (${WITH_SRV} STREQUAL ON)

add_definitions(-DWITH_BROKER -DWITH_BROKER_LIB)

target_link_libraries(libmosquitto_broker ${LIBRARIES})

set_target_properties(libmosquitto_broker PROPERTIES
	OUTPUT_NAME mosquitto_broker
	VERSION ${VERSION}
	SOVERSION 1
)

install(TARGETS libmosquitto_broker ARCHIVE DESTINATION ${BINDIR} RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
install(FILES mosquitto.h DESTINATION ${INCLUDEDIR})

if (UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif (UNIX)

add_subdirectory(test)

