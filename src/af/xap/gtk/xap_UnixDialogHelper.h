/* AbiSource Program Utilities
 * Copyright (C) 1998 AbiSource, Inc.
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

#pragma once

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <functional>

#include "ut_types.h"
#include "xap_Strings.h"
#include "xap_GtkUtils.h"

class XAP_Frame;
class XAP_FrameHelper;
class XAP_App;
class XAP_Dialog;

// This macro sets up stack pointers to be used with the ConvertToUnixString
// macro.
#define SETUP_UNIX_STRING  	gchar * newstr = nullptr;

// This macro is for use in Unix dialogs where the strings are to
// be stripped of ampersands (Windows accelerator characters).
#define	CONVERT_TO_UNIX_STRING(str, id, newstr)		do { \
                                                    FREEP(newstr); \
                                                    std::string _s;           \
                                                    pSS->getValueUTF8(id,_s); \
                                                    UT_XML_cloneNoAmpersands(newstr, _s.c_str()); \
							} while (0);

// This macro is for use in Unix dialogs where the accelerator characters
// strings need to be converted to underscores.
#define	CONVERT_TO_ACC_STRING(str, id, newstr)		do { \
                                                    FREEP(newstr); \
                                                    std::string _s;           \
                                                    pSS->getValueUTF8(id,_s); \
                                                    UT_XML_cloneConvAmpersands(newstr, _s.c_str()); \
							} while (0);


/** load a GtkBuilder for a dialog using the standard path.
 * @param name the filename of the dialog (no path)
 * @return the GtkBuilder or nullptr. The returned object my be freed as usual.
 */
GtkBuilder * newDialogBuilder(const char * name);
GtkBuilder* newDialogBuilderFromResource(const char* name);
void connectFocus(GtkWidget *widget,const XAP_Frame *frame);
void connectFocusModeless(GtkWidget *widget,const XAP_App *pApp);
void connectFocusModelessOther(GtkWidget *widget, const XAP_App *pApp,
                               std::function<gboolean(int)> *other_function);
bool isTransientWindow(GtkWindow *window,GtkWindow *parent);

// This is a very thin message box; only use it for startup errors
// or places where you can't use the message box class (like when
// you don't have a frame yet).
void messageBoxOK(const char * message);

// Centers a GTK window inside a parent window
void centerDialog(GtkWidget * parent, GtkWidget * child, bool set_transient_for = true);

void abiSetupModalDialog(GtkDialog * me, XAP_Frame *pFrame, XAP_Dialog * pDlg, gint dfl_id);
gint abiRunModalDialog(GtkDialog * me, bool destroyDialog, AtkRole role = ATK_ROLE_DIALOG);
gint abiRunModalDialog(GtkDialog * me, XAP_Frame *pFrame, XAP_Dialog * pDlg, gint dfl_id, bool destroyDialog, AtkRole role = ATK_ROLE_DIALOG);
void abiSetupModelessDialog(GtkDialog * me, XAP_Frame * pFrame, XAP_Dialog * pDlg, gint dfl_id, bool abi_modeless = true, AtkRole role = ATK_ROLE_DIALOG);
void abiDestroyWidget(GtkWidget * me);

GtkWidget* abiAddButton(GtkDialog * me, std::string label, gint response_id);

GtkWidget * abiDialogNew(const char * role, gboolean resizable = FALSE);
GtkWidget * abiDialogNew(const char * role, gboolean resizable, const char * title, ...);

void abiDialogSetTitle(GtkWidget * dlg, const char * title, ...)
    ABI_PRINTF_FORMAT(2,3);

void convertMnemonics(gchar * s);
std::string & convertMnemonics(std::string & s);

void localizeLabel(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeLabelUnderline(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeLabelMarkup(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeButton(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeButtonUnderline(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeButtonMarkup(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void localizeMenuItem(GtkWidget * widget, const XAP_StringSet * pSS, XAP_String_Id id);
void setLabelMarkup(GtkWidget * widget, const gchar * str);

void abiSetActivateOnWidgetToActivateButton( GtkWidget* source, GtkWidget* button );

