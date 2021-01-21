deps_config := \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/aws_iot/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/bt/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/esp32/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/ethernet/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/fatfs/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/freertos/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/log/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/lwip/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/mbedtls/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/openssl/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/spi_flash/Kconfig \
	/home/aithinker/project/esp-idf/examples/bluetooth/gatt_server/main/Kconfig \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/bootloader/Kconfig.projbuild \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/esptool_py/Kconfig.projbuild \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/components/partition_table/Kconfig.projbuild \
	/cygdrive/e/AiThinkerIDE_V0.5/cygwin/home/aithinker/project/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
