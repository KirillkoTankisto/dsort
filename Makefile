CC = clang
CFLAGS = -O3 -pedantic -Wall -Wextra -ffunction-sections -fdata-sections -fomit-frame-pointer -fvisibility=hidden
LDFLAGS = -static -Wl,--gc-sections,--as-needed,--icf=all -flto=full -fuse-ld=lld
LDLIBS = $(shell pkg-config --static --libs libmagic)
INCLUDE = -Iinclude $(shell pkg-config --static --cflags libmagic)

OUT_DIR = out
SRC_DIR = src
BIN_DIR = bin
INS_DIR = /usr/local/bin/

COMPILE = $(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LDLIBS)
STRIP = llvm-strip --strip-all -R .comment -R .note.gnu.build-id
GETSIZE = @du --bytes

C_prefix = ":CC: "
C = @echo -n $(C_prefix)

all: $(OUT_DIR)/dsort

clean:
	rm -rf $(OUT_DIR)

$(OUT_DIR)/dsort: $(SRC_DIR)/*.c
	$C
	mkdir -p $(OUT_DIR)
	$C
	$(COMPILE) $^ -o $@
	$C
	$(STRIP) $@
	$C
	$(GETSIZE) $@

install: $(OUT_DIR)/dsort
	mkdir -p $(INS_DIR)
	install $^ $(INS_DIR)
	