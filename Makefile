# 生成的文件名<项目名>
PROJECT = Stm32MessingAbout

# 定义文件格式和文件名
TARGET := $(PROJECT)
TARGET_ELF := $(TARGET).elf
TARGET_BIN := $(TARGET).bin
TARGET_HEX := $(TARGET).hex
OBJCPFLAGS_ELF_TO_BIN = -Obinary
OBJCPFLAGS_ELF_TO_HEX = -Oihex
OBJCPFLAGS_BIN_TO_HEX = -Ibinary -Oihex

# 定义路径
TOP_DIR = .
TARGET_ABS_PATH = $(realpath $(TOP_DIR)/$(TARGET).hex) #获取绝对路径，用于openocd烧写
SCRIPT_DIR := $(TOP_DIR)/scripts
STARTUP_DIR := $(TOP_DIR)/Startup
INC_DIR := -I$(TOP_DIR)/App -I$(TOP_DIR)/Startup -I$(TOP_DIR)/Libraries/inc -I$(TOP_DIR)/Driver

# openocd烧写相关路径
OPENOCD_INTER = /usr/share/openocd/scripts/interface/stlink-v2.cfg
OPENOCD_TARGET = /usr/share/openocd/scripts/target/stm32f1x.cfg
FLASH_OFFSET = 0x08000000 # flash偏移量，stm32_flash.ld中指定，在openocd写入hex到flash时偏移

# 设置ld链接脚本文件
LDSCRIPT := $(TOP_DIR)/stm32_flash.ld

# 定义编译工具
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
OBJCP = arm-none-eabi-objcopy

# 链接文件
LIBS = -lc -lm -lnosys 
LIBDIR = 

# CPU架构，用于编译选项
CPU = -mcpu=cortex-m3
MCU = -mthumb $(CPU)

# .s汇编文件编译选项
ASFLAGS += $(MCU)

# .c文件编译选项
CCFLAGS += $(MCU) -Wall -g -mfloat-abi=soft -march=armv7-m -specs=nosys.specs
CCFLAGS += $(INC_DIR)
CCFLAGS += -D STM32F10X_HD -D USE_STDPERIPH_DRIVER

# 链接选项
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(TOP_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# .s汇编启动文件
ASM_SRC += $(STARTUP_DIR)/startup_stm32f10x_hd.s

# .c源文件，即当前目录所有.c文件
C_SRC = $(shell find ./ -name '*.c')

# 替换文件后缀
C_OBJS := $(C_SRC:%.c=%.o)
ASM_OBJS := $(ASM_SRC:%.s=%.o)

# 编译命令的定义
COMPILE = $(CC) $(CCFLAGS) -c $< -o $@ 
ASSEMBLE = $(AS) $(ASFLAGS) -c $< -o $@ 
LINK = $(CC) $+ $(LDFLAGS) $(LDLIBS) -o $@ 
ELF_TO_BIN = $(OBJCP) $(OBJCPFLAGS_ELF_TO_BIN) $< $@
BIN_TO_HEX = $(OBJCP) $(OBJCPFLAGS_BIN_TO_HEX) $< $@ 

# 定义伪目标
.PHONY: all clean printf download

# 定义规则
all: $(TARGET_HEX)
	@echo "build done"

$(TARGET_HEX): $(TARGET_BIN)
	$(BIN_TO_HEX)

$(TARGET_BIN): $(TARGET_ELF)
	$(ELF_TO_BIN)

$(TARGET_ELF): $(C_OBJS) $(ASM_OBJS)
	$(LINK)

$(C_OBJS): %.o:%.c
	$(COMPILE)

$(ASM_OBJS): %.o:%.s
	$(ASSEMBLE) 

printf:
	@echo $(ASM_OBJS)
	@echo $(ASSEMBLE)
	@echo $(TARGET_ABS_PATH)

# 清理项
clean:
	rm -f $(TARGET_HEX)
	rm -f $(TARGET_BIN)
	rm -f $(TARGET_ELF)
	rm -f $(C_OBJS) $(ASM_OBJS)
	rm -f $(TARGET).map
	@echo "clean done"

# openocd下载
download:
	openocd -f $(OPENOCD_INTER) -f $(OPENOCD_TARGET) -c init -c halt -c "flash write_image erase $(TARGET_ABS_PATH) $(FLASH_OFFSET)" -c reset -c shutdown