#ifndef MYSQL_SERVICE_MY_SNPRINTF_INCLUDED
/* Copyright (c) 2009, 2012, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1335  USA */

/**
  @file
  my_snprintf service

  Portable and limited vsnprintf() implementation.

  This is a portable, limited vsnprintf() implementation, with some
  extra features. "Portable" means that it'll produce identical result
  on all platforms (for example, on Windows and Linux system printf %e
  formats the exponent differently, on different systems %p either
  prints leading 0x or not, %s may accept null pointer or crash on
  it). "Limited" means that it does not support all the C89 features.
  But it supports few extensions, not in any standard.

  my_vsnprintf(to, n, fmt, ap)

  @param[out] to     A buffer to store the result in
  @param[in]  n      Store up to n-1 characters, followed by an end 0
  @param[in]  fmt    printf-like format string
  @param[in]  ap     Arguments

  @return a number of bytes written to a buffer *excluding* terminating '\0'

  @post
  The syntax of a format string is generally the same:
  % <flag> <width> <.precision> <length modifier> <format> <format extension>
  where everything but the format is optional.

  Two one-character flags are recognized:
    '0' has the standard zero-padding semantics;
    '-' is parsed, but silently ignored;

  Both <width> and <precision> can be specified as numbers or '*'.
  If an asterisk is used, an argument of type int is consumed.

  <length modifier> can be 'l', 'll', or 'z'.

  Supported formats are 's' (null pointer is accepted, printed as "(null)"),
  'c', 'd', 'i', 'u', 'x', 'X', 'o', 'p' (works as 0x%x), 'f', and 'g'.

  Standard syntax for positional arguments $n is supported.

  Format extensions:

  Format 'sQ': quotes the string according to MySQL identifier quoting rules.

  Format 'sB': binary buffer, prints exactly <precision> bytes from the
  argument, without stopping at '\0'. The behavior for unspecified <precision>
  is not yet defined.

  Format 'uE': takes one integer, prints this integer, space, double quote,
  error message corresponding to this integer errno, double quote. In other words:
    printf("%uE", n) === printf("%d \"%sT\"", n, strerror(n))

  Format 'sT': takes string and print it like s but if the strings should be
  truncated puts "..." at the end.

  Format 'sS' and 'uU': prints synonymously as s and u respectively. These two
  escape simple s and u from consuming the following plain text as one of the
  above extension suffixes; for example, "Data size: %uUEiB".
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#include <stdlib.h>
#endif

extern struct my_snprintf_service_st {
  size_t (*my_snprintf_type)(char*, size_t, const char*, ...);
  size_t (*my_vsnprintf_type)(char *, size_t, const char*, va_list);
} *my_snprintf_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define my_vsnprintf my_snprintf_service->my_vsnprintf_type
#define my_snprintf my_snprintf_service->my_snprintf_type

#else

size_t my_snprintf(char* to, size_t n, const char* fmt, ...);
size_t my_vsnprintf(char *to, size_t n, const char* fmt, va_list ap);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_MY_SNPRINTF_INCLUDED
#endif
