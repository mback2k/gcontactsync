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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>
#include <string.h>

#include <curl/curl.h>

#include <internal_gcal.h>
#include <gcal_parser.h>
#include <gcal_status.h>
#include <gcalendar.h>
#include <gcontact.h>

#include <purple.h>

#define PLUGIN_ID "core-mback2k-gcontactsync"
#define PLUGIN_AUTHOR "Marc Hörsken <info@marc-hoersken.de>"
//#define PLUGIN_DEBUG

PurplePlugin *gcontactsync_plugin = NULL;
gcal_t gcontactsync_gcal = NULL;

static gboolean plugin_check_gcal(int result, const char* function) {
	purple_debug_misc(PLUGIN_ID, "%s result = %d\n", function, result);	
	if (result != 0) {
		if (gcontactsync_gcal) {
			purple_debug_error(PLUGIN_ID, "%s: %d\n%s\n", function, gcal_status_httpcode(gcontactsync_gcal), gcal_status_msg(gcontactsync_gcal));
		}
		return FALSE;
	}
	return TRUE;
}

static gboolean plugin_check_protocol(const char* protocol_id) {
	return (strcmp(protocol_id, "prpl-aim") == 0)
	    || (strcmp(protocol_id, "prpl-msn") == 0)
	    || (strcmp(protocol_id, "prpl-yahoo") == 0)
	    || (strcmp(protocol_id, "prpl-qq") == 0)
	    || (strcmp(protocol_id, "prpl-icq") == 0)
	    || (strcmp(protocol_id, "prpl-jabber") == 0);
}

static gboolean plugin_compare_protocols(const char* protocol_id, const char* protocol) {
	return ((strcmp(protocol_id, "prpl-aim") == 0) && (strcmp(protocol, "AIM") == 0))
	    || ((strcmp(protocol_id, "prpl-msn") == 0) && (strcmp(protocol, "MSN") == 0))
	    || ((strcmp(protocol_id, "prpl-yahoo") == 0) && (strcmp(protocol, "YAHOO") == 0))
	    || ((strcmp(protocol_id, "prpl-qq") == 0) && (strcmp(protocol, "QQ") == 0))
	    || ((strcmp(protocol_id, "prpl-icq") == 0) && (strcmp(protocol, "ICQ") == 0))
	    || ((strcmp(protocol_id, "prpl-jabber") == 0) && (strcmp(protocol, "JABBER") == 0))
	    || ((strcmp(protocol_id, "prpl-jabber") == 0) && (strcmp(protocol, "GOOGLE_TALK") == 0));
}

static const char* plugin_get_protocol(const char* protocol_id) {
	if (strcmp(protocol_id, "prpl-aim") == 0)
		return "AIM";

	if (strcmp(protocol_id, "prpl-msn") == 0)
		return "MSN";

	if (strcmp(protocol_id, "prpl-yahoo") == 0)
		return "YAHOO";

	if (strcmp(protocol_id, "prpl-qq") == 0)
		return "QQ";

	if (strcmp(protocol_id, "prpl-icq") == 0)
		return "ICQ";

	if (strcmp(protocol_id, "prpl-jabber") == 0)
		return "JABBER";

	return NULL;
}

// WORKAROUND: Need to set the CA PATH on Windows
static void plugin_set_curl_capath(struct gcal_resource *gcal_data) {
	curl_easy_setopt(gcal_data->curl, CURLOPT_SSL_VERIFYPEER, FALSE);
}

// WORKAROUND: Empty fields are not allow in the update, need to set all empty strings to NULL
static void plugin_cleanup_contact_data(struct gcal_contact *gcontact_data) {
	if (!gcontact_data)
		return;
	
	if (gcontact_data->nickname && !strlen(gcontact_data->nickname)) {
		free(gcontact_data->nickname);
		gcontact_data->nickname = NULL;
	}

	if (gcontact_data->homepage && !strlen(gcontact_data->homepage)) {
		free(gcontact_data->homepage);
		gcontact_data->homepage = NULL;
	}

	if (gcontact_data->blog && !strlen(gcontact_data->blog)) {
		free(gcontact_data->blog);
		gcontact_data->blog = NULL;
	}

	if (gcontact_data->occupation && !strlen(gcontact_data->occupation)) {
		free(gcontact_data->occupation);
		gcontact_data->occupation = NULL;
	}

	if (gcontact_data->post_address && !strlen(gcontact_data->post_address)) {
		free(gcontact_data->post_address);
		gcontact_data->post_address = NULL;
	}

	if (gcontact_data->birthday && !strlen(gcontact_data->birthday)) {
		free(gcontact_data->birthday);
		gcontact_data->birthday = NULL;
	}
}

