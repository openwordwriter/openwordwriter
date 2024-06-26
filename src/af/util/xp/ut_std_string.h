/* AbiSource Program Utilities
 * Copyright (C) 1998,1999 AbiSource, Inc.
 * Copyright (C) 2009-2023 Hubert Figuiere
 * Copyright (C) 2011 Ben Martin
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

#include <string>
#include <vector>
#include <locale>

#include "ut_types.h"


/** replacement for UT_UTF8String::escapeXML
 *  escapes '<', '>', '"' and '&' in the current string
 */
ABI_EXPORT std::string UT_escapeXML(const std::string &);
/** replacement for UT_UTF8String::escapeURL
 *  makes string conform to RFC 1738
 */
ABI_EXPORT std::string UT_escapeURL(const std::string &);

/** Unescape (decode) XML. Reverse of UT_escapeXML
 */
ABI_EXPORT std::string UT_decodeXML(const std::string &);

ABI_EXPORT std::string& UT_std_string_vprintf (std::string & inStr,
                                               const char *format,
                                               va_list      args1)
    ABI_PRINTF_FORMAT(2,0);

ABI_EXPORT std::string UT_std_string_sprintf(const char * inFormat, ...)
    ABI_PRINTF_FORMAT(1,2);


/**
 * Create a string form a Unicode stream. Will be UTF8 encoded.
 * @param unicode the Unicode UCS4 string. Can't be nullptr
 * @param len the length of the Unicode UCS4 string. Must match the length.
 * @return the std::string. Empty string if an invalid parameter or error.
 */
ABI_EXPORT std::string UT_std_string_unicode(const UT_UCS4Char * unicode,
                                             UT_uint32 len);
/**
 * Returns a std::string (UTF-8) converted from an encoding.
 * \param bytes the input bytes (not nullptr)
 * \param encoding the encoding (not nullptr)
 * \return an UTF-8 enconded std::string
 */
ABI_EXPORT std::string UT_std_stringFromEncoding(const char* bytes,
                                                 const char* encoding) ABI_NONNULL(1,2);

/**
 * true if fullstring starts with exactly prefix.
 */
ABI_EXPORT bool starts_with( const std::string& fullstring, const std::string& prefix );
ABI_EXPORT bool ends_with(   const std::string& fullstring, const std::string& ending );
ABI_EXPORT std::string replace_all( const std::string& s, char oldc, char newc );
ABI_EXPORT std::string replace_all( const std::string& s,
                                    const std::string& olds,
                                    const std::string& news );

/** convert to lower case in-place. Use std::tolower.
 * \param s the string to lower case. Mutable.
 * \return s
 */
ABI_EXPORT std::string & UT_tolower(std::string& s);

ABI_EXPORT std::string UT_XML_cloneNoAmpersands( const std::string& szSource );

ABI_EXPORT std::vector<std::string> UT_simpleSplit(const std::string & str,
                                                   char separator = ' ');


/**
 * Shorten a %path with ellispis of over %maxlen. Then keep the last %cut chars
 */
ABI_EXPORT std::string UT_ellipsisPath(const std::string & path, size_t maxlen, size_t cut);

/*!
 * Some functions to add/subtract and extract std::string properties from a std::string of properties.
 */
ABI_EXPORT std::string UT_std_string_getPropVal(const std::string & sPropertyString, const std::string & sProp);
ABI_EXPORT void UT_std_string_removeProperty(std::string & sPropertyString, const std::string & sProp);
ABI_EXPORT void UT_std_string_setProperty(std::string & sPropertyString, const std::string &sProp, const std::string & sVal);

ABI_EXPORT void UT_std_string_addPropertyString(std::string & sPropertyString, const std::string &sNewProp);


ABI_EXPORT const std::string StreamToString( std::istream& iss );
ABI_EXPORT std::string toTimeString( time_t TT );
ABI_EXPORT time_t toTime( struct tm *tm );
ABI_EXPORT time_t parseTimeString( const std::string& stddatestr );
