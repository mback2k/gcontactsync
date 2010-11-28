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

#ifndef GCONTACTSYNC_H_
#define GCONTACTSYNC_H_

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <string.h>
#include <glib.h>

#include <purple.h>
#include <gcontact.h>

#include <gcontactsync_sync.h>

#define PLUGIN_ID "core-mback2k-gcontactsync"
#define PLUGIN_NAME "Google Contact Sync"
#define PLUGIN_VERSION "0.9"
#define PLUGIN_DESCRIPTION "Synchronizes meta contacts with Google Contacts."
#define PLUGIN_AUTHOR "Marc Hörsken <info@marc-hoersken.de>"
#define PLUGIN_WEBSITE "http://www.marc-hoersken.de/"
#define PLUGIN_DEBUG TRUE

#define PLUGIN_PREF_ROOT1 "/plugins/core/mback2k"
#define PLUGIN_PREF_ROOT2 "/plugins/core/mback2k/gcontactsync"
#define PLUGIN_PREF_USERNAME "/plugins/core/mback2k/gcontactsync/username"
#define PLUGIN_PREF_PASSWORD "/plugins/core/mback2k/gcontactsync/password"

PurplePlugin *gcontactsync_plugin;
gcal_t gcontactsync_gcal;

#endif /* GCONTACTSYNC_H_ */
