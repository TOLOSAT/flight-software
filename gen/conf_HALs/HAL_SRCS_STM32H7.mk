# This file declares which sources files we need in STM32H7xx HAL

HAL_SRCS  = $(HAL_SRCDIR)/stm32h7xx_hal.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_gpio.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_uart.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_uart_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_i2c.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_i2c_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_qspi.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_spi.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_spi_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sd.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sd_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_ll_sdmmc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_dma.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rcc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rcc_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_tim.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_tim_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rtc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rtc_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_iwdg.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_ramecc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sdram.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_nand.c \
			$(HAL_SRCDIR)/stm32h7xx_ll_fmc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_flash.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_flash_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pwr.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pwr_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_cortex.c
