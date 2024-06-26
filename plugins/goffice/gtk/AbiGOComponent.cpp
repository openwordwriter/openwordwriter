/* AbiWord
 * Copyright (C) 2004 Luca Padovani <lpadovan@cs.unibo.it>
 * Copyright (C) 2005 Martin Sevior <msevior@physics.unimelb.edu.au>
 * Copyright (C) 2005 Jean Brefort <jean.brefort@normalesup.org>
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "ap_Strings.h"
#include "ev_EditMethod.h"
#include "fv_View.h"
#include "fp_Run.h"
#include "ie_impGraphic.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "gr_Painter.h"
#include "gr_UnixImage.h"
#include "ut_units.h"
#include "gr_UnixCairoGraphics.h"
#include "xap_Dialog_Id.h"
#include "xap_Dlg_FileOpenSaveAs.h"
#include "xap_Dlg_MessageBox.h"
#include "xap_Frame.h"
#include "xap_UnixFrameImpl.h"
#include "xap_UnixDialogHelper.h"
#include "AbiGOComponent.h"
#include "ie_imp_GOComponent.h"
#include "AbiGOffice.h"
#include <math.h>
#include <goffice/component/go-component-factory.h>
#include <goffice/goffice.h>
#ifndef G_VALUE_INIT
#define G_VALUE_INIT { 0, { { 0 } } }
#endif

static void changed_cb (GOComponent*, gpointer);

XAP_Dialog_MessageBox::tAnswer s_CouldNotLoadFileMessage(XAP_Frame * pFrame, const char * pNewFile, UT_Error errorCode)
{
	XAP_String_Id String_id;

	switch (errorCode)
	  {
	  case -301:
		String_id = AP_STRING_ID_MSG_IE_FileNotFound;
		break;
	  case -302:
		String_id = AP_STRING_ID_MSG_IE_NoMemory;
		break;

	  case -303:
		String_id = AP_STRING_ID_MSG_IE_UnsupportedType;
		//AP_STRING_ID_MSG_IE_UnknownType;
		break;

	  case -304:
		String_id = AP_STRING_ID_MSG_IE_BogusDocument;
		break;

	  case -305:
		String_id = AP_STRING_ID_MSG_IE_CouldNotOpen;
		break;

	  case -306:
		String_id = AP_STRING_ID_MSG_IE_CouldNotWrite;
		break;

	  case -307:
		String_id = AP_STRING_ID_MSG_IE_FakeType;
		break;

	  case -311:
		String_id = AP_STRING_ID_MSG_IE_UnsupportedType;
		break;

	  default:
		String_id = AP_STRING_ID_MSG_ImportError;
	  }

	return pFrame->showMessageBox(String_id,
									XAP_Dialog_MessageBox::b_O,
									XAP_Dialog_MessageBox::a_OK,
									pNewFile);
}

static bool s_AskForGOComponentPathname(XAP_Frame * pFrame,
					   char ** ppPathname,
					   IEGraphicFileType * iegft)
{
	// raise the file-open dialog for inserting a component.
	// return a_OK or a_CANCEL depending on which button
	// the user hits.
	// return a pointer a g_strdup()'d string containing the
	// pathname the user entered -- ownership of this goes
	// to the caller (so free it when you're done with it).

	UT_DEBUGMSG(("s_AskForGOComponentPathname: frame %p\n",
				 pFrame));

	UT_return_val_if_fail (ppPathname, false);
	*ppPathname = nullptr;

	pFrame->raise();

	XAP_DialogFactory * pDialogFactory
		= static_cast<XAP_DialogFactory *>(pFrame->getDialogFactory());

	XAP_Dialog_FileOpenSaveAs * pDialog
		= static_cast<XAP_Dialog_FileOpenSaveAs *>(pDialogFactory->requestDialog(XAP_DIALOG_ID_INSERTOBJECT));
	UT_return_val_if_fail (pDialog, false);

	pDialog->setCurrentPathname("");
	pDialog->setSuggestFilename(false);

	// to fill the file types popup list, we need to convert AP-level
	// ImpGraphic descriptions, suffixes, and types into strings.

	UT_uint32 filterCount = IE_ImpGraphic::getImporterCount();

	const char ** szDescList = static_cast<const char **>(UT_calloc(filterCount + 1, sizeof(char *)));
	const char ** szSuffixList = static_cast<const char **>(UT_calloc(filterCount + 1, sizeof(char *)));
	IEGraphicFileType * nTypeList = (IEGraphicFileType *)
		 UT_calloc(filterCount + 1,	sizeof(IEGraphicFileType));
	UT_uint32 k = 0;

	while (IE_ImpGraphic::enumerateDlgLabels(k, &szDescList[k], &szSuffixList[k], &nTypeList[k]))
		k++;

	pDialog->setFileTypeList(szDescList, szSuffixList, static_cast<const UT_sint32 *>(nTypeList));
	if (iegft != nullptr)
	  pDialog->setDefaultFileType(*iegft);
	pDialog->runModal(pFrame);

	XAP_Dialog_FileOpenSaveAs::tAnswer ans = pDialog->getAnswer();
	bool bOK = (ans == XAP_Dialog_FileOpenSaveAs::a_OK);

	if (bOK)
	{
		const std::string & resultPathname = pDialog->getPathname();
		UT_DEBUGMSG(("OBJECT Path Name selected = %s \n", resultPathname.c_str()));
		if (!resultPathname.empty()) {
			*ppPathname = g_strdup(resultPathname.c_str());
		}

		UT_sint32 type = pDialog->getFileType();

		// If the number is negative, it's a special type.
		// Some operating systems which depend solely on filename
		// suffixes to identify type (like Windows) will always
		// want auto-detection.
		if (type < 0)
			switch (type)
			{
			case XAP_DIALOG_FILEOPENSAVEAS_FILE_TYPE_AUTO:
				// do some automagical detecting
				*iegft = IEGFT_Unknown;
				break;
			default:
				// it returned a type we don't know how to handle
				UT_ASSERT_HARMLESS(UT_SHOULD_NOT_HAPPEN);
			}
		else
			*iegft = static_cast<IEGraphicFileType>(pDialog->getFileType());
	}

	FREEP(szDescList);
	FREEP(szSuffixList);
	FREEP(nTypeList);

	pDialogFactory->releaseDialog(pDialog);

	return bOK;
}

//
// AbiGOComponent_FileInsert
// -------------------
//   This is the function that we actually call to insert a component using
//	data from a file.
//
bool 
AbiGOComponent_FileInsert(G_GNUC_UNUSED AV_View* v, G_GNUC_UNUSED EV_EditMethodCallData *d)
{
    // Get the current view that the user is in.
    XAP_Frame *pFrame = XAP_App::getApp()->getLastFocussedFrame();
    PD_Document * pDoc = static_cast<PD_Document *>(pFrame->getCurrentDoc());
    char* pNewFile = nullptr;


    IEGraphicFileType iegft = IEGFT_Unknown;
    bool bOK = s_AskForGOComponentPathname(pFrame,&pNewFile,&iegft);
	
    if (!bOK || !pNewFile)
    {
      UT_DEBUGMSG(("ARRG! bOK = %d pNewFile = %s \n",bOK,pNewFile));
      return false;
    }
    UT_UTF8String sNewFile = pNewFile;

    // we own storage for pNewFile and must free it.
    FREEP(pNewFile);


    UT_DEBUGMSG(("fileInsertObject: loading [%s]\n",sNewFile.utf8_str()));

	char *mime_type = go_get_mime_type(sNewFile.utf8_str());
    IE_Imp_Component * pImpComponent = new IE_Imp_Component(pDoc, mime_type);
	g_free(mime_type);
    UT_Error errorCode = pImpComponent->importFile(sNewFile.utf8_str());

  DELETEP(pImpComponent);
    if(errorCode != UT_OK)
    {
      s_CouldNotLoadFileMessage(pFrame, sNewFile.utf8_str(), errorCode);
      return false;
    }

    return true;
}

static bool button_press_cb(GtkDialog *dlg, GdkEventButton *ev)
{
	if (gdk_event_get_event_type((GdkEvent*)ev) == GDK_DOUBLE_BUTTON_PRESS)
		gtk_dialog_response(dlg, GTK_RESPONSE_OK);
	return false;
}
//
// AbiGOComponent_Create
// -------------------
//   This is the function that we actually call to insert a new intially empty component.
bool 
AbiGOComponent_Create (G_GNUC_UNUSED AV_View* v, G_GNUC_UNUSED EV_EditMethodCallData *d)
{
    XAP_Frame *pFrame = XAP_App::getApp()->getLastFocussedFrame();
	XAP_UnixFrameImpl *pFrameImpl = static_cast<XAP_UnixFrameImpl*>(pFrame->getFrameImpl());
	std::string cancel, ok;
	const XAP_StringSet *pSS = XAP_App::getApp()->getStringSet();
	pSS->getValueUTF8(XAP_STRING_ID_DLG_Cancel, cancel);
	pSS->getValueUTF8(XAP_STRING_ID_DLG_OK, ok);
	GtkDialog *dialog = GTK_DIALOG (gtk_dialog_new_with_buttons ("New Object",
		GTK_WINDOW(pFrameImpl->getTopLevelWindow()),
		(GtkDialogFlags) (GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
		convertMnemonics(cancel).c_str(), GTK_RESPONSE_CANCEL,
		convertMnemonics(ok).c_str(), GTK_RESPONSE_OK, nullptr));
	GtkListStore *list = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
	GtkWidget *w = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list));
	g_signal_connect_swapped(w, "button-press-event", G_CALLBACK(button_press_cb), dialog);
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Object type:", renderer, "text", 0, nullptr);
	gtk_tree_view_append_column (GTK_TREE_VIEW (w), column);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW (w));
	gtk_tree_selection_set_mode (sel, GTK_SELECTION_BROWSE);
	GtkTreeIter iter;
	GSList *l = mime_types;
	gchar const *mime_type;
	while (l) {
		mime_type = (gchar const *) l->data;
		if (strcmp(mime_type, "application/mathml+xml")
		    && go_components_get_priority(mime_type) >= GO_MIME_PRIORITY_PARTIAL)
		{
			gtk_list_store_append (list, &iter);
			gtk_list_store_set (list, &iter,
					  0, go_mime_type_get_description (mime_type),
					  1, mime_type,
					  -1);
		}
		l = l->next;
	}
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(dialog)), w, false, false, 0);
	gtk_widget_show_all (GTK_WIDGET(dialog));
	gint result = gtk_dialog_run (dialog);
	if (result == GTK_RESPONSE_OK &&
		gtk_tree_selection_get_selected (sel, nullptr, &iter)) {
		gtk_tree_model_get (GTK_TREE_MODEL (list), &iter, 1, &mime_type, -1);
		GOComponent *component = go_component_new_by_mime_type (mime_type);
		go_component_set_inline (component, true);
		go_component_set_use_font_from_app (component, true);
		g_signal_connect (G_OBJECT (component), "changed",
								G_CALLBACK (changed_cb), nullptr);
		GtkWindow *win = go_component_edit(component);
		gtk_window_set_transient_for(win, GTK_WINDOW(pFrameImpl->getTopLevelWindow()));
	}
	gtk_widget_destroy(GTK_WIDGET (dialog)); // TOPLEVEL
	return result == GTK_RESPONSE_OK;
}

GR_AbiGOComponentItems::GR_AbiGOComponentItems(void):
  m_iAPI(0),
  m_bHasSnapshot(false)
{
}

GR_AbiGOComponentItems::~GR_AbiGOComponentItems(void)
{
}


GR_GOComponentManager::GR_GOComponentManager(GR_Graphics* pG)
  : GR_EmbedManager(pG), 
    m_CurrentUID(-1),
    m_pDoc(nullptr)
{
  m_vecGOComponentView.clear();
  m_vecItems.clear();
}

GR_GOComponentManager::~GR_GOComponentManager()
{ 
     UT_VECTOR_PURGEALL(GR_AbiGOComponentItems *,m_vecItems);
     UT_VECTOR_SPARSEPURGEALL(GOComponentView *,m_vecGOComponentView);
}

GR_EmbedManager * GR_GOComponentManager::create(GR_Graphics * pG)
{
  return static_cast<GR_EmbedManager *>(new GR_GOComponentManager(pG));
}

const char * GR_GOComponentManager::getObjectType(void) const
{
  return "GOComponent";
}

void GR_GOComponentManager::initialize(void)
{
}

/*!
 * Create a specific GOComponent view and associate it with a UID.
 */ 
