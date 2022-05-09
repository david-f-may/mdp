/*  Include File:   mtoken.h    */

/*
 * RCS information.
 *
 * $Id: mtoken.h,v 1.1 2019/05/23 17:04:21 dmay Exp dmay $
 *
 * $Log: mtoken.h,v $
 * Revision 1.1  2019/05/23 17:04:21  dmay
 * Initial revision
 *
 */

/*
 * Copyright (c) 2020, 2021 David F. May Jr <david.f.may@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
**  Constants and macros for the parse engine.
*/

#ifndef __TOKEN_H__
#define __TOKEN_H__

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * [BeginDoc]
 * 
 * This is the Token module.  It provides functions that allow the
 * user to break a file into lexical tokens.  The algorithm used 
 * here is a fairly simple (yet elegant) finite state machine based
 * on goto's.  Three macros are used to implement the FSM in a
 * structured way.  They are as follows:
 * 
 * state(st);
 * set_state(st);
 * set_state_keep(st);
 * 
 * state (st) sets up two labels, st and st##keep.  set_state(st)
 * sends control to the label st, which gets another character from
 * the input stream and continues processing.  set_state_keep (st)
 * sends control to the label st##keep which just continues processing
 * without getting another character.
 * 
 * Consider the following example:
 * 
 * state (OUTSIDE):
 * 
 * This set's up two labels, OUTSIDE: and OUTSIDEkeep:.  A call to
 * set_state (OUTSIDE) does a goto OUTSIDE; where a call to 
 * set_state_keep (OUTSIDE) does a goto OUTSIDEkeep.  Processing
 * then continues from one of those points.
 * 
 * [EndDoc]
 */
#include <stdio.h>
#include <ctype.h>
/* #include <sort.h> */

#define state(s)                s: pi->ch = get_next_char(pi); keep##s
#define state_nk(s)             s: pi->ch = get_next_char(pi)
#define set_state(s)            goto s
#define set_state_keep(s)       goto keep##s
#ifndef	iswhite
#define iswhite(ch)             ((ch==' ')||(ch=='\t'))
#endif
#ifndef	iswhitenl
#define iswhitenl(ch)           ((ch==' ')||(ch=='\n')||(ch=='\t')||(ch=='\r'))
#endif
#ifdef  TRUE
#undef  TRUE
#endif
#ifdef  FALSE
#undef  FALSE
#endif

#define MAXIDENTSIZE 131072
#define	MAXTOKENFILESIZE	4096  // max path on linux

typedef enum {
  TOKFALSE,
  TOKTRUE,
  NONE,
  IDENT,
  NUMBER,
  FLOAT,
  STRING,
  OPERATOR,
  PREPROC,
  CHAR_CONST,
  PARSE_ERROR,
  DONE
} CDBTokenType;

typedef enum {
  PARS_NOERROR,                        /* no error */
  PARS_PARAM,                          /* bad function parameter */
  PARS_NOMEMORY,                       /* out of memory */
  PARS_NEWLINE,                        /* CR in middle of a line - not used anymore */
  PARS_NOPEN,                          /* couldn't open the necessary file */
  PARS_TOOLONG,                        /* item too long */
  PARS_EOF,                            /* EOF in the middle of a string */
  /* new ones here */
  PARS_END                             /* end of parse errors */
} parseErrorType;

extern parseErrorType parseError;
extern char *parseErrorString[];

typedef struct _parse_item {
  char *id;                            /* allocd - this token */
  int idx;
  char *tokenFile;                     /* alloced - file being parsed */
  char *databuf;                       /* allocd buffer for storage of the file */
  int keep;
  char ch;
  CDBTokenType ptoken;
  CDBTokenType ttoken;
  unsigned lnctr;
  size_t datasize;                     /* size of the data in databuf */
  char *datap;                         /* pointer to the data in databuf */
} parseItem;

/*
**  Function interface
*/

parseItem *initParser (const char *fname);
void deleteParser (parseItem *pi);
CDBTokenType getToken (parseItem *pi);
parseItem *initStrParser (const char *parstr);

#ifdef	__cplusplus
}
#endif

#endif          /*  __TOKEN_H__   */
