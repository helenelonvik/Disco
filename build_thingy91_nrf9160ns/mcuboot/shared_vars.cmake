set(mcuboot_KERNEL_HEX_NAME zephyr.hex)
set(mcuboot_ZEPHYR_BINARY_DIR C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/mcuboot/zephyr)
set(mcuboot_KERNEL_ELF_NAME zephyr.elf)
list(APPEND mcuboot_BUILD_BYPRODUCTS C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/mcuboot/zephyr/zephyr.hex)
list(APPEND mcuboot_BUILD_BYPRODUCTS C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/mcuboot/zephyr/zephyr.elf)
set(mcuboot_SIGNATURE_KEY_FILE root-ec-p256.pem)
list(APPEND mcuboot_PM_YML_DEP_FILES C:/Users/helo/v1.6.0/bootloader/mcuboot/boot/zephyr/pm.yml)
list(APPEND mcuboot_PM_YML_FILES C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/mcuboot/zephyr/include/generated/pm.yml)
