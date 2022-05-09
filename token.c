/*	Source File:	token.c	*/

/*
 * $Id: token.c,v 1.1 2019/05/23 16:58:40 dmay Exp dmay $
 *
 * $Log: token.c,v $
 * Revision 1.1  2019/05/23 16:58:40  dmay
 * Initial revision
 *
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
 * Implements a Finite State Machine for parsing.
 */

#include "mtoken.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int get_next_char (parseItem * pi);

/*
 * [BeginDoc]
 * 
 * The Token module has several external variables that are used for
 * different tasks.  They are as follows:
 * 
 * char *id;
 * unsigned int lnctr;
 * char *tokenFile;
 * ErrorType parse_error = PARS_NOERROR;
 * char *ErrorString[];
 * 
 * When GetToken() returns, id will contain a token string
 * or "(EOF)" if processing is done.  lnctr will contain the line
 * number of the file being processed.  tokenFile will contain
 * the name of the file being processed.  If there is a parsing
 * error, parse_error will contain the numerical representation
 * of the error and ErrorString[parse_error] will point to a 
 * string describing the error.
 * 
 * =================================================================
 * [EndDoc]
 */
parseErrorType parseError;
char *parseErrorString[] = {
  "no error",                                           /* PARS_NOERROR */
  "bad function parameter",                             /* PARS_PARAM */
  "dynamic memory exhausted",                           /* PARS_NOMEMORY */
  "newline before string termination",                  /* PARS_NEWLINE - not used now */
  "couldn't open file for reading",                     /* PARS_NOPEN */
  "item too long",                                      /* PARS_TOOLONG */
  "EOF encountered in the middle of a string constant", /* PARS_EOF */
  /*
   * new ones here 
   */
  "parse end errors"
};

/*
 * For when you want to parse the entire file and process it for 
 * testing or what-not.
 */

/*
 * [BeginDoc]
 * 
 * int InitParser (const char *file);
 * 
 * InitParser() initializes the Token module.  This function must
 * be called before any other Token routines.  It opens the file
 * to be parsed, allocates a buffer for all of the file contents,
 * then reads the entire file into memory (for speed).  Then it
 * allocates memory for id and tokenFile.  If any of these
 * allocations fail, the Token module fails, and InitParser() returns
 * TOKFALSE.  If InitParser() succeeds, it returns TOKTRUE.  Upon successful
 * completion, other modules in the Token module can be used.
 * 
 * Please note: The result is undefined if you continue using the Token
 * routines if InitParser() fails.  The pointers involved will not
 * necessarily point to valid areas of memory, so it is impossible
 * to say what the result would be, although it is fairly safe to 
 * say that the result would not be good!
 * 
 * =================================================================
 * [EndDoc]
 */
parseItem *initParser (const char *file)
{
  parseItem *pi;
  FILE *fp;
  struct stat statbuf;
  size_t len;

  assert (file != NULL);
  if (file == 0) {
    parseError = PARS_PARAM;
    return 0;
  }
  fp = fopen (file, "rb");
  if (fp == NULL) {
    parseError = PARS_NOPEN;
    return 0;
  }
  pi = malloc (sizeof (parseItem));
  if (0 == pi) {
    parseError = PARS_NOMEMORY;
    return 0;
  }
  memset (pi, 0, sizeof (parseItem));
  ////check_pointer (pi);
  stat (file, &statbuf);
  pi->databuf = malloc ((size_t) (statbuf.st_size + 1));
  if (NULL == pi->databuf) {
    parseError = PARS_NOMEMORY;
    free (pi);
    return 0;
  }
  memset (pi->databuf, 0, (size_t) (statbuf.st_size + 1));
  //check_pointer (pi->databuf);
  pi->datasize = (size_t) (statbuf.st_size);
  pi->id = malloc (MAXIDENTSIZE);
  if (NULL == pi->id) {
    parseError = PARS_NOMEMORY;
    free (pi->databuf);
    free (pi);
    return 0;
  }
  memset (pi->id, 0, MAXIDENTSIZE);
  //check_pointer (pi->id);
  len = strlen (file) + 1;
  pi->tokenFile = malloc (len);
  if (NULL == pi->tokenFile) {
    parseError = PARS_NOMEMORY;
    free (pi->databuf);
    free (pi->id);
    free (pi);
    return 0;
  }
  memset (pi->tokenFile, 0, len);
  //check_pointer (pi->tokenFile);
  strncpy (pi->tokenFile, file, len - 1);
  pi->tokenFile[len - 1] = '\0';
  //check_pointer (pi->tokenFile);
  /*
   * Read the data into the buffer - the return value from
   * fread is pretty useless!
   */
  fread (pi->databuf, (size_t) statbuf.st_size, 1, fp);
  pi->datap = pi->databuf;             /* initialize the pointer */
  pi->databuf[(size_t) statbuf.st_size] = '\0'; /* close off any string-handling probs. */
  fclose (fp);                         /* don't need it any more */
  /*
   * The file was opened fine, so initialize the rest 
   * of the variables
   */
  parseError = PARS_NOERROR;
  pi->lnctr = 1;
  pi->keep = TOKFALSE;
  pi->ptoken = pi->ttoken = NONE;
  return pi;
}

