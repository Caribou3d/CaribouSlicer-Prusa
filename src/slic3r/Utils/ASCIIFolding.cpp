///|/ Copyright (c) Prusa Research 2018 - 2020 Lukáš Matěna @lukasmatena, Vojtěch Bubník @bubnikv
///|/
///|/ PrusaSlicer is released under the terms of the AGPLv3 or higher
///|/
#include "ASCIIFolding.hpp"

#include <stdio.h>
#include <string.h>
#include <locale>
#include <boost/locale/encoding_utf.hpp>

namespace Slic3r {

// Based on http://svn.apache.org/repos/asf/lucene/java/tags/lucene_solr_4_5_1/lucene/analysis/common/src/java/org/apache/lucene/analysis/miscellaneous/ASCIIFoldingFilter.java
// Convert the input UNICODE character to a string of maximum 4 output ASCII characters.
// Return the end of the string written to the output.
// The output buffer must be at least 4 characters long.
wchar_t* fold_to_ascii(wchar_t c, wchar_t *out)
{
    if (c < 0x080) {
        *out ++ = c;
    } else {
        switch (c) {
        case L'\u00C0': // [LATIN CAPITAL LETTER A WITH GRAVE]
        case L'\u00C1': // [LATIN CAPITAL LETTER A WITH ACUTE]
        case L'\u00C2': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX]
        case L'\u00C3': // [LATIN CAPITAL LETTER A WITH TILDE]
        case L'\u00C4': // [LATIN CAPITAL LETTER A WITH DIAERESIS]
        case L'\u00C5': // [LATIN CAPITAL LETTER A WITH RING ABOVE]
        case L'\u0100': // [LATIN CAPITAL LETTER A WITH MACRON]
        case L'\u0102': // [LATIN CAPITAL LETTER A WITH BREVE]
        case L'\u0104': // [LATIN CAPITAL LETTER A WITH OGONEK]
        case L'\u018F': // [LATIN CAPITAL LETTER SCHWA]
        case L'\u01CD': // [LATIN CAPITAL LETTER A WITH CARON]
        case L'\u01DE': // [LATIN CAPITAL LETTER A WITH DIAERESIS AND MACRON]
        case L'\u01E0': // [LATIN CAPITAL LETTER A WITH DOT ABOVE AND MACRON]
        case L'\u01FA': // [LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE]
        case L'\u0200': // [LATIN CAPITAL LETTER A WITH DOUBLE GRAVE]
        case L'\u0202': // [LATIN CAPITAL LETTER A WITH INVERTED BREVE]
        case L'\u0226': // [LATIN CAPITAL LETTER A WITH DOT ABOVE]
        case L'\u023A': // [LATIN CAPITAL LETTER A WITH STROKE]
        case L'\u1D00': // [LATIN LETTER SMALL CAPITAL A]
        case L'\u1E00': // [LATIN CAPITAL LETTER A WITH RING BELOW]
        case L'\u1EA0': // [LATIN CAPITAL LETTER A WITH DOT BELOW]
        case L'\u1EA2': // [LATIN CAPITAL LETTER A WITH HOOK ABOVE]
        case L'\u1EA4': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND ACUTE]
        case L'\u1EA6': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND GRAVE]
        case L'\u1EA8': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1EAA': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND TILDE]
        case L'\u1EAC': // [LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u1EAE': // [LATIN CAPITAL LETTER A WITH BREVE AND ACUTE]
        case L'\u1EB0': // [LATIN CAPITAL LETTER A WITH BREVE AND GRAVE]
        case L'\u1EB2': // [LATIN CAPITAL LETTER A WITH BREVE AND HOOK ABOVE]
        case L'\u1EB4': // [LATIN CAPITAL LETTER A WITH BREVE AND TILDE]
        case L'\u1EB6': // [LATIN CAPITAL LETTER A WITH BREVE AND DOT BELOW]
        case L'\u24B6': // [CIRCLED LATIN CAPITAL LETTER A]
        case L'\uFF21': // [FULLWIDTH LATIN CAPITAL LETTER A]
            *out ++ = 'A';
            break;
        case L'\u00E0': // [LATIN SMALL LETTER A WITH GRAVE]
        case L'\u00E1': // [LATIN SMALL LETTER A WITH ACUTE]
        case L'\u00E2': // [LATIN SMALL LETTER A WITH CIRCUMFLEX]
        case L'\u00E3': // [LATIN SMALL LETTER A WITH TILDE]
        case L'\u00E4': // [LATIN SMALL LETTER A WITH DIAERESIS]
        case L'\u00E5': // [LATIN SMALL LETTER A WITH RING ABOVE]
        case L'\u0101': // [LATIN SMALL LETTER A WITH MACRON]
        case L'\u0103': // [LATIN SMALL LETTER A WITH BREVE]
        case L'\u0105': // [LATIN SMALL LETTER A WITH OGONEK]
        case L'\u01CE': // [LATIN SMALL LETTER A WITH CARON]
        case L'\u01DF': // [LATIN SMALL LETTER A WITH DIAERESIS AND MACRON]
        case L'\u01E1': // [LATIN SMALL LETTER A WITH DOT ABOVE AND MACRON]
        case L'\u01FB': // [LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE]
        case L'\u0201': // [LATIN SMALL LETTER A WITH DOUBLE GRAVE]
        case L'\u0203': // [LATIN SMALL LETTER A WITH INVERTED BREVE]
        case L'\u0227': // [LATIN SMALL LETTER A WITH DOT ABOVE]
        case L'\u0250': // [LATIN SMALL LETTER TURNED A]
        case L'\u0259': // [LATIN SMALL LETTER SCHWA]
        case L'\u025A': // [LATIN SMALL LETTER SCHWA WITH HOOK]
        case L'\u1D8F': // [LATIN SMALL LETTER A WITH RETROFLEX HOOK]
        case L'\u1D95': // [LATIN SMALL LETTER SCHWA WITH RETROFLEX HOOK]
        case L'\u1E01': // [LATIN SMALL LETTER A WITH RING BELOW]
        case L'\u1E9A': // [LATIN SMALL LETTER A WITH RIGHT HALF RING]
        case L'\u1EA1': // [LATIN SMALL LETTER A WITH DOT BELOW]
        case L'\u1EA3': // [LATIN SMALL LETTER A WITH HOOK ABOVE]
        case L'\u1EA5': // [LATIN SMALL LETTER A WITH CIRCUMFLEX AND ACUTE]
        case L'\u1EA7': // [LATIN SMALL LETTER A WITH CIRCUMFLEX AND GRAVE]
        case L'\u1EA9': // [LATIN SMALL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1EAB': // [LATIN SMALL LETTER A WITH CIRCUMFLEX AND TILDE]
        case L'\u1EAD': // [LATIN SMALL LETTER A WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u1EAF': // [LATIN SMALL LETTER A WITH BREVE AND ACUTE]
        case L'\u1EB1': // [LATIN SMALL LETTER A WITH BREVE AND GRAVE]
        case L'\u1EB3': // [LATIN SMALL LETTER A WITH BREVE AND HOOK ABOVE]
        case L'\u1EB5': // [LATIN SMALL LETTER A WITH BREVE AND TILDE]
        case L'\u1EB7': // [LATIN SMALL LETTER A WITH BREVE AND DOT BELOW]
        case L'\u2090': // [LATIN SUBSCRIPT SMALL LETTER A]
        case L'\u2094': // [LATIN SUBSCRIPT SMALL LETTER SCHWA]
        case L'\u24D0': // [CIRCLED LATIN SMALL LETTER A]
        case L'\u2C65': // [LATIN SMALL LETTER A WITH STROKE]
        case L'\u2C6F': // [LATIN CAPITAL LETTER TURNED A]
        case L'\uFF41': // [FULLWIDTH LATIN SMALL LETTER A]
            *out ++ = 'a';
            break;
        case L'\uA732': // [LATIN CAPITAL LETTER AA]
            *out ++ = 'A';
            *out ++ = 'A';
            break;
        case L'\u00C6': // [LATIN CAPITAL LETTER AE]
        case L'\u01E2': // [LATIN CAPITAL LETTER AE WITH MACRON]
        case L'\u01FC': // [LATIN CAPITAL LETTER AE WITH ACUTE]
        case L'\u1D01': // [LATIN LETTER SMALL CAPITAL AE]
            *out ++ = 'A';
            *out ++ = 'E';
            break;
        case L'\uA734': // [LATIN CAPITAL LETTER AO]
            *out ++ = 'A';
            *out ++ = 'O';
            break;
        case L'\uA736': // [LATIN CAPITAL LETTER AU]
            *out ++ = 'A';
            *out ++ = 'U';
            break;
        case L'\uA738': // [LATIN CAPITAL LETTER AV]
        case L'\uA73A': // [LATIN CAPITAL LETTER AV WITH HORIZONTAL BAR]
            *out ++ = 'A';
            *out ++ = 'V';
            break;
        case L'\uA73C': // [LATIN CAPITAL LETTER AY]
            *out ++ = 'A';
            *out ++ = 'Y';
            break;
        case L'\u249C': // [PARENTHESIZED LATIN SMALL LETTER A]
            *out ++ = '(';
            *out ++ = 'a';
            *out ++ = ')';
            break;
        case L'\uA733': // [LATIN SMALL LETTER AA]
            *out ++ = 'a';
            *out ++ = 'a';
            break;
        case L'\u00E6': // [LATIN SMALL LETTER AE]
        case L'\u01E3': // [LATIN SMALL LETTER AE WITH MACRON]
        case L'\u01FD': // [LATIN SMALL LETTER AE WITH ACUTE]
        case L'\u1D02': // [LATIN SMALL LETTER TURNED AE]
            *out ++ = 'a';
            *out ++ = 'e';
            break;
        case L'\uA735': // [LATIN SMALL LETTER AO]
            *out ++ = 'a';
            *out ++ = 'o';
            break;
        case L'\uA737': // [LATIN SMALL LETTER AU]
            *out ++ = 'a';
            *out ++ = 'u';
            break;
        case L'\uA739': // [LATIN SMALL LETTER AV]
        case L'\uA73B': // [LATIN SMALL LETTER AV WITH HORIZONTAL BAR]
            *out ++ = 'a';
            *out ++ = 'v';
            break;
        case L'\uA73D': // [LATIN SMALL LETTER AY]
            *out ++ = 'a';
            *out ++ = 'y';
            break;
        case L'\u0181': // [LATIN CAPITAL LETTER B WITH HOOK]
        case L'\u0182': // [LATIN CAPITAL LETTER B WITH TOPBAR]
        case L'\u0243': // [LATIN CAPITAL LETTER B WITH STROKE]
        case L'\u0299': // [LATIN LETTER SMALL CAPITAL B]
        case L'\u1D03': // [LATIN LETTER SMALL CAPITAL BARRED B]
        case L'\u1E02': // [LATIN CAPITAL LETTER B WITH DOT ABOVE]
        case L'\u1E04': // [LATIN CAPITAL LETTER B WITH DOT BELOW]
        case L'\u1E06': // [LATIN CAPITAL LETTER B WITH LINE BELOW]
        case L'\u24B7': // [CIRCLED LATIN CAPITAL LETTER B]
        case L'\uFF22': // [FULLWIDTH LATIN CAPITAL LETTER B]
            *out ++ = 'B';
            break;
        case L'\u0180': // [LATIN SMALL LETTER B WITH STROKE]
        case L'\u0183': // [LATIN SMALL LETTER B WITH TOPBAR]
        case L'\u0253': // [LATIN SMALL LETTER B WITH HOOK]
        case L'\u1D6C': // [LATIN SMALL LETTER B WITH MIDDLE TILDE]
        case L'\u1D80': // [LATIN SMALL LETTER B WITH PALATAL HOOK]
        case L'\u1E03': // [LATIN SMALL LETTER B WITH DOT ABOVE]
        case L'\u1E05': // [LATIN SMALL LETTER B WITH DOT BELOW]
        case L'\u1E07': // [LATIN SMALL LETTER B WITH LINE BELOW]
        case L'\u24D1': // [CIRCLED LATIN SMALL LETTER B]
        case L'\uFF42': // [FULLWIDTH LATIN SMALL LETTER B]
            *out ++ = 'b';
            break;
        case L'\u249D': // [PARENTHESIZED LATIN SMALL LETTER B]
            *out ++ = '(';
            *out ++ = 'b';
            *out ++ = ')';
            break;
        case L'\u00C7': // [LATIN CAPITAL LETTER C WITH CEDILLA]
        case L'\u0106': // [LATIN CAPITAL LETTER C WITH ACUTE]
        case L'\u0108': // [LATIN CAPITAL LETTER C WITH CIRCUMFLEX]
        case L'\u010A': // [LATIN CAPITAL LETTER C WITH DOT ABOVE]
        case L'\u010C': // [LATIN CAPITAL LETTER C WITH CARON]
        case L'\u0187': // [LATIN CAPITAL LETTER C WITH HOOK]
        case L'\u023B': // [LATIN CAPITAL LETTER C WITH STROKE]
        case L'\u0297': // [LATIN LETTER STRETCHED C]
        case L'\u1D04': // [LATIN LETTER SMALL CAPITAL C]
        case L'\u1E08': // [LATIN CAPITAL LETTER C WITH CEDILLA AND ACUTE]
        case L'\u24B8': // [CIRCLED LATIN CAPITAL LETTER C]
        case L'\uFF23': // [FULLWIDTH LATIN CAPITAL LETTER C]
            *out ++ = 'C';
            break;
        case L'\u00E7': // [LATIN SMALL LETTER C WITH CEDILLA]
        case L'\u0107': // [LATIN SMALL LETTER C WITH ACUTE]
        case L'\u0109': // [LATIN SMALL LETTER C WITH CIRCUMFLEX]
        case L'\u010B': // [LATIN SMALL LETTER C WITH DOT ABOVE]
        case L'\u010D': // [LATIN SMALL LETTER C WITH CARON]
        case L'\u0188': // [LATIN SMALL LETTER C WITH HOOK]
        case L'\u023C': // [LATIN SMALL LETTER C WITH STROKE]
        case L'\u0255': // [LATIN SMALL LETTER C WITH CURL]
        case L'\u1E09': // [LATIN SMALL LETTER C WITH CEDILLA AND ACUTE]
        case L'\u2184': // [LATIN SMALL LETTER REVERSED C]
        case L'\u24D2': // [CIRCLED LATIN SMALL LETTER C]
        case L'\uA73E': // [LATIN CAPITAL LETTER REVERSED C WITH DOT]
        case L'\uA73F': // [LATIN SMALL LETTER REVERSED C WITH DOT]
        case L'\uFF43': // [FULLWIDTH LATIN SMALL LETTER C]
            *out ++ = 'c';
            break;
        case L'\u249E': // [PARENTHESIZED LATIN SMALL LETTER C]
            *out ++ = '(';
            *out ++ = 'c';
            *out ++ = ')';
            break;
        case L'\u00D0': // [LATIN CAPITAL LETTER ETH]
        case L'\u010E': // [LATIN CAPITAL LETTER D WITH CARON]
        case L'\u0110': // [LATIN CAPITAL LETTER D WITH STROKE]
        case L'\u0189': // [LATIN CAPITAL LETTER AFRICAN D]
        case L'\u018A': // [LATIN CAPITAL LETTER D WITH HOOK]
        case L'\u018B': // [LATIN CAPITAL LETTER D WITH TOPBAR]
        case L'\u1D05': // [LATIN LETTER SMALL CAPITAL D]
        case L'\u1D06': // [LATIN LETTER SMALL CAPITAL ETH]
        case L'\u1E0A': // [LATIN CAPITAL LETTER D WITH DOT ABOVE]
        case L'\u1E0C': // [LATIN CAPITAL LETTER D WITH DOT BELOW]
        case L'\u1E0E': // [LATIN CAPITAL LETTER D WITH LINE BELOW]
        case L'\u1E10': // [LATIN CAPITAL LETTER D WITH CEDILLA]
        case L'\u1E12': // [LATIN CAPITAL LETTER D WITH CIRCUMFLEX BELOW]
        case L'\u24B9': // [CIRCLED LATIN CAPITAL LETTER D]
        case L'\uA779': // [LATIN CAPITAL LETTER INSULAR D]
        case L'\uFF24': // [FULLWIDTH LATIN CAPITAL LETTER D]
            *out ++ = 'D';
            break;
        case L'\u00F0': // [LATIN SMALL LETTER ETH]
        case L'\u010F': // [LATIN SMALL LETTER D WITH CARON]
        case L'\u0111': // [LATIN SMALL LETTER D WITH STROKE]
        case L'\u018C': // [LATIN SMALL LETTER D WITH TOPBAR]
        case L'\u0221': // [LATIN SMALL LETTER D WITH CURL]
        case L'\u0256': // [LATIN SMALL LETTER D WITH TAIL]
        case L'\u0257': // [LATIN SMALL LETTER D WITH HOOK]
        case L'\u1D6D': // [LATIN SMALL LETTER D WITH MIDDLE TILDE]
        case L'\u1D81': // [LATIN SMALL LETTER D WITH PALATAL HOOK]
        case L'\u1D91': // [LATIN SMALL LETTER D WITH HOOK AND TAIL]
        case L'\u1E0B': // [LATIN SMALL LETTER D WITH DOT ABOVE]
        case L'\u1E0D': // [LATIN SMALL LETTER D WITH DOT BELOW]
        case L'\u1E0F': // [LATIN SMALL LETTER D WITH LINE BELOW]
        case L'\u1E11': // [LATIN SMALL LETTER D WITH CEDILLA]
        case L'\u1E13': // [LATIN SMALL LETTER D WITH CIRCUMFLEX BELOW]
        case L'\u24D3': // [CIRCLED LATIN SMALL LETTER D]
        case L'\uA77A': // [LATIN SMALL LETTER INSULAR D]
        case L'\uFF44': // [FULLWIDTH LATIN SMALL LETTER D]
            *out ++ = 'd';
            break;
        case L'\u01C4': // [LATIN CAPITAL LETTER DZ WITH CARON]
        case L'\u01F1': // [LATIN CAPITAL LETTER DZ]
            *out ++ = 'D';
            *out ++ = 'Z';
            break;
        case L'\u01C5': // [LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON]
        case L'\u01F2': // [LATIN CAPITAL LETTER D WITH SMALL LETTER Z]
            *out ++ = 'D';
            *out ++ = 'z';
            break;
        case L'\u249F': // [PARENTHESIZED LATIN SMALL LETTER D]
            *out ++ = '(';
            *out ++ = 'd';
            *out ++ = ')';
            break;
        case L'\u0238': // [LATIN SMALL LETTER DB DIGRAPH]
            *out ++ = 'd';
            *out ++ = 'b';
            break;
        case L'\u01C6': // [LATIN SMALL LETTER DZ WITH CARON]
        case L'\u01F3': // [LATIN SMALL LETTER DZ]
        case L'\u02A3': // [LATIN SMALL LETTER DZ DIGRAPH]
        case L'\u02A5': // [LATIN SMALL LETTER DZ DIGRAPH WITH CURL]
            *out ++ = 'd';
            *out ++ = 'z';
            break;
        case L'\u00C8': // [LATIN CAPITAL LETTER E WITH GRAVE]
        case L'\u00C9': // [LATIN CAPITAL LETTER E WITH ACUTE]
        case L'\u00CA': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX]
        case L'\u00CB': // [LATIN CAPITAL LETTER E WITH DIAERESIS]
        case L'\u0112': // [LATIN CAPITAL LETTER E WITH MACRON]
        case L'\u0114': // [LATIN CAPITAL LETTER E WITH BREVE]
        case L'\u0116': // [LATIN CAPITAL LETTER E WITH DOT ABOVE]
        case L'\u0118': // [LATIN CAPITAL LETTER E WITH OGONEK]
        case L'\u011A': // [LATIN CAPITAL LETTER E WITH CARON]
        case L'\u018E': // [LATIN CAPITAL LETTER REVERSED E]
        case L'\u0190': // [LATIN CAPITAL LETTER OPEN E]
        case L'\u0204': // [LATIN CAPITAL LETTER E WITH DOUBLE GRAVE]
        case L'\u0206': // [LATIN CAPITAL LETTER E WITH INVERTED BREVE]
        case L'\u0228': // [LATIN CAPITAL LETTER E WITH CEDILLA]
        case L'\u0246': // [LATIN CAPITAL LETTER E WITH STROKE]
        case L'\u1D07': // [LATIN LETTER SMALL CAPITAL E]
        case L'\u1E14': // [LATIN CAPITAL LETTER E WITH MACRON AND GRAVE]
        case L'\u1E16': // [LATIN CAPITAL LETTER E WITH MACRON AND ACUTE]
        case L'\u1E18': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX BELOW]
        case L'\u1E1A': // [LATIN CAPITAL LETTER E WITH TILDE BELOW]
        case L'\u1E1C': // [LATIN CAPITAL LETTER E WITH CEDILLA AND BREVE]
        case L'\u1EB8': // [LATIN CAPITAL LETTER E WITH DOT BELOW]
        case L'\u1EBA': // [LATIN CAPITAL LETTER E WITH HOOK ABOVE]
        case L'\u1EBC': // [LATIN CAPITAL LETTER E WITH TILDE]
        case L'\u1EBE': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND ACUTE]
        case L'\u1EC0': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND GRAVE]
        case L'\u1EC2': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1EC4': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND TILDE]
        case L'\u1EC6': // [LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u24BA': // [CIRCLED LATIN CAPITAL LETTER E]
        case L'\u2C7B': // [LATIN LETTER SMALL CAPITAL TURNED E]
        case L'\uFF25': // [FULLWIDTH LATIN CAPITAL LETTER E]
            *out ++ = 'E';
            break;
        case L'\u00E8': // [LATIN SMALL LETTER E WITH GRAVE]
        case L'\u00E9': // [LATIN SMALL LETTER E WITH ACUTE]
        case L'\u00EA': // [LATIN SMALL LETTER E WITH CIRCUMFLEX]
        case L'\u00EB': // [LATIN SMALL LETTER E WITH DIAERESIS]
        case L'\u0113': // [LATIN SMALL LETTER E WITH MACRON]
        case L'\u0115': // [LATIN SMALL LETTER E WITH BREVE]
        case L'\u0117': // [LATIN SMALL LETTER E WITH DOT ABOVE]
        case L'\u0119': // [LATIN SMALL LETTER E WITH OGONEK]
        case L'\u011B': // [LATIN SMALL LETTER E WITH CARON]
        case L'\u01DD': // [LATIN SMALL LETTER TURNED E]
        case L'\u0205': // [LATIN SMALL LETTER E WITH DOUBLE GRAVE]
        case L'\u0207': // [LATIN SMALL LETTER E WITH INVERTED BREVE]
        case L'\u0229': // [LATIN SMALL LETTER E WITH CEDILLA]
        case L'\u0247': // [LATIN SMALL LETTER E WITH STROKE]
        case L'\u0258': // [LATIN SMALL LETTER REVERSED E]
        case L'\u025B': // [LATIN SMALL LETTER OPEN E]
        case L'\u025C': // [LATIN SMALL LETTER REVERSED OPEN E]
        case L'\u025D': // [LATIN SMALL LETTER REVERSED OPEN E WITH HOOK]
        case L'\u025E': // [LATIN SMALL LETTER CLOSED REVERSED OPEN E]
        case L'\u029A': // [LATIN SMALL LETTER CLOSED OPEN E]
        case L'\u1D08': // [LATIN SMALL LETTER TURNED OPEN E]
        case L'\u1D92': // [LATIN SMALL LETTER E WITH RETROFLEX HOOK]
        case L'\u1D93': // [LATIN SMALL LETTER OPEN E WITH RETROFLEX HOOK]
        case L'\u1D94': // [LATIN SMALL LETTER REVERSED OPEN E WITH RETROFLEX HOOK]
        case L'\u1E15': // [LATIN SMALL LETTER E WITH MACRON AND GRAVE]
        case L'\u1E17': // [LATIN SMALL LETTER E WITH MACRON AND ACUTE]
        case L'\u1E19': // [LATIN SMALL LETTER E WITH CIRCUMFLEX BELOW]
        case L'\u1E1B': // [LATIN SMALL LETTER E WITH TILDE BELOW]
        case L'\u1E1D': // [LATIN SMALL LETTER E WITH CEDILLA AND BREVE]
        case L'\u1EB9': // [LATIN SMALL LETTER E WITH DOT BELOW]
        case L'\u1EBB': // [LATIN SMALL LETTER E WITH HOOK ABOVE]
        case L'\u1EBD': // [LATIN SMALL LETTER E WITH TILDE]
        case L'\u1EBF': // [LATIN SMALL LETTER E WITH CIRCUMFLEX AND ACUTE]
        case L'\u1EC1': // [LATIN SMALL LETTER E WITH CIRCUMFLEX AND GRAVE]
        case L'\u1EC3': // [LATIN SMALL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1EC5': // [LATIN SMALL LETTER E WITH CIRCUMFLEX AND TILDE]
        case L'\u1EC7': // [LATIN SMALL LETTER E WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u2091': // [LATIN SUBSCRIPT SMALL LETTER E]
        case L'\u24D4': // [CIRCLED LATIN SMALL LETTER E]
        case L'\u2C78': // [LATIN SMALL LETTER E WITH NOTCH]
        case L'\uFF45': // [FULLWIDTH LATIN SMALL LETTER E]
            *out ++ = 'e';
            break;
        case L'\u24A0': // [PARENTHESIZED LATIN SMALL LETTER E]
            *out ++ = '(';
            *out ++ = 'e';
            *out ++ = ')';
            break;
        case L'\u0191': // [LATIN CAPITAL LETTER F WITH HOOK]
        case L'\u1E1E': // [LATIN CAPITAL LETTER F WITH DOT ABOVE]
        case L'\u24BB': // [CIRCLED LATIN CAPITAL LETTER F]
        case L'\uA730': // [LATIN LETTER SMALL CAPITAL F]
        case L'\uA77B': // [LATIN CAPITAL LETTER INSULAR F]
        case L'\uA7FB': // [LATIN EPIGRAPHIC LETTER REVERSED F]
        case L'\uFF26': // [FULLWIDTH LATIN CAPITAL LETTER F]
            *out ++ = 'F';
            break;
        case L'\u0192': // [LATIN SMALL LETTER F WITH HOOK]
        case L'\u1D6E': // [LATIN SMALL LETTER F WITH MIDDLE TILDE]
        case L'\u1D82': // [LATIN SMALL LETTER F WITH PALATAL HOOK]
        case L'\u1E1F': // [LATIN SMALL LETTER F WITH DOT ABOVE]
        case L'\u1E9B': // [LATIN SMALL LETTER LONG S WITH DOT ABOVE]
        case L'\u24D5': // [CIRCLED LATIN SMALL LETTER F]
        case L'\uA77C': // [LATIN SMALL LETTER INSULAR F]
        case L'\uFF46': // [FULLWIDTH LATIN SMALL LETTER F]
            *out ++ = 'f';
            break;
        case L'\u24A1': // [PARENTHESIZED LATIN SMALL LETTER F]
            *out ++ = '(';
            *out ++ = 'f';
            *out ++ = ')';
            break;
        case L'\uFB00': // [LATIN SMALL LIGATURE FF]
            *out ++ = 'f';
            *out ++ = 'f';
            break;
        case L'\uFB03': // [LATIN SMALL LIGATURE FFI]
            *out ++ = 'f';
            *out ++ = 'f';
            *out ++ = 'i';
            break;
        case L'\uFB04': // [LATIN SMALL LIGATURE FFL]
            *out ++ = 'f';
            *out ++ = 'f';
            *out ++ = 'l';
            break;
        case L'\uFB01': // [LATIN SMALL LIGATURE FI]
            *out ++ = 'f';
            *out ++ = 'i';
            break;
        case L'\uFB02': // [LATIN SMALL LIGATURE FL]
            *out ++ = 'f';
            *out ++ = 'l';
            break;
        case L'\u011C': // [LATIN CAPITAL LETTER G WITH CIRCUMFLEX]
        case L'\u011E': // [LATIN CAPITAL LETTER G WITH BREVE]
        case L'\u0120': // [LATIN CAPITAL LETTER G WITH DOT ABOVE]
        case L'\u0122': // [LATIN CAPITAL LETTER G WITH CEDILLA]
        case L'\u0193': // [LATIN CAPITAL LETTER G WITH HOOK]
        case L'\u01E4': // [LATIN CAPITAL LETTER G WITH STROKE]
        case L'\u01E5': // [LATIN SMALL LETTER G WITH STROKE]
        case L'\u01E6': // [LATIN CAPITAL LETTER G WITH CARON]
        case L'\u01E7': // [LATIN SMALL LETTER G WITH CARON]
        case L'\u01F4': // [LATIN CAPITAL LETTER G WITH ACUTE]
        case L'\u0262': // [LATIN LETTER SMALL CAPITAL G]
        case L'\u029B': // [LATIN LETTER SMALL CAPITAL G WITH HOOK]
        case L'\u1E20': // [LATIN CAPITAL LETTER G WITH MACRON]
        case L'\u24BC': // [CIRCLED LATIN CAPITAL LETTER G]
        case L'\uA77D': // [LATIN CAPITAL LETTER INSULAR G]
        case L'\uA77E': // [LATIN CAPITAL LETTER TURNED INSULAR G]
        case L'\uFF27': // [FULLWIDTH LATIN CAPITAL LETTER G]
            *out ++ = 'G';
            break;
        case L'\u011D': // [LATIN SMALL LETTER G WITH CIRCUMFLEX]
        case L'\u011F': // [LATIN SMALL LETTER G WITH BREVE]
        case L'\u0121': // [LATIN SMALL LETTER G WITH DOT ABOVE]
        case L'\u0123': // [LATIN SMALL LETTER G WITH CEDILLA]
        case L'\u01F5': // [LATIN SMALL LETTER G WITH ACUTE]
        case L'\u0260': // [LATIN SMALL LETTER G WITH HOOK]
        case L'\u0261': // [LATIN SMALL LETTER SCRIPT G]
        case L'\u1D77': // [LATIN SMALL LETTER TURNED G]
        case L'\u1D79': // [LATIN SMALL LETTER INSULAR G]
        case L'\u1D83': // [LATIN SMALL LETTER G WITH PALATAL HOOK]
        case L'\u1E21': // [LATIN SMALL LETTER G WITH MACRON]
        case L'\u24D6': // [CIRCLED LATIN SMALL LETTER G]
        case L'\uA77F': // [LATIN SMALL LETTER TURNED INSULAR G]
        case L'\uFF47': // [FULLWIDTH LATIN SMALL LETTER G]
            *out ++ = 'g';
            break;
        case L'\u24A2': // [PARENTHESIZED LATIN SMALL LETTER G]
            *out ++ = '(';
            *out ++ = 'g';
            *out ++ = ')';
            break;
        case L'\u0124': // [LATIN CAPITAL LETTER H WITH CIRCUMFLEX]
        case L'\u0126': // [LATIN CAPITAL LETTER H WITH STROKE]
        case L'\u021E': // [LATIN CAPITAL LETTER H WITH CARON]
        case L'\u029C': // [LATIN LETTER SMALL CAPITAL H]
        case L'\u1E22': // [LATIN CAPITAL LETTER H WITH DOT ABOVE]
        case L'\u1E24': // [LATIN CAPITAL LETTER H WITH DOT BELOW]
        case L'\u1E26': // [LATIN CAPITAL LETTER H WITH DIAERESIS]
        case L'\u1E28': // [LATIN CAPITAL LETTER H WITH CEDILLA]
        case L'\u1E2A': // [LATIN CAPITAL LETTER H WITH BREVE BELOW]
        case L'\u24BD': // [CIRCLED LATIN CAPITAL LETTER H]
        case L'\u2C67': // [LATIN CAPITAL LETTER H WITH DESCENDER]
        case L'\u2C75': // [LATIN CAPITAL LETTER HALF H]
        case L'\uFF28': // [FULLWIDTH LATIN CAPITAL LETTER H]
            *out ++ = 'H';
            break;
        case L'\u0125': // [LATIN SMALL LETTER H WITH CIRCUMFLEX]
        case L'\u0127': // [LATIN SMALL LETTER H WITH STROKE]
        case L'\u021F': // [LATIN SMALL LETTER H WITH CARON]
        case L'\u0265': // [LATIN SMALL LETTER TURNED H]
        case L'\u0266': // [LATIN SMALL LETTER H WITH HOOK]
        case L'\u02AE': // [LATIN SMALL LETTER TURNED H WITH FISHHOOK]
        case L'\u02AF': // [LATIN SMALL LETTER TURNED H WITH FISHHOOK AND TAIL]
        case L'\u1E23': // [LATIN SMALL LETTER H WITH DOT ABOVE]
        case L'\u1E25': // [LATIN SMALL LETTER H WITH DOT BELOW]
        case L'\u1E27': // [LATIN SMALL LETTER H WITH DIAERESIS]
        case L'\u1E29': // [LATIN SMALL LETTER H WITH CEDILLA]
        case L'\u1E2B': // [LATIN SMALL LETTER H WITH BREVE BELOW]
        case L'\u1E96': // [LATIN SMALL LETTER H WITH LINE BELOW]
        case L'\u24D7': // [CIRCLED LATIN SMALL LETTER H]
        case L'\u2C68': // [LATIN SMALL LETTER H WITH DESCENDER]
        case L'\u2C76': // [LATIN SMALL LETTER HALF H]
        case L'\uFF48': // [FULLWIDTH LATIN SMALL LETTER H]
            *out ++ = 'h';
            break;
        case L'\u01F6': // [LATIN CAPITAL LETTER HWAIR]
            *out ++ = 'H';
            *out ++ = 'V';
            break;
        case L'\u24A3': // [PARENTHESIZED LATIN SMALL LETTER H]
            *out ++ = '(';
            *out ++ = 'h';
            *out ++ = ')';
            break;
        case L'\u0195': // [LATIN SMALL LETTER HV]
            *out ++ = 'h';
            *out ++ = 'v';
            break;
        case L'\u00CC': // [LATIN CAPITAL LETTER I WITH GRAVE]
        case L'\u00CD': // [LATIN CAPITAL LETTER I WITH ACUTE]
        case L'\u00CE': // [LATIN CAPITAL LETTER I WITH CIRCUMFLEX]
        case L'\u00CF': // [LATIN CAPITAL LETTER I WITH DIAERESIS]
        case L'\u0128': // [LATIN CAPITAL LETTER I WITH TILDE]
        case L'\u012A': // [LATIN CAPITAL LETTER I WITH MACRON]
        case L'\u012C': // [LATIN CAPITAL LETTER I WITH BREVE]
        case L'\u012E': // [LATIN CAPITAL LETTER I WITH OGONEK]
        case L'\u0130': // [LATIN CAPITAL LETTER I WITH DOT ABOVE]
        case L'\u0196': // [LATIN CAPITAL LETTER IOTA]
        case L'\u0197': // [LATIN CAPITAL LETTER I WITH STROKE]
        case L'\u01CF': // [LATIN CAPITAL LETTER I WITH CARON]
        case L'\u0208': // [LATIN CAPITAL LETTER I WITH DOUBLE GRAVE]
        case L'\u020A': // [LATIN CAPITAL LETTER I WITH INVERTED BREVE]
        case L'\u026A': // [LATIN LETTER SMALL CAPITAL I]
        case L'\u1D7B': // [LATIN SMALL CAPITAL LETTER I WITH STROKE]
        case L'\u1E2C': // [LATIN CAPITAL LETTER I WITH TILDE BELOW]
        case L'\u1E2E': // [LATIN CAPITAL LETTER I WITH DIAERESIS AND ACUTE]
        case L'\u1EC8': // [LATIN CAPITAL LETTER I WITH HOOK ABOVE]
        case L'\u1ECA': // [LATIN CAPITAL LETTER I WITH DOT BELOW]
        case L'\u24BE': // [CIRCLED LATIN CAPITAL LETTER I]
        case L'\uA7FE': // [LATIN EPIGRAPHIC LETTER I LONGA]
        case L'\uFF29': // [FULLWIDTH LATIN CAPITAL LETTER I]
            *out ++ = 'I';
            break;
        case L'\u00EC': // [LATIN SMALL LETTER I WITH GRAVE]
        case L'\u00ED': // [LATIN SMALL LETTER I WITH ACUTE]
        case L'\u00EE': // [LATIN SMALL LETTER I WITH CIRCUMFLEX]
        case L'\u00EF': // [LATIN SMALL LETTER I WITH DIAERESIS]
        case L'\u0129': // [LATIN SMALL LETTER I WITH TILDE]
        case L'\u012B': // [LATIN SMALL LETTER I WITH MACRON]
        case L'\u012D': // [LATIN SMALL LETTER I WITH BREVE]
        case L'\u012F': // [LATIN SMALL LETTER I WITH OGONEK]
        case L'\u0131': // [LATIN SMALL LETTER DOTLESS I]
        case L'\u01D0': // [LATIN SMALL LETTER I WITH CARON]
        case L'\u0209': // [LATIN SMALL LETTER I WITH DOUBLE GRAVE]
        case L'\u020B': // [LATIN SMALL LETTER I WITH INVERTED BREVE]
        case L'\u0268': // [LATIN SMALL LETTER I WITH STROKE]
        case L'\u1D09': // [LATIN SMALL LETTER TURNED I]
        case L'\u1D62': // [LATIN SUBSCRIPT SMALL LETTER I]
        case L'\u1D7C': // [LATIN SMALL LETTER IOTA WITH STROKE]
        case L'\u1D96': // [LATIN SMALL LETTER I WITH RETROFLEX HOOK]
        case L'\u1E2D': // [LATIN SMALL LETTER I WITH TILDE BELOW]
        case L'\u1E2F': // [LATIN SMALL LETTER I WITH DIAERESIS AND ACUTE]
        case L'\u1EC9': // [LATIN SMALL LETTER I WITH HOOK ABOVE]
        case L'\u1ECB': // [LATIN SMALL LETTER I WITH DOT BELOW]
        case L'\u2071': // [SUPERSCRIPT LATIN SMALL LETTER I]
        case L'\u24D8': // [CIRCLED LATIN SMALL LETTER I]
        case L'\uFF49': // [FULLWIDTH LATIN SMALL LETTER I]
            *out ++ = 'i';
            break;
        case L'\u0132': // [LATIN CAPITAL LIGATURE IJ]
            *out ++ = 'I';
            *out ++ = 'J';
            break;
        case L'\u24A4': // [PARENTHESIZED LATIN SMALL LETTER I]
            *out ++ = '(';
            *out ++ = 'i';
            *out ++ = ')';
            break;
        case L'\u0133': // [LATIN SMALL LIGATURE IJ]
            *out ++ = 'i';
            *out ++ = 'j';
            break;
        case L'\u0134': // [LATIN CAPITAL LETTER J WITH CIRCUMFLEX]
        case L'\u0248': // [LATIN CAPITAL LETTER J WITH STROKE]
        case L'\u1D0A': // [LATIN LETTER SMALL CAPITAL J]
        case L'\u24BF': // [CIRCLED LATIN CAPITAL LETTER J]
        case L'\uFF2A': // [FULLWIDTH LATIN CAPITAL LETTER J]
            *out ++ = 'J';
            break;
        case L'\u0135': // [LATIN SMALL LETTER J WITH CIRCUMFLEX]
        case L'\u01F0': // [LATIN SMALL LETTER J WITH CARON]
        case L'\u0237': // [LATIN SMALL LETTER DOTLESS J]
        case L'\u0249': // [LATIN SMALL LETTER J WITH STROKE]
        case L'\u025F': // [LATIN SMALL LETTER DOTLESS J WITH STROKE]
        case L'\u0284': // [LATIN SMALL LETTER DOTLESS J WITH STROKE AND HOOK]
        case L'\u029D': // [LATIN SMALL LETTER J WITH CROSSED-TAIL]
        case L'\u24D9': // [CIRCLED LATIN SMALL LETTER J]
        case L'\u2C7C': // [LATIN SUBSCRIPT SMALL LETTER J]
        case L'\uFF4A': // [FULLWIDTH LATIN SMALL LETTER J]
            *out ++ = 'j';
            break;
        case L'\u24A5': // [PARENTHESIZED LATIN SMALL LETTER J]
            *out ++ = '(';
            *out ++ = 'j';
            *out ++ = ')';
            break;
        case L'\u0136': // [LATIN CAPITAL LETTER K WITH CEDILLA]
        case L'\u0198': // [LATIN CAPITAL LETTER K WITH HOOK]
        case L'\u01E8': // [LATIN CAPITAL LETTER K WITH CARON]
        case L'\u1D0B': // [LATIN LETTER SMALL CAPITAL K]
        case L'\u1E30': // [LATIN CAPITAL LETTER K WITH ACUTE]
        case L'\u1E32': // [LATIN CAPITAL LETTER K WITH DOT BELOW]
        case L'\u1E34': // [LATIN CAPITAL LETTER K WITH LINE BELOW]
        case L'\u24C0': // [CIRCLED LATIN CAPITAL LETTER K]
        case L'\u2C69': // [LATIN CAPITAL LETTER K WITH DESCENDER]
        case L'\uA740': // [LATIN CAPITAL LETTER K WITH STROKE]
        case L'\uA742': // [LATIN CAPITAL LETTER K WITH DIAGONAL STROKE]
        case L'\uA744': // [LATIN CAPITAL LETTER K WITH STROKE AND DIAGONAL STROKE]
        case L'\uFF2B': // [FULLWIDTH LATIN CAPITAL LETTER K]
            *out ++ = 'K';
            break;
        case L'\u0137': // [LATIN SMALL LETTER K WITH CEDILLA]
        case L'\u0199': // [LATIN SMALL LETTER K WITH HOOK]
        case L'\u01E9': // [LATIN SMALL LETTER K WITH CARON]
        case L'\u029E': // [LATIN SMALL LETTER TURNED K]
        case L'\u1D84': // [LATIN SMALL LETTER K WITH PALATAL HOOK]
        case L'\u1E31': // [LATIN SMALL LETTER K WITH ACUTE]
        case L'\u1E33': // [LATIN SMALL LETTER K WITH DOT BELOW]
        case L'\u1E35': // [LATIN SMALL LETTER K WITH LINE BELOW]
        case L'\u24DA': // [CIRCLED LATIN SMALL LETTER K]
        case L'\u2C6A': // [LATIN SMALL LETTER K WITH DESCENDER]
        case L'\uA741': // [LATIN SMALL LETTER K WITH STROKE]
        case L'\uA743': // [LATIN SMALL LETTER K WITH DIAGONAL STROKE]
        case L'\uA745': // [LATIN SMALL LETTER K WITH STROKE AND DIAGONAL STROKE]
        case L'\uFF4B': // [FULLWIDTH LATIN SMALL LETTER K]
            *out ++ = 'k';
            break;
        case L'\u24A6': // [PARENTHESIZED LATIN SMALL LETTER K]
            *out ++ = '(';
            *out ++ = 'k';
            *out ++ = ')';
            break;
        case L'\u0139': // [LATIN CAPITAL LETTER L WITH ACUTE]
        case L'\u013B': // [LATIN CAPITAL LETTER L WITH CEDILLA]
        case L'\u013D': // [LATIN CAPITAL LETTER L WITH CARON]
        case L'\u013F': // [LATIN CAPITAL LETTER L WITH MIDDLE DOT]
        case L'\u0141': // [LATIN CAPITAL LETTER L WITH STROKE]
        case L'\u023D': // [LATIN CAPITAL LETTER L WITH BAR]
        case L'\u029F': // [LATIN LETTER SMALL CAPITAL L]
        case L'\u1D0C': // [LATIN LETTER SMALL CAPITAL L WITH STROKE]
        case L'\u1E36': // [LATIN CAPITAL LETTER L WITH DOT BELOW]
        case L'\u1E38': // [LATIN CAPITAL LETTER L WITH DOT BELOW AND MACRON]
        case L'\u1E3A': // [LATIN CAPITAL LETTER L WITH LINE BELOW]
        case L'\u1E3C': // [LATIN CAPITAL LETTER L WITH CIRCUMFLEX BELOW]
        case L'\u24C1': // [CIRCLED LATIN CAPITAL LETTER L]
        case L'\u2C60': // [LATIN CAPITAL LETTER L WITH DOUBLE BAR]
        case L'\u2C62': // [LATIN CAPITAL LETTER L WITH MIDDLE TILDE]
        case L'\uA746': // [LATIN CAPITAL LETTER BROKEN L]
        case L'\uA748': // [LATIN CAPITAL LETTER L WITH HIGH STROKE]
        case L'\uA780': // [LATIN CAPITAL LETTER TURNED L]
        case L'\uFF2C': // [FULLWIDTH LATIN CAPITAL LETTER L]
            *out ++ = 'L';
            break;
        case L'\u013A': // [LATIN SMALL LETTER L WITH ACUTE]
        case L'\u013C': // [LATIN SMALL LETTER L WITH CEDILLA]
        case L'\u013E': // [LATIN SMALL LETTER L WITH CARON]
        case L'\u0140': // [LATIN SMALL LETTER L WITH MIDDLE DOT]
        case L'\u0142': // [LATIN SMALL LETTER L WITH STROKE]
        case L'\u019A': // [LATIN SMALL LETTER L WITH BAR]
        case L'\u0234': // [LATIN SMALL LETTER L WITH CURL]
        case L'\u026B': // [LATIN SMALL LETTER L WITH MIDDLE TILDE]
        case L'\u026C': // [LATIN SMALL LETTER L WITH BELT]
        case L'\u026D': // [LATIN SMALL LETTER L WITH RETROFLEX HOOK]
        case L'\u1D85': // [LATIN SMALL LETTER L WITH PALATAL HOOK]
        case L'\u1E37': // [LATIN SMALL LETTER L WITH DOT BELOW]
        case L'\u1E39': // [LATIN SMALL LETTER L WITH DOT BELOW AND MACRON]
        case L'\u1E3B': // [LATIN SMALL LETTER L WITH LINE BELOW]
        case L'\u1E3D': // [LATIN SMALL LETTER L WITH CIRCUMFLEX BELOW]
        case L'\u24DB': // [CIRCLED LATIN SMALL LETTER L]
        case L'\u2C61': // [LATIN SMALL LETTER L WITH DOUBLE BAR]
        case L'\uA747': // [LATIN SMALL LETTER BROKEN L]
        case L'\uA749': // [LATIN SMALL LETTER L WITH HIGH STROKE]
        case L'\uA781': // [LATIN SMALL LETTER TURNED L]
        case L'\uFF4C': // [FULLWIDTH LATIN SMALL LETTER L]
            *out ++ = 'l';
            break;
        case L'\u01C7': // [LATIN CAPITAL LETTER LJ]
            *out ++ = 'L';
            *out ++ = 'J';
            break;
        case L'\u1EFA': // [LATIN CAPITAL LETTER MIDDLE-WELSH LL]
            *out ++ = 'L';
            *out ++ = 'L';
            break;
        case L'\u01C8': // [LATIN CAPITAL LETTER L WITH SMALL LETTER J]
            *out ++ = 'L';
            *out ++ = 'j';
            break;
        case L'\u24A7': // [PARENTHESIZED LATIN SMALL LETTER L]
            *out ++ = '(';
            *out ++ = 'l';
            *out ++ = ')';
            break;
        case L'\u01C9': // [LATIN SMALL LETTER LJ]
            *out ++ = 'l';
            *out ++ = 'j';
            break;
        case L'\u1EFB': // [LATIN SMALL LETTER MIDDLE-WELSH LL]
            *out ++ = 'l';
            *out ++ = 'l';
            break;
        case L'\u02AA': // [LATIN SMALL LETTER LS DIGRAPH]
            *out ++ = 'l';
            *out ++ = 's';
            break;
        case L'\u02AB': // [LATIN SMALL LETTER LZ DIGRAPH]
            *out ++ = 'l';
            *out ++ = 'z';
            break;
        case L'\u019C': // [LATIN CAPITAL LETTER TURNED M]
        case L'\u1D0D': // [LATIN LETTER SMALL CAPITAL M]
        case L'\u1E3E': // [LATIN CAPITAL LETTER M WITH ACUTE]
        case L'\u1E40': // [LATIN CAPITAL LETTER M WITH DOT ABOVE]
        case L'\u1E42': // [LATIN CAPITAL LETTER M WITH DOT BELOW]
        case L'\u24C2': // [CIRCLED LATIN CAPITAL LETTER M]
        case L'\u2C6E': // [LATIN CAPITAL LETTER M WITH HOOK]
        case L'\uA7FD': // [LATIN EPIGRAPHIC LETTER INVERTED M]
        case L'\uA7FF': // [LATIN EPIGRAPHIC LETTER ARCHAIC M]
        case L'\uFF2D': // [FULLWIDTH LATIN CAPITAL LETTER M]
            *out ++ = 'M';
            break;
        case L'\u026F': // [LATIN SMALL LETTER TURNED M]
        case L'\u0270': // [LATIN SMALL LETTER TURNED M WITH LONG LEG]
        case L'\u0271': // [LATIN SMALL LETTER M WITH HOOK]
        case L'\u1D6F': // [LATIN SMALL LETTER M WITH MIDDLE TILDE]
        case L'\u1D86': // [LATIN SMALL LETTER M WITH PALATAL HOOK]
        case L'\u1E3F': // [LATIN SMALL LETTER M WITH ACUTE]
        case L'\u1E41': // [LATIN SMALL LETTER M WITH DOT ABOVE]
        case L'\u1E43': // [LATIN SMALL LETTER M WITH DOT BELOW]
        case L'\u24DC': // [CIRCLED LATIN SMALL LETTER M]
        case L'\uFF4D': // [FULLWIDTH LATIN SMALL LETTER M]
            *out ++ = 'm';
            break;
        case L'\u24A8': // [PARENTHESIZED LATIN SMALL LETTER M]
            *out ++ = '(';
            *out ++ = 'm';
            *out ++ = ')';
            break;
        case L'\u00D1': // [LATIN CAPITAL LETTER N WITH TILDE]
        case L'\u0143': // [LATIN CAPITAL LETTER N WITH ACUTE]
        case L'\u0145': // [LATIN CAPITAL LETTER N WITH CEDILLA]
        case L'\u0147': // [LATIN CAPITAL LETTER N WITH CARON]
        case L'\u014A': // [LATIN CAPITAL LETTER ENG]
        case L'\u019D': // [LATIN CAPITAL LETTER N WITH LEFT HOOK]
        case L'\u01F8': // [LATIN CAPITAL LETTER N WITH GRAVE]
        case L'\u0220': // [LATIN CAPITAL LETTER N WITH LONG RIGHT LEG]
        case L'\u0274': // [LATIN LETTER SMALL CAPITAL N]
        case L'\u1D0E': // [LATIN LETTER SMALL CAPITAL REVERSED N]
        case L'\u1E44': // [LATIN CAPITAL LETTER N WITH DOT ABOVE]
        case L'\u1E46': // [LATIN CAPITAL LETTER N WITH DOT BELOW]
        case L'\u1E48': // [LATIN CAPITAL LETTER N WITH LINE BELOW]
        case L'\u1E4A': // [LATIN CAPITAL LETTER N WITH CIRCUMFLEX BELOW]
        case L'\u24C3': // [CIRCLED LATIN CAPITAL LETTER N]
        case L'\uFF2E': // [FULLWIDTH LATIN CAPITAL LETTER N]
            *out ++ = 'N';
            break;
        case L'\u00F1': // [LATIN SMALL LETTER N WITH TILDE]
        case L'\u0144': // [LATIN SMALL LETTER N WITH ACUTE]
        case L'\u0146': // [LATIN SMALL LETTER N WITH CEDILLA]
        case L'\u0148': // [LATIN SMALL LETTER N WITH CARON]
        case L'\u0149': // [LATIN SMALL LETTER N PRECEDED BY APOSTROPHE]
        case L'\u014B': // [LATIN SMALL LETTER ENG]
        case L'\u019E': // [LATIN SMALL LETTER N WITH LONG RIGHT LEG]
        case L'\u01F9': // [LATIN SMALL LETTER N WITH GRAVE]
        case L'\u0235': // [LATIN SMALL LETTER N WITH CURL]
        case L'\u0272': // [LATIN SMALL LETTER N WITH LEFT HOOK]
        case L'\u0273': // [LATIN SMALL LETTER N WITH RETROFLEX HOOK]
        case L'\u1D70': // [LATIN SMALL LETTER N WITH MIDDLE TILDE]
        case L'\u1D87': // [LATIN SMALL LETTER N WITH PALATAL HOOK]
        case L'\u1E45': // [LATIN SMALL LETTER N WITH DOT ABOVE]
        case L'\u1E47': // [LATIN SMALL LETTER N WITH DOT BELOW]
        case L'\u1E49': // [LATIN SMALL LETTER N WITH LINE BELOW]
        case L'\u1E4B': // [LATIN SMALL LETTER N WITH CIRCUMFLEX BELOW]
        case L'\u207F': // [SUPERSCRIPT LATIN SMALL LETTER N]
        case L'\u24DD': // [CIRCLED LATIN SMALL LETTER N]
        case L'\uFF4E': // [FULLWIDTH LATIN SMALL LETTER N]
            *out ++ = 'n';
            break;
        case L'\u01CA': // [LATIN CAPITAL LETTER NJ]
            *out ++ = 'N';
            *out ++ = 'J';
            break;
        case L'\u01CB': // [LATIN CAPITAL LETTER N WITH SMALL LETTER J]
            *out ++ = 'N';
            *out ++ = 'j';
            break;
        case L'\u24A9': // [PARENTHESIZED LATIN SMALL LETTER N]
            *out ++ = '(';
            *out ++ = 'n';
            *out ++ = ')';
            break;
        case L'\u01CC': // [LATIN SMALL LETTER NJ]
            *out ++ = 'n';
            *out ++ = 'j';
            break;
        case L'\u00D2': // [LATIN CAPITAL LETTER O WITH GRAVE]
        case L'\u00D3': // [LATIN CAPITAL LETTER O WITH ACUTE]
        case L'\u00D4': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX]
        case L'\u00D5': // [LATIN CAPITAL LETTER O WITH TILDE]
        case L'\u00D6': // [LATIN CAPITAL LETTER O WITH DIAERESIS]
        case L'\u00D8': // [LATIN CAPITAL LETTER O WITH STROKE]
        case L'\u014C': // [LATIN CAPITAL LETTER O WITH MACRON]
        case L'\u014E': // [LATIN CAPITAL LETTER O WITH BREVE]
        case L'\u0150': // [LATIN CAPITAL LETTER O WITH DOUBLE ACUTE]
        case L'\u0186': // [LATIN CAPITAL LETTER OPEN O]
        case L'\u019F': // [LATIN CAPITAL LETTER O WITH MIDDLE TILDE]
        case L'\u01A0': // [LATIN CAPITAL LETTER O WITH HORN]
        case L'\u01D1': // [LATIN CAPITAL LETTER O WITH CARON]
        case L'\u01EA': // [LATIN CAPITAL LETTER O WITH OGONEK]
        case L'\u01EC': // [LATIN CAPITAL LETTER O WITH OGONEK AND MACRON]
        case L'\u01FE': // [LATIN CAPITAL LETTER O WITH STROKE AND ACUTE]
        case L'\u020C': // [LATIN CAPITAL LETTER O WITH DOUBLE GRAVE]
        case L'\u020E': // [LATIN CAPITAL LETTER O WITH INVERTED BREVE]
        case L'\u022A': // [LATIN CAPITAL LETTER O WITH DIAERESIS AND MACRON]
        case L'\u022C': // [LATIN CAPITAL LETTER O WITH TILDE AND MACRON]
        case L'\u022E': // [LATIN CAPITAL LETTER O WITH DOT ABOVE]
        case L'\u0230': // [LATIN CAPITAL LETTER O WITH DOT ABOVE AND MACRON]
        case L'\u1D0F': // [LATIN LETTER SMALL CAPITAL O]
        case L'\u1D10': // [LATIN LETTER SMALL CAPITAL OPEN O]
        case L'\u1E4C': // [LATIN CAPITAL LETTER O WITH TILDE AND ACUTE]
        case L'\u1E4E': // [LATIN CAPITAL LETTER O WITH TILDE AND DIAERESIS]
        case L'\u1E50': // [LATIN CAPITAL LETTER O WITH MACRON AND GRAVE]
        case L'\u1E52': // [LATIN CAPITAL LETTER O WITH MACRON AND ACUTE]
        case L'\u1ECC': // [LATIN CAPITAL LETTER O WITH DOT BELOW]
        case L'\u1ECE': // [LATIN CAPITAL LETTER O WITH HOOK ABOVE]
        case L'\u1ED0': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND ACUTE]
        case L'\u1ED2': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND GRAVE]
        case L'\u1ED4': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1ED6': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND TILDE]
        case L'\u1ED8': // [LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u1EDA': // [LATIN CAPITAL LETTER O WITH HORN AND ACUTE]
        case L'\u1EDC': // [LATIN CAPITAL LETTER O WITH HORN AND GRAVE]
        case L'\u1EDE': // [LATIN CAPITAL LETTER O WITH HORN AND HOOK ABOVE]
        case L'\u1EE0': // [LATIN CAPITAL LETTER O WITH HORN AND TILDE]
        case L'\u1EE2': // [LATIN CAPITAL LETTER O WITH HORN AND DOT BELOW]
        case L'\u24C4': // [CIRCLED LATIN CAPITAL LETTER O]
        case L'\uA74A': // [LATIN CAPITAL LETTER O WITH LONG STROKE OVERLAY]
        case L'\uA74C': // [LATIN CAPITAL LETTER O WITH LOOP]
        case L'\uFF2F': // [FULLWIDTH LATIN CAPITAL LETTER O]
            *out ++ = 'O';
            break;
        case L'\u00F2': // [LATIN SMALL LETTER O WITH GRAVE]
        case L'\u00F3': // [LATIN SMALL LETTER O WITH ACUTE]
        case L'\u00F4': // [LATIN SMALL LETTER O WITH CIRCUMFLEX]
        case L'\u00F5': // [LATIN SMALL LETTER O WITH TILDE]
        case L'\u00F6': // [LATIN SMALL LETTER O WITH DIAERESIS]
        case L'\u00F8': // [LATIN SMALL LETTER O WITH STROKE]
        case L'\u014D': // [LATIN SMALL LETTER O WITH MACRON]
        case L'\u014F': // [LATIN SMALL LETTER O WITH BREVE]
        case L'\u0151': // [LATIN SMALL LETTER O WITH DOUBLE ACUTE]
        case L'\u01A1': // [LATIN SMALL LETTER O WITH HORN]
        case L'\u01D2': // [LATIN SMALL LETTER O WITH CARON]
        case L'\u01EB': // [LATIN SMALL LETTER O WITH OGONEK]
        case L'\u01ED': // [LATIN SMALL LETTER O WITH OGONEK AND MACRON]
        case L'\u01FF': // [LATIN SMALL LETTER O WITH STROKE AND ACUTE]
        case L'\u020D': // [LATIN SMALL LETTER O WITH DOUBLE GRAVE]
        case L'\u020F': // [LATIN SMALL LETTER O WITH INVERTED BREVE]
        case L'\u022B': // [LATIN SMALL LETTER O WITH DIAERESIS AND MACRON]
        case L'\u022D': // [LATIN SMALL LETTER O WITH TILDE AND MACRON]
        case L'\u022F': // [LATIN SMALL LETTER O WITH DOT ABOVE]
        case L'\u0231': // [LATIN SMALL LETTER O WITH DOT ABOVE AND MACRON]
        case L'\u0254': // [LATIN SMALL LETTER OPEN O]
        case L'\u0275': // [LATIN SMALL LETTER BARRED O]
        case L'\u1D16': // [LATIN SMALL LETTER TOP HALF O]
        case L'\u1D17': // [LATIN SMALL LETTER BOTTOM HALF O]
        case L'\u1D97': // [LATIN SMALL LETTER OPEN O WITH RETROFLEX HOOK]
        case L'\u1E4D': // [LATIN SMALL LETTER O WITH TILDE AND ACUTE]
        case L'\u1E4F': // [LATIN SMALL LETTER O WITH TILDE AND DIAERESIS]
        case L'\u1E51': // [LATIN SMALL LETTER O WITH MACRON AND GRAVE]
        case L'\u1E53': // [LATIN SMALL LETTER O WITH MACRON AND ACUTE]
        case L'\u1ECD': // [LATIN SMALL LETTER O WITH DOT BELOW]
        case L'\u1ECF': // [LATIN SMALL LETTER O WITH HOOK ABOVE]
        case L'\u1ED1': // [LATIN SMALL LETTER O WITH CIRCUMFLEX AND ACUTE]
        case L'\u1ED3': // [LATIN SMALL LETTER O WITH CIRCUMFLEX AND GRAVE]
        case L'\u1ED5': // [LATIN SMALL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE]
        case L'\u1ED7': // [LATIN SMALL LETTER O WITH CIRCUMFLEX AND TILDE]
        case L'\u1ED9': // [LATIN SMALL LETTER O WITH CIRCUMFLEX AND DOT BELOW]
        case L'\u1EDB': // [LATIN SMALL LETTER O WITH HORN AND ACUTE]
        case L'\u1EDD': // [LATIN SMALL LETTER O WITH HORN AND GRAVE]
        case L'\u1EDF': // [LATIN SMALL LETTER O WITH HORN AND HOOK ABOVE]
        case L'\u1EE1': // [LATIN SMALL LETTER O WITH HORN AND TILDE]
        case L'\u1EE3': // [LATIN SMALL LETTER O WITH HORN AND DOT BELOW]
        case L'\u2092': // [LATIN SUBSCRIPT SMALL LETTER O]
        case L'\u24DE': // [CIRCLED LATIN SMALL LETTER O]
        case L'\u2C7A': // [LATIN SMALL LETTER O WITH LOW RING INSIDE]
        case L'\uA74B': // [LATIN SMALL LETTER O WITH LONG STROKE OVERLAY]
        case L'\uA74D': // [LATIN SMALL LETTER O WITH LOOP]
        case L'\uFF4F': // [FULLWIDTH LATIN SMALL LETTER O]
            *out ++ = 'o';
            break;
        case L'\u0152': // [LATIN CAPITAL LIGATURE OE]
        case L'\u0276': // [LATIN LETTER SMALL CAPITAL OE]
            *out ++ = 'O';
            *out ++ = 'E';
            break;
        case L'\uA74E': // [LATIN CAPITAL LETTER OO]
            *out ++ = 'O';
            *out ++ = 'O';
            break;
        case L'\u0222': // [LATIN CAPITAL LETTER OU]
        case L'\u1D15': // [LATIN LETTER SMALL CAPITAL OU]
            *out ++ = 'O';
            *out ++ = 'U';
            break;
        case L'\u24AA': // [PARENTHESIZED LATIN SMALL LETTER O]
            *out ++ = '(';
            *out ++ = 'o';
            *out ++ = ')';
            break;
        case L'\u0153': // [LATIN SMALL LIGATURE OE]
        case L'\u1D14': // [LATIN SMALL LETTER TURNED OE]
            *out ++ = 'o';
            *out ++ = 'e';
            break;
        case L'\uA74F': // [LATIN SMALL LETTER OO]
            *out ++ = 'o';
            *out ++ = 'o';
            break;
        case L'\u0223': // [LATIN SMALL LETTER OU]
            *out ++ = 'o';
            *out ++ = 'u';
            break;
        case L'\u01A4': // [LATIN CAPITAL LETTER P WITH HOOK]
        case L'\u1D18': // [LATIN LETTER SMALL CAPITAL P]
        case L'\u1E54': // [LATIN CAPITAL LETTER P WITH ACUTE]
        case L'\u1E56': // [LATIN CAPITAL LETTER P WITH DOT ABOVE]
        case L'\u24C5': // [CIRCLED LATIN CAPITAL LETTER P]
        case L'\u2C63': // [LATIN CAPITAL LETTER P WITH STROKE]
        case L'\uA750': // [LATIN CAPITAL LETTER P WITH STROKE THROUGH DESCENDER]
        case L'\uA752': // [LATIN CAPITAL LETTER P WITH FLOURISH]
        case L'\uA754': // [LATIN CAPITAL LETTER P WITH SQUIRREL TAIL]
        case L'\uFF30': // [FULLWIDTH LATIN CAPITAL LETTER P]
            *out ++ = 'P';
            break;
        case L'\u01A5': // [LATIN SMALL LETTER P WITH HOOK]
        case L'\u1D71': // [LATIN SMALL LETTER P WITH MIDDLE TILDE]
        case L'\u1D7D': // [LATIN SMALL LETTER P WITH STROKE]
        case L'\u1D88': // [LATIN SMALL LETTER P WITH PALATAL HOOK]
        case L'\u1E55': // [LATIN SMALL LETTER P WITH ACUTE]
        case L'\u1E57': // [LATIN SMALL LETTER P WITH DOT ABOVE]
        case L'\u24DF': // [CIRCLED LATIN SMALL LETTER P]
        case L'\uA751': // [LATIN SMALL LETTER P WITH STROKE THROUGH DESCENDER]
        case L'\uA753': // [LATIN SMALL LETTER P WITH FLOURISH]
        case L'\uA755': // [LATIN SMALL LETTER P WITH SQUIRREL TAIL]
        case L'\uA7FC': // [LATIN EPIGRAPHIC LETTER REVERSED P]
        case L'\uFF50': // [FULLWIDTH LATIN SMALL LETTER P]
            *out ++ = 'p';
            break;
        case L'\u24AB': // [PARENTHESIZED LATIN SMALL LETTER P]
            *out ++ = '(';
            *out ++ = 'p';
            *out ++ = ')';
            break;
        case L'\u024A': // [LATIN CAPITAL LETTER SMALL Q WITH HOOK TAIL]
        case L'\u24C6': // [CIRCLED LATIN CAPITAL LETTER Q]
        case L'\uA756': // [LATIN CAPITAL LETTER Q WITH STROKE THROUGH DESCENDER]
        case L'\uA758': // [LATIN CAPITAL LETTER Q WITH DIAGONAL STROKE]
        case L'\uFF31': // [FULLWIDTH LATIN CAPITAL LETTER Q]
            *out ++ = 'Q';
            break;
        case L'\u0138': // [LATIN SMALL LETTER KRA]
        case L'\u024B': // [LATIN SMALL LETTER Q WITH HOOK TAIL]
        case L'\u02A0': // [LATIN SMALL LETTER Q WITH HOOK]
        case L'\u24E0': // [CIRCLED LATIN SMALL LETTER Q]
        case L'\uA757': // [LATIN SMALL LETTER Q WITH STROKE THROUGH DESCENDER]
        case L'\uA759': // [LATIN SMALL LETTER Q WITH DIAGONAL STROKE]
        case L'\uFF51': // [FULLWIDTH LATIN SMALL LETTER Q]
            *out ++ = 'q';
            break;
        case L'\u24AC': // [PARENTHESIZED LATIN SMALL LETTER Q]
            *out ++ = '(';
            *out ++ = 'q';
            *out ++ = ')';
            break;
        case L'\u0239': // [LATIN SMALL LETTER QP DIGRAPH]
            *out ++ = 'q';
            *out ++ = 'p';
            break;
        case L'\u0154': // [LATIN CAPITAL LETTER R WITH ACUTE]
        case L'\u0156': // [LATIN CAPITAL LETTER R WITH CEDILLA]
        case L'\u0158': // [LATIN CAPITAL LETTER R WITH CARON]
        case L'\u0210': // [LATIN CAPITAL LETTER R WITH DOUBLE GRAVE]
        case L'\u0212': // [LATIN CAPITAL LETTER R WITH INVERTED BREVE]
        case L'\u024C': // [LATIN CAPITAL LETTER R WITH STROKE]
        case L'\u0280': // [LATIN LETTER SMALL CAPITAL R]
        case L'\u0281': // [LATIN LETTER SMALL CAPITAL INVERTED R]
        case L'\u1D19': // [LATIN LETTER SMALL CAPITAL REVERSED R]
        case L'\u1D1A': // [LATIN LETTER SMALL CAPITAL TURNED R]
        case L'\u1E58': // [LATIN CAPITAL LETTER R WITH DOT ABOVE]
        case L'\u1E5A': // [LATIN CAPITAL LETTER R WITH DOT BELOW]
        case L'\u1E5C': // [LATIN CAPITAL LETTER R WITH DOT BELOW AND MACRON]
        case L'\u1E5E': // [LATIN CAPITAL LETTER R WITH LINE BELOW]
        case L'\u24C7': // [CIRCLED LATIN CAPITAL LETTER R]
        case L'\u2C64': // [LATIN CAPITAL LETTER R WITH TAIL]
        case L'\uA75A': // [LATIN CAPITAL LETTER R ROTUNDA]
        case L'\uA782': // [LATIN CAPITAL LETTER INSULAR R]
        case L'\uFF32': // [FULLWIDTH LATIN CAPITAL LETTER R]
            *out ++ = 'R';
            break;
        case L'\u0155': // [LATIN SMALL LETTER R WITH ACUTE]
        case L'\u0157': // [LATIN SMALL LETTER R WITH CEDILLA]
        case L'\u0159': // [LATIN SMALL LETTER R WITH CARON]
        case L'\u0211': // [LATIN SMALL LETTER R WITH DOUBLE GRAVE]
        case L'\u0213': // [LATIN SMALL LETTER R WITH INVERTED BREVE]
        case L'\u024D': // [LATIN SMALL LETTER R WITH STROKE]
        case L'\u027C': // [LATIN SMALL LETTER R WITH LONG LEG]
        case L'\u027D': // [LATIN SMALL LETTER R WITH TAIL]
        case L'\u027E': // [LATIN SMALL LETTER R WITH FISHHOOK]
        case L'\u027F': // [LATIN SMALL LETTER REVERSED R WITH FISHHOOK]
        case L'\u1D63': // [LATIN SUBSCRIPT SMALL LETTER R]
        case L'\u1D72': // [LATIN SMALL LETTER R WITH MIDDLE TILDE]
        case L'\u1D73': // [LATIN SMALL LETTER R WITH FISHHOOK AND MIDDLE TILDE]
        case L'\u1D89': // [LATIN SMALL LETTER R WITH PALATAL HOOK]
        case L'\u1E59': // [LATIN SMALL LETTER R WITH DOT ABOVE]
        case L'\u1E5B': // [LATIN SMALL LETTER R WITH DOT BELOW]
        case L'\u1E5D': // [LATIN SMALL LETTER R WITH DOT BELOW AND MACRON]
        case L'\u1E5F': // [LATIN SMALL LETTER R WITH LINE BELOW]
        case L'\u24E1': // [CIRCLED LATIN SMALL LETTER R]
        case L'\uA75B': // [LATIN SMALL LETTER R ROTUNDA]
        case L'\uA783': // [LATIN SMALL LETTER INSULAR R]
        case L'\uFF52': // [FULLWIDTH LATIN SMALL LETTER R]
            *out ++ = 'r';
            break;
        case L'\u24AD': // [PARENTHESIZED LATIN SMALL LETTER R]
            *out ++ = '(';
            *out ++ = 'r';
            *out ++ = ')';
            break;
        case L'\u015A': // [LATIN CAPITAL LETTER S WITH ACUTE]
        case L'\u015C': // [LATIN CAPITAL LETTER S WITH CIRCUMFLEX]
        case L'\u015E': // [LATIN CAPITAL LETTER S WITH CEDILLA]
        case L'\u0160': // [LATIN CAPITAL LETTER S WITH CARON]
        case L'\u0218': // [LATIN CAPITAL LETTER S WITH COMMA BELOW]
        case L'\u1E60': // [LATIN CAPITAL LETTER S WITH DOT ABOVE]
        case L'\u1E62': // [LATIN CAPITAL LETTER S WITH DOT BELOW]
        case L'\u1E64': // [LATIN CAPITAL LETTER S WITH ACUTE AND DOT ABOVE]
        case L'\u1E66': // [LATIN CAPITAL LETTER S WITH CARON AND DOT ABOVE]
        case L'\u1E68': // [LATIN CAPITAL LETTER S WITH DOT BELOW AND DOT ABOVE]
        case L'\u24C8': // [CIRCLED LATIN CAPITAL LETTER S]
        case L'\uA731': // [LATIN LETTER SMALL CAPITAL S]
        case L'\uA785': // [LATIN SMALL LETTER INSULAR S]
        case L'\uFF33': // [FULLWIDTH LATIN CAPITAL LETTER S]
            *out ++ = 'S';
            break;
        case L'\u015B': // [LATIN SMALL LETTER S WITH ACUTE]
        case L'\u015D': // [LATIN SMALL LETTER S WITH CIRCUMFLEX]
        case L'\u015F': // [LATIN SMALL LETTER S WITH CEDILLA]
        case L'\u0161': // [LATIN SMALL LETTER S WITH CARON]
        case L'\u017F': // [LATIN SMALL LETTER LONG S]
        case L'\u0219': // [LATIN SMALL LETTER S WITH COMMA BELOW]
        case L'\u023F': // [LATIN SMALL LETTER S WITH SWASH TAIL]
        case L'\u0282': // [LATIN SMALL LETTER S WITH HOOK]
        case L'\u1D74': // [LATIN SMALL LETTER S WITH MIDDLE TILDE]
        case L'\u1D8A': // [LATIN SMALL LETTER S WITH PALATAL HOOK]
        case L'\u1E61': // [LATIN SMALL LETTER S WITH DOT ABOVE]
        case L'\u1E63': // [LATIN SMALL LETTER S WITH DOT BELOW]
        case L'\u1E65': // [LATIN SMALL LETTER S WITH ACUTE AND DOT ABOVE]
        case L'\u1E67': // [LATIN SMALL LETTER S WITH CARON AND DOT ABOVE]
        case L'\u1E69': // [LATIN SMALL LETTER S WITH DOT BELOW AND DOT ABOVE]
        case L'\u1E9C': // [LATIN SMALL LETTER LONG S WITH DIAGONAL STROKE]
        case L'\u1E9D': // [LATIN SMALL LETTER LONG S WITH HIGH STROKE]
        case L'\u24E2': // [CIRCLED LATIN SMALL LETTER S]
        case L'\uA784': // [LATIN CAPITAL LETTER INSULAR S]
        case L'\uFF53': // [FULLWIDTH LATIN SMALL LETTER S]
            *out ++ = 's';
            break;
        case L'\u1E9E': // [LATIN CAPITAL LETTER SHARP S]
            *out ++ = 'S';
            *out ++ = 'S';
            break;
        case L'\u24AE': // [PARENTHESIZED LATIN SMALL LETTER S]
            *out ++ = '(';
            *out ++ = 's';
            *out ++ = ')';
            break;
        case L'\u00DF': // [LATIN SMALL LETTER SHARP S]
            *out ++ = 's';
            *out ++ = 's';
            break;
        case L'\uFB06': // [LATIN SMALL LIGATURE ST]
            *out ++ = 's';
            *out ++ = 't';
            break;
        case L'\u0162': // [LATIN CAPITAL LETTER T WITH CEDILLA]
        case L'\u0164': // [LATIN CAPITAL LETTER T WITH CARON]
        case L'\u0166': // [LATIN CAPITAL LETTER T WITH STROKE]
        case L'\u01AC': // [LATIN CAPITAL LETTER T WITH HOOK]
        case L'\u01AE': // [LATIN CAPITAL LETTER T WITH RETROFLEX HOOK]
        case L'\u021A': // [LATIN CAPITAL LETTER T WITH COMMA BELOW]
        case L'\u023E': // [LATIN CAPITAL LETTER T WITH DIAGONAL STROKE]
        case L'\u1D1B': // [LATIN LETTER SMALL CAPITAL T]
        case L'\u1E6A': // [LATIN CAPITAL LETTER T WITH DOT ABOVE]
        case L'\u1E6C': // [LATIN CAPITAL LETTER T WITH DOT BELOW]
        case L'\u1E6E': // [LATIN CAPITAL LETTER T WITH LINE BELOW]
        case L'\u1E70': // [LATIN CAPITAL LETTER T WITH CIRCUMFLEX BELOW]
        case L'\u24C9': // [CIRCLED LATIN CAPITAL LETTER T]
        case L'\uA786': // [LATIN CAPITAL LETTER INSULAR T]
        case L'\uFF34': // [FULLWIDTH LATIN CAPITAL LETTER T]
            *out ++ = 'T';
            break;
        case L'\u0163': // [LATIN SMALL LETTER T WITH CEDILLA]
        case L'\u0165': // [LATIN SMALL LETTER T WITH CARON]
        case L'\u0167': // [LATIN SMALL LETTER T WITH STROKE]
        case L'\u01AB': // [LATIN SMALL LETTER T WITH PALATAL HOOK]
        case L'\u01AD': // [LATIN SMALL LETTER T WITH HOOK]
        case L'\u021B': // [LATIN SMALL LETTER T WITH COMMA BELOW]
        case L'\u0236': // [LATIN SMALL LETTER T WITH CURL]
        case L'\u0287': // [LATIN SMALL LETTER TURNED T]
        case L'\u0288': // [LATIN SMALL LETTER T WITH RETROFLEX HOOK]
        case L'\u1D75': // [LATIN SMALL LETTER T WITH MIDDLE TILDE]
        case L'\u1E6B': // [LATIN SMALL LETTER T WITH DOT ABOVE]
        case L'\u1E6D': // [LATIN SMALL LETTER T WITH DOT BELOW]
        case L'\u1E6F': // [LATIN SMALL LETTER T WITH LINE BELOW]
        case L'\u1E71': // [LATIN SMALL LETTER T WITH CIRCUMFLEX BELOW]
        case L'\u1E97': // [LATIN SMALL LETTER T WITH DIAERESIS]
        case L'\u24E3': // [CIRCLED LATIN SMALL LETTER T]
        case L'\u2C66': // [LATIN SMALL LETTER T WITH DIAGONAL STROKE]
        case L'\uFF54': // [FULLWIDTH LATIN SMALL LETTER T]
            *out ++ = 't';
            break;
        case L'\u00DE': // [LATIN CAPITAL LETTER THORN]
        case L'\uA766': // [LATIN CAPITAL LETTER THORN WITH STROKE THROUGH DESCENDER]
            *out ++ = 'T';
            *out ++ = 'H';
            break;
        case L'\uA728': // [LATIN CAPITAL LETTER TZ]
            *out ++ = 'T';
            *out ++ = 'Z';
            break;
        case L'\u24AF': // [PARENTHESIZED LATIN SMALL LETTER T]
            *out ++ = '(';
            *out ++ = 't';
            *out ++ = ')';
            break;
        case L'\u02A8': // [LATIN SMALL LETTER TC DIGRAPH WITH CURL]
            *out ++ = 't';
            *out ++ = 'c';
            break;
        case L'\u00FE': // [LATIN SMALL LETTER THORN]
        case L'\u1D7A': // [LATIN SMALL LETTER TH WITH STRIKETHROUGH]
        case L'\uA767': // [LATIN SMALL LETTER THORN WITH STROKE THROUGH DESCENDER]
            *out ++ = 't';
            *out ++ = 'h';
            break;
        case L'\u02A6': // [LATIN SMALL LETTER TS DIGRAPH]
            *out ++ = 't';
            *out ++ = 's';
            break;
        case L'\uA729': // [LATIN SMALL LETTER TZ]
            *out ++ = 't';
            *out ++ = 'z';
            break;
        case L'\u00D9': // [LATIN CAPITAL LETTER U WITH GRAVE]
        case L'\u00DA': // [LATIN CAPITAL LETTER U WITH ACUTE]
        case L'\u00DB': // [LATIN CAPITAL LETTER U WITH CIRCUMFLEX]
        case L'\u00DC': // [LATIN CAPITAL LETTER U WITH DIAERESIS]
        case L'\u0168': // [LATIN CAPITAL LETTER U WITH TILDE]
        case L'\u016A': // [LATIN CAPITAL LETTER U WITH MACRON]
        case L'\u016C': // [LATIN CAPITAL LETTER U WITH BREVE]
        case L'\u016E': // [LATIN CAPITAL LETTER U WITH RING ABOVE]
        case L'\u0170': // [LATIN CAPITAL LETTER U WITH DOUBLE ACUTE]
        case L'\u0172': // [LATIN CAPITAL LETTER U WITH OGONEK]
        case L'\u01AF': // [LATIN CAPITAL LETTER U WITH HORN]
        case L'\u01D3': // [LATIN CAPITAL LETTER U WITH CARON]
        case L'\u01D5': // [LATIN CAPITAL LETTER U WITH DIAERESIS AND MACRON]
        case L'\u01D7': // [LATIN CAPITAL LETTER U WITH DIAERESIS AND ACUTE]
        case L'\u01D9': // [LATIN CAPITAL LETTER U WITH DIAERESIS AND CARON]
        case L'\u01DB': // [LATIN CAPITAL LETTER U WITH DIAERESIS AND GRAVE]
        case L'\u0214': // [LATIN CAPITAL LETTER U WITH DOUBLE GRAVE]
        case L'\u0216': // [LATIN CAPITAL LETTER U WITH INVERTED BREVE]
        case L'\u0244': // [LATIN CAPITAL LETTER U BAR]
        case L'\u1D1C': // [LATIN LETTER SMALL CAPITAL U]
        case L'\u1D7E': // [LATIN SMALL CAPITAL LETTER U WITH STROKE]
        case L'\u1E72': // [LATIN CAPITAL LETTER U WITH DIAERESIS BELOW]
        case L'\u1E74': // [LATIN CAPITAL LETTER U WITH TILDE BELOW]
        case L'\u1E76': // [LATIN CAPITAL LETTER U WITH CIRCUMFLEX BELOW]
        case L'\u1E78': // [LATIN CAPITAL LETTER U WITH TILDE AND ACUTE]
        case L'\u1E7A': // [LATIN CAPITAL LETTER U WITH MACRON AND DIAERESIS]
        case L'\u1EE4': // [LATIN CAPITAL LETTER U WITH DOT BELOW]
        case L'\u1EE6': // [LATIN CAPITAL LETTER U WITH HOOK ABOVE]
        case L'\u1EE8': // [LATIN CAPITAL LETTER U WITH HORN AND ACUTE]
        case L'\u1EEA': // [LATIN CAPITAL LETTER U WITH HORN AND GRAVE]
        case L'\u1EEC': // [LATIN CAPITAL LETTER U WITH HORN AND HOOK ABOVE]
        case L'\u1EEE': // [LATIN CAPITAL LETTER U WITH HORN AND TILDE]
        case L'\u1EF0': // [LATIN CAPITAL LETTER U WITH HORN AND DOT BELOW]
        case L'\u24CA': // [CIRCLED LATIN CAPITAL LETTER U]
        case L'\uFF35': // [FULLWIDTH LATIN CAPITAL LETTER U]
            *out ++ = 'U';
            break;
        case L'\u00F9': // [LATIN SMALL LETTER U WITH GRAVE]
        case L'\u00FA': // [LATIN SMALL LETTER U WITH ACUTE]
        case L'\u00FB': // [LATIN SMALL LETTER U WITH CIRCUMFLEX]
        case L'\u00FC': // [LATIN SMALL LETTER U WITH DIAERESIS]
        case L'\u0169': // [LATIN SMALL LETTER U WITH TILDE]
        case L'\u016B': // [LATIN SMALL LETTER U WITH MACRON]
        case L'\u016D': // [LATIN SMALL LETTER U WITH BREVE]
        case L'\u016F': // [LATIN SMALL LETTER U WITH RING ABOVE]
        case L'\u0171': // [LATIN SMALL LETTER U WITH DOUBLE ACUTE]
        case L'\u0173': // [LATIN SMALL LETTER U WITH OGONEK]
        case L'\u01B0': // [LATIN SMALL LETTER U WITH HORN]
        case L'\u01D4': // [LATIN SMALL LETTER U WITH CARON]
        case L'\u01D6': // [LATIN SMALL LETTER U WITH DIAERESIS AND MACRON]
        case L'\u01D8': // [LATIN SMALL LETTER U WITH DIAERESIS AND ACUTE]
        case L'\u01DA': // [LATIN SMALL LETTER U WITH DIAERESIS AND CARON]
        case L'\u01DC': // [LATIN SMALL LETTER U WITH DIAERESIS AND GRAVE]
        case L'\u0215': // [LATIN SMALL LETTER U WITH DOUBLE GRAVE]
        case L'\u0217': // [LATIN SMALL LETTER U WITH INVERTED BREVE]
        case L'\u0289': // [LATIN SMALL LETTER U BAR]
        case L'\u1D64': // [LATIN SUBSCRIPT SMALL LETTER U]
        case L'\u1D99': // [LATIN SMALL LETTER U WITH RETROFLEX HOOK]
        case L'\u1E73': // [LATIN SMALL LETTER U WITH DIAERESIS BELOW]
        case L'\u1E75': // [LATIN SMALL LETTER U WITH TILDE BELOW]
        case L'\u1E77': // [LATIN SMALL LETTER U WITH CIRCUMFLEX BELOW]
        case L'\u1E79': // [LATIN SMALL LETTER U WITH TILDE AND ACUTE]
        case L'\u1E7B': // [LATIN SMALL LETTER U WITH MACRON AND DIAERESIS]
        case L'\u1EE5': // [LATIN SMALL LETTER U WITH DOT BELOW]
        case L'\u1EE7': // [LATIN SMALL LETTER U WITH HOOK ABOVE]
        case L'\u1EE9': // [LATIN SMALL LETTER U WITH HORN AND ACUTE]
        case L'\u1EEB': // [LATIN SMALL LETTER U WITH HORN AND GRAVE]
        case L'\u1EED': // [LATIN SMALL LETTER U WITH HORN AND HOOK ABOVE]
        case L'\u1EEF': // [LATIN SMALL LETTER U WITH HORN AND TILDE]
        case L'\u1EF1': // [LATIN SMALL LETTER U WITH HORN AND DOT BELOW]
        case L'\u24E4': // [CIRCLED LATIN SMALL LETTER U]
        case L'\uFF55': // [FULLWIDTH LATIN SMALL LETTER U]
            *out ++ = 'u';
            break;
        case L'\u24B0': // [PARENTHESIZED LATIN SMALL LETTER U]
            *out ++ = '(';
            *out ++ = 'u';
            *out ++ = ')';
            break;
        case L'\u1D6B': // [LATIN SMALL LETTER UE]
            *out ++ = 'u';
            *out ++ = 'e';
            break;
        case L'\u01B2': // [LATIN CAPITAL LETTER V WITH HOOK]
        case L'\u0245': // [LATIN CAPITAL LETTER TURNED V]
        case L'\u1D20': // [LATIN LETTER SMALL CAPITAL V]
        case L'\u1E7C': // [LATIN CAPITAL LETTER V WITH TILDE]
        case L'\u1E7E': // [LATIN CAPITAL LETTER V WITH DOT BELOW]
        case L'\u1EFC': // [LATIN CAPITAL LETTER MIDDLE-WELSH V]
        case L'\u24CB': // [CIRCLED LATIN CAPITAL LETTER V]
        case L'\uA75E': // [LATIN CAPITAL LETTER V WITH DIAGONAL STROKE]
        case L'\uA768': // [LATIN CAPITAL LETTER VEND]
        case L'\uFF36': // [FULLWIDTH LATIN CAPITAL LETTER V]
            *out ++ = 'V';
            break;
        case L'\u028B': // [LATIN SMALL LETTER V WITH HOOK]
        case L'\u028C': // [LATIN SMALL LETTER TURNED V]
        case L'\u1D65': // [LATIN SUBSCRIPT SMALL LETTER V]
        case L'\u1D8C': // [LATIN SMALL LETTER V WITH PALATAL HOOK]
        case L'\u1E7D': // [LATIN SMALL LETTER V WITH TILDE]
        case L'\u1E7F': // [LATIN SMALL LETTER V WITH DOT BELOW]
        case L'\u24E5': // [CIRCLED LATIN SMALL LETTER V]
        case L'\u2C71': // [LATIN SMALL LETTER V WITH RIGHT HOOK]
        case L'\u2C74': // [LATIN SMALL LETTER V WITH CURL]
        case L'\uA75F': // [LATIN SMALL LETTER V WITH DIAGONAL STROKE]
        case L'\uFF56': // [FULLWIDTH LATIN SMALL LETTER V]
            *out ++ = 'v';
            break;
        case L'\uA760': // [LATIN CAPITAL LETTER VY]
            *out ++ = 'V';
            *out ++ = 'Y';
            break;
        case L'\u24B1': // [PARENTHESIZED LATIN SMALL LETTER V]
            *out ++ = '(';
            *out ++ = 'v';
            *out ++ = ')';
            break;
        case L'\uA761': // [LATIN SMALL LETTER VY]
            *out ++ = 'v';
            *out ++ = 'y';
            break;
        case L'\u0174': // [LATIN CAPITAL LETTER W WITH CIRCUMFLEX]
        case L'\u01F7': // [LATIN CAPITAL LETTER WYNN]
        case L'\u1D21': // [LATIN LETTER SMALL CAPITAL W]
        case L'\u1E80': // [LATIN CAPITAL LETTER W WITH GRAVE]
        case L'\u1E82': // [LATIN CAPITAL LETTER W WITH ACUTE]
        case L'\u1E84': // [LATIN CAPITAL LETTER W WITH DIAERESIS]
        case L'\u1E86': // [LATIN CAPITAL LETTER W WITH DOT ABOVE]
        case L'\u1E88': // [LATIN CAPITAL LETTER W WITH DOT BELOW]
        case L'\u24CC': // [CIRCLED LATIN CAPITAL LETTER W]
        case L'\u2C72': // [LATIN CAPITAL LETTER W WITH HOOK]
        case L'\uFF37': // [FULLWIDTH LATIN CAPITAL LETTER W]
            *out ++ = 'W';
            break;
        case L'\u0175': // [LATIN SMALL LETTER W WITH CIRCUMFLEX]
        case L'\u01BF': // [LATIN LETTER WYNN]
        case L'\u028D': // [LATIN SMALL LETTER TURNED W]
        case L'\u1E81': // [LATIN SMALL LETTER W WITH GRAVE]
        case L'\u1E83': // [LATIN SMALL LETTER W WITH ACUTE]
        case L'\u1E85': // [LATIN SMALL LETTER W WITH DIAERESIS]
        case L'\u1E87': // [LATIN SMALL LETTER W WITH DOT ABOVE]
        case L'\u1E89': // [LATIN SMALL LETTER W WITH DOT BELOW]
        case L'\u1E98': // [LATIN SMALL LETTER W WITH RING ABOVE]
        case L'\u24E6': // [CIRCLED LATIN SMALL LETTER W]
        case L'\u2C73': // [LATIN SMALL LETTER W WITH HOOK]
        case L'\uFF57': // [FULLWIDTH LATIN SMALL LETTER W]
            *out ++ = 'w';
            break;
        case L'\u24B2': // [PARENTHESIZED LATIN SMALL LETTER W]
            *out ++ = '(';
            *out ++ = 'w';
            *out ++ = ')';
            break;
        case L'\u1E8A': // [LATIN CAPITAL LETTER X WITH DOT ABOVE]
        case L'\u1E8C': // [LATIN CAPITAL LETTER X WITH DIAERESIS]
        case L'\u24CD': // [CIRCLED LATIN CAPITAL LETTER X]
        case L'\uFF38': // [FULLWIDTH LATIN CAPITAL LETTER X]
            *out ++ = 'X';
            break;
        case L'\u1D8D': // [LATIN SMALL LETTER X WITH PALATAL HOOK]
        case L'\u1E8B': // [LATIN SMALL LETTER X WITH DOT ABOVE]
        case L'\u1E8D': // [LATIN SMALL LETTER X WITH DIAERESIS]
        case L'\u2093': // [LATIN SUBSCRIPT SMALL LETTER X]
        case L'\u24E7': // [CIRCLED LATIN SMALL LETTER X]
        case L'\uFF58': // [FULLWIDTH LATIN SMALL LETTER X]
            *out ++ = 'x';
            break;
        case L'\u24B3': // [PARENTHESIZED LATIN SMALL LETTER X]
            *out ++ = '(';
            *out ++ = 'x';
            *out ++ = ')';
            break;
        case L'\u00DD': // [LATIN CAPITAL LETTER Y WITH ACUTE]
        case L'\u0176': // [LATIN CAPITAL LETTER Y WITH CIRCUMFLEX]
        case L'\u0178': // [LATIN CAPITAL LETTER Y WITH DIAERESIS]
        case L'\u01B3': // [LATIN CAPITAL LETTER Y WITH HOOK]
        case L'\u0232': // [LATIN CAPITAL LETTER Y WITH MACRON]
        case L'\u024E': // [LATIN CAPITAL LETTER Y WITH STROKE]
        case L'\u028F': // [LATIN LETTER SMALL CAPITAL Y]
        case L'\u1E8E': // [LATIN CAPITAL LETTER Y WITH DOT ABOVE]
        case L'\u1EF2': // [LATIN CAPITAL LETTER Y WITH GRAVE]
        case L'\u1EF4': // [LATIN CAPITAL LETTER Y WITH DOT BELOW]
        case L'\u1EF6': // [LATIN CAPITAL LETTER Y WITH HOOK ABOVE]
        case L'\u1EF8': // [LATIN CAPITAL LETTER Y WITH TILDE]
        case L'\u1EFE': // [LATIN CAPITAL LETTER Y WITH LOOP]
        case L'\u24CE': // [CIRCLED LATIN CAPITAL LETTER Y]
        case L'\uFF39': // [FULLWIDTH LATIN CAPITAL LETTER Y]
            *out ++ = 'Y';
            break;
        case L'\u00FD': // [LATIN SMALL LETTER Y WITH ACUTE]
        case L'\u00FF': // [LATIN SMALL LETTER Y WITH DIAERESIS]
        case L'\u0177': // [LATIN SMALL LETTER Y WITH CIRCUMFLEX]
        case L'\u01B4': // [LATIN SMALL LETTER Y WITH HOOK]
        case L'\u0233': // [LATIN SMALL LETTER Y WITH MACRON]
        case L'\u024F': // [LATIN SMALL LETTER Y WITH STROKE]
        case L'\u028E': // [LATIN SMALL LETTER TURNED Y]
        case L'\u1E8F': // [LATIN SMALL LETTER Y WITH DOT ABOVE]
        case L'\u1E99': // [LATIN SMALL LETTER Y WITH RING ABOVE]
        case L'\u1EF3': // [LATIN SMALL LETTER Y WITH GRAVE]
        case L'\u1EF5': // [LATIN SMALL LETTER Y WITH DOT BELOW]
        case L'\u1EF7': // [LATIN SMALL LETTER Y WITH HOOK ABOVE]
        case L'\u1EF9': // [LATIN SMALL LETTER Y WITH TILDE]
        case L'\u1EFF': // [LATIN SMALL LETTER Y WITH LOOP]
        case L'\u24E8': // [CIRCLED LATIN SMALL LETTER Y]
        case L'\uFF59': // [FULLWIDTH LATIN SMALL LETTER Y]
            *out ++ = 'y';
            break;
        case L'\u24B4': // [PARENTHESIZED LATIN SMALL LETTER Y]
            *out ++ = '(';
            *out ++ = 'y';
            *out ++ = ')';
            break;
        case L'\u0179': // [LATIN CAPITAL LETTER Z WITH ACUTE]
        case L'\u017B': // [LATIN CAPITAL LETTER Z WITH DOT ABOVE]
        case L'\u017D': // [LATIN CAPITAL LETTER Z WITH CARON]
        case L'\u01B5': // [LATIN CAPITAL LETTER Z WITH STROKE]
        case L'\u021C': // [LATIN CAPITAL LETTER YOGH]
        case L'\u0224': // [LATIN CAPITAL LETTER Z WITH HOOK]
        case L'\u1D22': // [LATIN LETTER SMALL CAPITAL Z]
        case L'\u1E90': // [LATIN CAPITAL LETTER Z WITH CIRCUMFLEX]
        case L'\u1E92': // [LATIN CAPITAL LETTER Z WITH DOT BELOW]
        case L'\u1E94': // [LATIN CAPITAL LETTER Z WITH LINE BELOW]
        case L'\u24CF': // [CIRCLED LATIN CAPITAL LETTER Z]
        case L'\u2C6B': // [LATIN CAPITAL LETTER Z WITH DESCENDER]
        case L'\uA762': // [LATIN CAPITAL LETTER VISIGOTHIC Z]
        case L'\uFF3A': // [FULLWIDTH LATIN CAPITAL LETTER Z]
            *out ++ = 'Z';
            break;
        case L'\u017A': // [LATIN SMALL LETTER Z WITH ACUTE]
        case L'\u017C': // [LATIN SMALL LETTER Z WITH DOT ABOVE]
        case L'\u017E': // [LATIN SMALL LETTER Z WITH CARON]
        case L'\u01B6': // [LATIN SMALL LETTER Z WITH STROKE]
        case L'\u021D': // [LATIN SMALL LETTER YOGH]
        case L'\u0225': // [LATIN SMALL LETTER Z WITH HOOK]
        case L'\u0240': // [LATIN SMALL LETTER Z WITH SWASH TAIL]
        case L'\u0290': // [LATIN SMALL LETTER Z WITH RETROFLEX HOOK]
        case L'\u0291': // [LATIN SMALL LETTER Z WITH CURL]
        case L'\u1D76': // [LATIN SMALL LETTER Z WITH MIDDLE TILDE]
        case L'\u1D8E': // [LATIN SMALL LETTER Z WITH PALATAL HOOK]
        case L'\u1E91': // [LATIN SMALL LETTER Z WITH CIRCUMFLEX]
        case L'\u1E93': // [LATIN SMALL LETTER Z WITH DOT BELOW]
        case L'\u1E95': // [LATIN SMALL LETTER Z WITH LINE BELOW]
        case L'\u24E9': // [CIRCLED LATIN SMALL LETTER Z]
        case L'\u2C6C': // [LATIN SMALL LETTER Z WITH DESCENDER]
        case L'\uA763': // [LATIN SMALL LETTER VISIGOTHIC Z]
        case L'\uFF5A': // [FULLWIDTH LATIN SMALL LETTER Z]
            *out ++ = 'z';
            break;
        case L'\u24B5': // [PARENTHESIZED LATIN SMALL LETTER Z]
            *out ++ = '(';
            *out ++ = 'z';
            *out ++ = ')';
            break;
        case L'\u2070': // [SUPERSCRIPT ZERO]
        case L'\u2080': // [SUBSCRIPT ZERO]
        case L'\u24EA': // [CIRCLED DIGIT ZERO]
        case L'\u24FF': // [NEGATIVE CIRCLED DIGIT ZERO]
        case L'\uFF10': // [FULLWIDTH DIGIT ZERO]
            *out ++ = '0';
            break;
        case L'\u00B9': // [SUPERSCRIPT ONE]
        case L'\u2081': // [SUBSCRIPT ONE]
        case L'\u2460': // [CIRCLED DIGIT ONE]
        case L'\u24F5': // [DOUBLE CIRCLED DIGIT ONE]
        case L'\u2776': // [DINGBAT NEGATIVE CIRCLED DIGIT ONE]
        case L'\u2780': // [DINGBAT CIRCLED SANS-SERIF DIGIT ONE]
        case L'\u278A': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT ONE]
        case L'\uFF11': // [FULLWIDTH DIGIT ONE]
            *out ++ = '1';
            break;
        case L'\u2488': // [DIGIT ONE FULL STOP]
            *out ++ = '1';
            *out ++ = '.';
            break;
        case L'\u2474': // [PARENTHESIZED DIGIT ONE]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = ')';
            break;
        case L'\u00B2': // [SUPERSCRIPT TWO]
        case L'\u2082': // [SUBSCRIPT TWO]
        case L'\u2461': // [CIRCLED DIGIT TWO]
        case L'\u24F6': // [DOUBLE CIRCLED DIGIT TWO]
        case L'\u2777': // [DINGBAT NEGATIVE CIRCLED DIGIT TWO]
        case L'\u2781': // [DINGBAT CIRCLED SANS-SERIF DIGIT TWO]
        case L'\u278B': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT TWO]
        case L'\uFF12': // [FULLWIDTH DIGIT TWO]
            *out ++ = '2';
            break;
        case L'\u2489': // [DIGIT TWO FULL STOP]
            *out ++ = '2';
            *out ++ = '.';
            break;
        case L'\u2475': // [PARENTHESIZED DIGIT TWO]
            *out ++ = '(';
            *out ++ = '2';
            *out ++ = ')';
            break;
        case L'\u00B3': // [SUPERSCRIPT THREE]
        case L'\u2083': // [SUBSCRIPT THREE]
        case L'\u2462': // [CIRCLED DIGIT THREE]
        case L'\u24F7': // [DOUBLE CIRCLED DIGIT THREE]
        case L'\u2778': // [DINGBAT NEGATIVE CIRCLED DIGIT THREE]
        case L'\u2782': // [DINGBAT CIRCLED SANS-SERIF DIGIT THREE]
        case L'\u278C': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT THREE]
        case L'\uFF13': // [FULLWIDTH DIGIT THREE]
            *out ++ = '3';
            break;
        case L'\u248A': // [DIGIT THREE FULL STOP]
            *out ++ = '3';
            *out ++ = '.';
            break;
        case L'\u2476': // [PARENTHESIZED DIGIT THREE]
            *out ++ = '(';
            *out ++ = '3';
            *out ++ = ')';
            break;
        case L'\u2074': // [SUPERSCRIPT FOUR]
        case L'\u2084': // [SUBSCRIPT FOUR]
        case L'\u2463': // [CIRCLED DIGIT FOUR]
        case L'\u24F8': // [DOUBLE CIRCLED DIGIT FOUR]
        case L'\u2779': // [DINGBAT NEGATIVE CIRCLED DIGIT FOUR]
        case L'\u2783': // [DINGBAT CIRCLED SANS-SERIF DIGIT FOUR]
        case L'\u278D': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FOUR]
        case L'\uFF14': // [FULLWIDTH DIGIT FOUR]
            *out ++ = '4';
            break;
        case L'\u248B': // [DIGIT FOUR FULL STOP]
            *out ++ = '4';
            *out ++ = '.';
            break;
        case L'\u2477': // [PARENTHESIZED DIGIT FOUR]
            *out ++ = '(';
            *out ++ = '4';
            *out ++ = ')';
            break;
        case L'\u2075': // [SUPERSCRIPT FIVE]
        case L'\u2085': // [SUBSCRIPT FIVE]
        case L'\u2464': // [CIRCLED DIGIT FIVE]
        case L'\u24F9': // [DOUBLE CIRCLED DIGIT FIVE]
        case L'\u277A': // [DINGBAT NEGATIVE CIRCLED DIGIT FIVE]
        case L'\u2784': // [DINGBAT CIRCLED SANS-SERIF DIGIT FIVE]
        case L'\u278E': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FIVE]
        case L'\uFF15': // [FULLWIDTH DIGIT FIVE]
            *out ++ = '5';
            break;
        case L'\u248C': // [DIGIT FIVE FULL STOP]
            *out ++ = '5';
            *out ++ = '.';
            break;
        case L'\u2478': // [PARENTHESIZED DIGIT FIVE]
            *out ++ = '(';
            *out ++ = '5';
            *out ++ = ')';
            break;
        case L'\u2076': // [SUPERSCRIPT SIX]
        case L'\u2086': // [SUBSCRIPT SIX]
        case L'\u2465': // [CIRCLED DIGIT SIX]
        case L'\u24FA': // [DOUBLE CIRCLED DIGIT SIX]
        case L'\u277B': // [DINGBAT NEGATIVE CIRCLED DIGIT SIX]
        case L'\u2785': // [DINGBAT CIRCLED SANS-SERIF DIGIT SIX]
        case L'\u278F': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SIX]
        case L'\uFF16': // [FULLWIDTH DIGIT SIX]
            *out ++ = '6';
            break;
        case L'\u248D': // [DIGIT SIX FULL STOP]
            *out ++ = '6';
            *out ++ = '.';
            break;
        case L'\u2479': // [PARENTHESIZED DIGIT SIX]
            *out ++ = '(';
            *out ++ = '6';
            *out ++ = ')';
            break;
        case L'\u2077': // [SUPERSCRIPT SEVEN]
        case L'\u2087': // [SUBSCRIPT SEVEN]
        case L'\u2466': // [CIRCLED DIGIT SEVEN]
        case L'\u24FB': // [DOUBLE CIRCLED DIGIT SEVEN]
        case L'\u277C': // [DINGBAT NEGATIVE CIRCLED DIGIT SEVEN]
        case L'\u2786': // [DINGBAT CIRCLED SANS-SERIF DIGIT SEVEN]
        case L'\u2790': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SEVEN]
        case L'\uFF17': // [FULLWIDTH DIGIT SEVEN]
            *out ++ = '7';
            break;
        case L'\u248E': // [DIGIT SEVEN FULL STOP]
            *out ++ = '7';
            *out ++ = '.';
            break;
        case L'\u247A': // [PARENTHESIZED DIGIT SEVEN]
            *out ++ = '(';
            *out ++ = '7';
            *out ++ = ')';
            break;
        case L'\u2078': // [SUPERSCRIPT EIGHT]
        case L'\u2088': // [SUBSCRIPT EIGHT]
        case L'\u2467': // [CIRCLED DIGIT EIGHT]
        case L'\u24FC': // [DOUBLE CIRCLED DIGIT EIGHT]
        case L'\u277D': // [DINGBAT NEGATIVE CIRCLED DIGIT EIGHT]
        case L'\u2787': // [DINGBAT CIRCLED SANS-SERIF DIGIT EIGHT]
        case L'\u2791': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT EIGHT]
        case L'\uFF18': // [FULLWIDTH DIGIT EIGHT]
            *out ++ = '8';
            break;
        case L'\u248F': // [DIGIT EIGHT FULL STOP]
            *out ++ = '8';
            *out ++ = '.';
            break;
        case L'\u247B': // [PARENTHESIZED DIGIT EIGHT]
            *out ++ = '(';
            *out ++ = '8';
            *out ++ = ')';
            break;
        case L'\u2079': // [SUPERSCRIPT NINE]
        case L'\u2089': // [SUBSCRIPT NINE]
        case L'\u2468': // [CIRCLED DIGIT NINE]
        case L'\u24FD': // [DOUBLE CIRCLED DIGIT NINE]
        case L'\u277E': // [DINGBAT NEGATIVE CIRCLED DIGIT NINE]
        case L'\u2788': // [DINGBAT CIRCLED SANS-SERIF DIGIT NINE]
        case L'\u2792': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT NINE]
        case L'\uFF19': // [FULLWIDTH DIGIT NINE]
            *out ++ = '9';
            break;
        case L'\u2490': // [DIGIT NINE FULL STOP]
            *out ++ = '9';
            *out ++ = '.';
            break;
        case L'\u247C': // [PARENTHESIZED DIGIT NINE]
            *out ++ = '(';
            *out ++ = '9';
            *out ++ = ')';
            break;
        case L'\u2469': // [CIRCLED NUMBER TEN]
        case L'\u24FE': // [DOUBLE CIRCLED NUMBER TEN]
        case L'\u277F': // [DINGBAT NEGATIVE CIRCLED NUMBER TEN]
        case L'\u2789': // [DINGBAT CIRCLED SANS-SERIF NUMBER TEN]
        case L'\u2793': // [DINGBAT NEGATIVE CIRCLED SANS-SERIF NUMBER TEN]
            *out ++ = '1';
            *out ++ = '0';
            break;
        case L'\u2491': // [NUMBER TEN FULL STOP]
            *out ++ = '1';
            *out ++ = '0';
            *out ++ = '.';
            break;
        case L'\u247D': // [PARENTHESIZED NUMBER TEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '0';
            *out ++ = ')';
            break;
        case L'\u246A': // [CIRCLED NUMBER ELEVEN]
        case L'\u24EB': // [NEGATIVE CIRCLED NUMBER ELEVEN]
            *out ++ = '1';
            *out ++ = '1';
            break;
        case L'\u2492': // [NUMBER ELEVEN FULL STOP]
            *out ++ = '1';
            *out ++ = '1';
            *out ++ = '.';
            break;
        case L'\u247E': // [PARENTHESIZED NUMBER ELEVEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '1';
            *out ++ = ')';
            break;
        case L'\u246B': // [CIRCLED NUMBER TWELVE]
        case L'\u24EC': // [NEGATIVE CIRCLED NUMBER TWELVE]
            *out ++ = '1';
            *out ++ = '2';
            break;
        case L'\u2493': // [NUMBER TWELVE FULL STOP]
            *out ++ = '1';
            *out ++ = '2';
            *out ++ = '.';
            break;
        case L'\u247F': // [PARENTHESIZED NUMBER TWELVE]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '2';
            *out ++ = ')';
            break;
        case L'\u246C': // [CIRCLED NUMBER THIRTEEN]
        case L'\u24ED': // [NEGATIVE CIRCLED NUMBER THIRTEEN]
            *out ++ = '1';
            *out ++ = '3';
            break;
        case L'\u2494': // [NUMBER THIRTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '3';
            *out ++ = '.';
            break;
        case L'\u2480': // [PARENTHESIZED NUMBER THIRTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '3';
            *out ++ = ')';
            break;
        case L'\u246D': // [CIRCLED NUMBER FOURTEEN]
        case L'\u24EE': // [NEGATIVE CIRCLED NUMBER FOURTEEN]
            *out ++ = '1';
            *out ++ = '4';
            break;
        case L'\u2495': // [NUMBER FOURTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '4';
            *out ++ = '.';
            break;
        case L'\u2481': // [PARENTHESIZED NUMBER FOURTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '4';
            *out ++ = ')';
            break;
        case L'\u246E': // [CIRCLED NUMBER FIFTEEN]
        case L'\u24EF': // [NEGATIVE CIRCLED NUMBER FIFTEEN]
            *out ++ = '1';
            *out ++ = '5';
            break;
        case L'\u2496': // [NUMBER FIFTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '5';
            *out ++ = '.';
            break;
        case L'\u2482': // [PARENTHESIZED NUMBER FIFTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '5';
            *out ++ = ')';
            break;
        case L'\u246F': // [CIRCLED NUMBER SIXTEEN]
        case L'\u24F0': // [NEGATIVE CIRCLED NUMBER SIXTEEN]
            *out ++ = '1';
            *out ++ = '6';
            break;
        case L'\u2497': // [NUMBER SIXTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '6';
            *out ++ = '.';
            break;
        case L'\u2483': // [PARENTHESIZED NUMBER SIXTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '6';
            *out ++ = ')';
            break;
        case L'\u2470': // [CIRCLED NUMBER SEVENTEEN]
        case L'\u24F1': // [NEGATIVE CIRCLED NUMBER SEVENTEEN]
            *out ++ = '1';
            *out ++ = '7';
            break;
        case L'\u2498': // [NUMBER SEVENTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '7';
            *out ++ = '.';
            break;
        case L'\u2484': // [PARENTHESIZED NUMBER SEVENTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '7';
            *out ++ = ')';
            break;
        case L'\u2471': // [CIRCLED NUMBER EIGHTEEN]
        case L'\u24F2': // [NEGATIVE CIRCLED NUMBER EIGHTEEN]
            *out ++ = '1';
            *out ++ = '8';
            break;
        case L'\u2499': // [NUMBER EIGHTEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '8';
            *out ++ = '.';
            break;
        case L'\u2485': // [PARENTHESIZED NUMBER EIGHTEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '8';
            *out ++ = ')';
            break;
        case L'\u2472': // [CIRCLED NUMBER NINETEEN]
        case L'\u24F3': // [NEGATIVE CIRCLED NUMBER NINETEEN]
            *out ++ = '1';
            *out ++ = '9';
            break;
        case L'\u249A': // [NUMBER NINETEEN FULL STOP]
            *out ++ = '1';
            *out ++ = '9';
            *out ++ = '.';
            break;
        case L'\u2486': // [PARENTHESIZED NUMBER NINETEEN]
            *out ++ = '(';
            *out ++ = '1';
            *out ++ = '9';
            *out ++ = ')';
            break;
        case L'\u2473': // [CIRCLED NUMBER TWENTY]
        case L'\u24F4': // [NEGATIVE CIRCLED NUMBER TWENTY]
            *out ++ = '2';
            *out ++ = '0';
            break;
        case L'\u249B': // [NUMBER TWENTY FULL STOP]
            *out ++ = '2';
            *out ++ = '0';
            *out ++ = '.';
            break;
        case L'\u2487': // [PARENTHESIZED NUMBER TWENTY]
            *out ++ = '(';
            *out ++ = '2';
            *out ++ = '0';
            *out ++ = ')';
            break;
        case L'\u00AB': // [LEFT-POINTING DOUBLE ANGLE QUOTATION MARK]
        case L'\u00BB': // [RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK]
        case L'\u201C': // [LEFT DOUBLE QUOTATION MARK]
        case L'\u201D': // [RIGHT DOUBLE QUOTATION MARK]
        case L'\u201E': // [DOUBLE LOW-9 QUOTATION MARK]
        case L'\u2033': // [DOUBLE PRIME]
        case L'\u2036': // [REVERSED DOUBLE PRIME]
        case L'\u275D': // [HEAVY DOUBLE TURNED COMMA QUOTATION MARK ORNAMENT]
        case L'\u275E': // [HEAVY DOUBLE COMMA QUOTATION MARK ORNAMENT]
        case L'\u276E': // [HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMENT]
        case L'\u276F': // [HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT]
        case L'\uFF02': // [FULLWIDTH QUOTATION MARK]
            *out ++ = '"';
            break;
        case L'\u2018': // [LEFT SINGLE QUOTATION MARK]
        case L'\u2019': // [RIGHT SINGLE QUOTATION MARK]
        case L'\u201A': // [SINGLE LOW-9 QUOTATION MARK]
        case L'\u201B': // [SINGLE HIGH-REVERSED-9 QUOTATION MARK]
        case L'\u2032': // [PRIME]
        case L'\u2035': // [REVERSED PRIME]
        case L'\u2039': // [SINGLE LEFT-POINTING ANGLE QUOTATION MARK]
        case L'\u203A': // [SINGLE RIGHT-POINTING ANGLE QUOTATION MARK]
        case L'\u275B': // [HEAVY SINGLE TURNED COMMA QUOTATION MARK ORNAMENT]
        case L'\u275C': // [HEAVY SINGLE COMMA QUOTATION MARK ORNAMENT]
        case L'\uFF07': // [FULLWIDTH APOSTROPHE]
            *out ++ = '\'';
            break;
        case L'\u2010': // [HYPHEN]
        case L'\u2011': // [NON-BREAKING HYPHEN]
        case L'\u2012': // [FIGURE DASH]
        case L'\u2013': // [EN DASH]
        case L'\u2014': // [EM DASH]
        case L'\u207B': // [SUPERSCRIPT MINUS]
        case L'\u208B': // [SUBSCRIPT MINUS]
        case L'\uFF0D': // [FULLWIDTH HYPHEN-MINUS]
            *out ++ = '-';
            break;
        case L'\u2045': // [LEFT SQUARE BRACKET WITH QUILL]
        case L'\u2772': // [LIGHT LEFT TORTOISE SHELL BRACKET ORNAMENT]
        case L'\uFF3B': // [FULLWIDTH LEFT SQUARE BRACKET]
            *out ++ = '[';
            break;
        case L'\u2046': // [RIGHT SQUARE BRACKET WITH QUILL]
        case L'\u2773': // [LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT]
        case L'\uFF3D': // [FULLWIDTH RIGHT SQUARE BRACKET]
            *out ++ = ']';
            break;
        case L'\u207D': // [SUPERSCRIPT LEFT PARENTHESIS]
        case L'\u208D': // [SUBSCRIPT LEFT PARENTHESIS]
        case L'\u2768': // [MEDIUM LEFT PARENTHESIS ORNAMENT]
        case L'\u276A': // [MEDIUM FLATTENED LEFT PARENTHESIS ORNAMENT]
        case L'\uFF08': // [FULLWIDTH LEFT PARENTHESIS]
            *out ++ = '(';
            break;
        case L'\u2E28': // [LEFT DOUBLE PARENTHESIS]
            *out ++ = '(';
            *out ++ = '(';
            break;
        case L'\u207E': // [SUPERSCRIPT RIGHT PARENTHESIS]
        case L'\u208E': // [SUBSCRIPT RIGHT PARENTHESIS]
        case L'\u2769': // [MEDIUM RIGHT PARENTHESIS ORNAMENT]
        case L'\u276B': // [MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT]
        case L'\uFF09': // [FULLWIDTH RIGHT PARENTHESIS]
            *out ++ = ')';
            break;
        case L'\u2E29': // [RIGHT DOUBLE PARENTHESIS]
            *out ++ = ')';
            *out ++ = ')';
            break;
        case L'\u276C': // [MEDIUM LEFT-POINTING ANGLE BRACKET ORNAMENT]
        case L'\u2770': // [HEAVY LEFT-POINTING ANGLE BRACKET ORNAMENT]
        case L'\uFF1C': // [FULLWIDTH LESS-THAN SIGN]
            *out ++ = '<';
            break;
        case L'\u276D': // [MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT]
        case L'\u2771': // [HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT]
        case L'\uFF1E': // [FULLWIDTH GREATER-THAN SIGN]
            *out ++ = '>';
            break;
        case L'\u2774': // [MEDIUM LEFT CURLY BRACKET ORNAMENT]
        case L'\uFF5B': // [FULLWIDTH LEFT CURLY BRACKET]
            *out ++ = '{';
            break;
        case L'\u2775': // [MEDIUM RIGHT CURLY BRACKET ORNAMENT]
        case L'\uFF5D': // [FULLWIDTH RIGHT CURLY BRACKET]
            *out ++ = '}';
            break;
        case L'\u207A': // [SUPERSCRIPT PLUS SIGN]
        case L'\u208A': // [SUBSCRIPT PLUS SIGN]
        case L'\uFF0B': // [FULLWIDTH PLUS SIGN]
            *out ++ = '+';
            break;
        case L'\u207C': // [SUPERSCRIPT EQUALS SIGN]
        case L'\u208C': // [SUBSCRIPT EQUALS SIGN]
        case L'\uFF1D': // [FULLWIDTH EQUALS SIGN]
            *out ++ = '=';
            break;
        case L'\uFF01': // [FULLWIDTH EXCLAMATION MARK]
            *out ++ = '!';
            break;
        case L'\u203C': // [DOUBLE EXCLAMATION MARK]
            *out ++ = '!';
            *out ++ = '!';
            break;
        case L'\u2049': // [EXCLAMATION QUESTION MARK]
            *out ++ = '!';
            *out ++ = '?';
            break;
        case L'\uFF03': // [FULLWIDTH NUMBER SIGN]
            *out ++ = '#';
            break;
        case L'\uFF04': // [FULLWIDTH DOLLAR SIGN]
            *out ++ = '$';
            break;
        case L'\u2052': // [COMMERCIAL MINUS SIGN]
        case L'\uFF05': // [FULLWIDTH PERCENT SIGN]
            *out ++ = '%';
            break;
        case L'\uFF06': // [FULLWIDTH AMPERSAND]
            *out ++ = '&';
            break;
        case L'\u204E': // [LOW ASTERISK]
        case L'\uFF0A': // [FULLWIDTH ASTERISK]
            *out ++ = '*';
            break;
        case L'\uFF0C': // [FULLWIDTH COMMA]
            *out ++ = ',';
            break;
        case L'\uFF0E': // [FULLWIDTH FULL STOP]
            *out ++ = '.';
            break;
        case L'\u2044': // [FRACTION SLASH]
        case L'\uFF0F': // [FULLWIDTH SOLIDUS]
            *out ++ = '/';
            break;
        case L'\uFF1A': // [FULLWIDTH COLON]
            *out ++ = ':';
            break;
        case L'\u204F': // [REVERSED SEMICOLON]
        case L'\uFF1B': // [FULLWIDTH SEMICOLON]
            *out ++ = ';';
            break;
        case L'\uFF1F': // [FULLWIDTH QUESTION MARK]
            *out ++ = '?';
            break;
        case L'\u2047': // [DOUBLE QUESTION MARK]
            *out ++ = '?';
            *out ++ = '?';
            break;
        case L'\u2048': // [QUESTION EXCLAMATION MARK]
            *out ++ = '?';
            *out ++ = '!';
            break;
        case L'\uFF20': // [FULLWIDTH COMMERCIAL AT]
            *out ++ = '@';
            break;
        case L'\uFF3C': // [FULLWIDTH REVERSE SOLIDUS]
            *out ++ = '\\';
            break;
        case L'\u2038': // [CARET]
        case L'\uFF3E': // [FULLWIDTH CIRCUMFLEX ACCENT]
            *out ++ = '^';
            break;
        case L'\uFF3F': // [FULLWIDTH LOW LINE]
            *out ++ = '_';
            break;
        case L'\u2053': // [SWUNG DASH]
        case L'\uFF5E': // [FULLWIDTH TILDE]
            *out ++ = '~';
            break;
        default:
            *out ++ = c;
            break;
        }
    }

    // Return end of the output string.
    return out;
}

static void fold_to_ascii(wchar_t c, std::back_insert_iterator<std::wstring>& out)
{
    wchar_t tmp[4];
    wchar_t *end = fold_to_ascii(c, tmp);
    for (wchar_t *it = tmp; it != end; ++ it)
        *out = *it;
}

std::string fold_utf8_to_ascii(const std::string &src)
{
    std::wstring wstr = boost::locale::conv::utf_to_utf<wchar_t>(src.c_str(), src.c_str() + src.size());
    std::wstring dst;
    dst.reserve(wstr.size());
    auto out = std::back_insert_iterator<std::wstring>(dst);
    for (wchar_t c : wstr)
        fold_to_ascii(c, out);
    return boost::locale::conv::utf_to_utf<char>(dst.c_str(), dst.c_str() + dst.size());
}


} // namespace Slic3r
