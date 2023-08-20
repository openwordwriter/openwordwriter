/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -L C++ common/xp/OXML_LangToScriptConverter.gperf  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "common/xp/OXML_LangToScriptConverter.gperf"

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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

/* DO NOT EDIT, edit the .gperf file isntead ! */

#line 29 "common/xp/OXML_LangToScriptConverter.gperf"
struct OXML_LangScriptAsso {
       const char *lang;
       const char *script;
};

#define TOTAL_KEYWORDS 185
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 2
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 499
/* maximum key range = 496, duplicates = 0 */

class OXML_LangToScriptConverter
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct OXML_LangScriptAsso *in_word_set (const char *str, size_t len);
};

inline /*ARGSUSED*/
unsigned int
OXML_LangToScriptConverter::hash (const char *str, size_t len)
{
  static unsigned short asso_values[] =
    {
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      195,  95,  39, 255, 240, 254,  89,  19,  80,  14,
      190,  94,   4, 165, 159, 214,  65,  99,   5,   0,
      100,  38,  55,  23,  15, 245, 170,  13,  28,   4,
       35, 500,  20, 185, 210, 179, 235, 119,  38, 500,
       44, 239, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
      500, 500, 500, 500, 500
    };
  return asso_values[static_cast<unsigned char>(str[1]+19)] + asso_values[static_cast<unsigned char>(str[0]+3)];
}