/*
 * [BeginDoc]
 * 
 * int InitStrParser (char *parstr);
 * 
 * InitStrParser() initializes the Token module as InitParser() does
 * with the exception that it uses the string parstr instead of
 * reading the data from a file.  This allows a user to pass in a
 * string of data to be parsed.  parstr should have been allocated
 * on the stack and will "belong" to the token module after the
 * call to InitStrParser() (i.e., DeleteParser() will free the
 * storage containing the string).
 * 
 * Note:  if there is an error on the call to this function, 
 * parstr is not freed and should be freed by the caller.
 * 
 * =================================================================
 * [EndDoc]
 */
parseItem *initStrParser (const char *parstr)
{
  parseItem *pi;
  size_t len;

  if (parstr == 0) {
    parseError = PARS_PARAM;
    return 0;
  }
  pi = malloc (sizeof (parseItem));
  if (0 == pi) {
    parseError = PARS_NOMEMORY;
    return 0;
  }
  memset (pi, 0, sizeof (parseItem));
  //check_pointer (pi);
  len = strlen (parstr) + 1;
  pi->databuf = malloc (len);
  if (NULL == pi->databuf) {
    parseError = PARS_NOMEMORY;
    free (pi);
    return 0;
  }
  pi->datasize = len;
  memset (pi->databuf, 0, pi->datasize);
  //check_pointer (pi->databuf);
  strcpy (pi->databuf, parstr);
  pi->id = malloc (MAXIDENTSIZE);
  if (NULL == pi->id) {
    parseError = PARS_NOMEMORY;
    free (pi->databuf);
    free (pi);
    return 0;
  }
  memset (pi->id, 0, MAXIDENTSIZE);
  //check_pointer (pi->id);
  pi->tokenFile = malloc (MAXTOKENFILESIZE);
  if (NULL == pi->tokenFile) {
    parseError = PARS_NOMEMORY;
    free (pi->id);
    free (pi->databuf);
    free (pi);
    return 0;
  }
  memset (pi->tokenFile, 0, MAXTOKENFILESIZE);
  //check_pointer (pi->tokenFile);
  strcpy (pi->tokenFile, "User String");
  pi->datap = pi->databuf;             /* initialize the pointer */
  /*
   * The file was opened fine, so initialize the rest 
   * of the variables
   */
  parseError = PARS_NOERROR;
  pi->lnctr = 1;
  pi->keep = TOKFALSE;
  pi->ptoken = pi->ttoken = NONE;
  return pi;
}

/*
 * [BeginDoc]
 * 
 * void DeleteParser(void);
 * 
 * DeleteParser() deallocates any heap memory and closes Token
 * processing.
 * 
 * =================================================================
 * [EndDoc]
 */
void deleteParser (parseItem * pi)
{
  //check_pointer (pi->databuf);
  free (pi->databuf);
  pi->databuf = NULL;
  pi->datasize = 0;
  //check_pointer (pi->id);
  free (pi->id);
  pi->id = NULL;
  //check_pointer (pi->tokenFile);
  free (pi->tokenFile);
  pi->tokenFile = NULL;
  free (pi);
}

/*
 * [BeginDoc]
 * 
 * CDBTokenType GetToken (void);
 * 
 * GetToken() is the primary user function of the Token module.
 * Typically, the user will call InitParser() with the name of
 * the file to parse as the argument, and will then call GetToken()
 * until it returns DONE or indicates an error occurred.  GetToken()
 * takes no arguments and returns a CDBTokenType, an enumerated value
 * including the following:
 * 
 * TOKFALSE, TOKTRUE, NONE, IDENT, NUMBER, STRING, OPERATOR, PREPROC,
 * CHAR_CONST, PARSE_ERROR, DONE
 * 
 * Under normal circumstances, the user can expect GetToken() to
 * return one of IDENT (identifier), NUMBER (numerical constant),
 * STRING (string constant), OPERATOR (+,-,/,*,+=,etc.), PREPROC
 * (preprocessor directive) or CHAR_CONST (character constant).
 * Any other return values should be considered exceptions and
 * handled accordingly.
 * 
 * =================================================================
 * [EndDoc]
 */
