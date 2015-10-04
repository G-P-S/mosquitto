include_directories(${mosquitto_SOURCE_DIR}/lib
			${STDBOOL_H_PATH} ${STDINT_H_PATH} ..)
link_directories(${mosquitto_BINARY_DIR}/lib)

if (${WITH_SRV} STREQUAL ON)
	add_definitions("-DWITH_SRV")
endif (${WITH_SRV} STREQUAL ON)

add_executable(broker_lib_test broker_lib_test.c)

target_link_libraries(broker_lib_test libmosquitto_broker)

install(TARGETS broker_lib_test RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})