UT_sint32  GR_GOComponentManager::_makeGOComponentView(void)
{
     GOComponentView * pGOComponentView = new GOComponentView(this);
     m_vecGOComponentView.addItem(pGOComponentView);
     return m_vecGOComponentView.getItemCount()-1;
}

void GR_GOComponentManager::setDefaultFontSize(UT_sint32 uid, UT_sint32 iSize)
{
  GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
  UT_return_if_fail(pGOComponentView);
  pGOComponentView->setDefaultFontSize(iSize);
}

UT_sint32 GR_GOComponentManager::makeEmbedView(AD_Document * pDoc, UT_uint32 api, G_GNUC_UNUSED const char * szDataID)
{
  if(m_pDoc == nullptr)
  {
    m_pDoc = static_cast<PD_Document *>(pDoc);
  }
  else
  {
    UT_ASSERT(m_pDoc == static_cast<PD_Document *>(pDoc));
  }
  UT_sint32 iNew = _makeGOComponentView();
  GR_AbiGOComponentItems * pItem = new GR_AbiGOComponentItems();
  pItem->m_iAPI = api;
  pItem->m_bHasSnapshot = false;
  m_vecItems.addItem(pItem);
  UT_ASSERT(m_vecItems.getItemCount() == (iNew+1));
  return iNew;
}

