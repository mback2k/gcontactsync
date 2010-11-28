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

#include <gcontactsync_sync.h>

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

void plugin_action_link_conctacts_cb(PurplePluginAction *action) {
	char *username = NULL, *password = NULL;
    GHashTable *htcontacts = NULL;
	gcal_contact_t gcontact;
	struct gcal_contact_array gcontacts;
	int i;

	username = strdup(purple_prefs_get_string(PLUGIN_PREF_USERNAME));
	password = strdup(purple_prefs_get_string(PLUGIN_PREF_PASSWORD));

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