struct OXML_LangScriptAsso *
OXML_LangToScriptConverter::in_word_set (const char *str, size_t len)
{
  static struct OXML_LangScriptAsso wordlist[] =
    {
      {"",""}, {"",""}, {"",""}, {"",""},
#line 198 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tn", "Latn"},
#line 174 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sd", "Arab"},
      {"",""}, {"",""},
#line 139 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mn", "Mong"},
#line 182 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sn", "Latn"},
#line 173 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sc", "Latn"},
      {"",""}, {"",""},
#line 197 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tl", "Latn"},
      {"",""},
#line 195 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ti", "Ethi"},
      {"",""},
#line 138 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ml", "Mlym"},
#line 180 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sl", "Latn"},
#line 136 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mi", "Latn"},
#line 178 "common/xp/OXML_LangToScriptConverter.gperf"
      {"si", "Sinh"},
      {"",""}, {"",""},
#line 194 "common/xp/OXML_LangToScriptConverter.gperf"
      {"th", "Thai"},
      {"",""},
#line 184 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sq", "Latn"},
      {"",""},
#line 135 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mh", "Latn"},
#line 177 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sh", "Cyrl"},
      {"",""},
#line 215 "common/xp/OXML_LangToScriptConverter.gperf"
      {"yi", "Hebr"},
      {"",""}, {"",""},
#line 181 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sm", "Latn"},
#line 91 "common/xp/OXML_LangToScriptConverter.gperf"
      {"hi", "Deva"},
#line 199 "common/xp/OXML_LangToScriptConverter.gperf"
      {"to", "Latn"},
      {"",""}, {"",""},
#line 203 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tw", "Latn"},
#line 140 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mo", "Cyrl"},
#line 183 "common/xp/OXML_LangToScriptConverter.gperf"
      {"so", "Latn"},
      {"",""}, {"",""},
#line 190 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sw", "Latn"},
#line 204 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ty", "Latn"},
      {"",""},
#line 214 "common/xp/OXML_LangToScriptConverter.gperf"
      {"xh", "Latn"},
      {"",""},
#line 144 "common/xp/OXML_LangToScriptConverter.gperf"
      {"my", "Mymr"},
      {"",""},
#line 216 "common/xp/OXML_LangToScriptConverter.gperf"
      {"yo", "Latn"},
      {"",""}, {"",""},
#line 210 "common/xp/OXML_LangToScriptConverter.gperf"
      {"vi", "Latn"},
#line 92 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ho", "Latn"},
#line 193 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tg", "Arab"},
      {"",""}, {"",""}, {"",""},
#line 134 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mg", "Latn"},
#line 176 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sg", "Latn"},
      {"",""}, {"",""},
#line 96 "common/xp/OXML_LangToScriptConverter.gperf"
      {"hy", "Armn"},
      {"",""},
#line 191 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ta", "Taml"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 172 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sa", "Deva"},
      {"",""}, {"",""},
#line 211 "common/xp/OXML_LangToScriptConverter.gperf"
      {"vo", "Latn"},
#line 59 "common/xp/OXML_LangToScriptConverter.gperf"
      {"co", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 108 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ja", "Jpan"},
#line 99 "common/xp/OXML_LangToScriptConverter.gperf"
      {"id", "Latn"},
      {"",""}, {"",""},
#line 64 "common/xp/OXML_LangToScriptConverter.gperf"
      {"cy", "Latn"},
#line 89 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ha", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 84 "common/xp/OXML_LangToScriptConverter.gperf"
      {"gd", "Latn"},
#line 213 "common/xp/OXML_LangToScriptConverter.gperf"
      {"wo", "Latn"},
      {"",""}, {"",""},
#line 86 "common/xp/OXML_LangToScriptConverter.gperf"
      {"gn", "Latn"},
      {"",""},
#line 102 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ii", "Yiii"},
      {"",""}, {"",""},
#line 129 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ln", "Latn"},
#line 53 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bn", "Beng"},
#line 192 "common/xp/OXML_LangToScriptConverter.gperf"
      {"te", "Telu"},
      {"",""},
#line 85 "common/xp/OXML_LangToScriptConverter.gperf"
      {"gl", "Latn"},
#line 168 "common/xp/OXML_LangToScriptConverter.gperf"
      {"rn", "Latn"},
#line 57 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ca", "Latn"},
#line 175 "common/xp/OXML_LangToScriptConverter.gperf"
      {"se", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 128 "common/xp/OXML_LangToScriptConverter.gperf"
      {"li", "Latn"},
#line 51 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bi", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 104 "common/xp/OXML_LangToScriptConverter.gperf"
      {"io", "Latn"},
      {"",""}, {"",""},
#line 50 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bh", "Deva"},
#line 90 "common/xp/OXML_LangToScriptConverter.gperf"
      {"he", "Hebr"},
#line 212 "common/xp/OXML_LangToScriptConverter.gperf"
      {"wa", "Latn"},
      {"",""}, {"",""},
#line 52 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bm", "Latn"},
#line 189 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sv", "Latn"},
      {"",""}, {"",""},
#line 167 "common/xp/OXML_LangToScriptConverter.gperf"
      {"rm", "Latn"},
      {"",""},
#line 130 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lo", "Laoo"},
#line 54 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bo", "Tibt"},
      {"",""}, {"",""},
#line 109 "common/xp/OXML_LangToScriptConverter.gperf"
      {"jv", "Java"},
#line 169 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ro", "Latn"},
#line 101 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ig", "Latn"},
      {"",""},
#line 171 "common/xp/OXML_LangToScriptConverter.gperf"
      {"rw", "Latn"},
#line 209 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ve", "Latn"},
#line 58 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ce", "Cyrl"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
#line 98 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ia", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 127 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lg", "Latn"},
#line 49 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bg", "Cyrl"},
      {"",""}, {"",""}, {"",""},
#line 83 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ga", "Latn"},
#line 205 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ug", "Arab"},
      {"",""}, {"",""},
#line 63 "common/xp/OXML_LangToScriptConverter.gperf"
      {"cv", "Cyrl"},
#line 125 "common/xp/OXML_LangToScriptConverter.gperf"
      {"la", "Latn"},
#line 47 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ba", "Cyrl"},
      {"",""}, {"",""}, {"",""},
#line 156 "common/xp/OXML_LangToScriptConverter.gperf"
      {"oc", "Latn"},
#line 147 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nd", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 151 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nn", "Latn"},
#line 196 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tk", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 137 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mk", "Cyrl"},
#line 179 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sk", "Latn"},
      {"",""}, {"",""},
#line 150 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nl", "Latn"},
#line 202 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tt", "Cyrl"},
#line 100 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ie", "Latn"},
      {"",""}, {"",""},
#line 143 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mt", "Latn"},
#line 187 "common/xp/OXML_LangToScriptConverter.gperf"
      {"st", "Latn"},
#line 200 "common/xp/OXML_LangToScriptConverter.gperf"
      {"tr", "Latn"},
      {"",""},
#line 158 "common/xp/OXML_LangToScriptConverter.gperf"
      {"om", "Latn"},
      {"",""},
#line 141 "common/xp/OXML_LangToScriptConverter.gperf"
      {"mr", "Deva"},
#line 185 "common/xp/OXML_LangToScriptConverter.gperf"
      {"sr", "Cyrl"},
      {"",""}, {"",""},
#line 126 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lb", "Latn"},
#line 117 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kn", "Knda"},
#line 48 "common/xp/OXML_LangToScriptConverter.gperf"
      {"be", "Cyrl"},
      {"",""}, {"",""},
#line 94 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ht", "Latn"},
#line 41 "common/xp/OXML_LangToScriptConverter.gperf"
      {"an", "Latn"},
#line 152 "common/xp/OXML_LangToScriptConverter.gperf"
      {"no", "Latn"},
      {"",""}, {"",""},
#line 115 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kl", "Latn"},
#line 93 "common/xp/OXML_LangToScriptConverter.gperf"
      {"hr", "Latn"},
#line 112 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ki", "Latn"},
      {"",""}, {"",""},
#line 88 "common/xp/OXML_LangToScriptConverter.gperf"
      {"gv", "Latn"},
#line 155 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ny", "Latn"},
#line 201 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ts", "Latn"},
      {"",""}, {"",""},
#line 133 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lv", "Latn"},
#line 142 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ms", "Latn"},
#line 186 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ss", "Latn"},
      {"",""}, {"",""},
#line 116 "common/xp/OXML_LangToScriptConverter.gperf"
      {"km", "Khmr"},
      {"",""},
#line 149 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ng", "Latn"},
      {"",""}, {"",""},
#line 40 "common/xp/OXML_LangToScriptConverter.gperf"
      {"am", "Ethi"},
#line 60 "common/xp/OXML_LangToScriptConverter.gperf"
      {"cr", "Cans"},
#line 118 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ko", "Hang"},
      {"",""},
#line 163 "common/xp/OXML_LangToScriptConverter.gperf"
      {"pl", "Latn"},
#line 123 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kw", "Latn"},
#line 162 "common/xp/OXML_LangToScriptConverter.gperf"
      {"pi", "Deva"},
#line 145 "common/xp/OXML_LangToScriptConverter.gperf"
      {"na", "Latn"},
      {"",""}, {"",""},
#line 38 "common/xp/OXML_LangToScriptConverter.gperf"
      {"af", "Latn"},
#line 124 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ky", "Cyrl"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 45 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ay", "Latn"},
#line 188 "common/xp/OXML_LangToScriptConverter.gperf"
      {"su", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 71 "common/xp/OXML_LangToScriptConverter.gperf"
      {"en", "Latn"},
#line 111 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kg", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 61 "common/xp/OXML_LangToScriptConverter.gperf"
      {"cs", "Latn"},
#line 103 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ik", "Latn"},
      {"",""}, {"",""},
#line 70 "common/xp/OXML_LangToScriptConverter.gperf"
      {"el", "Grek"},
#line 95 "common/xp/OXML_LangToScriptConverter.gperf"
      {"hu", "Latn"},
#line 110 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ka", "Geor"},
      {"",""}, {"",""},
#line 97 "common/xp/OXML_LangToScriptConverter.gperf"
      {"hz", "Latn"},
#line 106 "common/xp/OXML_LangToScriptConverter.gperf"
      {"it", "Latn"},
#line 35 "common/xp/OXML_LangToScriptConverter.gperf"
      {"aa", "Ethi"},
      {"",""}, {"",""}, {"",""},
#line 146 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nb", "Latn"},
#line 148 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ne", "Deva"},
      {"",""}, {"",""},
#line 218 "common/xp/OXML_LangToScriptConverter.gperf"
      {"zh", "Hans"},
#line 78 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fi", "Latn"},
#line 206 "common/xp/OXML_LangToScriptConverter.gperf"
      {"uk", "Cyrl"},
      {"",""}, {"",""},
#line 131 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lt", "Latn"},
#line 62 "common/xp/OXML_LangToScriptConverter.gperf"
      {"cu", "Cyrl"},
#line 72 "common/xp/OXML_LangToScriptConverter.gperf"
      {"eo", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 161 "common/xp/OXML_LangToScriptConverter.gperf"
      {"pa", "Guru"},
#line 55 "common/xp/OXML_LangToScriptConverter.gperf"
      {"br", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 154 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nv", "Latn"},
#line 207 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ur", "Arab"},
      {"",""}, {"",""}, {"",""},
#line 80 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fo", "Latn"},
#line 105 "common/xp/OXML_LangToScriptConverter.gperf"
      {"is", "Latn"},
      {"",""},
#line 77 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ff", "Latn"},
      {"",""},
#line 36 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ab", "Cyrl"},
#line 37 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ae", "Avst"},
      {"",""}, {"",""},
#line 82 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fy", "Latn"},
      {"",""},
#line 166 "common/xp/OXML_LangToScriptConverter.gperf"
      {"qu", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 56 "common/xp/OXML_LangToScriptConverter.gperf"
      {"bs", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 122 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kv", "Cyrl"},
#line 217 "common/xp/OXML_LangToScriptConverter.gperf"
      {"za", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 44 "common/xp/OXML_LangToScriptConverter.gperf"
      {"av", "Cyrl"},
#line 107 "common/xp/OXML_LangToScriptConverter.gperf"
      {"iu", "Cans"},
      {"",""}, {"",""}, {"",""},
#line 76 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fa", "Arab"},
#line 65 "common/xp/OXML_LangToScriptConverter.gperf"
      {"da", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 87 "common/xp/OXML_LangToScriptConverter.gperf"
      {"gu", "Gujr"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 132 "common/xp/OXML_LangToScriptConverter.gperf"
      {"lu", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 170 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ru", "Cyrl"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 208 "common/xp/OXML_LangToScriptConverter.gperf"
      {"uz", "Cyrl"},
#line 69 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ee", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 159 "common/xp/OXML_LangToScriptConverter.gperf"
      {"or", "Orya"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
#line 153 "common/xp/OXML_LangToScriptConverter.gperf"
      {"nr", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 66 "common/xp/OXML_LangToScriptConverter.gperf"
      {"de", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 114 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kk", "Cyrl"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 39 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ak", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 160 "common/xp/OXML_LangToScriptConverter.gperf"
      {"os", "Cyrl"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 67 "common/xp/OXML_LangToScriptConverter.gperf"
      {"dv", "Thaa"},
#line 119 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kr", "Arab"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 42 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ar", "Arab"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
#line 165 "common/xp/OXML_LangToScriptConverter.gperf"
      {"pt", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
#line 120 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ks", "Arab"},
      {"",""}, {"",""}, {"",""},
#line 157 "common/xp/OXML_LangToScriptConverter.gperf"
      {"oj", "Cans"},
#line 43 "common/xp/OXML_LangToScriptConverter.gperf"
      {"as", "Beng"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""},
#line 74 "common/xp/OXML_LangToScriptConverter.gperf"
      {"et", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 164 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ps", "Arab"},
#line 121 "common/xp/OXML_LangToScriptConverter.gperf"
      {"ku", "Arab"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""},
#line 46 "common/xp/OXML_LangToScriptConverter.gperf"
      {"az", "Latn"},
#line 113 "common/xp/OXML_LangToScriptConverter.gperf"
      {"kj", "Latn"},
      {"",""}, {"",""}, {"",""},
#line 81 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fr", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 73 "common/xp/OXML_LangToScriptConverter.gperf"
      {"es", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
#line 75 "common/xp/OXML_LangToScriptConverter.gperf"
      {"eu", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 219 "common/xp/OXML_LangToScriptConverter.gperf"
      {"zu", "Latn"},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""}, {"",""}, {"",""}, {"",""}, {"",""}, {"",""},
      {"",""},
#line 68 "common/xp/OXML_LangToScriptConverter.gperf"
      {"dz", "Tibt"},
      {"",""}, {"",""}, {"",""}, {"",""},
#line 79 "common/xp/OXML_LangToScriptConverter.gperf"
      {"fj", "Latn"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].lang;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