void GR_GOComponentManager::makeSnapShot(UT_sint32 uid, G_GNUC_UNUSED UT_Rect & rec)
{
	GR_AbiGOComponentItems * pItem = m_vecItems.getNthItem(uid);
	UT_return_if_fail(pItem);  
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	const PP_AttrProp * pSpanAP = nullptr;
	PT_AttrPropIndex api = pItem->m_iAPI;
	/* bool b = */ m_pDoc->getAttrProp(api, &pSpanAP);
	const char * pszDataID = nullptr;
	pSpanAP->getAttribute("dataid", pszDataID);
	UT_ConstByteBufPtr pBuf;
	std::string mime_type;
	if ((pBuf = pGOComponentView->getSnapShot (mime_type)))
	  {
		UT_UTF8String sID = (mime_type == "image/svg")? "snapshot-svg-":  "snapshot-png-";
		sID += pszDataID;
		if(pItem->m_bHasSnapshot)
		  {
			m_pDoc->replaceDataItem(sID.utf8_str(), pBuf);
		  }
		else
		  {
			m_pDoc->createDataItem(sID.utf8_str(), false, pBuf, mime_type, nullptr);
			pItem->m_bHasSnapshot = true;
		  }
	  }
}

bool GR_GOComponentManager::isDefault(void)
{
  return false;
}

