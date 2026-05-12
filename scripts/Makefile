# EdgeInfer Makefile
# Target: ARM Cortex-M3 (QEMU mps2-an385)

# Toolchain
CROSS   ?= arm-none-eabi-
CC       = $(CROSS)gcc
AS       = $(CROSS)gcc
LD       = $(CROSS)gcc
OBJCOPY  = $(CROSS)objcopy
OBJDUMP  = $(CROSS)objdump

# Directories
SRCDIR   = src
PLATFORM = platform/arm
USERDIR  = user_extensions
BUILDDIR = build

# Sources
C_SRCS = $(wildcard $(SRCDIR)/common/*.c) \
         $(wildcard $(SRCDIR)/pipeline/*.c) \
         $(wildcard $(SRCDIR)/stages/*.c) \
         $(wildcard $(SRCDIR)/memory/*.c) \
         $(wildcard $(SRCDIR)/hooks/*.c) \
         $(SRCDIR)/main.c \
         $(wildcard $(USERDIR)/preprocess/*.c) \
         $(wildcard $(USERDIR)/postprocess/*.c) \
         $(wildcard $(PLATFORM)/*.c)

ASM_SRCS = $(PLATFORM)/startup.s

# Includes
INCLUDES = -I$(SRCDIR)/common \
           -I$(SRCDIR)/pipeline \
           -I$(SRCDIR)/stages \
           -I$(SRCDIR)/model \
           -I$(SRCDIR)/memory \
           -I$(SRCDIR)/hooks \
           -I$(PLATFORM) \
           -I$(USERDIR)/config \
           -Iconfig

# Flags
CPU      = -mcpu=cortex-m3 -mthumb
CFLAGS   = $(CPU) $(INCLUDES) -Wall -Wextra -Os -ffunction-sections -fdata-sections
LDFLAGS  = $(CPU) -T$(PLATFORM)/linker.ld -Wl,--gc-sections -nostartfiles

# Objects
C_OBJS   = $(patsubst %.c,$(BUILDDIR)/%.o,$(notdir $(C_SRCS)))
ASM_OBJS = $(patsubst %.s,$(BUILDDIR)/%.o,$(notdir $(ASM_SRCS)))
OBJS     = $(C_OBJS) $(ASM_OBJS)

TARGET   = $(BUILDDIR)/app.elf

.PHONY: all clean

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "Built: $@"

$(BUILDDIR)/%.o: $(SRCDIR)/common/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/pipeline/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/stages/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/memory/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/hooks/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(USERDIR)/preprocess/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(USERDIR)/postprocess/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(PLATFORM)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(PLATFORM)/%.s | $(BUILDDIR)
	$(AS) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR)
