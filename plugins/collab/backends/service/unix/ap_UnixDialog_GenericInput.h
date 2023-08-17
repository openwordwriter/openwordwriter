/* Copyright (C) 2008 AbiSource Corporation B.V.
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

#ifndef AP_UNIXDIALOG_GENERICINPUT_H
#define AP_UNIXDIALOG_GENERICINPUT_H

#include <gtk/gtk.h>
#include <xp/ap_Dialog_GenericInput.h>

class XAP_Frame;

class AP_UnixDialog_GenericInput : public AP_Dialog_GenericInput
{
public:
	AP_UnixDialog_GenericInput(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	static XAP_Dialog * static_constructor(XAP_DialogFactory * pFactory, XAP_Dialog_Id id);
	void						runModal(XAP_Frame * pFrame) override;

	void						eventTextChanged();
	void						eventOk();

private:
	GtkWidget*	 				_constructWindow(void);
	void						_populateWindowData(void);

	GtkWidget*					m_wWindowMain;
	GtkWidget*					m_wOk;
	GtkWidget*					m_wInput;
};

#endif /* AP_UNIXDIALOG_GENERICINPUT_H */
