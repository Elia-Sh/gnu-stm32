# STM32 minimal Development Environment


# General descriptions
* Board references: `https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html`
* Other references:
    * `https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/`
    * `https://www.linuxembedded.fr/2021/02/bare-metal-from-zero-to-blink`
* Working example with FreeRTOS:
    * `https://github.com/freelamb/stm32f10x_makefile_freertos`


# Board specs -
```
Blue Pill -> STM32F103C8T6
CPU model -> Arm Cortex-M3
SRAN	20KiB
FLASH   64KiB
```


## Install st-link
arch linux: `https://archlinux.org/packages/extra/x86_64/stlink/`
Other dist, consult with: `https://github.com/stlink-org/stlink`


## Clone stm cube1 "examples" git repo
```bash #
$ git clone https://github.com/STMicroelectronics/STM32CubeF1
```


```bash #
# build from example dir -
$ pwd
~/stm_micro/STM32CubeF1/Projects/STM32F103RB-Nucleo/Examples_LL/USART/USART_SyncCommunication_FullDuplex_IT

$ arm-none-eabi-gcc Src/main.c -o main.elf -I Inc/ \
    -I __FULL_PATH__/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc/ \
    -I __FULL_PATH__/STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include/ \
```



# Troubleshooting

```bash#
........STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f1xx.h:140:3: error: 
#error "Please select first the target STM32F1xx device used in your application (in stm32f1xx.h file)"
  140 |  #error "Please select first the target STM32F1xx device used in your application (in stm32f1xx.h
 file)"
      |   ^~~~~
```

