import qbs 1.0
import qbs.FileInfo
import qbs.ModUtils

Product {
    type: "application"
    Depends {name: "cpp" }
    cpp.positionIndependentCode: false
    cpp.executableSuffix: ".elf"
    files: ["*.cfg", "*.ld", "Src/**", "Inc/**", "Drivers/**", "Middlewares/**", "startup/**"]

    cpp.defines: [
        "USE_HAL_DRIVER", "STM32F103xB"
    ]

    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.debugInformation: true
        cpp.optimization: "none"
        cpp.defines: outer.concat(["DEBUG=1"])
    }

    Properties {
        condition: qbs.buildVariant === "release"
        cpp.debugInformation: false
        cpp.optimization: "none"
    }

    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m3", "-mthumb",
        "-ffunction-sections", "-fdata-sections",
        "-Wno-missing-braces", "-Wno-unused", "-Wno-pointer-sign"
    ]

    cpp.linkerFlags: [
        "-mcpu=cortex-m3", "-mthumb",
        "-ffunction-sections", "-fdata-sections",
        "-Wl,--gc-sections", "--specs=nano.specs",
        "--specs=nosys.specs",
        "-T", path + "/STM32F103C8Tx_FLASH.ld"
    ]

    cpp.includePaths: [
        "Inc",
        "Drivers/CMSIS/Include",
        "Drivers/CMSIS/Device/ST/STM32F1xx/Include",
        "Drivers/STM32F1xx_HAL_Driver/Inc",
        "Middlewares/ST/STM32_USB_Device_Library/Core/Inc",
        "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc",
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS",
        "Middlewares/Third_Party/FreeRTOS/Source/include",
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3",
    ]
    cpp.cxxFlags: ["-std=c++11"]
    cpp.cFlags: ["-std=gnu99"]
    cpp.warningLevel: "all"
}

