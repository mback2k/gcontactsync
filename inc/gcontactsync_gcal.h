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

#ifndef GCONTACTSYNC_GCAL_H_
#define GCONTACTSYNC_GCAL_H_

#include <string.h>
#include <glib.h>

#include <purple.h>
#include <gcal_status.h>

#include <gcontactsync.h>

gboolean plugin_check_gcal(int result, const char* function);
gboolean plugin_check_protocol(const char* protocol_id);
gboolean plugin_compare_protocols(const char* protocol_id, const char* protocol);
const char* plugin_get_protocol(const char* protocol_id);

#endif /* GCONTACTSYNC_GCAL_H_ */
