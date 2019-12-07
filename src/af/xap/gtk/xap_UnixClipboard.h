/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Application Framework
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

#ifndef XAP_UNIXCLIPBOARD_H
#define XAP_UNIXCLIPBOARD_H

#include <vector>

#include "ut_compiler.h"

#include <glib.h>
ABI_W_NO_CONST_QUAL
#include <gdk/gdk.h>
#include <gtk/gtk.h>
ABI_W_POP

#include "ut_types.h"
#include "ut_bytebuf.h"
#include "xap_FakeClipboard.h"
#include "xap_UnixApp.h"
#include "xap_EditMethods.h"
#include "ev_EditMethod.h"

//////////////////////////////////////////////////////////////////

class XAP_UnixClipboard
{
public:
	typedef enum _T_AllowGet { TAG_ClipboardOnly, TAG_PrimaryOnly } T_AllowGet;

	XAP_UnixClipboard(XAP_UnixApp * pUnixApp);
	virtual ~XAP_UnixClipboard();

	void				initialize();
	bool				assertSelection();

	bool				addData(T_AllowGet tTo, const char* format, const void* pData,
								UT_sint32 iNumBytes);

	void				clearData(bool bClipboard, bool bPrimary);
	void			finishedAddingData(void);
	bool				getData(T_AllowGet tFrom, const char** formatList,
								void ** ppData, UT_uint32 * pLen,
								const char **pszFormatFound);

	bool				getTextData(T_AllowGet tFrom, void ** ppData,
									UT_uint32 * pLen);

	bool canPaste(T_AllowGet tFrom) const;

#if GTK_CHECK_VERSION(3,96,0)
    GdkContentFormats* getTargets() const { return m_Targets ; }
#else
	GtkTargetEntry * getTargets () const { return m_Targets ; }
#endif
	UT_uint32 getNumTargets () const { return m_nTargets; }

protected:

	void				AddFmt(const char * fmt);
	void				deleteFmt(const char * fmt);

 private:

#if GTK_CHECK_VERSION(3,96,0)
	GdkClipboard *
#else
	GtkClipboard *
#endif
	gtkClipboardForTarget(XAP_UnixClipboard::_T_AllowGet get) const;

	bool				_getDataFromServer(T_AllowGet tFrom, const char** formatList,
							   void ** ppData, UT_uint32 * pLen,
							   const char **pszFormatFound);
	bool				_getDataFromFakeClipboard(T_AllowGet tFrom, const char** formatList,
								  void ** ppData, UT_uint32 * pLen,
								  const char **pszFormatFound);

	static inline void s_primary_get_func(GtkClipboard *clipboard,
				       GtkSelectionData *selection_data,
				       guint info,
				       gpointer ptr)
	  {
	    XAP_UnixClipboard * pThis = static_cast<XAP_UnixClipboard*>(ptr);
	    pThis->primary_get_func(clipboard, selection_data, info);
	  }

	static inline void s_primary_clear_func (GtkClipboard *clipboard,
					  gpointer ptr)
	  {
	    XAP_UnixClipboard * pThis = static_cast<XAP_UnixClipboard*>(ptr);
	    pThis->primary_clear_func(clipboard);
	  }

	void primary_get_func(GtkClipboard *clipboard,
			      GtkSelectionData *selection_data,
			      guint info);

	void primary_clear_func (GtkClipboard *clipboard);

	static inline void s_clipboard_get_func(GtkClipboard *clipboard,
					 GtkSelectionData *selection_data,
					 guint info,
					 gpointer ptr)
	  {
	    XAP_UnixClipboard * pThis = static_cast<XAP_UnixClipboard*>(ptr);
	    pThis->clipboard_get_func(clipboard, selection_data, info);
	  }

	static inline void s_clipboard_clear_func (GtkClipboard *clipboard,
					  gpointer ptr)
	  {
	    XAP_UnixClipboard * pThis = static_cast<XAP_UnixClipboard*>(ptr);
	    pThis->clipboard_clear_func(clipboard);
	  }

	void clipboard_get_func(GtkClipboard *clipboard,
				GtkSelectionData *selection_data,
				guint info);

	void clipboard_clear_func (GtkClipboard *clipboard);

	void common_get_func(GtkClipboard *clipboard,
			     GtkSelectionData *selection_data,
			     guint info, T_AllowGet which);

	std::vector<const char*>  m_vecFormat_AP_Name;
#if !GTK_CHECK_VERSION(3,96,0)
	std::vector<GdkAtom>  m_vecFormat_GdkAtom;
#endif

	UT_ByteBuf m_databuf; // for gets only

	XAP_UnixApp *		m_pUnixApp;
	XAP_FakeClipboard	m_fakeClipboard;		// internal clipboard to short-circut the XServer.

	XAP_FakeClipboard       m_fakePrimaryClipboard;
#if GTK_CHECK_VERSION(3,96,0)
	GdkContentFormats* m_Targets;
	GdkClipboard* m_clip;
	GdkClipboard* m_primary;
#else
	GtkTargetEntry * m_Targets ;
	GtkClipboard * m_clip;
	GtkClipboard * m_primary;
#endif
	UT_uint32 m_nTargets;

};

#endif /* XAP_UNIXCLIPBOARD_H */
