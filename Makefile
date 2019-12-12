hide := @
ECHO := echo

SYS_PATH := ../../buildroot/output/rockchip_puma/host/arm-buildroot-linux-gnueabihf/sysroot
OUT_PATH := ../../buildroot/output/rockchip_puma/target
STAGOUT_PATH := ../../buildroot/output/rockchip_puma/staging
INC_FILES := -I $(SYS_PATH)/usr/include \
             -I $(SYS_PATH)/usr/include/dbus-1.0 \
             -I $(SYS_PATH)/usr/lib/dbus-1.0/include \
             -I $(SYS_PATH)/usr/include/glib-2.0 \
             -I $(SYS_PATH)/usr/lib/glib-2.0/include \
             -I $(SYS_PATH)/usr/include/libdrm \
             -I $(OUT_PATH)/usr/include \
             -I $(STAGOUT_PATH)/usr/include \
             -I gdbus
LIB_FILES := -L $(OUT_PATH)/usr/lib

#LD_FLAGS := -lpthread -ljpeg -lpng12 -lm -ldbus-1 -ldbus-glib-1 -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lreadline -ldrm -lminigui_ths -lfreetype -lts -lrga
#SRC_FILES := agent.c dbus_helpers.c netctl.c gdbus/*.c ui/*.c
LD_FLAGS := -lpthread -lm -ldbus-1 -ldbus-glib-1 -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lreadline -lsqlite3 -ljson-c -lrkdb -lgdbus
SRC_FILES := main.c dbus_helpers.c  network.c storage.c common.c

BIN_FILE := dbserver

out:
	$(hide)$(ECHO) "Build dbserver ..."
	./../../buildroot/output/rockchip_puma/host/usr/bin/arm-buildroot-linux-gnueabihf-gcc $(SRC_FILES) $(INC_FILES) $(LIB_FILES) $(LD_FLAGS) -o $(BIN_FILE)
	cp dbusconfig/dbserver.conf $(STAGOUT_PATH)/etc/dbus-1/system.d/
	cp dbserver $(STAGOUT_PATH)/usr/bin/
	$(hide)$(ECHO) "Build dbserver Done ..."
