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
#include <libgen.h>

#include <glib.h>

#include <pthread.h>
#include <gdbus.h>

#include "json-c/json.h"
#include "rkdb.h"
#include "network.h"
#include "storage.h"
#include "media.h"
#include "common.h"
#include "system.h"
#include "event.h"

int main(int argc , char ** argv)
{
    GMainLoop *main_loop;
    DBusError dbus_err;
    DBusConnection *dbus_conn;
    char *db_file = "/userdata/sysconfig.db";
    char *db_path;

    if (argc == 2)
        db_file = argv[1];

    db_path = g_strdup(db_file);
    db_path = dirname(db_path);

    if (access(db_path, 0)) {
        printf("dbserver: %s folder does not exist\n", db_path);
        return 0;
    }

    if (db_path)
        g_free(db_path);

    rkdb_init(db_file);

    network_init();
    storage_init();
    media_init();
    system_init();
    event_init();

    printf("dbserver init finish\n");

    dbus_error_init(&dbus_err);
    dbus_conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, DB_SERVER, &dbus_err);

    main_loop = g_main_loop_new(NULL, FALSE);

    network_dbus_register(dbus_conn);
    storage_dbus_register(dbus_conn);
    media_dbus_register(dbus_conn);
    system_dbus_register(dbus_conn);
    event_dbus_register(dbus_conn);

    printf("dbserver dbus register finish\n");

    g_main_loop_run(main_loop);
    rkdb_deinit();
    if (main_loop)
        g_main_loop_unref(main_loop);

    return 0;
}
