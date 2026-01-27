CC = clang
PKG_CONFIG = x11 xcomposite xi xrandr xfixes cairo dbus-1
CFLAGS = -Wall -Wextra -g -MMD -MP $(shell pkg-config --cflags $(PKG_CONFIG))
LIBS = $(shell pkg-config --libs $(PKG_CONFIG))

# Build Configuration

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/limeos-window-manager

SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES = $(shell find $(SRCDIR) -type d -exec printf "-I{} " \;)
CFLAGS += $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

setup:
	@echo "[" > compile_commands.json
	@first=1; for src in $(SOURCES); do \
		[ $$first -eq 0 ] && echo "," >> compile_commands.json; \
		first=0; \
		echo "{\"directory\":\"$(CURDIR)\",\"file\":\"$$src\",\"arguments\":[\"$(CC)\",$(foreach f,$(CFLAGS),\"$(f)\",)\"-c\",\"$$src\"]}" >> compile_commands.json; \
	done
	@echo "]" >> compile_commands.json
	@echo "Generated compile_commands.json"

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Special Directives

.PHONY: all clean setup
