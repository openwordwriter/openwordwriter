 /* AbiWord
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

#ifndef AP_WIN32DIALOG_FORMATFRAME_H
#define AP_WIN32DIALOG_FORMATFRAME_H

#include "ap_Dialog_FormatFrame.h"
#include "xap_Frame.h"
#include "xap_Win32PreviewWidget.h"
#include "gr_Win32Graphics.h"
#include "xap_Win32ColourButton.h"
#include "xap_Win32DialogBase.h"

/*****************************************************************/

class ABI_EXPORT AP_Win32Dialog_FormatFrame: public AP_Dialog_FormatFrame, XAP_Win32DialogBase
{
public:
	AP_Win32Dialog_FormatFrame(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_Win32Dialog_FormatFrame(void);

	virtual void			runModeless(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	virtual void			event_Close(void);
	void					event_previewExposed(void);
	virtual void            	setSensitivity(bool bsens);
	virtual void            	destroy(void);
	virtual void            	activate(void);
	virtual void            	notifyActiveFrame(XAP_Frame * pFrame);
	virtual void                setBorderThicknessInGUI(const std::string & /*sThick*/) override { UT_ASSERT_HARMLESS(UT_NOT_IMPLEMENTED); };
	virtual void *				pGetWindowHandle( void ) {  return (void *) m_hDlg; }

	virtual BOOL			_onInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual BOOL 			_onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
    virtual BOOL 			_onDlgMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HBITMAP					m_hBitmapBottom;
	HBITMAP					m_hBitmapTop;
	HBITMAP					m_hBitmapRight;
	HBITMAP					m_hBitmapLeft;
	XAP_Win32PreviewWidget*	m_pPreviewWidget;
	XAP_Win32ColourButton	m_backgButton;
	XAP_Win32ColourButton	m_borderButton;
	double					m_dThickness[FORMAT_FRAME_NUMTHICKNESS];

};

#endif /* AP_WIN32DIALOG_FORMATFRAME+_H */
