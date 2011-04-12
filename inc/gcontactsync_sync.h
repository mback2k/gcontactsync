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

#ifndef GCONTACTSYNC_SYNC_H_
#define GCONTACTSYNC_SYNC_H_

#include <string.h>
#include <glib.h>

#include <gcal_parser.h>
#include <gcalendar.h>
#include <gcontact.h>

#include <gcontactsync.h>
#include <gcontactsync_debug.h>
#include <gcontactsync_gcal.h>

void plugin_action_link_conctacts_cb(PurplePluginAction *action);

#endif /* GCONTACTSYNC_SYNC_H_ */