CDBTokenType getToken (parseItem * pi)
{
  if (pi->keep) {
    pi->keep = TOKFALSE;
    set_state_keep (OUTSIDE);
  }
  else
    set_state (OUTSIDE);
state (OUTSIDE):
  if (iswhite (pi->ch))                /* starting state */
    set_state (OUTSIDE);
  if (isalpha ((int)pi->ch) || pi->ch == '_') {
    pi->id[0] = pi->ch;
    pi->idx = 1;
    set_state (IDENTIFIER);
  }
  else if (isdigit ((int) pi->ch)) {
    pi->id[0] = pi->ch;
    pi->idx = 1;
    set_state (NUMBER);
  }
  else if (pi->ch == '\"') {
    pi->ch = get_next_char (pi);
    /*
     * check for empty string 
     */
    if (pi->ch == '\"') {
      pi->id[0] = '\0';
      pi->idx = 1;
      return (STRING);
    }
    /*
     * Check for \<nl>
     */
    if (pi->ch == '\\') {
      pi->ch = get_next_char (pi);
      if (pi->ch == '\n')
        pi->lnctr++;
    }
    pi->id[0] = pi->ch;
    pi->idx = 1;
    set_state (STRING_CONSTANT);
  }
  else if (pi->ch == '!') {            /* check to see if != or just ! */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '=') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else if (pi->ch == '|') {            /* check to see if || or just | */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '|') {
      pi->id[pi->idx++] = '|';
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else if (pi->ch == '&') {            /* check to see if && or just & */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '&') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else if (pi->ch == '>') {            /*      check for >= */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '=') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else if (pi->ch == '<') {            /*      check for <= */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '=') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else if (pi->ch == '>') {
        pi->id[pi->idx++] = pi->ch;
        pi->id[pi->idx++] = '\0';
        pi->keep = TOKFALSE;
        return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else if (pi->ch == '^') {            /*      check for <= */
    pi->id[0] = pi->ch;
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '=') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    else {
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKTRUE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
  }
  else
    switch (pi->ch) {
      case '#':
        pi->id[0] = pi->ch;
        pi->idx = 1;
        set_state (PREPROCESSOR);
      case '/':
        set_state (AWAIT_ASTERISK);
      case '\'':
        pi->ch = get_next_char (pi);
        if (pi->ch == '\\') {
          pi->id[0] = '\\';
          pi->idx = 1;
          pi->ch = get_next_char (pi);
          if (pi->ch == '\'') {
            pi->id[pi->idx++] = pi->ch;
            set_state (CHAR_CONSTANT);
          }
          else
            set_state_keep (CHAR_CONSTANT);
        }
        else {
          pi->id[0] = pi->ch;
          pi->idx = 1;
        }
        set_state (CHAR_CONSTANT);
      case '\n':
        pi->lnctr++;
        pi->ch = get_next_char (pi);
        if (pi->ch == EOF)
          pi->lnctr--;
        set_state_keep (OUTSIDE);
        /*
         * 20020308 - DFM 
         */
        /*
         * (sigh) Make cygwin work. 
         */
      case '\r':
        pi->ch = get_next_char (pi);
        if (pi->ch == EOF)
          pi->lnctr--;
        set_state_keep (OUTSIDE);
      case EOF:
        set_state (END_OF_FILE);
      case '=':                       /* check to see if == or just = */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char (pi);
        if (pi->ch == '=') {
          pi->id[pi->idx++] = pi->ch;
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKFALSE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
        else if (pi->ch == '>') {
            pi->id[pi->idx++] = pi->ch;
            pi->id[pi->idx++] = '\0';
            pi->keep = TOKFALSE;
            return (OPERATOR);
        }
        else {
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKTRUE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
      case '+':                       /* check to see if ++ or just + */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char (pi);
        if (pi->ch == '+' || pi->ch == '=') {
          pi->id[pi->idx++] = pi->ch;
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKFALSE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
        else {
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKTRUE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
      case '-':                       /* check to see if --, ->, -= or just - */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char (pi);
        if (pi->ch == '-' || pi->ch == '>' || pi->ch == '=') {
          pi->id[pi->idx++] = pi->ch;
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKFALSE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
        else {
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKTRUE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
      case '*':                       /* check to see if *= or just */
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->ch = get_next_char (pi);
        if (pi->ch == '=') {
          pi->id[pi->idx++] = pi->ch;
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKFALSE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
        else {
          pi->id[pi->idx++] = '\0';
          pi->keep = TOKTRUE;
          //check_pointer (pi->id);
          return (OPERATOR);
        }
      default:
        pi->id[0] = pi->ch;
        pi->idx = 1;
        pi->id[pi->idx++] = '\0';
        pi->keep = TOKFALSE;
        //check_pointer (pi->id);
        return (OPERATOR);
    }

  state_nk (IDENTIFIER);
  if (isalnum ((int) pi->ch) || pi->ch == '_') {
    if (pi->idx >= MAXIDENTSIZE) {
      parseError = PARS_TOOLONG;
      return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state (IDENTIFIER);
  }
  else {
    pi->keep = TOKTRUE;
    pi->id[pi->idx++] = '\0';
    //check_pointer (pi->id);
    return (IDENT);
  }
  state_nk (NUMBER);
  if (isalnum ((int) pi->ch)) {
    if (pi->idx >= MAXIDENTSIZE) {
      parseError = PARS_TOOLONG;
      return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state (NUMBER);
  }
  else if (pi->ch == '.') {
    if (pi->idx >= MAXIDENTSIZE) {
      parseError = PARS_TOOLONG;
      return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state (FLOAT);
  }
  else {
    pi->keep = TOKTRUE;
    pi->id[pi->idx++] = '\0';
    //check_pointer (pi->id);
    return (NUMBER);
  }
  state (FLOAT):
  if (pi->ch != '.' && ! isalnum ((int)pi->ch)) {
      pi->keep = TOKFALSE;
      pi->id[pi->idx++] = '\0';
      return (FLOAT);
  }
  pi->id[pi->idx++] = pi->ch;
  set_state (FLOAT);
state (CHAR_CONSTANT):
  if (pi->ch == '\'') {
    pi->keep = TOKFALSE;
    pi->id[pi->idx++] = '\0';
    //check_pointer (pi->id);
    return (CHAR_CONST);
  }
  if (pi->idx >= MAXIDENTSIZE) {
    parseError = PARS_TOOLONG;
    return (PARSE_ERROR);
  }
  pi->id[pi->idx++] = pi->ch;
  set_state (CHAR_CONSTANT);
state (STRING_CONSTANT):
  if (pi->ch == '\"') {
    if (pi->id[pi->idx - 1] == '\\') {
      pi->id[pi->idx++] = pi->ch;
      set_state (STRING_CONSTANT);
    }
    pi->keep = TOKFALSE;
    pi->id[pi->idx++] = '\0';
    //check_pointer (pi->id);
    return (STRING);
  }
  else if (pi->ch == '\n') {
    pi->lnctr++;
    pi->id[pi->idx++] = pi->ch;
    set_state (STRING_CONSTANT);
  }
  else if (pi->ch == EOF) {
    pi->lnctr--;
    parseError = PARS_EOF;
    return (PARSE_ERROR);
  }
  else if (pi->ch == '\\') {
    pi->ch = get_next_char (pi);
    if (pi->ch == '\n') {
      pi->lnctr++;
      pi->ch = get_next_char (pi);
      while (iswhite (pi->ch))
        pi->ch = get_next_char (pi);
      set_state_keep (STRING_CONSTANT);
    }
    else {
      pi->id[pi->idx++] = '\\';
      pi->id[pi->idx++] = pi->ch;
      set_state (STRING_CONSTANT);
    }
  }
  else {
    if (pi->idx >= MAXTOKENFILESIZE) {
      parseError = PARS_TOOLONG;
      return (PARSE_ERROR);
    }
    pi->id[pi->idx++] = pi->ch;
    set_state (STRING_CONSTANT);
  }
  state_nk (AWAIT_ASTERISK);
  if (pi->ch == '*')
    set_state (REMARK);
  else if (pi->ch == '/')
    set_state (CPP_REMARK);
  else {                               /*      check for /= */
    pi->id[0] = '/';
    pi->idx = 1;
    pi->ch = get_next_char (pi);
    if (pi->ch == '=') {
      pi->id[pi->idx++] = pi->ch;
      pi->id[pi->idx++] = '\0';
      pi->keep = TOKFALSE;
      //check_pointer (pi->id);
      return (OPERATOR);
    }
    pi->keep = TOKTRUE;
    pi->id[pi->idx++] = '\0';
    //check_pointer (pi->id);
    return (OPERATOR);
  }
state (REMARK):
  if (pi->ch == '\n') {
    pi->lnctr++;
    set_state (REMARK);
  }
  else if (pi->ch == '*')
    set_state (AWAIT_SLASH);
  else
    set_state (REMARK);
  state_nk (AWAIT_SLASH);
  if (pi->ch == '/')
    set_state (OUTSIDE);
  else
    set_state_keep (REMARK);
  state_nk (CPP_REMARK);
  if (pi->ch == '\n') {
    pi->lnctr++;                       /* increment line counter */
    set_state (OUTSIDE);
  }
  else
    set_state (CPP_REMARK);
  state_nk (PREPROCESSOR);
  if (pi->ch == '\n') {
    pi->lnctr++;
    pi->id[pi->idx++] = '\0';
    pi->keep = TOKFALSE;
    //check_pointer (pi->id);
    return (PREPROC);
  }
  else if (pi->ch == '\\') {
    pi->ch = get_next_char (pi);
    if (pi->ch == '\n') {
      pi->lnctr++;
      pi->ch = get_next_char (pi);
    }
    while (iswhite (pi->ch))
      pi->ch = get_next_char (pi);
  }
  if (pi->idx >= MAXIDENTSIZE) {
    parseError = PARS_TOOLONG;
    return (PARSE_ERROR);
  }
  pi->id[pi->idx++] = pi->ch;
  set_state (PREPROCESSOR);
  state_nk (END_OF_FILE);
  strcpy (pi->id, "(EOF)");
  //check_pointer (pi->id);
  return (DONE);
}

int get_next_char (parseItem * pi)
{
  char ch;

  assert (pi->datap >= pi->databuf
          && pi->datap <= pi->databuf + pi->datasize + 1);
  //check_pointer (pi->databuf);
  if (pi->datap < pi->databuf || pi->datap > pi->databuf + pi->datasize)
    return EOF;
  ch = *pi->datap;
  pi->datap++;
  if (ch == '\0')
    return (EOF);
  else
    return ((int) ch);
}

/*
 * [BeginDoc]
 * 
 * To test the Token module, compile token.c with TEST_TOKEN #defined.
 * The resulting program will look for a parameter on the command line.
 * If one is available, it will assume it is a file name and try to
 * open it for parsing.  If a command is not available on the command
 * line, token.exe (or whatever the user calls it) will try to open
 * token.c and will break it into tokens, using the ParseFile()
 * function to output the tokens (see ParseFile()).
 * 
 * [EndDoc]
 */
#ifdef	TEST_TOKEN

static void ProcessToken (CDBTokenType token, parseItem * pi);
static CDBTokenType ParseFile (parseItem * pi);

static CDBTokenType ParseFile (parseItem * pi)
{
  CDBTokenType ThisToken;

  while ((ThisToken = getToken (pi)) != DONE) {
    ProcessToken (ThisToken, pi);
    if (ThisToken == PARSE_ERROR) {
      deleteParser (pi);
      return TOKFALSE;
    }
  }
  return TOKTRUE;
}

static void ProcessToken (CDBTokenType token, parseItem * pi)
{
  switch (token) {
    case NONE:
      printf ("%d:Strange Token (NONE): %s\n", pi->lnctr, pi->id);
      break;
    case IDENT:
      printf ("%d: (IDENT): %s\n", pi->lnctr, pi->id);
      break;
    case NUMBER:
      printf ("%d: (NUMBER): %s\n", pi->lnctr, pi->id);
      break;
    case STRING:
      printf ("%d: (STRING): \"%s\"\n", pi->lnctr, pi->id);
      break;
    case FLOAT:
      printf ("%d: (FLOAT): \"%s\"\n", pi->lnctr, pi->id);
      break;
    case OPERATOR:
      printf ("%d: (OPERATOR): %s\n", pi->lnctr, pi->id);
      break;
    case PREPROC:
      printf ("%d: (PREPROC): %s\n", pi->lnctr, pi->id);
      break;
    case CHAR_CONST:
      printf ("%d: (CHAR_CONST): %s\n", pi->lnctr, pi->id);
      break;
    case PARSE_ERROR:
      printf ("\n\n***Error at %d: %s\n", pi->lnctr,
              parseErrorString[parseError]);
      break;
    default:
      break;
  }
}

int main (int argc, char *argv[])
{
  char *parsfile;
  parseItem *pi;
  CDBTokenType tkn;

  if (argc < 2)
    parsfile = "token.c";
  else
    parsfile = argv[1];
  pi = initParser (parsfile);
  if (pi == 0) {
    printf ("\n\n***Error: couldn't create parser, %s\n",
            parseErrorString[parseError]);
    return TOKFALSE;
  }
  tkn = ParseFile (pi);
  if (tkn == TOKFALSE)
    return TOKFALSE;
  deleteParser (pi);
  return TOKTRUE;
}

#endif
