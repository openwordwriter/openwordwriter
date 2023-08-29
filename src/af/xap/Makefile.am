
noinst_LTLIBRARIES = libxap.la

AM_CPPFLAGS = \
	$(IMPEXP_CPPFLAGS) \
	$(AF_CPPFLAGS) \
	-I$(top_srcdir)/src/wp/ap/xp \
	-DICONDIR="\"$(ABIWORD_ICONDIR)\""

libxap_la_SOURCES = \
	xp/xad_Document.cpp \
	xp/xad_Document.h \
	xp/xap_App.cpp \
	xp/xap_App.h \
	xp/xap_AppImpl.cpp \
	xp/xap_AppImpl.h \
	xp/xap_Args.cpp \
	xp/xap_Args.h \
	xp/xap_Clipboard.cpp \
	xp/xap_Clipboard.h \
	xp/xap_CustomWidget.cpp \
	xp/xap_CustomWidget.h \
	xp/xap_Dialog.cpp \
	xp/xap_DialogFactory.cpp \
	xp/xap_DialogFactory.h \
	xp/xap_Dialog.h \
	xp/xap_Dialog_Id.h \
	xp/xap_Dictionary.cpp \
	xp/xap_Dictionary.h \
	xp/xap_Dlg_About.cpp \
	xp/xap_Dlg_About.h \
	xp/xap_Dlg_ClipArt.cpp \
	xp/xap_Dlg_ClipArt.h \
	xp/xap_Dlg_DocComparison.cpp \
	xp/xap_Dlg_DocComparison.h \
	xp/xap_Dlg_Encoding.cpp \
	xp/xap_Dlg_Encoding.h \
	xp/xap_Dlg_FileOpenSaveAs.cpp \
	xp/xap_Dlg_FileOpenSaveAs.h \
	xp/xap_Dlg_FontChooser.cpp \
	xp/xap_Dlg_FontChooser.h \
	xp/xap_Dlg_History.cpp \
	xp/xap_Dlg_History.h \
	xp/xap_Dlg_HTMLOptions.cpp \
	xp/xap_Dlg_HTMLOptions.h \
	xp/xap_Dlg_Image.cpp \
	xp/xap_Dlg_Image.h \
	xp/xap_Dlg_Insert_Symbol.cpp \
	xp/xap_Dlg_Insert_Symbol.h \
	xp/xap_Dlg_Language.cpp \
	xp/xap_Dlg_Language.h \
	xp/xap_Dlg_ListDocuments.cpp \
	xp/xap_Dlg_ListDocuments.h \
	xp/xap_Dlg_MessageBox.cpp \
	xp/xap_Dlg_MessageBox.h \
	xp/xap_Dlg_Password.cpp \
	xp/xap_Dlg_Password.h \
	xp/xap_Dlg_PluginManager.cpp \
	xp/xap_Dlg_PluginManager.h \
	xp/xap_Dlg_Print.cpp \
	xp/xap_Dlg_Print.h \
	xp/xap_Dlg_PrintPreview.cpp \
	xp/xap_Dlg_PrintPreview.h \
	xp/xap_Dlg_WindowMore.cpp \
	xp/xap_Dlg_WindowMore.h \
	xp/xap_Dlg_Zoom.cpp \
	xp/xap_Dlg_Zoom.h \
	xp/xap_Drawable.h \
	xp/xap_Draw_Symbol.cpp \
	xp/xap_Draw_Symbol.h \
	xp/xap_EditMethods.h \
	xp/xap_EncodingManager.cpp \
	xp/xap_EncodingManager.h \
	xp/xap_FakeClipboard.cpp \
	xp/xap_FakeClipboard.h \
	xp/xap_Features.h \
	xp/xap_FontPreview.cpp \
	xp/xap_FontPreview.h \
	xp/xap_Frame.cpp \
	xp/xap_Frame.h \
	xp/xap_FrameImpl.cpp \
	xp/xap_FrameImpl.h \
	xp/xap_InputModes.cpp \
	xp/xap_InputModes.h \
	xp/xap_LoadBindings.cpp \
	xp/xap_LoadBindings.h \
	xp/xap_Log.cpp \
	xp/xap_Log.h \
	xp/xap_Menu_ActionSet.h \
	xp/xap_Menu_LabelSet.h \
	xp/xap_Menu_Layouts.h \
	xp/xap_Module.cpp \
	xp/xap_ModuleFactory.h \
	xp/xap_Module.h \
	xp/xap_ModuleManager.cpp \
	xp/xap_ModuleManager.h \
	xp/xap_Prefs.cpp \
	xp/xap_Prefs.h \
	xp/xap_Prefs_SchemeIds.h \
	xp/xap_Preview.cpp \
	xp/xap_Preview.h \
	xp/xap_Preview_Zoom.cpp \
	xp/xap_Preview_Zoom.h \
	xp/xap_Resource.cpp \
	xp/xap_Resource.h \
	xp/xap_ResourceManager.cpp \
	xp/xap_ResourceManager.h \
	xp/xap_Scrollbar_ViewListener.cpp \
	xp/xap_Scrollbar_ViewListener.h \
	xp/xap_StatusBar.h \
	xp/xap_String_Id.h \
	xp/xap_Strings.cpp \
	xp/xap_Strings.h \
	xp/xap_Toolbar_ActionSet.h \
	xp/xap_Toolbar_ControlFactory.cpp \
	xp/xap_Toolbar_ControlFactory.h \
	xp/xap_Toolbar_Icons.h \
	xp/xap_Toolbar_LabelSet.h \
	xp/xap_Toolbar_Layouts.h \
	xp/xap_Types.h \
	xp/xap_ViewListener.cpp \
	xp/xap_ViewListener.h \
	xp/xap_Widget.h \
	xp/xav_Listener.h \
	xp/xav_View.cpp \
	xp/xav_View.h

