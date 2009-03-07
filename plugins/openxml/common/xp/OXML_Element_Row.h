/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource
 * 
 * Copyright (C) 2008 Firat Kiyak <firatkiyak@gmail.com>
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

#ifndef _OXML_ELEMENT_ROW_H_
#define _OXML_ELEMENT_ROW_H_

// Internal includes
#include <OXML_Element.h>
#include <OXML_Element_Table.h>
#include <ie_exp_OpenXML.h>

// AbiWord includes
#include <ut_types.h>
#include <ut_string.h>
#include <pd_Document.h>

class OXML_Element_Table;

class OXML_Element_Row : public OXML_Element
{
public:
	OXML_Element_Row(std::string id, OXML_Element_Table* table);
	virtual ~OXML_Element_Row();

	virtual UT_Error serialize(IE_Exp_OpenXML* exporter);
	virtual UT_Error addToPT(PD_Document * pDocument);
	virtual void setNumCols(UT_sint32 numCols);

protected:
	UT_Error serializeChildren(IE_Exp_OpenXML* exporter);

private:
	virtual UT_Error serializeProperties(IE_Exp_OpenXML* exporter);
	UT_sint32 numCols;
	OXML_Element_Table* table;
};

#endif //_OXML_ELEMENT_ROW_H_
