#!/bin/bash
PORT=/dev/cu.wchusbserial1420
#BIN=./bin/upgrade/HUAWEI_HAIGE_V0.0.1.0.A.bin
BIN=./bin/upgrade/user1.4096.new.6.bin
#erase flash
#esptool.py --chip auto --port $PORT --baud 921600 erase_flash

#burn all
#esptool.py --chip esp8266 --port $PORT --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode qio --flash_freq 40m --flash_size 4MB-c1  0x0000 ./bin/boot_v1.7.bin 0x3fb000 ./bin/blank.bin 0x3fc000 ./bin/esp_init_data_default.bin 0x3fe000 ./bin/blank.bin  0x1000 $BIN

#burn_A
esptool.py --chip esp8266 --port $PORT --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode qio --flash_freq 40m --flash_size 4MB-c1  0x1000 $BIN

#dump_flash 2093056-2040kb   2097152-2048kb
#esptool.py --chip esp8266 --port $PORT --baud 921600  read_flash  0x0 2093056 dump_flash.bin