static void plugin_sync_from_google_to_pidgin(struct gcal_contact_array gcontacts, GHashTable *htcontacts) {
	char *title, *protocol, *address;
	const char *protocol_id, *alias;
	GList *accounts = NULL;
	PurpleAccount *account = NULL;
	PurpleBuddy *buddy = NULL;
	gcal_contact_t gcontact;
	int i, j;

	for (accounts = purple_accounts_get_all(); accounts != NULL; accounts = accounts->next) {
		account = (PurpleAccount*)accounts->data;

		protocol_id = purple_account_get_protocol_id(account);

		if (plugin_check_protocol(protocol_id)) {
			purple_debug_misc(PLUGIN_ID, "G2P: account:\nalias:\t%s\nprotocol:\t%s\n",
				              purple_account_get_alias(account),
				              protocol_id);

			for (i = 0; i < gcontacts.length; ++i) {
				gcontact = gcal_contact_element(&gcontacts, i);
				if (!gcontact)
					break;

				title = gcal_contact_get_title(gcontact);

#ifdef PLUGIN_DEBUG
				purple_debug_misc(PLUGIN_ID, "G2P: contact: %d\ntitle:\t\t%s\nemail:\t%s\nupdated:\t%s\n",
					              i, title,
					              gcal_contact_get_email(gcontact),
					              gcal_contact_get_updated(gcontact));
#endif
				
				for (j = 0; j < gcal_contact_get_im_count(gcontact); j++) {
					protocol = gcal_contact_get_im_protocol(gcontact, j);
					address = gcal_contact_get_im_address(gcontact, j);

#ifdef PLUGIN_DEBUG
					purple_debug_misc(PLUGIN_ID, "G2P: im: %d\nprotocol:\t%s\naddress:\t%s\n",
						              j, protocol, address);
#endif

					if (plugin_compare_protocols(protocol_id, protocol)) {
						buddy = purple_find_buddy(account, address);
						if (buddy) {
							alias = purple_buddy_get_alias(buddy);
							if (strcmp(title, alias) != 0) {
								purple_blist_alias_buddy(buddy, title);

								purple_debug_info(PLUGIN_ID, "G2P: Alias buddy %s as contact %s\n",
								                  address, title);
							}
						}
					}
				}
			}
		}
	}
}

static void plugin_sync_from_pidgin_to_google(struct gcal_contact_array gcontacts, GHashTable *htcontacts) {
	char *title, *protocol, *address;
	const char *protocol_id, *buddy_name, *buddy_alias;
	GList *accounts = NULL;
	PurpleAccount *account = NULL;
	GSList *buddies = NULL;
	PurpleBuddy *buddy = NULL;
	gboolean found_im = FALSE;
	gcal_contact_t gcontact;
	int i, j;

	for (accounts = purple_accounts_get_all(); accounts != NULL; accounts = accounts->next) {
		account = (PurpleAccount*)accounts->data;

		protocol_id = purple_account_get_protocol_id(account);

		if (plugin_check_protocol(protocol_id)) {
			purple_debug_misc(PLUGIN_ID, "P2G: account:\nalias:\t%s\nprotocol:\t%s\n",
				              purple_account_get_alias(account),
				              protocol_id);

			for (buddies = purple_find_buddies(account, NULL); buddies != NULL; buddies = buddies->next) {
				buddy = (PurpleBuddy*)buddies->data;

				buddy_name = purple_buddy_get_name(buddy);
				buddy_alias = purple_buddy_get_alias(buddy);

#ifdef PLUGIN_DEBUG
				purple_debug_misc(PLUGIN_ID, "P2G: buddy:\nname:\t%s\nalias:\t%s\n",
					              buddy_name, buddy_alias);
#endif

				for (i = 0; i < gcontacts.length; ++i) {
					gcontact = gcal_contact_element(&gcontacts, i);
					if (!gcontact)
						break;

					title = gcal_contact_get_title(gcontact);

#ifdef PLUGIN_DEBUG
					purple_debug_misc(PLUGIN_ID, "P2G: contact: %d\ntitle:\t\t%s\nemail:\t%s\nupdated:\t%s\n",
						              i, title,
						              gcal_contact_get_email(gcontact),
						              gcal_contact_get_updated(gcontact));
#endif

					if (strcmp(buddy_alias, title) == 0) {
						found_im = FALSE;

#ifdef PLUGIN_DEBUG
						purple_debug_misc(PLUGIN_ID, "P2G: match:\nbuddy:\t%s\ncontact:\t%s\n",
							              buddy_name, title);
#endif

						for (j = 0; j < gcal_contact_get_im_count(gcontact); j++) {
							protocol = gcal_contact_get_im_protocol(gcontact, j);

							if (plugin_compare_protocols(protocol_id, protocol)) {
								address = gcal_contact_get_im_address(gcontact, j);

								if (strcmp(buddy_name, address) == 0) {
									found_im = TRUE;
									break;
								}
							}
						}

						if (!found_im) {
#ifdef PLUGIN_DEBUG
							int result, length;
							char *xml_contact = NULL;
#endif
							
							purple_debug_info(PLUGIN_ID, "P2G: Add buddy %s to contact %s\n",
								                         buddy_name, title);

#ifdef PLUGIN_DEBUG
							result = xmlcontact_create(gcontact, &xml_contact, &length);
							if (result == 0 && xml_contact) {
								purple_debug_misc(PLUGIN_ID, "P2G: Original contact XML:\n%s\n", xml_contact);
								free(xml_contact);
							}
#endif
							
							if (plugin_check_gcal(gcal_contact_add_im(gcontact, plugin_get_protocol(protocol_id), buddy_name, I_OTHER, FALSE), "gcal_contact_add_im")) {
								plugin_cleanup_contact_data(gcontact);

#ifdef PLUGIN_DEBUG
								result = xmlcontact_create(gcontact, &xml_contact, &length);
								if (result == 0 && xml_contact) {
									purple_debug_misc(PLUGIN_ID, "P2G: Updated contact XML:\n%s\n", xml_contact);
									free(xml_contact);
								}
#endif

								plugin_check_gcal(gcal_update_contact(gcontactsync_gcal, gcontact), "gcal_update_contact");
							}
						}

						break;
					}
				}
			}
		}
	}
}

