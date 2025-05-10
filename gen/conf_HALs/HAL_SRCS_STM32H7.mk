# This file declares which sources files we need in STM32H7xx HAL

HAL_SRCS  = $(HAL_SRCDIR)/stm32h7xx_hal.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_gpio.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sd.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sd_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_ll_sdmmc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_qspi.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_dma.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rcc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_rcc_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_tim.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_tim_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_sdram.c \
			$(HAL_SRCDIR)/stm32h7xx_ll_fmc.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pcd.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pcd_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_ll_usb.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pwr.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_pwr_ex.c \
			$(HAL_SRCDIR)/stm32h7xx_hal_cortex.c