bool GR_GOComponentManager::modify(UT_sint32 uid)
{
  GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
  pGOComponentView->modify();
  return false;
}

void GR_GOComponentManager::initializeEmbedView(G_GNUC_UNUSED UT_sint32 uid)
{
  // FIXME write code for this.
}

void GR_GOComponentManager::loadEmbedData(G_GNUC_UNUSED UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	UT_return_if_fail(pGOComponentView);
	const PP_AttrProp * pSpanAP = nullptr;
	GR_AbiGOComponentItems * pItem = m_vecItems.getNthItem(uid);
	UT_return_if_fail(pItem);  
	PT_AttrPropIndex api = pItem->m_iAPI;
	/* bool b = */ m_pDoc->getAttrProp(api, &pSpanAP);
	const char * pszDataID = nullptr;
	bool bFoundDataID = pSpanAP->getAttribute("dataid", pszDataID);
	std::string mime_type;

	if (bFoundDataID && pszDataID)
	{
		UT_ConstByteBufPtr pByteBuf;
		bFoundDataID = m_pDoc->getDataItemDataByName(pszDataID,
							pByteBuf,
							&mime_type, nullptr);
		UT_return_if_fail(bFoundDataID);
		UT_return_if_fail(pszDataID);
		UT_DEBUGMSG(("GO Component string is... \n %s \n", pByteBuf->getPointer(0)));
		pGOComponentView->loadBuffer(pByteBuf, mime_type.c_str());
	}
}

