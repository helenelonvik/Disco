set(spm_KERNEL_HEX_NAME zephyr.hex)
set(spm_ZEPHYR_BINARY_DIR C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/zephyr)
set(spm_KERNEL_ELF_NAME zephyr.elf)
list(APPEND spm_BUILD_BYPRODUCTS C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/zephyr/zephyr.hex)
list(APPEND spm_BUILD_BYPRODUCTS C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/zephyr/zephyr.elf)
list(APPEND spm_BUILD_BYPRODUCTS
    C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/libspmsecureentries.a)
list(APPEND spm_PM_YML_DEP_FILES C:/Users/helo/v1.6.0/nrf/samples/spm/pm.yml)
list(APPEND spm_PM_YML_FILES C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/zephyr/include/generated/pm.yml)
list(APPEND spm_PM_YML_DEP_FILES C:/Users/helo/v1.6.0/nrf/subsys/partition_manager/pm.yml.trustzone)
list(APPEND spm_PM_YML_FILES C:/Users/helo/appsToBoard/buzz/build_thingy91_nrf9160ns/spm/modules/nrf/subsys/partition_manager/pm.yml.trustzone)