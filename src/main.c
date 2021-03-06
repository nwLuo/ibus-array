/*
 ibus-array - The Array 30 Engine for IBus

 Copyright (c) 2009-2014 Yu-Chun Wang <mainlander1122@gmail.com>
                         Keng-Yu Lin <kengyu@lexical.tw>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <ibus.h>
#include "engine.h"


static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

static gboolean ibus = FALSE;
static gboolean verbose = FALSE;

static const GOptionEntry entries[] =
{
    { "ibus", 'i', 0, G_OPTION_ARG_NONE, &ibus, "component is executed by ibus", NULL },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose", NULL },
    { NULL },
};

static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    ibus_quit ();
}


static void
init (void)
{
    IBusComponent *component;

    ibus_init ();

    bus = ibus_bus_new ();
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);
	
    ibus_array_init(bus);

    component = ibus_component_new ("org.freedesktop.IBus.Array",
                                    "Array 30 Input Method",
                                    "0.0.1",
                                    "GPL",
                                    "Yu-Chun Wang <mainlander1122@gmail.com>",
                                    "http://code.google.com/p/ibus/",
                                    "",
                                    "ibus-array");
    ibus_component_add_engine (component,
                               ibus_engine_desc_new ("array",
                                                     "Array 30 Input Method",
                                                     "Array 30 Input Method",
                                                     "zh_TW",
                                                     "GPL",
                                                     "Yu-Chun Wang <mainlander1122@gmail.com>",
                                                     PKGDATADIR"/icon/ibus-array.png",
                                                     "us"));

    factory = ibus_factory_new (ibus_bus_get_connection (bus));
    ibus_factory_add_engine (factory, "array", IBUS_TYPE_ARRAY_ENGINE);

    if (ibus) {
        ibus_bus_request_name (bus, "org.freedesktop.IBus.Array", 0);
    }
    else {
        ibus_bus_register_component (bus, component);
    }
    g_object_unref (component);
}

int main(gint argc, gchar **argv)
{
    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new ("- ibus array engine component");

    g_option_context_add_main_entries (context, entries, "ibus-array");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("Option parsing failed: %s\n", error->message);
        exit (-1);
    }

    init ();
    ibus_main ();

    ibus_array_exit();
}
