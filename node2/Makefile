VPATH = ../utils
# List all source files to be compiled; separate with space
SOURCE_FILES := main.c
SOURCE_FILES += uart/uart.c
SOURCE_FILES += can/SPI_driver.c can/MCP_driver.c can/CAN_driver.c
SOURCE_FILES += pwm_driver.c game_board_driver.c adc_driver.c ir_driver.c dac_driver.c TWI_Master.c motor_box_driver.c

INCLUDE_FOLDERS := constants ../utils
INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_FOLDERS))

# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := no 

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega2560
TARGET_DEVICE := m2560 

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) $(INCLUDE_FLAGS)

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e