if ENABLE_STATUSBAR
libxap_la_SOURCES += \
	xp/xap_StatusBar.cpp
endif

if ENABLE_SPELL
libxap_la_SOURCES += \
	xp/barbarisms.cpp \
	xp/barbarisms.h \
	xp/enchant_checker.cpp \
	xp/enchant_checker.h \
	xp/ispell_def.h \
	xp/spell_manager.cpp \
	xp/spell_manager.h
endif

if TOOLKIT_GTK_ALL
AM_CPPFLAGS += \
	-I$(top_srcdir)/src/wp/ap/gtk

libxap_la_SOURCES += \
	gtk/xap_GtkUtils.h \
	gtk/xap_GtkUtils.cpp \
	gtk/xap_GtkObjectHolder.h \
	gtk/xap_GtkSignalBlocker.h \
	gtk/xap_GtkComboBoxHelpers.h \
	gtk/xap_GtkComboBoxHelpers.cpp \
	gtk/xap_GtkStyle.h \
	gtk/xap_GtkStyle.cpp \
	gtk/xap_EmbeddedFeatures.h \
	gtk/xap_UnixApp.cpp \
	gtk/xap_UnixApp.h \
	gtk/xap_UnixAppImpl.cpp \
	gtk/xap_UnixAppImpl.h \
	gtk/xap_UnixClipboard.cpp \
	gtk/xap_UnixClipboard.h \
	gtk/xap_UnixCustomWidget.cpp \
	gtk/xap_UnixCustomWidget.h \
	gtk/xap_UnixDialogFactory.h \
	gtk/xap_UnixDialogHelper.cpp \
	gtk/xap_UnixDialogHelper.h \
	gtk/xap_UnixDialog.cpp \
	gtk/xap_UnixDialog.h \
	gtk/xap_UnixDlg_About.cpp \
	gtk/xap_UnixDlg_About.h \
	gtk/xap_UnixDlg_ClipArt.cpp \
	gtk/xap_UnixDlg_ClipArt.h \
	gtk/xap_UnixDlg_ColorChooser.h \
	gtk/xap_UnixDlg_ColorChooser.cpp \
	gtk/xap_UnixDlg_DocComparison.cpp \
	gtk/xap_UnixDlg_DocComparison.h \
	gtk/xap_UnixDlg_Encoding.cpp \
	gtk/xap_UnixDlg_Encoding.h \
	gtk/xap_UnixDlg_FileOpenSaveAs.cpp \
	gtk/xap_UnixDlg_FileOpenSaveAs.h \
	gtk/xap_UnixDlg_FontChooser.cpp \
	gtk/xap_UnixDlg_FontChooser.h \
	gtk/xap_UnixDlg_History.cpp \
	gtk/xap_UnixDlg_History.h \
	gtk/xap_UnixDlg_HTMLOptions.cpp \
	gtk/xap_UnixDlg_HTMLOptions.h \
	gtk/xap_UnixDlg_Image.cpp \
	gtk/xap_UnixDlg_Image.h \
	gtk/xap_UnixDlg_Insert_Symbol.cpp \
	gtk/xap_UnixDlg_Insert_Symbol.h \
	gtk/xap_UnixDlg_Language.cpp \
	gtk/xap_UnixDlg_Language.h \
	gtk/xap_UnixDlg_ListDocuments.cpp \
	gtk/xap_UnixDlg_ListDocuments.h \
	gtk/xap_UnixDlg_MessageBox.cpp \
	gtk/xap_UnixDlg_MessageBox.h \
	gtk/xap_UnixDlg_Password.cpp \
	gtk/xap_UnixDlg_Password.h \
	gtk/xap_UnixDlg_PluginManager.cpp \
	gtk/xap_UnixDlg_PluginManager.h \
	gtk/xap_UnixDlg_Print.cpp \
	gtk/xap_UnixDlg_Print.h \
	gtk/xap_UnixDlg_PrintPreview.cpp \
	gtk/xap_UnixDlg_PrintPreview.h \
	gtk/xap_UnixDlg_WindowMore.cpp \
	gtk/xap_UnixDlg_WindowMore.h \
	gtk/xap_UnixDlg_Zoom.cpp \
	gtk/xap_UnixDlg_Zoom.h \
	gtk/xap_UnixEncodingManager.cpp \
	gtk/xap_UnixEncodingManager.h \
	gtk/xap_UnixFeatures.h \
	gtk/xap_UnixFontPreview.cpp \
	gtk/xap_UnixFontPreview.h \
	gtk/xap_UnixFrameImpl.cpp \
	gtk/xap_UnixFrameImpl.h \
	gtk/xap_UnixModule.cpp \
	gtk/xap_UnixModule.h \
	gtk/xap_UnixTableWidget.cpp \
	gtk/xap_UnixTableWidget.h \
	gtk/xap_Unix_TB_CFactory.h \
	gtk/xap_UnixWidget.cpp \
	gtk/xap_UnixWidget.h

