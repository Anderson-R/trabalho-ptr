deps_config := \
	/home/anderson/esp/esp-mdf/esp-idf/components/app_trace/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/aws_iot/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/bt/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/driver/Kconfig \
	/home/anderson/esp/esp-mdf/components/third_party/esp-aliyun/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/esp32/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/esp_adc_cal/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/esp_event/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/esp_http_client/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/esp_http_server/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/ethernet/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/fatfs/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/freemodbus/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/freertos/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/heap/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/libsodium/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/log/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/lwip/Kconfig \
	/home/anderson/esp/esp-mdf/components/maliyun_linkkit/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/mbedtls/Kconfig \
	/home/anderson/esp/esp-mdf/components/mcommon/Kconfig \
	/home/anderson/esp/esp-mdf/components/mconfig/Kconfig \
	/home/anderson/esp/esp-mdf/components/mdebug/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/mdns/Kconfig \
	/home/anderson/esp/esp-mdf/components/mespnow/Kconfig \
	/home/anderson/esp/esp-mdf/components/third_party/miniz/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/mqtt/Kconfig \
	/home/anderson/esp/esp-mdf/components/mupgrade/Kconfig \
	/home/anderson/esp/esp-mdf/components/mwifi/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/nvs_flash/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/openssl/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/pthread/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/spi_flash/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/spiffs/Kconfig \
	/home/anderson/esp/esp-mdf/components/third_party/esp-aliyun/components/ssl/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/tcpip_adapter/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/vfs/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/wear_levelling/Kconfig \
	/home/anderson/esp/esp-mdf/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/anderson/esp/esp-mdf/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/anderson/esp/esp-mdf/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/anderson/esp/esp-mdf/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