UT_sint32 GR_GOComponentManager::getWidth(UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	double dim;
	g_object_get (G_OBJECT (pGOComponentView->getComponent ()), "width", &dim, nullptr);
	return pGOComponentView->width =  (UT_sint32) rint (dim * UT_LAYOUT_RESOLUTION);
}


UT_sint32 GR_GOComponentManager::getAscent(UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	double dim;
	g_object_get (G_OBJECT (pGOComponentView->getComponent ()), "ascent", &dim, nullptr);
	return pGOComponentView->ascent = (UT_sint32) rint (dim* UT_LAYOUT_RESOLUTION);
}


UT_sint32 GR_GOComponentManager::getDescent(UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	double dim;
	g_object_get (G_OBJECT (pGOComponentView->getComponent ()), "descent", &dim, nullptr);
	return pGOComponentView->descent = (UT_sint32) rint (dim * UT_LAYOUT_RESOLUTION);
}

void GR_GOComponentManager::setColor(G_GNUC_UNUSED UT_sint32 uid, G_GNUC_UNUSED UT_RGBColor c)
{
  // FIXME write code
}

/*!
 * This method renders the GOComponent identified by uid onto the screen
 * rectangle given by rec
 */
void GR_GOComponentManager::render(UT_sint32 uid, UT_Rect & rec)
{
  GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
  UT_return_if_fail(pGOComponentView);
	if (pGOComponentView->getComponent () != nullptr)
		pGOComponentView->render(rec);
	else
		GR_EmbedManager::render (uid, rec);
}

void GR_GOComponentManager::releaseEmbedView(UT_sint32 uid)
{
  GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
  delete pGOComponentView;
  m_vecGOComponentView.setNthItem(uid,nullptr,nullptr); //nullptr it out so we don't affect the other uid's
}

bool GR_GOComponentManager::convert(G_GNUC_UNUSED UT_uint32 iConType, G_GNUC_UNUSED const UT_ConstByteBufPtr & From, G_GNUC_UNUSED const UT_ByteBufPtr & To)
{
  return false;
}

bool GR_GOComponentManager::isEdittable(UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	UT_nonnull_or_return(pGOComponentView, false);
	return pGOComponentView->IsEdittable ();
}

bool GR_GOComponentManager::isResizeable(UT_sint32 uid)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	UT_nonnull_or_return(pGOComponentView, false);
	return pGOComponentView->IsResizable ();
}

void GR_GOComponentManager::setRun(UT_sint32 uid, fp_Run *pRun)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	UT_nonnull_or_return(pGOComponentView, );
	pGOComponentView->SetRun (pRun);
}

