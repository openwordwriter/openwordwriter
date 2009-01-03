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

// Class definition include
#include <OXML_Image.h>

// Internal includes
#include <OXML_Types.h>
#include <OXML_Document.h>

// AbiWord includes
#include <ut_types.h>
#include <ut_misc.h>
#include <pd_Document.h>

// External includes
#include <string>

OXML_Image::OXML_Image() : 
	OXML_ObjectWithAttrProp(),
	id(NULL),
	mimeType(NULL),
	data(NULL)
{

}

OXML_Image::~OXML_Image()
{
}

void OXML_Image::setId(const char* imageId)
{
	id = imageId;
}

void OXML_Image::setMimeType(const char* imageMimeType)
{
	mimeType = imageMimeType;
}

void OXML_Image::setData(const UT_ByteBuf* imageData)
{
	data = imageData;
}

const char* OXML_Image::getId()
{
	return id;	
}

UT_Error OXML_Image::serialize(IE_Exp_OpenXML* exporter)
{
	std::string filename(id);
	filename += ".png";
	return exporter->writeImage(filename.c_str(), data);
}

UT_Error OXML_Image::addToPT(PD_Document * /*pDocument*/)
{
	//TODO
	return UT_OK;
}
