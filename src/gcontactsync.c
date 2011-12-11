/*
 * Google Contact Sync Plugin
 *
 * Copyright (C) 2010, Marc Hörsken <info@marc-hoersken.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 */

#include <gcontactsync.h>

PurplePlugin *gcontactsync_plugin = NULL;
gcal_t gcontactsync_gcal = NULL;

static GList* plugin_actions(PurplePlugin* plugin, gpointer context) {
	GList *list = NULL;
	PurplePluginAction *action = NULL;

	action = purple_plugin_action_new("Sync all contacts", plugin_action_link_conctacts_cb);
	list = g_list_append(list, action);

	action = purple_plugin_action_new("GUI test", plugin_action_gui_test_cb);
	list = g_list_append(list, action);

	return list;
}

static gboolean plugin_load(PurplePlugin *plugin) {
	gcontactsync_plugin = plugin;
	gcontactsync_gcal = gcal_new(GCONTACT);

	if (!gcontactsync_gcal)
		return FALSE;

	gcal_set_curl_debug_callback(gcontactsync_gcal, plugin_curl_debug_callback);

#ifdef WIN32
	gcal_set_ca_info(gcontactsync_gcal, NULL);
	gcal_set_ca_path(gcontactsync_gcal, "ca-certs");
#endif

	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	gcal_delete(gcontactsync_gcal);
	
	gcontactsync_plugin = NULL;
	gcontactsync_gcal = NULL;

	return TRUE;
}

static void plugin_init(PurplePlugin *plugin) {
	purple_prefs_add_none(PLUGIN_PREF_ROOT1);
	purple_prefs_add_none(PLUGIN_PREF_ROOT2);
	purple_prefs_add_string(PLUGIN_PREF_USERNAME, "Username");
	purple_prefs_add_string(PLUGIN_PREF_PASSWORD, "Password");
}

static PurplePluginPrefFrame* plugin_pref_frame(PurplePlugin *plugin) {
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;

	frame = purple_plugin_pref_frame_new();

	ppref = purple_plugin_pref_new_with_label("Google Account");
	purple_plugin_pref_frame_add(frame, ppref);

	ppref = purple_plugin_pref_new_with_name_and_label(PLUGIN_PREF_USERNAME, "Username");
	purple_plugin_pref_frame_add(frame, ppref);

	ppref = purple_plugin_pref_new_with_name_and_label(PLUGIN_PREF_PASSWORD, "Password");
	purple_plugin_pref_set_masked(ppref, TRUE);
	purple_plugin_pref_frame_add(frame, ppref);

	return frame;
}

static PurplePluginUiInfo plugin_prefs = {
	plugin_pref_frame,
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static PurplePluginInfo plugin_info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	PLUGIN_ID,
	PLUGIN_NAME,
	PLUGIN_VERSION,

	PLUGIN_DESCRIPTION,
	PLUGIN_DESCRIPTION,
	PLUGIN_AUTHOR,
	PLUGIN_WEBSITE,

	plugin_load,
	plugin_unload,
	NULL,

	NULL,
	NULL,
	&plugin_prefs,
	plugin_actions,
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(gcontactsync, plugin_init, plugin_info)