bool GR_GOComponentManager::setFont(UT_sint32 uid, const GR_Font * pFont)
{
	GOComponentView * pGOComponentView = m_vecGOComponentView.getNthItem(uid);
	UT_nonnull_or_return(pGOComponentView, false);
	return pGOComponentView->setFont (pFont);
}

void GR_GOComponentManager:: updateData(UT_sint32 uid, UT_sint32 api)
{
	GR_AbiGOComponentItems * pItem = m_vecItems.getNthItem(uid);
	UT_return_if_fail(pItem);  
	pItem->m_iAPI = api;
}

GOComponentView::GOComponentView(GR_GOComponentManager * pGOMan): m_pGOMan(pGOMan)
{
	m_Image = nullptr;
	width = ascent = descent = 0;
	pix_width = pix_height = 0;
	pixbuf = nullptr;
	m_pRun = nullptr;
	component = nullptr;
}

GOComponentView::~GOComponentView(void)
{
	if (m_Image)
		delete m_Image;
	if (component)
		g_object_unref (component);
}

void GOComponentView::render(UT_Rect & rec)
{
	UT_return_if_fail (component);
	if (rec.width == 0 || rec.height == 0) // Nothing to render
		return;
	GR_CairoGraphics *pUGG = static_cast<GR_CairoGraphics*>(m_pGOMan->getGraphics());
	UT_sint32 myWidth = pUGG->tdu(rec.width);
	UT_sint32 myHeight = pUGG->tdu(rec.height);
	UT_sint32 x = pUGG->tdu(rec.left), y;
	if ((width != rec.width || ascent + descent != rec.height) && go_component_is_resizable (component))
	{
		double _ascent, _descent;
		go_component_set_size (component, (double) rec.width / UT_LAYOUT_RESOLUTION, (double) rec.height / UT_LAYOUT_RESOLUTION);
		g_object_get (G_OBJECT (component), "ascent", &_ascent, "descent", &_descent, nullptr);
		ascent =  (UT_sint32) rint (_ascent * UT_LAYOUT_RESOLUTION);
		descent =  (UT_sint32) rint (_descent * UT_LAYOUT_RESOLUTION);
	}
	y = pUGG->tdu(rec.top - ascent);
	pUGG->beginPaint();
	cairo_t *cr = pUGG->getCairo ();
	cairo_save (cr);
	cairo_translate (cr, x, y);
	go_component_render (component, cr, myWidth, myHeight);
	cairo_new_path (cr); // just in case a path has not been ended
	cairo_restore (cr);
	pUGG->endPaint();
}

