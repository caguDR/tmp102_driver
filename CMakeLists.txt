cmake_minimum_required(VERSION 3.10)

project(tmp102_driver)

add_executable(getTemp
 "${PROJECT_SOURCE_DIR}/tmp102_driver.c"
)

install(TARGETS getTemp DESTINATION bin)
