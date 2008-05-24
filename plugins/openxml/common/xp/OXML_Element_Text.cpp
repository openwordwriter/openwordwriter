/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource
 * 
 * Copyright (C) 2007 Philippe Milot <PhilMilot@gmail.com>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

// Class definition include
#include <OXML_Element_Text.h>

// AbiWord includes
#include <ut_types.h>
#include <ut_string.h>
#include <pd_Document.h>
#include <ut_exception.h>

OXML_Element_Text::OXML_Element_Text() : 
	OXML_Element("", T_TAG, SPAN), 
	m_pString(NULL), 
	m_range(UNKNOWN_RANGE)
{
}

OXML_Element_Text::~OXML_Element_Text()
{
	DELETEP(m_pString);
}

OXML_Element_Text::OXML_Element_Text(const gchar * text, int length) :
	OXML_Element("", T_TAG, SPAN)
{
	setText(text, length);
}

void OXML_Element_Text::setText(const gchar * text, int length)
{
	UT_TRY {
		m_pString = new UT_UCS4String(text, length);
	} UT_CATCH (UT_CATCH_ANY) {
		m_pString = NULL;
	} UT_END_CATCH
}

const UT_UCS4Char * OXML_Element_Text::getText_UCS4String()
{
	UT_return_val_if_fail(m_pString != NULL, NULL);
	return m_pString->ucs4_str();
}

UT_Error OXML_Element_Text::serialize(std::string path)
{
	//TODO whenever we're ready to write the export filter
	return OXML_Element::serialize(path);
}

UT_Error OXML_Element_Text::addToPT(PD_Document * pDocument)
{
	UT_return_val_if_fail(pDocument != NULL && m_pString != NULL, UT_ERROR);

	//a text tag does not have children, so no need to call addChildrenToPT()
	return pDocument->appendSpan(m_pString->ucs4_str(), m_pString->length()) ? UT_OK : UT_ERROR; 
}