static void plugin_action_link_conctacts_cb(PurplePluginAction *action) {
	char *username = NULL, *password = NULL;
    GHashTable *htcontacts = NULL;
	gcal_contact_t gcontact;
	struct gcal_contact_array gcontacts;
	int i;

	username = strdup(purple_prefs_get_string("/plugins/core/mback2k/gcontactsync/username"));
	password = strdup(purple_prefs_get_string("/plugins/core/mback2k/gcontactsync/password"));

	if (username && password) {
		purple_debug_misc(PLUGIN_ID, "username: %s\tpassword length: %d\n",
		                  username, (int)strlen(password));

		htcontacts = g_hash_table_new(&g_str_hash, &g_str_equal);
		if (htcontacts) {
			purple_debug_misc(PLUGIN_ID, "GHashTable created\n");

			if (plugin_check_gcal(gcal_get_authentication(gcontactsync_gcal, username, password), "gcal_get_authentication")) {
				if (plugin_check_gcal(gcal_get_contacts(gcontactsync_gcal, &gcontacts), "gcal_get_contacts")) {
					for (i = 0; i < gcontacts.length; ++i) {
						gcontact = gcal_contact_element(&gcontacts, i);
						if (!gcontact)
							break;

						g_hash_table_insert(htcontacts, gcal_contact_get_title(gcontact), gcontact);
					}

					plugin_sync_from_google_to_pidgin(gcontacts, htcontacts);
					plugin_sync_from_pidgin_to_google(gcontacts, htcontacts);

					gcal_cleanup_contacts(&gcontacts);
				}
			}

			g_hash_table_destroy(htcontacts);
		}
	}

	if (username)
		free(username);

	if (password)
		free(password);
}

static GList* plugin_actions(PurplePlugin* plugin, gpointer context) {
	GList *list = NULL;
	PurplePluginAction *action = NULL;

	action = purple_plugin_action_new("Google Contacts Sync", plugin_action_link_conctacts_cb);
	list = g_list_append(list, action);

	return list;
}

static gboolean plugin_load(PurplePlugin *plugin) {
	gcontactsync_plugin = plugin;
	gcontactsync_gcal = gcal_new(GCONTACT);

	if (!gcontactsync_gcal)
		return FALSE;

	plugin_set_curl_capath(gcontactsync_gcal);

	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin) {
	gcal_delete(gcontactsync_gcal);
	
	gcontactsync_plugin = NULL;
	gcontactsync_gcal = NULL;

	return TRUE;
}

static void plugin_init(PurplePlugin *plugin) {
	purple_prefs_add_none("/plugins/core/mback2k");
	purple_prefs_add_none("/plugins/core/mback2k/gcontactsync");
	purple_prefs_add_string("/plugins/core/mback2k/gcontactsync/username", "Username");
	purple_prefs_add_string("/plugins/core/mback2k/gcontactsync/password", "Password");
}

static PurplePluginPrefFrame* plugin_pref_frame(PurplePlugin *plugin) {
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;

	frame = purple_plugin_pref_frame_new();

	ppref = purple_plugin_pref_new_with_label("Google Account");
	purple_plugin_pref_frame_add(frame, ppref);

	ppref = purple_plugin_pref_new_with_name_and_label("/plugins/core/mback2k/gcontactsync/username", "Username");
	purple_plugin_pref_frame_add(frame, ppref);

	ppref = purple_plugin_pref_new_with_name_and_label("/plugins/core/mback2k/gcontactsync/password", "Password");
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
	"Google Contact Sync",
	"0.9",

	"Synchronizes meta contacts with Google Contacts.",
	"Synchronizes meta contacts with Google Contacts.",
	PLUGIN_AUTHOR,
	"http://www.marc-hoersken.de/",

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
