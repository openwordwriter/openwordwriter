/* AbiSource Application Framework
 * Copyright (C) 2005-2023 Hubert Figuière
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA.
 */


#include "../xp/main.cpp"

#include "all_test_unix.h"
#include "ap_UnixApp.h"

static AP_UnixApp *app = nullptr;

void init_platform(void)
{
	app = new AP_UnixApp(PACKAGE);
	app->initialize(TRUE);
}



void terminate_platform(void)
{
	delete app;
	app = nullptr;
}
