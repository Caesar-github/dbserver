#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include <glib.h>

#include <pthread.h>
#include <gdbus.h>

#include "json-c/json.h"
#include "rkdb.h"
#include "network.h"
#include "storage.h"
#include "media.h"
#include "common.h"

int main( int argc , char ** argv)
{
    GMainLoop *main_loop;
    DBusError dbus_err;
    DBusConnection *dbus_conn;

    dbus_error_init(&dbus_err);
    dbus_conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, DB_SERVER, &dbus_err);
    printf("dbus_conn %p\n", dbus_conn);

    main_loop = g_main_loop_new(NULL, FALSE);
    rkdb_init("/userdata/sysconfig.db");
    network_init(dbus_conn);
    storage_init(dbus_conn);
    media_init(dbus_conn);

    g_main_loop_run(main_loop);
    rkdb_deinit();
    if (main_loop)
        g_main_loop_unref(main_loop);

    return 0;
}