static void
changed_cb (GOComponent *component, gpointer data)
{
	if (data != nullptr)
	{
		GOComponentView * pGOComponentView = static_cast<GOComponentView*> (data);
		pGOComponentView->update ();
	}
	else
	{
		XAP_Frame *pFrame = XAP_App::getApp()->getLastFocussedFrame();
		FV_View* pView = static_cast<FV_View*>(pFrame->getCurrentView());
		UT_Byte *buf;
		int length;
		void (*clearfunc) (gpointer);
		gpointer user_data = nullptr;
		if (go_component_get_data (component, (void**) &buf, &length, &clearfunc, &user_data)) {
			if (buf && length) {
				UT_ByteBufPtr myByteBuf(new UT_ByteBuf);
				myByteBuf->append(buf, length);
				UT_String Props="embed-type: GOComponent";
				guint i, nbprops;
				GType    prop_type;
				GValue	 value = G_VALUE_INIT;
				char *prop = nullptr;
				GParamSpec **specs = g_object_class_list_properties (
							G_OBJECT_GET_CLASS (component), &nbprops);
				for (i = 0; i < nbprops; i++) {
					if (specs[i]->flags & GO_PARAM_PERSISTENT) {
						prop_type = G_PARAM_SPEC_VALUE_TYPE (specs[i]);
						memset(&value, 0, sizeof(value));
						g_value_init (&value, prop_type);
						g_object_get_property  (G_OBJECT (component), specs[i]->name, &value);
						if (!g_param_value_defaults (specs[i], &value))
							switch (G_TYPE_FUNDAMENTAL (prop_type)) {
							case G_TYPE_CHAR:
							case G_TYPE_UCHAR:
							case G_TYPE_BOOLEAN:
							case G_TYPE_INT:
							case G_TYPE_UINT:
							case G_TYPE_LONG:
							case G_TYPE_ULONG:
							case G_TYPE_FLOAT:
							case G_TYPE_DOUBLE: {
								GValue str;
								memset(&str, 0, sizeof(str));
								g_value_init (&str, G_TYPE_STRING);
								g_value_transform (&value, &str);
								prop = g_strdup (g_value_get_string (&str));
								g_value_unset (&str);
								break;
							}
						
							case G_TYPE_STRING: {
								prop = g_strdup (g_value_get_string (&value));
								break;
							}
		
							default:
								prop = nullptr;
								break;
						}
						g_value_unset (&value);
						if (prop) {
							Props += UT_String_sprintf("; %s:%s", specs[i]->name, prop);
							g_free (prop);
							prop = nullptr;
						}
					}
				}
				PT_DocPosition pos = pView->getPoint();
				pView->cmdInsertEmbed(myByteBuf, pView->getPoint(), component->mime_type, Props.c_str ());
				pView->cmdSelect(pos,pos+1);
			}
			if (clearfunc)
				clearfunc ((user_data)? user_data: buf);
		}
		g_object_unref (component); // destroy since we created a new one this will close the editor
	}
}

void GOComponentView::loadBuffer(const UT_ConstByteBufPtr & sGOComponentData, const char *_mime_type)
{
	if (!component) {
		mime_type = _mime_type;
		component = go_component_new_by_mime_type (_mime_type);
	}
	UT_return_if_fail (component);
	go_component_set_inline (component, true);
	go_component_set_use_font_from_app (component, true);
	g_signal_connect (G_OBJECT (component), "changed",
								G_CALLBACK (changed_cb), this);
	if (component == nullptr) {
		// we should do something intelligent in that case
		return;
	}
	go_component_set_default_size (component, 2.5, 2.5, 0.);
	if (sGOComponentData->getLength () > 0) {
		if (m_pRun) {
			PP_AttrProp const *Props = m_pRun->getSpanAP ();
			GParamSpec *prop_spec;
			int i = 0;
			GValue res = G_VALUE_INIT;
			gchar const *szName, *szValue;
			while (Props->getNthProperty (i++, szName, szValue)) {
				prop_spec = g_object_class_find_property (
						G_OBJECT_GET_CLASS (component), szName);
				if (prop_spec && (prop_spec->flags & GO_PARAM_PERSISTENT) &&
					gsf_xml_gvalue_from_str (&res,
						G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (prop_spec)),
						szValue)) {
					g_object_set_property (G_OBJECT (component), szName, &res);
					g_value_unset (&res);
				}
			}
		}
		go_component_set_data (component,
				(char const*) sGOComponentData->getPointer (0),
				(int) sGOComponentData->getLength ());
	} else 
		go_component_edit (component);
	width = 0; // force pixbuf update
	// update ascent and descent now, otherwise it will not be updated when loading
	double _ascent, _descent;
	g_object_get (G_OBJECT (component), "ascent", &_ascent, "descent", &_descent, nullptr);
	ascent =  (UT_sint32) rint (_ascent * UT_LAYOUT_RESOLUTION);
	descent =  (UT_sint32) rint (_descent * UT_LAYOUT_RESOLUTION);
}

void GOComponentView::setDefaultFontSize(UT_sint32 /*iSize*/)
{
}

