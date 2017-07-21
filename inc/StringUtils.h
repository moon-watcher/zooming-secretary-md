/**
 *  \file StringUtils.h
 *  \brief Extra string manipulations tools
 *  \author PaspallasDev (Modified by JackNolddor)
 *  \date 08/2015
 *
 * This unit provides extra string operations.
 */

#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

/**
 *  \brief
 *      Create a formatted string.
 *
 *  \param buffer
 *      Destination string (it must be large enough to receive result).
 *
 *  \param str
 *      C string that contains the text to be written to destination string.<br />
 *      It can optionally contain embedded format specifiers.
 *
 *  \param ... (additional arguments)
 *      Depending on the format string, the function may expect a sequence of additional arguments, <br/>
 *      each containing a value to be used to replace a format specifier in the format string.
 *
 *      There should be at least as many of these arguments as the number of values specified in the format specifiers. <br/>
 *      Additional arguments are ignored by the function.
 *
 *  \return On success, the total number of characters written is returned..
 *
 *  Copy the string pointed by 'fmt' param to the 'buffer' param.<br/>
 *  If 'fmt' includes format specifiers (subsequences beginning with %), the additional arguments following format are formatted and inserted in the resulting string replacing their respective specifiers
 *
 */
unsigned short strbuilder( char *buffer, const char *fmt, ... );

#endif // _STRINGUTILS_H_
