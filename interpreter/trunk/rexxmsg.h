/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 2001  Mark Hessling <M.Hessling@qut.edu.au>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
struct textindex
{
   unsigned int errorno;
   unsigned int suberrorno;
   unsigned int textlength;
   unsigned int fileoffset;
};

/*
 * The following number is generated by running 'msgcmp'. This number MUST
 * match the first word in the binary error message files (*.mtb), or an
 * error indicating that the error message files are corrupt will be
 * displayed instead of a real error message.
 */
#define NUMBER_ERROR_MESSAGES 269
#define NUMBER_PREFIX_MESSAGES  3

/*
 * Default language index
 */
#define LANGUAGE_ENGLISH     0

/*
 * Format of .mtb files:
 * uint - number of messages (must match NUMBER_ERROR_MESSAGES)
 * uint - language index (see errlang[] in error.c)
 **uint - number of prefix lines (must match NUMBER_PREFIX_MESSAGES)
 * struct textindexs (number messages + number prefix lines)
 * char * error mesage text and prefix text in one chunk
 */
