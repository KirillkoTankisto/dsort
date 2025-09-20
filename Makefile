CC = clang
CFLAGS = -O3 -pedantic -Wall -Wextra -ffunction-sections -fdata-sections -fomit-frame-pointer -fvisibility=hidden
LDFLAGS = -static -Wl,--gc-sections,--as-needed,--icf=all -flto=full -fuse-ld=lld
LDLIBS = $(shell pkg-config --static --libs libmagic)
INCLUDE = -Iinclude $(shell pkg-config --static --cflags libmagic)

OUT_DIR = out
SRC_DIR = src
BIN_DIR = bin

STRIP = strip --strip-all -R .comment -R .note.gnu.build-id
GETSIZE = @du --bytes
ADD_SECTION = objcopy --add-section .build=$(BIN_DIR)/.build

C_prefix = ":CC: "
C = @echo -n $(C_prefix)

all: $(OUT_DIR)/dsort

clean:
	rm -rf $(OUT_DIR)

$(OUT_DIR)/dsort: $(SRC_DIR)/main.c $(SRC_DIR)/lib.c
	$C
	mkdir -p $(OUT_DIR)
	$C
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/lib.c $(LDLIBS) -o $(OUT_DIR)/main
	$C
	$(STRIP) $(OUT_DIR)/main
	$C
	$(ADD_SECTION) $(OUT_DIR)/main $(OUT_DIR)/dsort
	$C
	$(GETSIZE) $@