endif

if TOOLKIT_COCOA
libxap_la_SOURCES += \
	xap_QtApp.h \
	xap_QtApp.cpp \
	xap_QtAppImpl.cpp \
	xap_QtAppImpl.h \
	xap_QtDialogFactory.h \
	xap_QtEncodingManager.cpp \
	xap_QtEncodingManager.h \
	xap_QtFeatures.h \
	xap_QtFrameImpl.h \
	xap_QtFrameImpl.cpp \
	xap_QtModule.h \
	xap_QtModule.cpp \
	xap_Qt_TB_CFactory.h \
	xap_QtDlg_FileOpenSaveAs.cpp \
	xap_QtDlg_FileOpenSaveAs.h
endif

if TOOLKIT_WIN
libxap_la_SOURCES += \
	win/winezmouse.h \
	win/xap_Win32App.cpp \
	win/xap_Win32App.h \
	win/xap_Win32AppImpl.cpp \
	win/xap_Win32AppImpl.h \
	win/xap_Win32Clipboard.cpp \
	win/xap_Win32Clipboard.h \
	win/xap_Win32ColourButton.h \
	win/xap_Win32DialogBase.cpp \
	win/xap_Win32DialogBase.h \
	win/xap_Win32DialogFactory.h \
	win/xap_Win32DialogHelper.cpp \
	win/xap_Win32DialogHelper.h \
	win/xap_Win32Dlg_About.cpp \
	win/xap_Win32Dlg_About.h \
	win/xap_Win32Dlg_ClipArt.cpp \
	win/xap_Win32Dlg_ClipArt.h \
	win/xap_Win32Dlg_DocComparison.cpp \
	win/xap_Win32Dlg_DocComparison.h \
	win/xap_Win32Dlg_Encoding.cpp \
	win/xap_Win32Dlg_Encoding.h \
	win/xap_Win32Dlg_FileOpenSaveAs.cpp \
	win/xap_Win32Dlg_FileOpenSaveAs.h \
	win/xap_Win32Dlg_FontChooser.cpp \
	win/xap_Win32Dlg_FontChooser.h \
	win/xap_Win32Dlg_History.cpp \
	win/xap_Win32Dlg_History.h \
	win/xap_Win32Dlg_HTMLOptions.cpp \
	win/xap_Win32Dlg_HTMLOptions.h \
	win/xap_Win32Dlg_Image.cpp \
	win/xap_Win32Dlg_Image.h \
	win/xap_Win32Dlg_Insert_Symbol.cpp \
	win/xap_Win32Dlg_Insert_Symbol.h \
	win/xap_Win32Dlg_Language.cpp \
	win/xap_Win32Dlg_Language.h \
	win/xap_Win32Dlg_ListDocuments.cpp \
	win/xap_Win32Dlg_ListDocuments.h \
	win/xap_Win32Dlg_MessageBox.cpp \
	win/xap_Win32Dlg_MessageBox.h \
	win/xap_Win32Dlg_Password.cpp \
	win/xap_Win32Dlg_Password.h \
	win/xap_Win32Dlg_PluginManager.cpp \
	win/xap_Win32Dlg_PluginManager.h \
	win/xap_Win32Dlg_Print.cpp \
	win/xap_Win32Dlg_Print.h \
	win/xap_Win32Dlg_PrintPreview.cpp \
	win/xap_Win32Dlg_PrintPreview.h \
	win/xap_Win32Dlg_WindowMore.cpp \
	win/xap_Win32Dlg_WindowMore.h \
	win/xap_Win32Dlg_Zoom.cpp \
	win/xap_Win32Dlg_Zoom.h \
	win/xap_Win32DragAndDrop.cpp \
	win/xap_Win32DragAndDrop.h \
	win/xap_Win32EncodingManager.cpp \
	win/xap_Win32EncodingManager.h \
	win/xap_Win32Features.h \
	win/xap_Win32FontPreview.cpp \
	win/xap_Win32FontPreview.h \
	win/xap_Win32FrameImpl.cpp \
	win/xap_Win32FrameImpl.h \
	win/xap_Win32LabelledSeparator.cpp \
	win/xap_Win32LabelledSeparator.h \
	win/xap_Win32Module.cpp \
	win/xap_Win32Module.h \
	win/xap_Win32PreviewWidget.cpp \
	win/xap_Win32PreviewWidget.h \
	win/xap_Win32PropertySheet.cpp \
	win/xap_Win32PropertySheet.h \
	win/xap_Win32Slurp.cpp \
	win/xap_Win32Slurp.h \
	win/xap_Win32_TB_CFactory.h \
	win/xap_Win32Toolbar_Icons.cpp \
	win/xap_Win32Toolbar_Icons.h

