/**
 * @file
 * Conversion between different character encodings
 *
 * @authors
 * Copyright (C) 1999-2002,2007 Thomas Roessler <roessler@does-not-exist.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MUTT_CHARSET_H
#define _MUTT_CHARSET_H

#include <iconv.h>
#include <stdbool.h>
#include <stdio.h>

struct Buffer;

extern char *AssumedCharset;
extern char *Charset;

/**
 * struct FgetConv - Cursor for converting a file's encoding
 */
struct FgetConv
{
  FILE *file;
  iconv_t cd;
  char bufi[512];
  char bufo[512];
  char *p;
  char *ob;
  char *ib;
  size_t ibl;
  const char **inrepls;
};

/**
 * struct FgetConvNot - A dummy converter
 */
struct FgetConvNot
{
  FILE *file;
  iconv_t cd;
};

/**
 * struct MimeNames - MIME name lookup entry
 */
struct MimeNames
{
  const char *key;
  const char *pref;
};

/**
 * enum LookupType - Types of character set lookups
 */
enum LookupType
{
  MUTT_LOOKUP_CHARSET,
  MUTT_LOOKUP_ICONV
};

#define MUTT_ICONV_HOOK_FROM 1 /**< apply charset-hooks to fromcode */

extern const struct MimeNames PreferredMIMENames[];

void             mutt_cs_canonical_charset(char *buf, size_t buflen, const char *name);
int              mutt_cs_chscmp(const char *cs1, const char *cs2);
char *           mutt_cs_get_default_charset(void);
void             mutt_cs_set_langinfo_charset(void);

bool             mutt_cs_lookup_add(enum LookupType type, const char *pat, const char *replace, struct Buffer *err);
void             mutt_cs_lookup_remove(void);
const char *     mutt_cs_charset_lookup(const char *chs);

iconv_t          mutt_cs_iconv_open(const char *tocode, const char *fromcode, int flags);
size_t           mutt_cs_iconv(iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft, const char **inrepls, const char *outrepl);
const char *     mutt_cs_iconv_lookup(const char *chs);
int              mutt_cs_convert_string(char **ps, const char *from, const char *to, int flags);
bool             mutt_cs_check_charset(const char *cs, bool strict);

struct FgetConv *mutt_cs_fgetconv_open(FILE *file, const char *from, const char *to, int flags);
void             mutt_cs_fgetconv_close(struct FgetConv **fc);
int              mutt_cs_fgetconv(struct FgetConv *fc);
char *           mutt_cs_fgetconvs(char *buf, size_t buflen, struct FgetConv *fc);

#define mutt_cs_is_utf8(a)     mutt_cs_chscmp(a, "utf-8")
#define mutt_cs_is_us_ascii(a) mutt_cs_chscmp(a, "us-ascii")

#endif
