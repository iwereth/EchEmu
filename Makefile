BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
VPATH := src
SRCS := cpu.c main.c memory.c input.c cartridge.c ppu.c
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

CFLAGS := -w -g -I includes
LDFLAGS := -lSDL2

nesemu: $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