rc2files = \
	win/xap_Win32Res_Cursors.rc2 \
	win/xap_Win32Res_DlgDocComparison.rc2 \
	win/xap_Win32Res_DlgEncoding.rc2 \
	win/xap_Win32Res_DlgFont.rc2 \
	win/xap_Win32Res_DlgHistory.rc2 \
	win/xap_Win32Res_DlgHTMLOptions.rc2 \
	win/xap_Win32Res_DlgImage.rc2 \
	win/xap_Win32Res_DlgInsertPicture.rc2 \
	win/xap_Win32Res_DlgInsertSymbol.rc2 \
	win/xap_Win32Res_DlgLanguage.rc2 \
	win/xap_Win32Res_DlgListDocuments.rc2 \
	win/xap_Win32Res_DlgPassword.rc2 \
	win/xap_Win32Res_DlgPlugin.rc2 \
	win/xap_Win32Res_DlgWindowMore.rc2 \
	win/xap_Win32Res_DlgZoom.rc2 \
	win/xap_Win32Resources.rc2

curfiles = \
	win/downarrow.cur \
	win/exchange.cur \
	win/hand1.cur \
	win/rightcur.cur
endif

if TOOLKIT_QT
AM_CPPFLAGS += \
	-I$(top_srcdir)/src/wp/ap/qt

libxap_la_SOURCES += \
	xap_QtApp.h \
	xap_QtApp.cpp \
	xap_QtAppImpl.cpp \
	xap_QtAppImpl.h \
	xap_QtDialogFactory.h \
	xap_QtEncodingManager.cpp \
	xap_QtEncodingManager.h \
	xap_QtFeatures.h \
	xap_QtFrameImpl.h \
	xap_QtFrameImpl.cpp \
	xap_QtModule.h \
	xap_QtModule.cpp \
	xap_Qt_TB_CFactory.h \
	xap_QtDlg_FileOpenSaveAs.cpp \
	xap_QtDlg_FileOpenSaveAs.h
endif

EXTRA_DIST = \
	xp/xap_Prefs.t.cpp \
	gtk/xap_UnixWidget.t.cpp \
	gtk/xap_UnixDlg_Encoding.ui \
	gtk/xap_UnixDlg_Image.ui \
	gtk/xap_UnixDlg_Language.ui \
	gtk/xap_UnixDlg_Password.ui \
	gtk/xap_UnixDlg_PluginManager.ui \
	gtk/xap_UnixDlg_WindowMore.ui \
	gtk/xap_UnixDlg_Zoom.ui \
	gtk/xap_UnixDlg_History.ui \
	gtk/xap_UnixDlg_DocComparison.ui \
	gtk/xap_UnixDlg_ListDocuments.ui \
	$(rc2files) \
	$(curfiles)
