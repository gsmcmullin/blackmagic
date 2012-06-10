CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS += -Istm32/include -mcpu=cortex-m3 -mthumb -DSTM32F1
LDFLAGS_BOOT = -lopencm3_stm32f1 -Wl,--defsym,_stack=0x20005000 \
	-Wl,-T,$(PLATFORM_DIR)/blackmagic.ld -nostartfiles -lc -lnosys -Wl,-Map=mapfile \
	-mthumb -mcpu=cortex-m3 -Wl,-gc-sections
LDFLAGS = $(LDFLAGS_BOOT) -Wl,-Ttext=0x8002000

SRC += 	cdcacm.c	\
	platform.c	\
	traceswo.c	\
	usbuart.c	\

all:	blackmagic.bin blackmagic_dfu.bin blackmagic_dfu.hex

blackmagic.bin:	blackmagic
	$(OBJCOPY) -O binary $^ $@

blackmagic_dfu: usbdfu.o
	$(CC) $^ -o $@ $(LDFLAGS_BOOT)

blackmagic_dfu.bin:	blackmagic_dfu
	$(OBJCOPY) -O binary $^ $@

blackmagic_dfu.hex:	blackmagic_dfu
	$(OBJCOPY) -O ihex $^ $@

host_clean:
	-rm blackmagic.bin blackmagic_dfu blackmagic_dfu.bin blackmagic_dfu.hex