bool GOComponentView::setFont(const GR_Font * pFont)
{
	UT_return_val_if_fail(component && pFont, false);
	const GR_PangoFont *pPF = dynamic_cast<const GR_PangoFont *>(pFont);
	UT_return_val_if_fail(pPF, false);
	return go_component_set_font(component, pPF->getPangoDescription());
}

void GOComponentView::modify()
{
	UT_return_if_fail (component);
	go_component_edit (component);
}

void GOComponentView::update ()
{
	UT_return_if_fail (component);
	UT_Byte *buf;
	int length;
	void (*clearfunc) (gpointer);
	gpointer user_data = nullptr;
	FV_View *pView = m_pRun->getBlock ()->getView ();
	if (go_component_get_data (component, (void**) &buf, &length, &clearfunc, &user_data)) {
		if (buf && length) {
		UT_ByteBufPtr myByteBuf(new UT_ByteBuf);
		myByteBuf->append(buf, length);
		mime_type = component->mime_type;
		UT_String Props="embed-type: GOComponent";
		guint i, nbprops;
		GType    prop_type;
		GValue	 value = G_VALUE_INIT;
		char *prop = nullptr;
		GParamSpec **specs = g_object_class_list_properties (
					G_OBJECT_GET_CLASS (component), &nbprops);
		for (i = 0; i < nbprops; i++) {
			if (specs[i]->flags & GO_PARAM_PERSISTENT) {
				prop_type = G_PARAM_SPEC_VALUE_TYPE (specs[i]);
				g_value_init (&value, prop_type);
				g_object_get_property  (G_OBJECT (component), specs[i]->name, &value);
				if (!g_param_value_defaults (specs[i], &value))
					switch (G_TYPE_FUNDAMENTAL (prop_type)) {
					case G_TYPE_CHAR:
					case G_TYPE_UCHAR:
					case G_TYPE_BOOLEAN:
					case G_TYPE_INT:
					case G_TYPE_UINT:
					case G_TYPE_LONG:
					case G_TYPE_ULONG:
					case G_TYPE_FLOAT:
					case G_TYPE_DOUBLE: {
						GValue str = G_VALUE_INIT;
						g_value_init (&str, G_TYPE_STRING);
						g_value_transform (&value, &str);
						prop = g_strdup (g_value_get_string (&str));
						g_value_unset (&str);
						break;
					}
				
					case G_TYPE_STRING: {
						prop = g_strdup (g_value_get_string (&value));
						break;
					}

					default:
						prop = nullptr;
						break;
				}
				g_value_unset (&value);
				if (prop) {
					Props += UT_String_sprintf("; %s:%s", specs[i]->name, prop);
					g_free (prop);
					prop = nullptr;
				}
			}
		}
		pView->cmdUpdateEmbed(m_pRun, myByteBuf, mime_type.c_str(), Props.c_str());
		} else
			pView->cmdDeleteEmbed(m_pRun);
		if (clearfunc)
			clearfunc ((user_data)? user_data: buf);
	}
}

UT_ConstByteBufPtr GOComponentView::getSnapShot(std::string &snap_mime_type)
{
	UT_return_val_if_fail (component, nullptr);
	int height = ascent + descent;
	if (height == 0 || (int) width == 0)
		return nullptr;
	size_t length;
	GOSnapshotType type;
	const UT_Byte *buf = reinterpret_cast <const UT_Byte*> (go_component_get_snapshot (component, &type, &length));
	if (!buf || !length)
		return nullptr;
	switch (type) {
		case GO_SNAPSHOT_PNG:
			snap_mime_type = "image/png";
			break;
		case GO_SNAPSHOT_SVG:
			snap_mime_type = "image/svg";
			break;
		default:
			return nullptr;
	}
	UT_ByteBufPtr pBuf(new UT_ByteBuf);
	pBuf->append (buf, length);
	return pBuf;
}
