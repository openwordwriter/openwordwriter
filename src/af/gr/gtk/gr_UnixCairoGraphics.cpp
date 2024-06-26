/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*- */
/* AbiWord
 * Copyright (C) 2004-2006 Tomas Frydrych <dr.tomas@yahoo.co.uk>
 * Copyright (C) 2009-2016 Hubert Figuiere
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

#include <gtk/gtk.h>
#include "ut_bytebuf.h"

#include "gr_UnixCairoGraphics.h"
#include "gr_CairoImage.h"
#include "gr_Painter.h"
#include "gr_UnixImage.h"

#include "xap_App.h"
#include "xap_EncodingManager.h"
#include "xap_GtkStyle.h"

GR_UnixCairoGraphicsBase::~GR_UnixCairoGraphicsBase()
{
}

/*!
 * Create a new image from the Raster rgba byte buffer defined by pBB.
 * The dimensions of iWidth and iHeight are in logical units but the image
 * doesn't scale if the resolution or zoom changes. Instead you must create
 * a new image.
 */
GR_Image* GR_UnixCairoGraphicsBase::createNewImage (const char* pszName,
													const UT_ConstByteBufPtr & pBB,
                                                    const std::string& mimetype,
													UT_sint32 iWidth,
													UT_sint32 iHeight,
													GR_Image::GRType iType)
{
	GR_Image* pImg = nullptr;

	if (iType == GR_Image::GRT_Raster) {
		pImg = new GR_UnixImage(pszName);
		pImg->convertFromBuffer(pBB, mimetype, tdu(iWidth), tdu(iHeight));
	} else if (iType == GR_Image::GRT_Vector) {
		pImg = new GR_RSVGVectorImage(pszName);
		pImg->convertFromBuffer(pBB, mimetype, tdu(iWidth), tdu(iHeight));
	} else {
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

   	return pImg;
}

GR_UnixCairoGraphicsBase::GR_UnixCairoGraphicsBase()
	: GR_CairoGraphics()
{
}

GR_UnixCairoGraphicsBase::GR_UnixCairoGraphicsBase(cairo_t *cr, UT_uint32 iDeviceResolution)
	: GR_CairoGraphics(cr, iDeviceResolution)
{
}

GR_UnixCairoGraphics::GR_UnixCairoGraphics(GtkWidget * win)
	: GR_UnixCairoGraphicsBase(),
	  m_pWin(win ? gtk_widget_get_window(win) : nullptr),
	  m_context(nullptr),
	  m_CairoCreated(false),
	  m_Painting(false),
	  m_Signal(0),
 	  m_DestroySignal(0),
	  m_Widget(win),
	  m_styleBg(nullptr),
	  m_styleHighlight(nullptr)
{
	m_cr = nullptr;
	if (m_Widget) {
		_initWidget();
	}
	if (_getWindow())
	{
		// Set GraphicsExposes so that XCopyArea() causes an expose on
		// obscured regions rather than just tiling in the default background.
		// TODO: is this still needed with cairo, and if yes can it be emulated
		// without having m_pGC any more?
		// gdk_gc_set_exposures(m_pGC, 1);
		setCursor(GR_CURSOR_DEFAULT);	
	}
}

GR_UnixCairoGraphics::~GR_UnixCairoGraphics()
{
	if (m_Widget) {
		g_signal_handler_disconnect (m_Widget, m_Signal);
		g_signal_handler_disconnect (m_Widget, m_DestroySignal);
	}
	if (m_styleBg) {
		g_object_unref(m_styleBg);
	}
	if (m_styleHighlight) {
		g_object_unref(m_styleHighlight);
	}
}


GR_Graphics *   GR_UnixCairoGraphics::graphicsAllocator(GR_AllocInfo& info)
{
	UT_return_val_if_fail(info.getType() == GRID_UNIX, nullptr);
	xxx_UT_DEBUGMSG(("GR_CairoGraphics::graphicsAllocator\n"));

//	UT_return_val_if_fail(!info.isPrinterGraphics(), nullptr);
	GR_UnixCairoAllocInfo &AI = (GR_UnixCairoAllocInfo&)info;

	return new GR_UnixCairoGraphics(AI.m_win);
}

inline UT_RGBColor _convertGdkRGBA(const GdkRGBA &c)
{
	UT_RGBColor color;
	color.m_red = c.red * 255;
	color.m_grn = c.green * 255;
	color.m_blu = c.blue * 255;
	return color;
}

void GR_UnixCairoGraphics::widget_size_allocate(GtkWidget* /*widget*/, GtkAllocation* /*allocation*/, GR_UnixCairoGraphics* me)
{
	UT_return_if_fail(me);
	me->m_clipRectDirty = TRUE;
}

void GR_UnixCairoGraphics::widget_destroy(GtkWidget* widget, GR_UnixCairoGraphics* me)
{
	UT_return_if_fail(me && me->m_Widget == widget);
	me->m_Widget = nullptr;
	me->m_Signal = 0;
	me->m_DestroySignal = 0;
}

void GR_UnixCairoGraphics::_initWidget()
{
	UT_return_if_fail(m_Widget);
	m_Signal = g_signal_connect_after(G_OBJECT(m_Widget), "size_allocate", G_CALLBACK(widget_size_allocate), this);
	m_DestroySignal = g_signal_connect(G_OBJECT(m_Widget), "destroy", G_CALLBACK(widget_destroy), this);
}

#define COLOR_MIX 0.67   //COLOR_MIX should be between 0 and 1
#define SQUARE(A) (A)*(A)
void GR_UnixCairoGraphics::init3dColors(GtkWidget* /*w*/)
{
	if (m_styleBg) {
		g_object_unref(m_styleBg);
	}
	g_type_ensure(GTK_TYPE_BUTTON);
	m_styleBg = XAP_GtkStyle_get_style(nullptr, "GtkButton"); // "button"
	// guess colours
	// WHITE
	GdkRGBA rgba2;
	rgba2.red = 1.;
	rgba2.green = 1.;
	rgba2.blue = 1.;
	rgba2.alpha = 1;
	// this is the colour used notably for the the ruler.
	// Gray-ish in adwaita, black in Dark theme
	m_3dColors[CLR3D_Background] = _convertGdkRGBA(rgba2);

	// this is white in Adwaita, and black in a dark theme.
	GdkRGBA rgba1;
	if (m_styleHighlight) {
		g_object_unref(m_styleHighlight);
	}
	g_type_ensure(GTK_TYPE_TREE_VIEW);
	m_styleHighlight = XAP_GtkStyle_get_style(nullptr, "GtkTreeView.view"); // "textview.view"
	gtk_style_context_get_color (m_styleHighlight, GTK_STATE_FLAG_NORMAL, &rgba1);
	m_3dColors[CLR3D_Highlight] = _convertGdkRGBA(rgba1);

	// guess colours.
	// BLACK
	rgba1.red = 0.;
	rgba1.green = 0.;
	rgba1.blue = 0.;
	rgba1.alpha = 1;

	GdkRGBA rgba_;
	rgba_.alpha = 1.;   // we don't really care, abiword does not use transparency
	rgba_.red = rgba1.red*COLOR_MIX + rgba2.red*(1.-COLOR_MIX);
	rgba_.green = rgba1.green*COLOR_MIX + rgba2.green*(1.-COLOR_MIX);
	rgba_.blue = rgba1.blue*COLOR_MIX + rgba2.blue*(1.-COLOR_MIX);
	m_3dColors[CLR3D_BevelUp]    = _convertGdkRGBA(rgba_);

	rgba_.red = rgba1.red*(1.-COLOR_MIX) + rgba2.red*COLOR_MIX;
	rgba_.green = rgba1.green*(1.-COLOR_MIX) + rgba2.green*COLOR_MIX;
	rgba_.blue = rgba1.blue*(1.-COLOR_MIX) + rgba2.blue*COLOR_MIX;
	m_3dColors[CLR3D_BevelDown]  = _convertGdkRGBA(rgba_);


	g_type_ensure(GTK_TYPE_LABEL);
	GtkStyleContext *text_style = XAP_GtkStyle_get_style(nullptr, "GtkLabel.view"); // "label.view"
	gtk_style_context_get_color (text_style, GTK_STATE_FLAG_NORMAL, &rgba2);
	m_3dColors[CLR3D_Foreground]	= _convertGdkRGBA(rgba2);
	g_object_unref(text_style);

	m_bHave3DColors = true;
}
#undef COLOR_MIX
#undef SQUARE

GR_Font * GR_UnixCairoGraphics::getGUIFont(void)
{
	if (!m_pPFontGUI)
	{
		PangoContext* context = gtk_widget_get_pango_context(m_Widget);
		const PangoFontDescription* fontDesc = pango_context_get_font_description(context);
		const char *guiFontName = pango_font_description_get_family(fontDesc);

		if (!guiFontName)
			guiFontName = "'Times New Roman'";

		std::string s = XAP_EncodingManager::get_instance()->getLanguageISOName();

		const char * pCountry
			= XAP_EncodingManager::get_instance()->getLanguageISOTerritory();

		if(pCountry)
		{
			s += "-";
			s += pCountry;
		}

		m_pPFontGUI = new GR_PangoFont(guiFontName, 11.0, this, s.c_str(), true);
		UT_ASSERT(m_pPFontGUI);
	}

	return m_pPFontGUI;
}

const char* GR_UnixCairoGraphics::_getCursor(GR_Graphics::Cursor c)
{
	const char* cursor_name;

	switch (c)
	{
	default:
		UT_ASSERT(UT_NOT_IMPLEMENTED);
		/*FALLTHRU*/
	case GR_CURSOR_DEFAULT:
		cursor_name = "default";
		break;

	case GR_CURSOR_IBEAM:
		cursor_name = "text";
		break;

	//I have changed the shape of the arrow so get a consistent
	//behaviour in the bidi build; I think the new arrow is better
	//for the purpose anyway

	case GR_CURSOR_RIGHTARROW:
		cursor_name = "default"; //XXX GDK_ARROW;
		break;

	case GR_CURSOR_LEFTARROW:
		cursor_name = "default"; //XXX GDK_LEFT_PTR;
		break;

	case GR_CURSOR_IMAGE:
		cursor_name = "move";
		break;

	case GR_CURSOR_IMAGESIZE_NW:
		cursor_name = "nw-resize";
		break;

	case GR_CURSOR_IMAGESIZE_N:
		cursor_name = "n-resize";
		break;

	case GR_CURSOR_IMAGESIZE_NE:
		cursor_name = "ne-resize";
		break;

	case GR_CURSOR_IMAGESIZE_E:
		cursor_name = "e-resize";
		break;

	case GR_CURSOR_IMAGESIZE_SE:
		cursor_name = "se-resize";
		break;

	case GR_CURSOR_IMAGESIZE_S:
		cursor_name = "s-resize";
		break;

	case GR_CURSOR_IMAGESIZE_SW:
		cursor_name = "sw-resize";
		break;

	case GR_CURSOR_IMAGESIZE_W:
		cursor_name = "w-resize";
		break;

	case GR_CURSOR_LEFTRIGHT:
		cursor_name = "col-resize";
		break;

	case GR_CURSOR_UPDOWN:
		cursor_name = "row-resize";
		break;

	case GR_CURSOR_EXCHANGE:
		cursor_name = "default"; // XXX no equivalent
		break;

	case GR_CURSOR_GRAB:
		cursor_name = "grab";
		break;

	case GR_CURSOR_LINK:
		cursor_name = "alias";
		break;

	case GR_CURSOR_WAIT:
		cursor_name = "wait";
		break;

	case GR_CURSOR_HLINE_DRAG:
		cursor_name = "col-resize";
		break;

	case GR_CURSOR_VLINE_DRAG:
		cursor_name = "row-resize";
		break;

	case GR_CURSOR_CROSSHAIR:
		cursor_name = "crosshair";
		break;

	case GR_CURSOR_DOWNARROW:
		cursor_name = "s-resize"; // not sure
		break;

	case GR_CURSOR_DRAGTEXT:
		cursor_name = "grabbing";
		break;

	case GR_CURSOR_COPYTEXT:
		cursor_name = "copy";
		break;
	}

	return cursor_name;
}

void GR_UnixCairoGraphics::setCursor(GR_Graphics::Cursor c)
{
	if (m_cursor == c)
		return;
	const char* cursor_name = _getCursor(c);
	m_cursor = c;
	xxx_UT_DEBUGMSG(("cursor set to %d	gdk %s \n", c, cursor_name));
	GdkCursor * cursor = gdk_cursor_new_from_name(
		gdk_window_get_display(m_pWin), cursor_name);
	gdk_window_set_cursor(m_pWin, cursor);
	g_object_unref(cursor);
}


void GR_UnixCairoGraphics::scroll(UT_sint32 dx, UT_sint32 dy)
{
	UT_sint32 oldDY = tdu(getPrevYOffset());
	UT_sint32 oldDX = tdu(getPrevXOffset());
	
	UT_sint32 newY = getPrevYOffset() + dy;
	UT_sint32 newX = getPrevXOffset() + dx;
	UT_sint32 ddx = -(tdu(newX) - oldDX);
	UT_sint32 ddy = -(tdu(newY) - oldDY);
	setPrevYOffset(newY);
	setPrevXOffset(newX);
	if(ddx == 0 && ddy == 0)
	{
		return;
	}

	disableAllCarets();

	UT_sint32 iddy = labs(ddy);
	bool bEnableSmooth = XAP_App::getApp()->isSmoothScrollingEnabled();
	bEnableSmooth = bEnableSmooth && (iddy < 30) && (ddx == 0);
	if(bEnableSmooth)
	{
		if(ddy < 0)
		{
			UT_sint32 i = 0;
			for(i = 0; i< iddy; i++)
			{
				gdk_window_scroll(m_pWin,0,-1);
			}
		}
		else
		{
			UT_sint32 i = 0;
			for(i = 0; i< iddy; i++)
			{
				gdk_window_scroll(m_pWin,0,1);
			}
		}
	}
	else
	{
		gdk_window_scroll(m_pWin,ddx,ddy);
	}
	enableAllCarets();
}

void GR_UnixCairoGraphics::scroll(UT_sint32 x_dest, UT_sint32 y_dest,
						  UT_sint32 x_src, UT_sint32 y_src,
						  G_GNUC_UNUSED UT_sint32 width, G_GNUC_UNUSED UT_sint32 height)
{
	scroll(x_src - x_dest, y_src - y_dest);
}

void GR_UnixCairoGraphics::_resetClip(void)
{
	
	cairo_reset_clip (m_cr);
	xxx_UT_DEBUGMSG(("Reset clip in gtk cairo \n"));
}

/*!
 * Take a screenshot of the graphics and convert it to an image.
 */
GR_Image * GR_UnixCairoGraphics::genImageFromRectangle(const UT_Rect &rec)
{
	UT_sint32 idx = _tduX(rec.left);
	UT_sint32 idy = _tduY(rec.top);
	UT_sint32 idw = _tduR(rec.width);
	UT_sint32 idh = _tduR(rec.height);
	UT_return_val_if_fail (idw > 0 && idh > 0 && idx >= 0, nullptr);
	cairo_surface_flush ( cairo_get_target(m_cr));
	GdkPixbuf * pix = gdk_pixbuf_get_from_window(getWindow(),
	                                             idx, idy,
	                                             idw, idh);
	UT_return_val_if_fail(pix, nullptr);

	GR_UnixImage * pImg = new GR_UnixImage("ScreenShot");
	pImg->setData(pix);
	pImg->setDisplaySize(idw,idh);
	return pImg;
}

void GR_UnixCairoGraphics::_beginPaint()
{
	UT_ASSERT(m_Painting == false);
	GR_CairoGraphics::_beginPaint();

	if (m_cr == nullptr)
	{
		UT_ASSERT(m_pWin);
		auto region = cairo_region_create();
		m_context = gdk_window_begin_draw_frame(m_pWin, region);
		cairo_region_destroy(region);
		m_cr = gdk_drawing_context_get_cairo_context(m_context);
		m_CairoCreated = true;
	}

	UT_ASSERT(m_cr);
	m_Painting = true;
	_initCairo();
}

void GR_UnixCairoGraphics::_endPaint()
{
	if (m_CairoCreated)
	{
		gdk_window_end_draw_frame(m_pWin, m_context);
	}
	m_context = nullptr;
	m_cr = nullptr;

	m_Painting = false;
	m_CairoCreated = false;

	GR_CairoGraphics::_endPaint();
}

void GR_UnixCairoGraphics::queueDraw(const UT_Rect* clip)
{
	UT_ASSERT(m_Widget);

	if (!clip) {
		gtk_widget_queue_draw(m_Widget);
	} else {
		gtk_widget_queue_draw_area(
				m_Widget,
				clip->left,
				clip->top,
				clip->width,
				clip->height
			);
	}
}

void GR_UnixCairoGraphics::flush(void)
{

	if (m_Widget) {
		gtk_widget_queue_draw(m_Widget);
	}
	
/*
	if(m_cr)
	{
		cairo_surface_flush(cairo_get_target(m_cr));
	}
*/
}

bool GR_UnixCairoGraphics::queryProperties(GR_Graphics::Properties gp) const
{
	switch (gp)
	{
		case DGP_SCREEN:
		case DGP_OPAQUEOVERLAY:
			return m_pWin != nullptr;
		case DGP_PAPER:
			return false;
		default:
			UT_ASSERT(0);
			return false;
	}
}

bool GR_UnixCairoGraphics::getColor3D(GR_Color3D name, UT_RGBColor &color)
{
	switch(name) {
	case GR_Graphics::CLR3D_Background:
	case GR_Graphics::CLR3D_Highlight:
		return false;

	default:
		return GR_CairoGraphics::getColor3D(name, color);
	}
}

/**
 * This is an override that will use the GtkStyle code to actually render
 * directly.
 */
void GR_UnixCairoGraphics::fillRect(GR_Color3D c, UT_sint32 x, UT_sint32 y,
                                    UT_sint32 w, UT_sint32 h)
{
	switch(c) {
	case GR_Graphics::CLR3D_Background:
	case GR_Graphics::CLR3D_Highlight:
	{
		if (m_cr == nullptr) {
			return;
		}
		_setProps();
		cairo_save (m_cr);

		GtkStyleContext *context = nullptr;
		switch(c) {
		case GR_Graphics::CLR3D_Background:
			context = m_styleBg;
			break;
		case GR_Graphics::CLR3D_Highlight:
			context = m_styleHighlight;
			break;
		default:
			UT_ASSERT(0);
			return;
		}
		gtk_render_background (context, m_cr, tdu(x), tdu(y), tdu(w), tdu(h));
		gtk_render_frame (context, m_cr, tdu(x), tdu(y), tdu(w), tdu(h));
		cairo_restore (m_cr);
		break;
	}
	default:
		GR_CairoGraphics::fillRect(c, x, y, w, h);
		return;
	}
}
