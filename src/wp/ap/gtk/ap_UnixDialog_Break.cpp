/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t; -*- */
/* AbiWord
 * Copyright (C) 1998-2000 AbiSource, Inc.
 * Copyright (c) 2023 Hubert Figuière
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

#include <stdlib.h>

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// This header defines some functions for Unix dialogs,
// like centering them, measuring them, etc.
#include "xap_UnixDialogHelper.h"

#include "xap_App.h"
#include "ap_UnixApp.h"
#include "xap_UnixApp.h"
#include "xap_Frame.h"

#include "ap_Strings.h"
#include "ap_Dialog_Id.h"
#include "ap_Dialog_Break.h"
#include "ap_UnixDialog_Break.h"

/*****************************************************************/

#define	WIDGET_ID_TAG_KEY "id"
#define CUSTOM_RESPONSE_INSERT 1

/*****************************************************************/

XAP_Dialog * AP_UnixDialog_Break::static_constructor(XAP_DialogFactory * pFactory,
													   XAP_Dialog_Id id)
{
	AP_UnixDialog_Break * p = new AP_UnixDialog_Break(pFactory,id);
	return p;
}

AP_UnixDialog_Break::AP_UnixDialog_Break(XAP_DialogFactory * pDlgFactory,
										 XAP_Dialog_Id id)
	: AP_Dialog_Break(pDlgFactory,id)
	, m_windowMain(nullptr)
{
}

AP_UnixDialog_Break::~AP_UnixDialog_Break(void)
{
}

/*****************************************************************/
/*****************************************************************/

void AP_UnixDialog_Break::runModal(XAP_Frame * pFrame)
{
	UT_return_if_fail(pFrame);
	
    // Build the dialog's window
	m_windowMain = _constructWindow();
	UT_return_if_fail(m_windowMain);

	_populateWindowData();

	switch ( abiRunModalDialog ( GTK_DIALOG(m_windowMain),
								 pFrame, this, CUSTOM_RESPONSE_INSERT, false ) )
	{
		case CUSTOM_RESPONSE_INSERT:
			m_answer = AP_Dialog_Break::a_OK;
			break;
		default:
			m_answer = AP_Dialog_Break::a_CANCEL;
			break;
	}

	_storeWindowData();
	
	abiDestroyWidget ( m_windowMain ) ;
}

/*****************************************************************/
GtkWidget * AP_UnixDialog_Break::_constructWindow(void)
{
	GtkWidget * window;
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	

	GtkBuilder * builder = newDialogBuilderFromResource("ap_UnixDialog_Break.ui");
	
	// Update our member variables with the important widgets that 
	// might need to be queried or altered later
	window = GTK_WIDGET(gtk_builder_get_object(builder, "ap_UnixDialog_Break"));

	// set the dialog title
	std::string s;
	pSS->getValueUTF8(AP_STRING_ID_DLG_Break_BreakTitle,s);
	abiDialogSetTitle(window, "%s", s.c_str());
	
	// localize the strings in our dialog, and set tags for some widgets
	
	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbInsertBreak")), pSS, AP_STRING_ID_DLG_Break_Insert);

	auto rbPageBreak = GTK_WIDGET(gtk_builder_get_object(builder, "rbPageBreak"));
	localizeButton(rbPageBreak, pSS, AP_STRING_ID_DLG_Break_PageBreak);
	m_radioGroup.insert(std::make_pair(b_PAGE, rbPageBreak));

	auto rbColumnBreak = GTK_WIDGET(gtk_builder_get_object(builder, "rbColumnBreak"));
	localizeButton(rbColumnBreak, pSS, AP_STRING_ID_DLG_Break_ColumnBreak);
	m_radioGroup.insert(std::make_pair(b_COLUMN, rbColumnBreak));

	localizeLabelMarkup(GTK_WIDGET(gtk_builder_get_object(builder, "lbInsertSectionBreak")), pSS, AP_STRING_ID_DLG_Break_SectionBreaks);

	auto rbNextPage = GTK_WIDGET(gtk_builder_get_object(builder, "rbNextPage"));
	localizeButton(rbNextPage, pSS, AP_STRING_ID_DLG_Break_NextPage);
	m_radioGroup.insert(std::make_pair(b_NEXTPAGE, rbNextPage));

	auto rbContinuous = GTK_WIDGET(gtk_builder_get_object(builder, "rbContinuous"));
	localizeButton(rbContinuous, pSS, AP_STRING_ID_DLG_Break_Continuous);
	m_radioGroup.insert(std::make_pair(b_CONTINUOUS, rbContinuous));

	auto rbEvenPage = GTK_WIDGET(gtk_builder_get_object(builder, "rbEvenPage"));
	localizeButton(rbEvenPage, pSS, AP_STRING_ID_DLG_Break_EvenPage);
	m_radioGroup.insert(std::make_pair(b_EVENPAGE, rbEvenPage));

	auto rbOddPage = GTK_WIDGET(gtk_builder_get_object(builder, "rbOddPage"));
	localizeButton(rbOddPage, pSS, AP_STRING_ID_DLG_Break_OddPage);
	m_radioGroup.insert(std::make_pair(b_ODDPAGE, rbOddPage));

	localizeButtonUnderline(GTK_WIDGET(gtk_builder_get_object(builder, "btInsert")), pSS, AP_STRING_ID_DLG_InsertButton);

	g_object_unref(G_OBJECT(builder));

	return window;
}

void AP_UnixDialog_Break::_populateWindowData(void) const
{
	// We're a pretty stateless dialog, so we just set up
	// the defaults from our members.

	GtkWidget * widget = _findRadioByID(m_break);
	UT_ASSERT(widget);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE);
}

void AP_UnixDialog_Break::_storeWindowData(void)
{
	m_break = _getActiveRadioItem();
}

// TODO if this function is useful elsewhere, move it to Unix dialog
// TODO helpers and standardize on a user-data tag for WIDGET_ID_TAG_KEY
GtkWidget * AP_UnixDialog_Break::_findRadioByID(AP_Dialog_Break::breakType b) const
{
	auto w = m_radioGroup.find(b);
	if (w != m_radioGroup.cend()) {
		return w->second;
	}

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	return nullptr;
}

AP_Dialog_Break::breakType AP_UnixDialog_Break::_getActiveRadioItem(void) const
{
	for (auto item : m_radioGroup) {
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(item.second))) {
			return item.first;
		}
	}

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return AP_Dialog_Break::b_PAGE;
}
