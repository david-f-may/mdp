/* Source File: sort.c */

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

#ifdef __STDC__
#define _DEFAULT_SOURCE
#endif

////////////////////////////////////////////////////////////////////////////////
// Configuration parameters.
// Edit these to your needs. Currently configured for Linux X86_64.
////////////////////////////////////////////////////////////////////////////////
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Enable debugging features in the sort algorithm */
/* #undef DEBUG */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <dir.h> header file. */
/* #undef HAVE_DIR_H */

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <io.h> header file. */
/* #undef HAVE_IO_H */

/* Define to 1 if you have the <libintl.h> header file. */
/* #undef HAVE_LIBINTL_H */

/* Define to 1 if you have the `ssl' library (-lssl). */
/* #undef HAVE_LIBSSL */

/* Define to 1 if you have the <locale.h> header file. */
/* #undef HAVE_LOCALE_H */

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
/* #undef HAVE_STRERROR */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlwr' function. */
/* #undef HAVE_STRLWR */

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to 1 if you have the <pthread.h> header file. */
/* #define HAVE_PTHREAD_H 1 */

/* Name of package */
#define PACKAGE "sort"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "dmay@cnm.edu"

/* Define to the full name of this package. */
#define PACKAGE_NAME "sort"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "sort 1.0-5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "sort"

#define PACKAGE_VERSION "1.0-5"

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Version number of package */
#define VERSION "1.0-5"

/* Compile with large file system 64 */
/* #undef WITH_LFS64 */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to `long' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

////////////////////////////////////////////////////////////////////////////////
// Configuration parameters.
////////////////////////////////////////////////////////////////////////////////

// Now include sort.h, where other #include files are provided.
#include "sort.h"


/*
 * Static functions
 */
static int nsort_list_store (nsort_list_t *lh, nsort_store_t *ts, int reclen,
    char *fname);
static int nsort_list_retrieve (nsort_list_t *lh, nsort_store_t *ts,
    const char *fname, long magic);

/*
 * Uncomment this to get libdmalloc debugging
 */
/* #define DMALLOC_DEBUGGING 1 */
#ifdef DMALLOC_DEBUGGING
#include <dmalloc.h>
#endif

nsort_error_t sortError = SORT_NOERROR;

#ifdef HAVE_PTHREAD_H
pthread_mutex_t sortError_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

char *sortErrorString[] = {
  "no sort error",                  /* SORT_NOERROR */
  "memory allocation error",        /* SORT_NOMEMORY */
  "look at C standard errno",       /* SORT_ERRNO */
  "file permission denied",         /* SORT_FDENIED */
  "too many open files",            /* SORT_FTOOMANY */
  "file does not exist",            /* SORT_FNOFILE */
  "bad file descriptor",            /* SORT_FEBADF */
  "operation prohibited",           /* SORT_FPROHIB */
  "operation would cause a deadlk", /* SORT_FDEADLK*/
  "locking failed",                 /* SORT_FNOLOCK */
  "file write or read incomplete",  /* SORT_FRDWR */
  "bad function parameter",         /* SORT_PARAM */
  "list initialization failed",     /* SORT_LIST_INIT */
  "invalid list used",              /* SORT_LIST_INVALID */
  "cannot delete a populated list", /* SORT_LIST_NOTEMPTY */
  "bad list file",                  /* SORT_LIST_BADFILE */
  "unique constraint violated",     /* SORT_UNIQUE */
  "corrupt data detected",          /* SORT_CORRUPT */
  /* Add new ones here */
  "unspecified sort error",         /* SORT_UNSPECIFIED */
  NULL
};

nsort_list_t *__nsort_head = NULL;
nsort_list_t *__nsort_tail = NULL;
int __nsort_listsInUse = FALSE;

#ifdef HAVE_PTHREAD_H
pthread_mutex_t __nsort_head_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t __nsort_tail_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t __nsort_listsInUse_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

/*
 * [BeginDoc]
 *
 * \subsection{nsort Functions}
 *
 * This section will describe the functions that provide the capabilities
 * in the nsort library.  All API functions will be descibed in some
 * \index{API functions}
 * detail here.
 *
 *
 * \subsubsection{nsort_elapsed}
 * \index{nsort_elapsed}
 *
 * [Verbatim] */

int nsort_elapsed (double *sec)

/* [EndDoc] */
/*
 * [BeginDoc]
 * nsort_elapsed is a utility that uses the standard C library call
 * gettimeofday() to determine the current time (in sconds and
 * milliseconds since the epoch).  This information can be used to
 * get very accurate timings as follows:
 * [Verbatim] *

  double first, last;
  ...
  nsort_elapsed(&first);
  ...do some stuff here...
  nsort_elapsed(&last);
  printf ("Elapsed time while doing stuff: %f\n\n", last-first);

 * [EndDoc] */
/*
 * [BeginDoc]
 *
 * \emph{Note:} it should be emphasized that this function does not compile
 * if strict adherence to the ISO C standard is required.
 *
 * [EndDoc]
 */
{
  struct timeval t;
  struct timezone tz;
  int stat;

  stat = gettimeofday (&t, &tz);
  *sec = (double)(t.tv_sec + t.tv_usec/1000000.0);
  return stat;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{set_sortError}
 * \index{set_sortError}
 *
 * [Verbatim] */

void set_sortError (nsort_error_t er)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The set_sortError() function sets the global sortError variable
 * to the value given by ``er'' in a thread-safe manner.  This is
 * \index{thread safety}
 * the \emph{only} way the programmer should set the global error
 * variable.
 *
 * [EndDoc]
 */
{
#ifdef HAVE_PTHREAD_H
  pthread_mutex_lock (&sortError_mutex);
#endif
  sortError = er;
#ifdef HAVE_PTHREAD_H
  pthread_mutex_unlock (&sortError_mutex);
#endif
}

/*
 * [BeginDoc]
 *
 * \subsubsection{get_sortError}
 * \index{get_sortError}
 *
 * [Verbatim] */

nsort_error_t get_sortError(void)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The get_sortError() function gets the value of the global
 * sortError variable in a thread-safe manner.  This is the
 * \index{thread safety}
 * \emph{only} way the programmer should set the global error
 * variable.
 *
 * [EndDoc]
 */
{
  nsort_error_t tmp;
#ifdef HAVE_PTHREAD_H
  pthread_mutex_lock (&sortError_mutex);
#endif
  tmp = sortError;
#ifdef HAVE_PTHREAD_H
  pthread_mutex_unlock (&sortError_mutex);
#endif
  return tmp;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_check_error}
 * \index{nsort_check_error}
 *
 * [Verbatim] */

int nsort_check_error (void)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_check_error() function checks to see if the global
 * sortError variable is anything but SORT_NOERROR.  If it does
 * reflect an error condition, nsort_check_error() returns TRUE,
 * otherwise it returns FALSE.
 *
 * [EndDoc]
 */
{
  int nsort_st = get_sortError();
  if (nsort_st != SORT_NOERROR)
    return TRUE;
  return FALSE;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_check_list_error}
 * \index{nsort_check_list_error}
 *
 * [Verbatim] */

int nsort_check_list_error (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_check_list_error() function checks to see if
 * either a list error occurred (lh->listError > SORT_NOERROR)
 * or a global error occurred.  It returns TRUE if there is an
 * error in one of these contexts; otherwise, it returns FALSE.
 *
 * [EndDoc]
 */
{
  if (lh->listError != SORT_NOERROR)
    return TRUE;
  return (nsort_check_error());
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_check_sort_error}
 * \index{nsort_check_sort_error}
 *
 * [Verbatim] */

int nsort_check_sort_error (nsort_t *srt)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_check_sort_error() function checks to see if
 * either a sort error occurred (srt->sortError > SORT_NOERROR),
 * a list error occurred (lh->listError > SORT_NOERROR)
 * or a global error occurred.  It returns TRUE if there is an
 * error in one of these contexts; otherwise, it returns FALSE.
 *
 * [EndDoc]
 */
{
  if (srt->sortError != SORT_NOERROR)
    return TRUE;
  return nsort_check_list_error(srt->lh);
}

//
// XXX - need snprintf and strerror_r.
// XXX - need strlen, strcat
//
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_show_error}
 * \index{nsort_show_error}
 *
 * [Verbatim] */

int nsort_show_error (char *str, size_t len)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_show_error() generates an error message for global
 * error conditions.  The message is printed to ``str'' and will
 * be kept to a size no greater than ``len''.  If there is no
 * error, the message will indicate so.
 *
 * [EndDoc]
 */
{
  nsort_error_t nsort_error;
  char *GE = "generic error";
  size_t len_GE = 13;
  char *FE = "file error";
  size_t len_FE = 10;

  if (len < len_GE)
    return 0;
  if (str == 0)
    return _ERROR_;
  nsort_error = get_sortError ();
  if (! (nsort_error <= SORT_UNSPECIFIED))
    // don't even go there.
    return 0;
  switch (nsort_error) {
    case SORT_ERRNO:
      strcat (str, FE);
#if defined(__solaris__)
      strncat (str+len_FE, strerror(errno), len-len_FE);
#else
      (void)strerror_r (errno, (char *)(str+len_FE), len-len_FE);
#endif
      return (strlen (str));
      break;
    case SORT_FDENIED:
    case SORT_FTOOMANY:
    case SORT_FNOFILE:
    case SORT_FEBADF:
    case SORT_FPROHIB:
    case SORT_FDEADLK:
    case SORT_FNOLOCK:
    case SORT_FRDWR:
      return snprintf (str, len, "%s: %s", FE, sortErrorString[nsort_error]);
      break;
    case SORT_NOMEMORY:
    case SORT_PARAM:
    case SORT_UNSPECIFIED:
    default:
      return snprintf (str, len, "%s: %s", GE, sortErrorString[nsort_error]);
      break;
  }
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_show_error_num}
 * \index{nsort_show_error_num}
 *
 * [Verbatim] */

int nsort_show_error_num (nsort_error_t nsort_error, char *str, size_t len)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_show_error() generates an error message for
 * the error condition given by ``nsort_error''.
 * The message is printed to ``str'' and will
 * be kept to a size no greater than ``len''.  If there is no
 * error, the message will indicate so.
 *
 * [EndDoc]
 */
{
  char *GE = "generic error";
  size_t len_GE = 13;
  char *FE = "file error";
  size_t len_FE = 10;

  if (len < len_GE)
    return 0;
  if (str == 0)
    return _ERROR_;
  if (! (nsort_error <= SORT_UNSPECIFIED))
    // don't even go there.
    return 0;
  switch (nsort_error) {
    case SORT_ERRNO:
      strcat (str, FE);
#if defined(__solaris__)
      strncat (str+len_FE, strerror(errno), len-len_FE);
#else
      (void)strerror_r (errno, (char *)(str+len_FE), len-len_FE);
#endif
      return (strlen (str));
      break;
    case SORT_FDENIED:
    case SORT_FTOOMANY:
    case SORT_FNOFILE:
    case SORT_FEBADF:
    case SORT_FPROHIB:
    case SORT_FDEADLK:
    case SORT_FNOLOCK:
    case SORT_FRDWR:
      return snprintf (str, len, "%s: %s", FE, sortErrorString[nsort_error]);
      break;
    case SORT_NOMEMORY:
    case SORT_PARAM:
    case SORT_UNSPECIFIED:
    default:
      return snprintf (str, len, "%s: %s", GE, sortErrorString[nsort_error]);
      break;
  }
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_show_list_error}
 * \index{nsort_show_list_error}
 *
 * [Verbatim] */

int nsort_show_list_error (nsort_list_t *lh, char *str, size_t len)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_show_list_error() generates an error message for list
 * related error conditions.  The message is printed to ``str'' and will
 * be kept to a size no greater than ``len''.  If lh->listError does not
 * indicate an error condition, then nsort_show_error_num() will be called
 * with the current global error condition from get_sortError().
 *
 * [EndDoc]
 */
{
  nsort_error_t nsort_error;
  char *LE = "list error";
  size_t len_LE = 10;

  if (len < len_LE)
    return 0; // why bother?
  if (lh == 0 || str == 0)
    return _ERROR_;
  if (lh->listError != SORT_NOERROR)
    nsort_error = lh->listError;
  else {
    nsort_error = get_sortError();
    if (nsort_error == SORT_NOERROR)
      return 0; // nothing written
  }
  /* OK, we have an error condition */
  switch (nsort_error) {
    case SORT_LIST_INIT:
    case SORT_LIST_INVALID:
    case SORT_LIST_NOTEMPTY:
    case SORT_LIST_BADFILE:
      return snprintf (str, len, "%s: %s", LE, sortErrorString[nsort_error]);
      break;
    default:  // pass everything else down to the generic handler.
      return nsort_show_error_num (nsort_error, str, len);
  }
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_show_sort_error}
 * \index{nsort_show_sort_error}
 *
 * [Verbatim] */

int nsort_show_sort_error (nsort_t *srt, char *str, size_t len)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_show_sort_error() generates an error message for list
 * related error conditions.  The message is printed to ``str'' and will
 * be kept to a size no greater than ``len''.  If srt->sortError does not
 * indicate an error condition, then nsort_show_list_error() will be called
 * to pass the error condition up.
 *
 * [EndDoc]
 */
{
  nsort_error_t nsort_error;
  char *SE = "sort error";
  size_t len_SE = 10;

  if (len < len_SE)
    return 0;
  if (srt == 0 || srt->lh == 0 || str == 0)
    return _ERROR_;
  if (!(nsort_check_sort_error(srt) || 
        nsort_check_list_error(srt->lh) || 
        nsort_check_error()))
    return 0;
  if (srt->sortError != SORT_NOERROR)
    nsort_error = srt->sortError;
  else
    return nsort_show_list_error (srt->lh, str, len);
  return snprintf (str, len, "%s: %s", SE, sortErrorString[nsort_error]);
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_text_file_split}
 * \index{nsort_text_file_split}
 *
 * [Verbatim] */

int nsort_text_file_split (char **cpp, int maxnum, char *ln, int ch)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_text_file_split() function is a reentrant safe function that
 * splits the char string given by ``ln'' into strings delimited by the
 * character given by ``ch''. 
 * The ``cpp'' argument is a preallocated array
 * of character pointers and ``maxnum'' is the number of
 * slots in cpp and the maximum number of items that will be placed in
 * the cpp array.  The return value is the count of items that the function
 * processed.
 *
 *
 * [EndDoc]
 */
{
  char *field, *nextfield = 0, *cp;
  int i;
  int counter = 1;

  field = ln;
  cp = strchr (ln, ch);
  if (cp != 0) {
    *cp = '\0';
    nextfield = cp+1;
  }
  else {
    cpp[0] = field;
    cpp[1] = 0;
    return 1;
  }

  for (i = 0; i < maxnum; i++, counter++) {
    cpp[i] = field;
    cpp[i+1] = 0; // precautionary.
    cp = strchr (nextfield, ch);
    if (cp != 0) {
      *cp = '\0';
      field = nextfield;
      nextfield = cp+1;
    }
    else {
      if (nextfield[0] != '\0') {
        cpp[i+1] = nextfield;
        cpp[i+2] = 0;
        counter++;
      }
      return counter;
    }
  }
  // Keep our gcc happy.
  return counter;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_create}
 * \index{nsort_list_create}
 *
 * [Verbatim] */

nsort_list_t *nsort_list_create (void)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_create() function simply allocates an nsort_list_t
 * object and returns a pointer to it.  If an error condition is set
 * after the call to this function (or the return value is 0), the
 * global sortError variable will be set to SORT_NOMEMORY.
 *
 * [EndDoc]
 */
{
  nsort_list_t *lh;
  lh = malloc (sizeof (nsort_list_t));
  if (0 == lh) {
    set_sortError (SORT_NOMEMORY);
    return 0;
  }
  return lh;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_destroy}
 * \index{nsort_list_destroy}
 *
 * [Verbatim] */

int nsort_list_destroy (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_destroy() function simply destroys the list header
 * for the list given by lh.  Essentially, it frees memory for the
 * object.  Obviously, dire things will happen if lh does not point
 * to a valid memory segment allocated on the stack.
 *
 * [EndDoc]
 */
{
#ifdef DMALLOC_DEBUGGING
  if (dmalloc_verify(lh) == DMALLOC_VERIFY_ERROR)
    return _ERROR_;
#endif
  check_pointer (lh);
  free (lh);
  return _OK_;
}

/*
 * Assumptions: *nlh is allocated using nsort_list_create().
 */
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_init}
 * \index{nsort_list_init}
 *
 * [Verbatim] */

int nsort_list_init (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_init() function initializes the list header given by
 * lh so that it can be used for list related functions.  The lh parameter
 * can either be an object allocated on the heap with nsort_list_create()
 * or it can be a the address of a local variable.  Either way, segments
 * of memory are allocated that are necessary to be able to use the list.
 * This function returns _OK_ if it is successful and _ERROR_ if there
 * is an error.  If there is an error, lh->listError will contain information
 * about that error.
 *
 * [EndDoc]
 */
{
  if (lh == 0) {
    set_sortError(SORT_PARAM);
    return _ERROR_;
  }
  memset (lh, 0, sizeof (nsort_list_t));
  lh->head = malloc (sizeof (nsort_link_t));
  if (0 == lh->head) {
    lh->listError = SORT_NOMEMORY;
    return _ERROR_;
  }
  lh->tail = malloc (sizeof (nsort_link_t));
  if (0 == lh->tail) {
    lh->listError = SORT_NOMEMORY;
    free (lh->head);
    return _ERROR_;
  }
  memset (lh->tail, 0, sizeof (nsort_link_t));
  if (__nsort_listsInUse == FALSE) {
#ifdef HAVE_PTHREAD_H
    pthread_mutex_lock (&__nsort_listsInUse_mutex);
#endif
    __nsort_listsInUse = TRUE;
#ifdef HAVE_PTHREAD_H
    pthread_mutex_unlock (&__nsort_listsInUse_mutex);
#endif
#ifdef HAVE_PTHREAD_H
    pthread_mutex_lock (&__nsort_head_mutex);
    pthread_mutex_lock (&__nsort_tail_mutex);
#endif
    __nsort_head = malloc (sizeof (nsort_list_t));
    if (0 == __nsort_head) {
      lh->listError = SORT_NOMEMORY;
      free (lh->head);
      free (lh->tail);
      return _ERROR_;
    }
    memset (__nsort_head, 0, sizeof (nsort_list_t));
    __nsort_head->__n = lh;
    lh->__p = __nsort_head;

    __nsort_tail = malloc (sizeof (nsort_list_t));
    if (0 == __nsort_tail) {
      lh->listError = SORT_NOMEMORY;
      free (lh->head);
      free (lh->tail);
      free (__nsort_head);
      return _ERROR_;
    }
    memset (__nsort_tail, 0, sizeof (nsort_list_t));
    __nsort_tail->__p = lh;
    lh->__n = __nsort_tail;
#ifdef HAVE_PTHREAD_H
    pthread_mutex_unlock (&__nsort_head_mutex);
    pthread_mutex_unlock (&__nsort_tail_mutex);
#endif
  }
  else { /* __nsort_listsInUse != FALSE */
    nsort_list_t *plh = __nsort_tail->__p;
    plh->__n = lh;
#ifdef HAVE_PTHREAD_H
    pthread_mutex_lock (&__nsort_tail_mutex);
#endif
    __nsort_tail->__p = lh;
    lh->__n = __nsort_tail;
#ifdef HAVE_PTHREAD_H
    pthread_mutex_unlock (&__nsort_tail_mutex);
#endif
    lh->__p = plh;
  }
  lh->head->next = lh->tail;
  lh->tail->prev = lh->head;
  lh->current = lh->head;

  return _OK_;
}

/*
 * Assumption: user has to free lh.
 */
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_del}
 * \index{nsort_list_del}
 *
 * [Verbatim] */

int nsort_list_del (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_del() function basically undoes what the nsort_list_init()
 * function did.  It frees the memory that was allocated to allow the
 * list header to be used.  It is an error to call nsort_list_del() on a list
 * that is not empty.
 *
 * [EndDoc]
 */
{
  nsort_list_t *plh;

  if (lh->head->next != lh->tail || lh->tail->prev != lh->head) {
    lh->listError = SORT_LIST_NOTEMPTY;
    return _ERROR_;
  }

#ifdef DEBUG
  
#ifdef HAVE_PTHREAD_H
  pthread_mutex_lock (&__nsort_head_mutex);
  pthread_mutex_lock (&__nsort_tail_mutex);
#endif
  plh = __nsort_head;
  while (TRUE) {
    if (plh->__n == lh) {
      lh->__n->__p = plh;
      plh->__n = lh->__n;
      lh->__n = lh->__p = NULL;
      break;
    }
    plh = plh->__n;
    if (plh == __nsort_tail) {
      set_sortError (SORT_LIST_INVALID);
      return _ERROR_;
    }
  }
#ifdef HAVE_PTHREAD_H
  pthread_mutex_unlock (&__nsort_head_mutex);
  pthread_mutex_unlock (&__nsort_tail_mutex);
#endif

#endif /* DEBUG */

#ifndef DEBUG

#ifdef HAVE_PTHREAD_H
  pthread_mutex_lock (&__nsort_head_mutex);
  pthread_mutex_lock (&__nsort_tail_mutex);
#endif
  plh = lh->__p;
  lh->__n->__p = plh;
  plh->__n = lh->__n;
  lh->__n = lh->__p = NULL;
#ifdef HAVE_PTHREAD_H
  pthread_mutex_unlock (&__nsort_head_mutex);
  pthread_mutex_unlock (&__nsort_tail_mutex);
#endif

#endif /* DEBUG */

  /*
   * Free global list manager if this is the last list.
   */
#ifdef HAVE_PTHREAD_H
  pthread_mutex_lock (&__nsort_head_mutex);
  pthread_mutex_lock (&__nsort_tail_mutex);
  pthread_mutex_lock (&__nsort_listsInUse_mutex);
#endif
  if (__nsort_head->__n == __nsort_tail && __nsort_tail->__p == __nsort_head) {
    free (__nsort_head);
    __nsort_head = NULL;
    free (__nsort_tail);
    __nsort_tail = NULL;
    __nsort_listsInUse = FALSE;
  }
#ifdef HAVE_PTHREAD_H
  pthread_mutex_unlock (&__nsort_head_mutex);
  pthread_mutex_unlock (&__nsort_tail_mutex);
  pthread_mutex_unlock (&__nsort_listsInUse_mutex);
#endif

  free (lh->head);
  free (lh->tail);
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_insert_link}
 * \index{nsort_list_insert_link}
 *
 * [Verbatim] */

int nsort_list_insert_link (nsort_list_t *lh, nsort_link_t *lnk)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_insert_link() function inserts the link ``lnk'' into the
 * list given by ``lh''.  lnk->data should point to a user defined data
 * object.  The link will be inserted right after lh->current and after the
 * call, lh->current will point to lnk.
 *
 * [EndDoc]
 */
{
  nsort_link_t *link;
  if (lh->current == NULL)
    lh->current = lh->tail->prev;
  link = lh->current;
  if (link == lh->tail)
    link = link->prev;
  lnk->prev = link;
  lnk->next = link->next;
  link->next->prev = lnk;
  link->next = lnk;
  lh->number++;
  lh->current = lnk;
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_remove_link}
 * \index{nsort_list_remove_link}
 *
 * [Verbatim] */

nsort_link_t *nsort_list_remove_link (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * nsort_list_remove_link() removes the link from the list ``lh'' at
 * lh->current.  If it can remove the link, it does so and returns
 * a pointer to it.  If it cannot remove the link, it returns NULL.
 *
 * [EndDoc]
 */
{
  nsort_link_t *lnk;
  if (lh->head->next == lh->tail)
    return NULL;
  if (lh->current == lh->head || lh->current == lh->tail)
    lh->current = lh->head->next;  // first link by default
  if (lh->current->prev == lh->head && 
      lh->current->next == lh->tail) {
    lnk = lh->current;
    lh->current = lh->head;
    lh->head->next = lh->tail;
    lh->tail->prev = lh->head;
    lh->number--;
    return lnk;
  }
  if (lh->current == lh->head)
    lh->current = lh->head->next;
  if (lh->current == lh->tail)
    lh->current = lh->tail->prev;
  Assert (lh->current != lh->head && lh->current != lh->tail);
  lnk = lh->current;
  lh->current = lnk->next;
  lnk->prev->next = lnk->next;
  lnk->next->prev = lnk->prev;
  lnk->next = lnk->prev = NULL;
  lh->number--;
  return lnk;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_clear}
 * \index{nsort_list_clear}
 *
 * [Verbatim] */

int nsort_list_clear (nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_clear() function clears the contents of the list given by ``lh''.
 * It removes each link and calls free to free lnk->data and lnk.  This does not
 * check to insure that these are allocated on the heap, so if you have a list
 * with links or data that are allocated in a group, you should not call this
 * function.  The nsort_list_clear() function returns _OK_ when it is completed;
 * it does not return an error condition under any circumstance.
 *
 * [EndDoc]
 */
{
  nsort_link_t *lnk;
  if (nsort_list_is_empty(lh))
    return _OK_;
  nsort_list_first_link(lh);
  lnk = nsort_list_remove_link(lh);
  while (lnk != NULL) {
    free (lnk->data);
    free (lnk);
    lnk = nsort_list_remove_link(lh);
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_write_block}
 * \index{nsort_list_write_block}
 *
 * [Verbatim] */

size_t nsort_list_write_block (int fd, off_t where, size_t reclen,
    nsort_list_t *lh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_write_block() function allocates a memory buffer
 * that is of size reclen*lh->number.  Then, it copies the data from
 * every lnk->data member in the list to the memory buffer.  Then
 * this memory buffer gets written at offset where from the beginning
 * of the file.  This function returns _OK_ if it is successful or
 * _ERROR_ on error.  The following are descriptions of each of the
 * parameters:
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is the file descriptor of an open file.
 *
 * \item [where] This parameter is the offset from the beginning of the
 * file that the writing will occur.
 *
 * \item [reclen]  This is the length of each item that is being stored
 * in the list.  The data items have to be of equal lengths to be stored
 * to file using this function.
 *
 * \item [lh] This is the list header containing the data to be stored.
 *
 * \end{itemize}
 *
 * [EndDoc]
 */
{
  char *pdata;
  unsigned int i;
  nsort_link_t *lnk;
  off_t rtn;
  size_t totlen;
  int status = 0;

  totlen = reclen * lh->number;
  nsort_file_seek_begin(fd, where, status);
  if (nsort_check_error())
    return (size_t)_ERROR_;
  pdata = (char *)malloc (totlen);
  if (0 == pdata) {
    lh->listError = SORT_NOMEMORY;
    return (size_t)_ERROR_;
  }
  lnk = lh->head->next;
  for (i = 0; i < lh->number; i++) {
    memcpy ((void*)(pdata+(i*reclen)), lnk->data, reclen);
    lnk = lnk->next;
  }
  nsort_file_write (fd, pdata, totlen, rtn);
  if (nsort_check_error())
    return (size_t)_ERROR_;
  nsort_file_flush (fd);
  if (nsort_check_error())
    return (size_t)_ERROR_;
  free (pdata);
  return (size_t) rtn;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_read_block}
 * \index{nsort_list_read_block}
 *
 * [Verbatim] */

int nsort_list_read_block (nsort_list_t *lh, int fd, off_t where, size_t reclen,
    size_t numrecs)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_read_block() function allocates a buffer on the heap of
 * size reclen*numrecs.  Then, it reads the data from the file given by ``fd''
 * from ``where'' bytes from the beginning of the file.  Then, it breaks this
 * data up into numrecs chunks and uses these to populate the list ``lh'' with.
 * It is assumed that lh is an empty list whose purpose is to contain the 
 * data from the file.  If it is not empty, the data from the file will be appended
 * to the end of the list.  All the links and data objects for the list are
 * individually allocated on the heap.  It is a critical error if there is not
 * enough memory to complete this function.
 * This function returns _OK_ on success and _ERROR_ on error.
 *
 * [EndDoc]
 */
{
  char *pdata;
  nsort_link_t *lnk;
  unsigned int i;
  int j, num;
  size_t totlen;
  int status;
  void *vp;

  nsort_file_seek_begin(fd, where, status);
  if (nsort_check_error())
    return _ERROR_;
  totlen = reclen * numrecs;
  pdata = (char *)malloc (totlen);
  if (pdata == NULL) {
    lh->listError = SORT_NOMEMORY;
    return _ERROR_;
  }
  nsort_file_read (fd, pdata, totlen, status);
  if (nsort_check_error()) {
    free (pdata);
    return _ERROR_;
  }
  status = nsort_list_init (lh);
  if (status == _ERROR_) {
    return _ERROR_;
  }
  for (i = 0; i < numrecs; i++) {
    vp = malloc (reclen);
    lnk = malloc (sizeof (nsort_link_t));
    if (0 == vp || 0 == lnk) {
      if (vp != 0)
        free (vp);
      num = lh->number;
      for (j = 0; j < num; j++) {
        lnk = nsort_list_remove_link(lh);
        free (lnk->data);
        free (lnk);
      }
      free (pdata);
      lh->listError = SORT_NOMEMORY;
      return _ERROR_;
    }
    memcpy (vp, (void*)(pdata+(i*reclen)), reclen);
    lnk->data = vp;
    nsort_list_last_link(lh);
    status = nsort_list_insert_link (lh, lnk);
    if (_ERROR_ == status) {
      free (pdata);
      free (lnk);
      free (vp);
      num = lh->number;
      for (j = 0; j < num; j++) {
        lnk = nsort_list_remove_link (lh);
        free (lnk->data);
        free (lnk);
      }
      return _ERROR_;
    }
  }
  free (pdata);
  return _OK_;
}

/*
 * I don't want this function being used by end-users.  They should
 * use nsort_list_save().  Therefore, I am not going to document it.
 */
static int nsort_list_store (nsort_list_t *lh, nsort_store_t *ts, int reclen,
    char *fname)
{
  int fd;
  off_t offset = 0L;
  long status;
  off_t num;

  ts->number = lh->number;
  ts->size = reclen;
  nsort_file_create (fname,fd);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  nsort_file_close (fd);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  nsort_file_open (fname,fd);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  nsort_file_write (fd, ts, sizeof (nsort_store_t), status);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  offset += status;
  num = nsort_list_write_block (fd, offset, reclen, lh);
  nsort_file_close (fd);
  if (nsort_check_error())
    return _ERROR_;
  if (num != (off_t)(reclen * lh->number)) {
    lh->listError = SORT_FRDWR;
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_save}
 * \index{nsort_list_save}
 *
 * [Verbatim] */

int nsort_list_save (nsort_list_t *lh, const char *desc, int size,
    char *file)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_save() function saves the data that is stored in the list ``lh''
 * to the file given by the name ``file''.  If a file of that name doesn't exist, it
 * will be created.  If it does exist, the contents of the file will be overwritten.
 * \footnote{This will happen without any warning, so be careful.}  This function
 * uses reentrant safe library calls to get a time stamp; if these library calls are
 * not available in your implementation, you may have to use the unsafe versions and
 * lessen the thread-safety of your application.  The following are descriptions for
 * the parameters to this function:
 *
 * \begin{itemize}
 *
 * \item [lh] This is a list header for the list that contains the data.
 *
 * \item [desc] This is a string that contains a description that gets stamped into
 * the file at the top of the file, along with a time stamp and a magic number that
 * identifies this as a stored list.  This string can be just about anything up to
 * 127 characters in length.
 *
 * \item [size]  This is the size of each of the records stored in the list.
 *
 * \item [file] This is the name of the file to store the list in.  If a file with
 * this name already exists, it will get overwritten.
 *
 * \end{itemize}
 *
 * nsort_list_save() returns _OK_ if it is successful and _ERROR_ if it is not.
 * If it is not successful, lh->listError will hold the error and you can use
 * nsort_show_list_error() to get a description of the error.
 *
 * [EndDoc]
 */
{
  time_t now;
  struct tm result;
  nsort_store_t *ts;
  char tstamp[27];
  char *cp;
  int status;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    lh->listError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (ts, 0, sizeof (nsort_store_t));
  ts->thisMagic = DEFAULT_MAGIC;
  if (desc != 0)
    strncpy (ts->description, desc, 127);
  time (&now);
  localtime_r (&now, &result);
  asctime_r (&result, tstamp);
  cp = strchr (tstamp, '\n');
  if (cp != 0)
    *cp = '\0';
  strcpy (ts->timeStamp, tstamp);
  status = nsort_list_store (lh, ts, size, file);
  free (ts);
  return status;
}


/*
 * Don't document this.  I want users to use nsort_list_get().
 */
static int nsort_list_retrieve (nsort_list_t *lh, nsort_store_t *ts,
    const char *fname, long magic)
{
  int fd;
  int status;

  nsort_file_open (fname,fd);
  if (nsort_check_error())
    return _ERROR_;
  nsort_file_read (fd, ts, sizeof (nsort_store_t), status);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  if (ts->thisMagic != (unsigned long) magic) {
    nsort_file_close (fd);
    set_sortError (SORT_NOERROR);
    lh->listError = SORT_LIST_BADFILE;
    return _ERROR_;
  }
  status = nsort_list_read_block (lh, fd, sizeof (nsort_store_t), ts->size, ts->number);
  if (status == _ERROR_) {
    if (nsort_check_error()) {
      lh->listError = get_sortError();
      set_sortError(SORT_NOERROR);
    }
    else {
      lh->listError = SORT_UNSPECIFIED;
    }
    nsort_file_close (fd);
    return _ERROR_;
  }
  nsort_file_close (fd);
  if (nsort_check_error()) {
    lh->listError = get_sortError();
    set_sortError(SORT_NOERROR);
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_get}
 * \index{nsort_list_get}
 *
 * [Verbatim] */

int nsort_list_get (nsort_list_t *lh, char *file)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_get function get's the data that is stored in ``file'' and stores it
 * in the list given by ``lh''.  The following are the parameters to this function:
 *
 * \begin{itemize}
 *
 * \item [lh] This is the list header for the list that is to get the contents of the
 * file.  The number of items and the size of each item is stored in the header that
 * was stamped into the file when it was created.
 *
 * \item [file] This parameter is the name of a file to open and get the data from.
 * It is an error if the file doesn't exist or if this file is
 * not a stored list.
 *
 * \end{itemize}
 *
 * This function returns _OK_ if it is successful and _ERROR_ if not.  If it returns
 * _ERROR_, lh->listError will contain the error and you can get a description of it
 * by calling nsort_show_list_error().
 *
 * [EndDoc]
 */
{
  nsort_store_t *ts;
  int status;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    set_sortError (SORT_NOMEMORY);
    return _ERROR_;
  }
  memset (ts, 0, sizeof (nsort_store_t));
  status = nsort_list_retrieve (lh, ts, file, DEFAULT_MAGIC);
  free (ts);
  return status;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_get_all}
 * \index{nsort_list_get_all}
 *
 * [Verbatim] */

int nsort_list_get_all (nsort_list_t *lh, char *file, char *desc, char *timestamp)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_get_all() function is the same functionally as nsort_list_get()
 * except that it allows you to retrieve from the stored file the description and
 * the timestamp.  The following are the parameters for this function:
 *
 * \begin{itemize}
 *
 * \item [lh] This is the list header for the list that is to get the contents of the
 * file.  The number of items and the size of each item is stored in the header that
 * was stamped into the file when it was created.
 *
 * \item [file] This parameter is the name of a file to open and get the data from.
 * It is an error if the file doesn't exist or if this file is
 * not a stored list.
 *
 * \item [desc] This is a pointer to a character array that should have enough space
 * to store a description, up to 127 bytes.
 *
 * \item [timestamp] This is a pointer to a character array that should have enough
 * space to store a time stamp, 26 bytes.
 *
 * \end{itemize}
 *
 * [EndDoc]
 */
{
  nsort_store_t *ts;
  int status;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    set_sortError (SORT_NOMEMORY);
    return _ERROR_;
  }
  memset (ts, 0, sizeof (nsort_store_t));
  status = nsort_list_retrieve (lh, ts, file, DEFAULT_MAGIC);
  strncpy (desc, ts->description, 127);
  strncpy (timestamp, ts->timeStamp, 26);
  free (ts);
  return status;
}
// XXX l1 and l2 must be deleted by the caller although the links they contain
// will be moved.  The mrg parameter should be an allocated list but not initialized.
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_merge_sorted}
 * \index{nsort_list_merge_sorted}
 *
 * [Verbatim] */

int nsort_list_merge_sorted (nsort_list_t *mrg, nsort_list_t *l1, nsort_list_t *l2,
    int (*compare)(void*, void*))

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_merge_sorted() function merges sorted lists given by ``l1'' and
 * ``l2'' and stores them in ``mrg''.  The following are the parameters for the
 * function:
 *
 * \begin{itemize}
 *
 * \item [mrg] This parameter is the list header of a list that is to get all the
 * items from the l1 and l2 lists.  It should be an empty, initialized list created
 * just for this purpose.
 *
 * \item [l1] This parameter is the list header of the first list that is to be
 * merged.  After this function completes, this list is empty and needs to be
 * deleted.
 *
 * \item [l2] This parameter is the list header of the second list that is to be
 * merged.  The contents of this list, as well as those of the first list, must
 * be sorted or the results will be indeterminite.  After this function completes,
 * this list is empty and needs to be deleted.
 *
 * \item [compare] This is the function that will be used to merge the two list
 * and insure that the result is sorted.  This should be the same compare function
 * that was used to sort the lists in the first place.
 *
 * \end{itemize}
 *
 * This function returns OK if it is successful, or _ERROR_ if there is an error.
 * If an error occurs, the error will be given by mrg->listError and a description
 * of the error can be attained with a call to nsort_show_list_error().
 *
 * [EndDoc]
 */
{
  nsort_link_t *lnk, *l1next, *l2next;
  int status;

  if (l1->head->next == l1->tail || l2->head->next == l2->tail) {
    mrg->listError = SORT_PARAM;
    return _ERROR_;
  }
  status = nsort_list_init (mrg);
  if (status == _ERROR_) {
    return _ERROR_;
  }
  l1next = l1->head->next;
  l2next = l2->head->next;
  while (l1next != l1->tail && l2next != l2->tail) {
    status = compare (l1next->data, l2next->data);
    if (status > 0) {
      lnk = l2next;
      l2next = l2next->next;
    }

    else {
      lnk = l1next;
      l1next = l1next->next;
    }
    nsort_list_insert_link (mrg, lnk);
  }
  if (l1next) {
    while (l1next != l1->tail) {
      lnk = l1next;
      l1next = l1next->next;
      nsort_list_insert_link (mrg, lnk);
    }
  }
  if (l2next) {
    while (l2next != l2->tail) {
      lnk = l2next;
      l2next = l2next->next;
      nsort_list_insert_link (mrg, lnk);
    }
  }

  l1->head->next = l1->tail;
  l1->tail->prev = l1->head;
  l1->number = 0;
  l2->head->next = l2->tail;
  l2->tail->prev = l2->head;
  l2->number = 0;
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{returnClean}
 * \index{returnClean}
 *
 * [Verbatim] */

void returnClean (void*p)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The returnClean function is a stub that is passed to nsort_del when the
 * elements of the list (the links and data objects) are not allocated
 * individually on the heap.  It doesn't do anything.
 *
 * [EndDoc]
 */
{
  if (p) return;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_create}
 * \index{nsort_create}
 *
 * [Verbatim] */

nsort_t *nsort_create (void)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_create() function allocates an item of nsort_t on the heap
 * and returns a pointer to it.  The returned parameter should be destroyed
 * by calling nsort_destroy().  If an error occurs, nsort_create() will return
 * NULL and you can get a description of the error by calling nsort_show_error().
 *
 * [EndDoc]
 */
{
  nsort_t *srt;
  srt = malloc (sizeof (nsort_t));
  if (srt == 0) {
    set_sortError (SORT_NOMEMORY);
    return 0;
  }
  return srt;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_destroy}
 * \index{nsort_destroy}
 *
 * [Verbatim] */

int nsort_destroy (nsort_t *srt)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_destroy() function frees the nsort_t object given by ``srt''.
 * It only returns _OK_.
 *
 * [EndDoc]
 */
{
#ifdef DMALLOC_DEBUGGING
  if (dmalloc_verify(srt) == DMALLOC_VERIFY_ERROR)
    return _ERROR_;
#endif
  check_pointer (srt);
  free (srt);
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_init}
 * \index{nsort_init}
 *
 * [Verbatim] */

int nsort_init (nsort_t *srt, int (*compare)(void*,void*),
    int isUnique, int manageAllocs)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_init() function prepares the sort object given by ``srt'' to
 * be manipulated by the nsort functions.  The following are the parameters
 * of this function:
 *
 * \begin{itemize}
 *
 * \item [srt] This parameter is an nsort_t object that should have been allocated
 * on the stack as an automatic variable or on the heap.
 *
 * \item [compare] This parameter is a pointer to a compare function that will be
 * used to determine the sort order that items are added to the nsort object.
 *
 * \item [isUnique] This parameter is a boolean that dictates whether unique
 * constraints will be applied to the data as it is sorted by the nsort routines.
 *
 * \item [manageAllocs] This parameter is a boolean that determines whether the
 * nsort routines should take responsibility for memory allocations or not.  Basically,
 * if this is TRUE, nsort_del() function can assume that all the data objects and all
 * the links in the srt->lh list are individually allocated on the heap and can free them.
 * If it is FALSE, it is the callers responsibility to see that they get freed.  This
 * would allow you, for example, to pre-allocate the links and data elements you need
 * at once.
 *
 * \end{itemize}
 *
 * If successful, this function will return _OK_.  Otherwise, _ERROR_ is returned
 * and srt->sortError is set to the error condition and you can use
 * nsort_show_sort_error() to get a description of the error.
 *
 * [EndDoc]
 */
{
  if (isUnique != TRUE && isUnique != FALSE) {
    srt->sortError = SORT_PARAM;
    return _ERROR_;
  }
  memset (srt, 0, sizeof (nsort_t));
  srt->head = malloc (sizeof (nsort_node_t));
  if (0 == srt->head) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->head, 0, sizeof (nsort_node_t));
  srt->tail = malloc (sizeof (nsort_node_t));
  if (0 == srt->tail) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->tail, 0, sizeof (nsort_node_t));
  srt->head->prev = 0;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->tail->next = 0;
  srt->current = srt->head;
  srt->isUnique = isUnique;
  srt->manageAllocs = manageAllocs;
  srt->compare = compare;
  srt->lh = nsort_list_create ();
  if (srt->lh == 0) {
    free (srt->head);
    free (srt->tail);
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->lh, 0, sizeof (nsort_list_t));
  if (nsort_list_init (srt->lh) == _ERROR_) {
    if (srt->lh->listError != SORT_NOERROR)
      srt->sortError = srt->lh->listError;
    else if (nsort_check_error())
      srt->sortError = get_sortError();
    else
      srt->sortError = SORT_UNSPECIFIED;
    free (srt->lh);
    free (srt->head);
    free (srt->tail);
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_del}
 * \index{nsort_del}
 *
 * [Verbatim] */

int nsort_del (nsort_t *srt, void (*delFunc)(void*))

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_del() function clears out the srt->lh list and then deallocates
 * the memory that was allocated by nsort_init().  The resulting nsort_t object
 * ``srt'' should then be destroyed using nsort_destroy().  The ``delFunc''
 * parameter is provided to clean up in the case that the items that are
 * contained by the list are complex.  In other words, if a single free() doesn't
 * free them up, the delFunc can do that unravelling of the data items.
 *
 * [EndDoc]
 */
{
  nsort_node_t *node, *nextNode;
  nsort_link_t *lnk;

  if (srt->head->next != srt->tail) {
    node = srt->head->next;
    nextNode = node->next;
    while (TRUE) {
      free (node);
      if (nextNode == srt->tail)
        break;
      node = nextNode;
      nextNode = node->next;
    }
  }
  if (!srt->manageAllocs)
    delFunc = returnClean;
  free (srt->head);
  free (srt->tail);
  lnk = nsort_list_remove_link (srt->lh);
  while (lnk != 0) {
    if (delFunc)
      delFunc (lnk->data);
    else {
      free (lnk->data);
    }
    if (srt->manageAllocs)
      free (lnk);
    lnk = nsort_list_remove_link (srt->lh);
  }
  nsort_list_del (srt->lh);
  if (nsort_list_destroy (srt->lh) == _ERROR_)
    return _ERROR_;
  return _OK_;
}

/*
 * This function is not part of the API.  Don't document it.
 */
int nsort_new_node (nsort_t *srt, nsort_node_t *prevNode,
    nsort_node_t *nextNode, nsort_link_t *here)
{
  nsort_node_t *newNode;
  int i;

  newNode = malloc (sizeof (nsort_node_t));
  if (newNode == 0) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  newNode->prev = prevNode;
  newNode->next = nextNode;
  prevNode->next = newNode;
  nextNode->prev = newNode;
  newNode->here = here;
  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    newNode->level[i] = srt->tail;
  srt->current = newNode;
  srt->numNodes++;
  return _OK_;
}


#define NSORT_RSTRUCT_THRESH 10
#define NSORT_OUTPOINT_THRESH (6*(NSORT_NODE_LEVEL*NSORT_OUTPOINT))
#define NSORT_CRIT_THRESH (500*(NSORT_NODE_LEVEL*NSORT_OUTPOINT))

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_add_item}
 * \index{nsort_add_item}
 *
 * [Verbatim] */

int nsort_add_item (nsort_t *srt, nsort_link_t *lnk)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_add_item() function is the most complex function that is part
 * of the nsort routines.  It's function is to add the item given by ``lnk''
 * to the sort object given by ``srt''.  The item will be added in a sorted
 * manner according to the compare function that was provided in the call
 * to nsort_init() function.  The complexity of this function arises from the
 * fact that the indexing must be built in the ``srt'' object as items are
 * added and that this must be done in the most efficient manner possible.
 *
 * While the function definition for this function is complex, using the function
 * is relatively trivial.  Simply allocate a link (lnk), point it's data member
 * to the data object you want to add to the srt object, and call nsort_add_item()
 * with the two parameters.  nsort_add_item() returns _OK_ if it is successful.
 * Otherwise, _ERROR_ is returned and you can call nsort_show_sort_error() to
 * get a description of the error.
 *
 * [EndDoc]
 */
{
    register int status;
  register nsort_link_t *link;
  nsort_link_t *midpoint = 0;
  int linkCount;
  register nsort_node_t *node;
  nsort_node_t *midnode = 0;
  nsort_node_t *oldLevel[NSORT_NODE_LEVEL];
  register int nodeCount = 0;
  int i;
#ifdef NSORT_STATS
  double t1, t2;
#endif

  if (srt->numCompares > NSORT_CRIT_THRESH) {
    srt->thresh = 0;
    status = nsort_restructure_nodes (srt);
    if (status == _ERROR_)
      return _ERROR_;
  }
  else if (srt->numCompares > NSORT_OUTPOINT_THRESH) {
    ++srt->thresh;
    if (srt->thresh > NSORT_RSTRUCT_THRESH) {
      srt->thresh = 0;
      status = nsort_restructure_nodes (srt);
      if (status == _ERROR_)
        return _ERROR_;
    }
  }
  srt->numCompares = 0;
#ifdef NSORT_STATS
  srt->traversal_time = 0.0;
#endif
  if (srt->head->next == srt->tail) {
    if (srt->lh->head->next == srt->lh->tail) {
      srt->lh->current = srt->lh->head;
      nsort_list_insert_link (srt->lh, lnk);
      return _OK_;
    }
    status = srt->compare (lnk->data, srt->lh->head->next->data);
    srt->numCompares++;
    if (status < 0) {
      srt->lh->current = srt->lh->head;
      nsort_list_insert_link (srt->lh, lnk);
      return _OK_;
    }
    if (status == 0) {
      if (srt->isUnique) {
        srt->sortError = SORT_UNIQUE;
        return _ERROR_;
      }
      link = srt->lh->head->next;
      if (link->next->data != 0) {
        status = srt->compare (lnk->data, link->next->data);
        srt->numCompares++;
      }
      while (status == 0 && link->next != srt->lh->tail) {
        link = link->next;
        if (link->next->data == 0)
          break;
        status = srt->compare (lnk->data, link->next->data);
        srt->numCompares++;
      }
      srt->lh->current = link;
      nsort_list_insert_link (srt->lh, lnk);
      return _OK_;
    }
    status = srt->compare (lnk->data, srt->lh->tail->prev->data);
    srt->numCompares++;
    if (status >= 0) {
      srt->lh->current = srt->lh->tail->prev;
      nsort_list_insert_link (srt->lh, lnk);
      return _OK_;
    }
    link = srt->lh->head->next;
    linkCount = 1;
    while (link != srt->lh->tail) {
      if (linkCount == NSORT_MIDPOINT) {
        midpoint = link;
      }
      if (linkCount == NSORT_OUTPOINT) {
        status = nsort_new_node (srt, srt->head, srt->tail, midpoint);
        if (status == _ERROR_)
          return _ERROR_;
        // BUGBUG
        // I am adding this to fix a persnickity bug that rears it's head only on very
        // rare occasions.  This is a kludge that I will remove when I have found the bug
        // and squashed it.
        /*if (nsort_restructure_nodes (srt) == _ERROR_)
          return _ERROR_;*/
        // BUGBUG
      }
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status < 0) {
        srt->lh->current = link->prev;
        nsort_list_insert_link (srt->lh, lnk);
        return _OK_;
      }
      link = link->next;
      linkCount++;
    }
    srt->sortError = SORT_CORRUPT;
    return _ERROR_;
  }
  status = srt->compare (lnk->data, srt->lh->head->next->data);
  srt->numCompares++;
  if (status < 0) {
    srt->current = srt->head->next;
    srt->lh->current = srt->lh->head;
    nsort_list_insert_link (srt->lh, lnk);
    return _OK_;
  }
  if (status == 0 && srt->isUnique == TRUE) {
    srt->sortError = SORT_UNIQUE;
    return _ERROR_;
  }
  if (status == 0) {
    srt->current = srt->head->next;
    link = srt->lh->head->next;
    srt->lh->current = link;
    nsort_list_insert_link (srt->lh, lnk);
    return _OK_;
  }
  status = srt->compare (lnk->data, srt->lh->tail->prev->data);
  srt->numCompares++;
  if (status == 0 && srt->isUnique == TRUE) {
    srt->sortError = SORT_UNIQUE;
    return _ERROR_;
  }
  if (status >= 0) {
    srt->current = srt->tail->prev;
    srt->lh->current = srt->lh->tail->prev;
    nsort_list_insert_link (srt->lh, lnk);
    return _OK_;
  }
#ifdef NSORT_STATS
  nsort_elapsed (&t1);
#endif

  node = srt->head->next;
  while (node->level[NSORT_NODE_LEVEL-1] != srt->tail) {
    status = srt->compare (lnk->data, node->level[NSORT_NODE_LEVEL-1]->here->data);
    srt->numCompares++;
    if (status == 0 && srt->isUnique == TRUE) {
      srt->sortError = SORT_UNIQUE;
      return _ERROR_;
    }
    if (status <= 0)
      break;
    node = node->level[NSORT_NODE_LEVEL-1];
  }
  for (i = NSORT_NODE_LEVEL-2; i >= 0; i--) {
    nodeCount = 0;
    oldLevel[i+1] = node;
    while (node->level[i] != srt->tail) {
      status = srt->compare (lnk->data, node->level[i]->here->data);
      srt->numCompares++;
      if (status == 0 && srt->isUnique == TRUE) {
        srt->sortError = SORT_UNIQUE;
        return _ERROR_;
      }
      if (status <= 0)
        break;
      node = node->level[i];
      if (node == node->level[i]) {
        srt->sortError = SORT_CORRUPT;
        return _ERROR_;
      }
      nodeCount++;
      if (nodeCount == NSORT_MIDPOINT)
        midnode = node;
      if (nodeCount == NSORT_OUTPOINT) {
        midnode->level[i+1] = oldLevel[i+1]->level[i+1];
        oldLevel[i+1]->level[i+1] = midnode;
      }
    }
  }
#ifdef NSORT_STATS
  nsort_elapsed (&t2);
  srt->traversal_time += (t2-t1);
  nsort_elapsed (&t1);
#endif
  nodeCount = 0;
  oldLevel[0] = node;
  while (node != srt->tail) {
    status = srt->compare (lnk->data, node->here->data);
    srt->numCompares++;
    if (status == 0 && srt->isUnique == TRUE) {
      srt->sortError = SORT_UNIQUE;
      return _ERROR_;
    }
    if (status <= 0) {
      goto insertNode;
    }
    if (node == node->next) {
      srt->sortError = SORT_CORRUPT;
      return _ERROR_;
    }
    node = node->next;
    nodeCount++;
    if (nodeCount == NSORT_MIDPOINT)
      midnode = node;
    if (nodeCount == NSORT_OUTPOINT) {
      midnode->level[0] = oldLevel[0]->level[0];
      oldLevel[0]->level[0] = midnode;
    }
  }

insertNode:

#ifdef NSORT_STATS
  nsort_elapsed (&t2);
  srt->traversal_time += (t2-t1);
  nsort_elapsed (&t1);
#endif

  if (node->prev == srt->head)
    link = srt->lh->head->next;
  else
    link = node->prev->here;
  linkCount = 0;
  while (link != srt->lh->tail) {
    if (linkCount == NSORT_MIDPOINT) {
      midpoint = link;
    }
    if (linkCount == NSORT_OUTPOINT) {
      linkCount= 0;
      status = nsort_new_node (srt, node->prev, node, midpoint);
      if (status == _ERROR_)
        return _ERROR_;
    }
    status = srt->compare (lnk->data, link->data);
    srt->numCompares++;
    if (status == 0 && srt->isUnique == TRUE) {
      srt->sortError = SORT_UNIQUE;
      return _ERROR_;
    }
    if (status <= 0) {
      srt->current = node->prev;
      srt->lh->current = link->prev;
      nsort_list_insert_link (srt->lh, lnk);
#ifdef NSORT_STATS
      nsort_elapsed (&t2);
      srt->traversal_time += (t2-t1);
#endif
      return _OK_;
    }
    if (link == link->next) {
      srt->sortError = SORT_CORRUPT;
      return _ERROR_;
    }
    link = link->next;
    linkCount++;
  }
  srt->sortError = SORT_CORRUPT;
  return _ERROR_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_restructure_nodes}
 * \index{nsort_restructure_nodes}
 *
 * [Verbatim] */

int nsort_restructure_nodes (nsort_t *srt)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_restructure_nodes() function rebuilds the indexing that is
 * maintained in the ``srt'' object.  This probably does not need to be
 * called by applications, because the nsort routines calls this function
 * as necessary to keep the indexing optimal.  As items are added to an
 * nsort object, they are added in a way that can produce indexing that is
 * suboptimal.  This is specially true if the items that are added are
 * random.\footnote{You will get much better results from the nsort sort
 * routines if you can add items to it that are \emph{almost} ordered rather
 * than randomly ordered items.}  However, statistical data
 * is maintained in the sort
 * object and when a certain threshold has been met a certain number of times,
 * the nsort_restructure_nodes() function is called to rebuild the indexing.
 *
 * This function returns _OK_ on success.  If an error occurs, it will return
 * _ERROR_ and you can use nsort_show_sort_error() to get a description of the
 * error.  If an error occurs here, all further use of the nsort routines on
 * the ``srt'' sort object should be abandoned.
 *
 * [EndDoc]
 */
{
  nsort_node_t *node, *nextNode, *oldLNode;
  nsort_link_t *lnk;
  int thisNumber = NSORT_RESTRUCT;
  int status;
  int i;
  int counter, ctr_lvl[NSORT_NODE_LEVEL];

  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    ctr_lvl[i] = 0;
  /* step 1 - remove nodes (if exist) */
  if (srt->head->next != srt->tail) {
    node = srt->head->next;
    nextNode = node->next;
    while (TRUE) {
      free (node);
      if (nextNode == srt->tail)
        break;
      node = nextNode;
      nextNode = node->next;
    }
  }
  srt->head->prev = 0;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->tail->next = 0;
  srt->current = srt->head;
  srt->numNodes = 0;

  /* step 2 - rebuild nodes (if necessary) */
  if (srt->lh->number < (size_t)thisNumber)
    return _OK_;
  lnk = srt->lh->head->next;
  status = nsort_new_node (srt, srt->head, srt->tail, lnk);
  if (_ERROR_ == status) {
    return _ERROR_;
  }
  while (lnk != srt->lh->tail) {
    for (counter = 0; counter < thisNumber && lnk != srt->lh->tail;
         counter++, lnk = lnk->next);
    if (lnk == srt->lh->tail)
      break;
    status = nsort_new_node (srt, srt->tail->prev, srt->tail, lnk);
    if (_ERROR_ == status) {
      return _ERROR_;
    }
  }
  /* step 3 - rebuild indexes */
  /* traverse the nodes first, building the first high-order index */
  if (srt->numNodes > (size_t)thisNumber) {
    node = srt->head->next;
    oldLNode = node;
    while (node != srt->tail) {
      for (counter = 0; counter < thisNumber && node != srt->tail;
           counter++, node = node->next);
      if (node == srt->tail)
        break;
      oldLNode->level[0] = node;
      oldLNode = node;
      ctr_lvl[0]++;
    }
  }
  /* Then, traverse subsequent high-order index levels */
  for (i = 0; i < NSORT_NODE_LEVEL-1; i++) {
    if (ctr_lvl[i] > thisNumber) {
      node = srt->head->next;
      oldLNode = node;
      while (node != srt->tail) {
        for (counter = 0; counter < thisNumber && node != srt->tail;
            counter++, node = node->level[i]);
        if (node == srt->tail)
          break;
        oldLNode->level[i+1] = node;
        oldLNode = node;
        ctr_lvl[i+1]++;
      }
    }
  }
  srt->numRestruct ++;
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_find_item}
 * \index{nsort_find_item}
 *
 * [Verbatim] */

nsort_link_t *nsort_find_item (nsort_t *srt, nsort_link_t *lnk)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_find_item() function finds the first item that is ``equal to''
 * the lnk->data object.\footnote{Equality here is determined by the compare
 * function.  If the compare function returns 0, the items are equal.}
 * The return value is a pointer to an nsort_link_t item.  Basically, this
 * is done so you can find a particular item and then traverse forward or
 * backward in the list from there.  If nsort_find_item() doesn't find an
 * object that is equal to the one being searched for, then it returns NULL.
 * This function also returns NULL if there is an error condition, so if you
 * get a NULL return value, make sure srt->sortError doesn't reflect an error
 * condition.  Note that ``error condition'' means that srt->sortError is
 * greater than 0 (SORT_NOERROR).
 *
 * [EndDoc]
 */
{
  int status = 0;
  register nsort_link_t *link;
  register nsort_node_t *node;
  int i;
#ifdef NSORT_STATS
  double t1, t2;
#endif

  if (srt->numCompares > NSORT_CRIT_THRESH) {
    srt->thresh = 0;
    status = nsort_restructure_nodes (srt);
    if (status == _ERROR_)
      return 0;
  }
  else if (srt->numCompares > NSORT_OUTPOINT_THRESH) {
    ++srt->thresh;
    if (srt->thresh > NSORT_RSTRUCT_THRESH) {
      srt->thresh = 0;
      status = nsort_restructure_nodes (srt);
      if (status == _ERROR_)
        return 0;
    }
  }
  srt->numCompares = 0;
#ifdef NSORT_STATS
  srt->traversal_time = 0.0;
  nsort_elapsed (&t1);
#endif
  if (srt->head->next == srt->tail) {
    link = srt->lh->head->next;
    while (link != srt->lh->tail) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status == 0) {
        srt->lh->current = link;
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return link;
      }
      link = link->next;
    }
    return 0;
  }
  status = srt->compare (lnk->data, srt->lh->head->next->data);
  srt->numCompares++;
  if (status == 0) {
    srt->current = srt->head->next;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return srt->lh->head->next;
  }
  if (status < 0) {
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return 0;
  }
  status = srt->compare (lnk->data, srt->lh->tail->prev->data);
  srt->numCompares++;
  if (status == 0) {
    link = srt->lh->tail->prev;
    while (link != srt->lh->head) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status != 0)
        break;
      link = link->prev;
    }
    srt->current = srt->tail->prev;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return link->next;
  }
  if (status > 0)
    return 0;
  node = srt->head->next;
  for (i = NSORT_NODE_LEVEL-1; i >= 0; i--) {
    while (node->level[i] != srt->tail) {
      status = srt->compare (lnk->data, node->level[i]->here->data);
      srt->numCompares++;
      if (status <= 0)
        break;
      node = node->level[i];
    }
  }
  // BUGBUG
  status = srt->compare (lnk->data, node->here->data);
  srt->numCompares++;
  if (status <= 0) {
    if (node->prev == srt->head)
      link = srt->lh->head->next;
    else
      link = node->prev->here;
    while (link != node->here) {
      status = srt->compare (lnk->data, link->data);
      if (status == 0) {
        srt->current = node->prev;
        srt->lh->current = link;
        return link;
      }
      if (status < 0)
        return 0;
      link = link->next;
    }
  }
  // BUGBUG
  while (node->next != srt->tail) {
    status = srt->compare (lnk->data, node->next->here->data);
    srt->numCompares++;
    if (status <= 0) {
      link = node->here;
      if (node == srt->head->next) {
        link = srt->lh->head->next;
      }
      while (link != node->next->here) {
        status = srt->compare (lnk->data, link->data);
        srt->numCompares++;
        if (status == 0) {
          srt->current = node;
          srt->lh->current = link;
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link;
        }
        if (status < 0) {
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return 0;
        }
        link = link->next;
      }
    }
    node = node->next;
  }
  if (node->next == srt->tail) {
    link = node->here;
    while (link != srt->lh->tail) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status == 0) {
        srt->current = srt->tail->prev;
        srt->lh->current = link;
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return link;
      }
      if (status < 0) {
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return 0;
      }
      link = link->next;
    }
  }
  return 0;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_query_item}
 * \index{nsort_query_item}
 *
 * [Verbatim] */

nsort_link_t *nsort_query_item (nsort_t *srt, nsort_link_t *lnk)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_query_item() is like nsort_find_item(), except that it only
 * returns a NULL if there is an error.  If it doesn't find an item that
 * is ``equal to'' the lnk->data item, then it returns the link that points
 * the item that would be before lnk->data if it were added to the list.  If
 * NULL is returned, srt->sortError will be set to the error and you can get
 * a description of the error with the nsort_show_sort_error() funciton.
 *
 * [EndDoc]
 */
{
  int status;
  nsort_link_t *link;
  nsort_node_t *node;
  int i;
#ifdef NSORT_STATS
  double t1, t2;
#endif

  if (srt->numCompares > NSORT_CRIT_THRESH) {
    srt->thresh = 0;
    status = nsort_restructure_nodes (srt);
    if (status == _ERROR_)
      return 0;
  }
  else if (srt->numCompares > NSORT_OUTPOINT_THRESH) {
    ++srt->thresh;
    if (srt->thresh > NSORT_RSTRUCT_THRESH) {
      srt->thresh = 0;
      status = nsort_restructure_nodes (srt);
      if (status == _ERROR_)
        return 0;
    }
  }
  srt->numCompares = 0;
#ifdef NSORT_STATS
  srt->traversal_time = 0.0;
  nsort_elapsed (&t1);
#endif
  if (srt->head->next == srt->tail) {
    if (srt->lh->head->next == srt->lh->tail) {
      srt->sortError = SORT_PARAM;
      return 0;
    }
    link = srt->lh->head->next;
    while (link != srt->lh->tail) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status == 0) {
        srt->lh->current = link;
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return link;
      }
      if (status < 0) {
        if (link->prev != srt->lh->head) {
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link->prev;
        }
        else {
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link;
        }
      }
      link = link->next;
    }
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return srt->lh->tail->prev;
  }
  status = srt->compare (lnk->data, srt->lh->head->next->data);
  srt->numCompares++;
  if (status == 0) {
    srt->current = srt->head->next;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return srt->lh->head->next;
  }
  if (status < 0) {
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return srt->lh->head->next;
  }
  status = srt->compare (lnk->data, srt->lh->tail->prev->data);
  srt->numCompares++;
  if (status == 0) {
    if (srt->isUnique) {
#ifdef NSORT_STATS
      nsort_elapsed (&t2);
      srt->traversal_time += (t2-t1);
#endif
      return srt->lh->tail->prev;
    }
    link = srt->lh->tail->prev;
    while (link != srt->lh->head) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status != 0)
        break;
      link = link->prev;
    }
    srt->current = srt->tail->prev;
    if (link->next != srt->lh->tail)
      link = link->next;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return link;
  }
  if (status > 0) {
    srt->current = srt->tail->prev;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return srt->lh->tail->prev;
  }
  node = srt->head->next;
  for (i = NSORT_NODE_LEVEL-1; i >= 0; i--) {
    while (node->level[i] != srt->tail) {
      status = srt->compare (lnk->data, node->level[i]->here->data);
      srt->numCompares++;
      if (status <= 0)
        break;
      node = node->level[i];
    }
  }
  while (node->next != srt->tail) {
    status = srt->compare (lnk->data, node->next->here->data);
    srt->numCompares++;
    if (status <= 0) {
      link = node->here;
      if (node == srt->head->next) {
        link = srt->lh->head->next;
      }
      while (link != node->next->here) {
        status = srt->compare (lnk->data, link->data);
        srt->numCompares++;
        if (status == 0) {
          srt->current = node;
          srt->lh->current = link;
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link;
        }
        if (status < 0) {
          srt->current = node;
          if (link->prev != srt->lh->head) {
            srt->lh->current = link->prev;
#ifdef NSORT_STATS
            nsort_elapsed (&t2);
            srt->traversal_time += (t2-t1);
#endif
            return link->prev;
          }
          else {
            srt->lh->current = link;
#ifdef NSORT_STATS
            nsort_elapsed (&t2);
            srt->traversal_time += (t2-t1);
#endif
            return link;
          }
        }
        link = link->next;
      }
    }
    node = node->next;
  }
  if (node->next == srt->tail) {
    link = node->here;
    while (link != srt->lh->tail) {
      status = srt->compare (lnk->data, link->data);
      srt->numCompares++;
      if (status == 0) {
        srt->current = srt->tail->prev;
        srt->lh->current = link;
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return link;
      }
      if (status < 0) {
        srt->current = node;
        if (link->prev != srt->lh->head) {
          srt->lh->current = link->prev;
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link->prev;
        }
        else {
          srt->lh->current = link;
#ifdef NSORT_STATS
          nsort_elapsed (&t2);
          srt->traversal_time += (t2-t1);
#endif
          return link;
        }
      }
      link = link->next;
    }
  }
  srt->current = srt->tail->prev;
#ifdef NSORT_STATS
  nsort_elapsed (&t2);
  srt->traversal_time += (t2-t1);
#endif
  return srt->lh->tail->prev;
}

/*
 * The user should use nsort_save, so don't document this.
 */
int nsort_store (nsort_t *srt, nsort_store_t *ts,
    int reclen, char *fname)
{
  int status;

  ts->isUnique = srt->isUnique;
  ts->manageAllocs = srt->manageAllocs;
  status = nsort_list_store (srt->lh, ts, reclen, fname);
  Assert (status != _ERROR_);
  if (_ERROR_ == status) {
    if (nsort_check_list_error(srt->lh))
      srt->sortError = srt->lh->listError;
    else if (nsort_check_error())
      srt->sortError = get_sortError();
    else
      srt->sortError = SORT_UNSPECIFIED;
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_save}
 * \index{nsort_save}
 *
 * [Verbatim] */

int nsort_save (nsort_t *srt, const char *desc, int reclen,
    char *fname)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_save() funciton saves the contents of the list managed by
 * ``srt'' to a file.  The parameters to this function are as follows:
 *
 * \begin{itemize}
 *
 * \item [srt] This parameter is an nsort header object that is to be
 * saved to disk.  The data contained in the srt->lh list is stored
 * in the file.
 *
 * \item [desc] The desc item is the same as the desc parameter for the
 * nsort_list_save() function.  It is a string that can be up to 127 bytes
 * long.
 *
 * \item [reclen] This parameter is the length of each record that is to
 * be saved.
 *
 * \item [fname] This is the name of the file to save the data to.  This
 * file will be created if it doesn't exist.  If another file exists with that
 * name, it will be destroyed and the data irrevocably lost.
 *
 * \end{itemize}
 *
 * nsort_save() returns _OK_ if it is successful or _ERROR_ if an error
 * occurs.  If there is an error, srt->sortError will contain the error.
 *
 * [EndDoc]
 */
{
  time_t now;
  struct tm result;
  nsort_store_t *ts;
  char tstamp[27];
  char *cp;
  int status;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (ts, 0, sizeof (nsort_store_t));

  ts->thisMagic = DEFAULT_MAGIC;
  if (desc)
    strncpy (ts->description, desc, 127);
  time (&now);
  localtime_r (&now, &result);
  asctime_r (&result, tstamp);
  cp = strchr (tstamp, '\n');
  if (cp != 0)
    *cp = '\0';
  strcpy (ts->timeStamp, tstamp);
  status = nsort_store (srt, ts, reclen, fname);
  free (ts);
  return status;
}

/*
 * Do not document this as part of the API.
 */
int nsort_retrieve (nsort_t *srt, nsort_store_t *ts,
    const char *fname, long magic)
{
  int status;
  int ctr_lvl[NSORT_NODE_LEVEL];
  int i;

  for (i = 0; i < NSORT_NODE_LEVEL; i++)
    ctr_lvl[i] = 0;
  srt->lh = nsort_list_create ();
  if (nsort_check_error()) {
    srt->sortError = get_sortError();
    set_sortError (SORT_NOERROR);
    return _ERROR_;
  }
  status = nsort_list_retrieve (srt->lh, ts, fname, magic);
  if (srt->lh == 0) {
    if (nsort_check_list_error(srt->lh))
      srt->sortError = srt->lh->listError;
    else if (nsort_check_error())
      srt->sortError = get_sortError();
    else
      srt->sortError = SORT_UNSPECIFIED;
    return _ERROR_;
  }
  srt->head = malloc (sizeof (nsort_node_t));
  if (0 == srt->head) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->head, 0, sizeof (nsort_node_t));
  srt->tail = malloc (sizeof (nsort_node_t));
  if (0 == srt->tail) {
    free (srt->head);
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->tail, 0, sizeof (nsort_node_t));
  srt->current = srt->head;
  srt->isUnique = ts->isUnique;
  srt->manageAllocs = ts->manageAllocs;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->head->prev = 0;
  srt->tail->next = 0;

  status = nsort_restructure_nodes (srt);
  if (status == _ERROR_)
    nsort_del (srt, 0);
  return status;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_get}
 * \index{nsort_get}
 *
 * [Verbatim] */

int nsort_get (nsort_t *srt, int (*compare)(void*,void*), const char *fname)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_get() function reads populates the nsort object ``srt'' with
 * the contents of the data in the file given by  ``fname''.  The following
 * are the parameters for this function:
 *
 * \begin{itemize}
 *
 * \item [srt] This parameter is the nsort object.  It should be created
 * with a call to nsort_create() just for the purpose of storing the data
 * that is retrieved from the file by this function.  It will be initialized
 * here.
 *
 * \item [compare] This is the compare function that will determine the sort
 * order.  It should be the same compare function that was used to sort the
 * data that was stored in the file in the first place.
 *
 * \item [fname] This parameter is the name of the file to read the data
 * from.  The file is opened read-only and the data is read and used to
 * populate the in memory objects.
 *
 * \end{itemize}
 *
 * The nsort_get() function returns _OK_ if it is successful or _ERROR_
 * if there is an error.  On error, you can get a description of the
 * error by calling nsort_show_sort_error().
 *
 * [EndDoc]
 */
{
  nsort_store_t *ts;
  int status;

  memset (srt, 0, sizeof (nsort_t));
  srt->compare = compare;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }

  status = nsort_retrieve (srt, ts, fname, DEFAULT_MAGIC);
  srt->isUnique = ts->isUnique;
  srt->manageAllocs = ts->manageAllocs;
  free (ts);
  if (status == _ERROR_) {
    nsort_del (srt, 0);
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_get_all}
 * \index{nsort_get_all}
 *
 * [Verbatim] */

int nsort_get_all (nsort_t *srt, int (*compare)(void*,void*), const char *fname,
    char *desc, char *timestamp)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_get_all() function is similar to the nsort_get function except
 * that it copies the description and the time stamp that are stored in the
 * file to ``desc'' and ``timestamp'' respectively.  The following are the
 * parameters for this function:
 *
 * \begin{itemize}
 *
 * \item [srt] This parameter is the nsort object.  It should be created
 * with a call to nsort_create() just for the purpose of storing the data
 * that is retrieved from the file by this function.  It will be initialized
 * here.
 *
 * \item [compare] This is the compare function that will determine the sort
 * order.  It should be the same compare function that was used to sort the
 * data that was stored in the file in the first place.
 *
 * \item [fname] This parameter is the name of the file to read the data
 * from.  The file is opened read-only and the data is read and used to
 * populate the in memory objects.
 *
 * \item [desc] This parameter should be a character array with enough
 * storage to hold 127 characters.  The description that was put in the
 * file when it was stored will be copied to this.
 *
 * \item [timestamp] This parameter should be a character array with
 * enough storage to hold 26 characters.  The time stamp that was put
 * in the file when it was stored will be copied to this.
 *
 * \end{itemize}
 *
 * The nsort_get_all() function returns _OK_ if it succeeds and _ERROR_ if there
 * is an error.  On error, you can get a description of what went wrong
 * by calling the nsort_show_sort_error() function.
 *
 * [EndDoc]
 */
{
  nsort_store_t *ts;
  int status;

  memset (srt, 0, sizeof (nsort_t));
  srt->compare = compare;

  ts = malloc (sizeof (nsort_store_t));
  if (0 == ts) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }

  status = nsort_retrieve (srt, ts, fname, DEFAULT_MAGIC);
  srt->isUnique = ts->isUnique;
  srt->manageAllocs = ts->manageAllocs;
  strncpy (desc, ts->description, 127);
  strncpy (timestamp, ts->timeStamp, 26);
  free (ts);
  if (status == _ERROR_) {
    nsort_del (srt, 0);
    return _ERROR_;
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_remove_item}
 * \index{nsort_remove_item}
 *
 * [Verbatim] */

nsort_link_t *nsort_remove_item (nsort_t *srt, nsort_link_t *lnk)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_remove_item() removes the item lnk->data from the list
 * in srt->lh.  If it is successful, it removes the item and returns
 * the link that points to it.  If it is not successful, it returns
 * NULL and srt->sortError points to the error.  A global error will
 * be set if srt == 0, srt->lh == 0 or lnk == 0.
 * (\emph{Note: removes are very inefficient with nsorts, which are
 * optimized for adds.  Use this function sparingly.})
 *
 * [EndDoc]
 */
{
  nsort_node_t *node, *nextNode;
  nsort_link_t *link, *found;
  int status;
#ifdef NSORT_STATS
  double t1, t2;
#endif

  if (srt->numCompares > NSORT_CRIT_THRESH) {
    srt->thresh = 0;
    status = nsort_restructure_nodes (srt);
    if (status == _ERROR_)
      return 0;
  }
  else if (srt->numCompares > NSORT_OUTPOINT_THRESH) {
    ++srt->thresh;
    if (srt->thresh > NSORT_RSTRUCT_THRESH) {
      srt->thresh = 0;
      status = nsort_restructure_nodes (srt);
      if (status == _ERROR_)
        return 0;
    }
  }
  srt->numCompares = 0;

  if (srt == 0 || srt->lh == 0 || lnk == 0) {
    set_sortError (SORT_PARAM);
    return 0;
  }

  // BUGBUG
  // Remove this and test with it gone.  Basically, we use the same algorithm
  // whether this is a unique index or not, which should be OK.  It protects us from having
  // performance issues with having to nsort_restructure_nodes().
  // BUGBUG
#if 0
  if (srt->isUnique) {
    // traversal_time will have the find time.
    if (0 == found) {
      srt->sortError = SORT_CORRUPT;
      return 0;
    }
#ifdef NSORT_STATS
    nsort_elapsed (&t1);
#endif
    if (lnk == srt->current->here) {
      srt->lh->current = lnk;
      link = nsort_list_remove_link (srt->lh);
      if (link == 0) {
        srt->sortError = srt->lh->listError;
        return 0;
      }
      // XXX: Fix this - find some way to insure that this doesn't have to
      // happen on every remove.  The issue I have is that I don't want
      // to slow down processing on adds, so I feel that right now this is
      // a fair trade off.  If I have an application that needs to delete
      // stuff from sorted items, though, this will have to be resolved.
      nsort_restructure_nodes (srt);
#ifdef NSORT_STATS
      nsort_elapsed (&t2);
      srt->traversal_time += (t2-t1);
#endif
      return link;
    }
    else {
      srt->lh->current = lnk;
      link = nsort_list_remove_link (srt->lh);
      if (link == 0) {
        srt->sortError = srt->lh->listError;
        return 0;
      }
      // XXX - Ditto here.
#ifdef NSORT_STATS
      nsort_elapsed (&t2);
      srt->traversal_time += (t2-t1);
#endif
      nsort_restructure_nodes (srt);
      return link;
    }
  }
  /*
   * Not unique - have to jump through some hoops.
   */
#endif
  found = nsort_query_item (srt, lnk);
  if (found == 0) {
    srt->sortError = SORT_CORRUPT;
    return 0;
  }
#ifdef NSORT_STATS
    nsort_elapsed (&t1);
#endif
  node = srt->current;
  if (node->here == lnk) {
    /*
     * We are on a node
     */
    srt->lh->current = lnk;
    link = nsort_list_remove_link (srt->lh);
    if (link == 0) {
      srt->sortError = srt->lh->listError;
      return 0;
    }
    status = nsort_restructure_nodes (srt);
    if (status == _ERROR_)
      return 0;
#ifdef NSORT_STATS
    nsort_elapsed (&t2);
    srt->traversal_time += (t2-t1);
#endif
    return link;
  } 
  nextNode = node->next;
  if (nextNode->here != 0) {
    status = srt->compare (lnk->data, nextNode->here->data);
    srt->numCompares++;
    while (status == 0) {
      if (nextNode->here == lnk) {
        srt->lh->current = lnk;
        link = nsort_list_remove_link (srt->lh);
        if (0 == link) {
          srt->sortError = srt->lh->listError;
          return 0;
        }
        status = nsort_restructure_nodes (srt);
        if (status == _ERROR_)
          return 0;
#ifdef NSORT_STATS
        nsort_elapsed (&t2);
        srt->traversal_time += (t2-t1);
#endif
        return link;
      }
      nextNode = nextNode->next;
      if (nextNode == srt->tail || nextNode->here == 0)
        break;
      else {
        status = srt->compare (lnk->data, nextNode->here->data);
        srt->numCompares++;
      }
    }
  }
  /*
   * It looks like the link is not on a node; just remove it.
   */
  srt->lh->current = lnk;
  link = nsort_list_remove_link (srt->lh);
  if (0 == link) {
    srt->sortError = SORT_CORRUPT;
    return 0;
  }
  return link;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_node_levels}
 * \index{nsort_node_levels}
 *
 * [Verbatim] */

int nsort_node_levels (nsort_t *srt, nsort_node_level_t *lvl)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_node_levels() function gathers information about the indexes
 * that are managed by the sort object ``srt'' and puts it in the
 * ``lvl'' variable.  This can be used to determine how many nodes are in
 * the higher level indexes.  Currently, this only returns _OK_.
 *
 * [EndDoc]
 */
{
  int counter;
  nsort_node_t *node;
  int i;

  for (i = NSORT_NODE_LEVEL-1; i >= 0; i--) {
    node = srt->head->next;
    if (node->level[i] == srt->tail)
      lvl->lvl[i] = 0;
    else {
      counter = 0;
      while (node->level[i] != srt->tail) {
        node = node->level[i];
        counter++;
      }
      lvl->lvl[i] = counter;
    }
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_sort_to_list}
 * \index{nsort_sort_to_list}
 *
 * [Verbatim] */

nsort_list_t *nsort_sort_to_list (nsort_t *srt)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_sort_to_list() function dismantles the nsort object ``srt''
 * and returns the list in srt->lh.  Everything is freed except the
 * srt object itself.  If it was created with a call to nsort_create(),
 * it will need to be freed by calling nsort_destroy().  If it is
 * successful, it returns a pointer to the list header that is used to
 * manage the list.  If it fails, it returns NULL.  The only possible
 * failure is that the function parameter is invalid, so the error
 * is put in the global variable.  You can look at a description of the
 * error with a call to nsort_show_error().
 *
 * [EndDoc]
 */
{
  nsort_list_t *lh;
  nsort_node_t *node, *nextNode;

  if (srt == 0 || srt->lh == 0) {
    set_sortError (SORT_CORRUPT);
    return 0;
  }
  lh = srt->lh;
  srt->lh = 0;

  /*
   * Now, just dismantle the shell and return the list.
   */
  if (srt->head->next != srt->tail) {
    node = srt->head->next;
    nextNode = node->next;
    while (TRUE) {
      free (node);
      if (nextNode == srt->tail)
        break;
      node = nextNode;
      nextNode = node->next;
    }
  }
  free (srt->head);
  free (srt->tail);
  return lh;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_list_to_sort}
 * \index{nsort_list_to_sort}
 *
 * [Verbatim] */

int nsort_list_to_sort (nsort_t *srt, nsort_list_t *lh, int (*cmp)(void*,void*))

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_to_sort() function takes the list given by ``lh'' and builds
 * the management objects around it needed to maintain a sorted object.  The
 * parameters of this function are as follows:
 *
 * \begin{itemize}
 *
 * \item [srt] This parameter is an nsort_t object that has been allocated
 * on the heap with a call to nsort_create() or allocated on the stack
 * as an automatic variable.  There is no need to call nsort_init() on this
 * item as all the necessary parts are built into the srt object by this
 * function.
 *
 * \item [lh] This parameter is the list header of a list that contains
 * sorted data.  Dire things will happen if the data contained by this list
 * is not sorted.  The list header lh needs to be created on the heap by
 * a call to nsort_list_create(); it should not be an item allocated on the
 * stack as a local variable.
 *
 * \item [cmp] This is the compare function that the sort object will
 * use for further adds and for finds.  It should be the same compare function
 * that was used to sort the items in the list.
 *
 * \end{itemize}
 *
 * This function returns _OK_ if it succeeds and _ERROR_ if there is an
 * error.  If there is an error, srt->sortError contains the error.
 *
 * [EndDoc]
 */
{
  int status;

  memset (srt, 0, sizeof (nsort_t));
  srt->compare = (int (*)(void *, void *)) cmp;

  srt->lh = lh;
  srt->head = malloc (sizeof (nsort_node_t));
  if (0 == srt->head) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->head, 0, sizeof (nsort_node_t));
  srt->tail = malloc (sizeof (nsort_node_t));
  if (0 == srt->tail) {
    free (srt->head);
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->tail, 0, sizeof (nsort_node_t));
  srt->current = srt->head;
  srt->isUnique = FALSE;
  srt->manageAllocs = TRUE;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->head->prev = 0;
  srt->tail->next = 0;

  status = nsort_restructure_nodes (srt);
  if (_ERROR_ == status) {
    /* error is set */
    nsort_del (srt, 0);
    return _ERROR_;
  }
  return _OK_;
}

/*
 * These sh functions are not API functions.  Don't document them.
 *
 * Make sure one of them and only one is called nsort_hash_function()
 */
/* unsigned long nsort_hash_function (register const unsigned char *key) */
unsigned long nsort_hash_function (register const unsigned char *key)
{
  unsigned long val = 0;

  while (*key != '\0') {
    unsigned int tmp;
    val = (val << 4) + (*key);
    tmp = (val & 0xf0000000);
    if (tmp) {
      val = val ^ (tmp >> 24);
      val = val ^ tmp;
    }
    key++;
  }
  return val % NSORT_HASH_SIZE;
}

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

/*
#define mix2(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<< 8); \
  c -= a; c -= b; c ^= ((b&0xffffffff)>>13); \
  a -= b; a -= c; a ^= ((c&0xffffffff)>>12); \
  b -= c; b -= a; b = (b ^ (a<<16)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>> 5)) & 0xffffffff; \
  a -= b; a -= c; a = (a ^ (c>> 3)) & 0xffffffff; \
  b -= c; b -= a; b = (b ^ (a<<10)) & 0xffffffff; \
  c -= a; c -= b; c = (c ^ (b>>15)) & 0xffffffff; \
}
*/

/* register unsigned char *k; */
/* unsigned long nsort_hash_function_jenkins ( register const unsigned char *k ) */
unsigned long nsort_hash_function_jenkins ( register const unsigned char *k )
{
    register unsigned long  length = strlen ((const char *)k);   /* the length of the key */
    register unsigned long  initval = (2<<16)-1;    /* the previous hash, or an arbitrary value */
    register unsigned long a,b,c,len;

    /* Set up the internal state */
    len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;           /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (k[0] +((unsigned long)k[1]<<8) +((unsigned long)k[2]<<16) +((unsigned long)k[3]<<24));
        b += (k[4] +((unsigned long)k[5]<<8) +((unsigned long)k[6]<<16) +((unsigned long)k[7]<<24));
        c += (k[8] +((unsigned long)k[9]<<8) +((unsigned long)k[10]<<16)+((unsigned long)k[11]<<24));
        mix(a,b,c);
        k += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += length;
    switch(len)              /* all the case statements fall through */
    {
    case 11: c+=((unsigned long)k[10]<<24);
    case 10: c+=((unsigned long)k[9]<<16);
    case 9 : c+=((unsigned long)k[8]<<8);
       /* the first byte of c is reserved for the length */
    case 8 : b+=((unsigned long)k[7]<<24);
    case 7 : b+=((unsigned long)k[6]<<16);
    case 6 : b+=((unsigned long)k[5]<<8);
    case 5 : b+=k[4];
    case 4 : a+=((unsigned long)k[3]<<24);
    case 3 : a+=((unsigned long)k[2]<<16);
    case 2 : a+=((unsigned long)k[1]<<8);
    case 1 : a+=k[0];
      /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c % NSORT_HASH_SIZE;
}


/*
 * [BeginDoc]
 *
 * \subsection{Nsort Hash Functions}
 *
 * The nsort hash functions provide the capabilities of a hash implementation
 * built on the nsort functionality.  Basically, the buckets for the hash
 * implementation are nsort sort objects.  The hash implementation herein is
 * high performance, but is overkill for a few number of items (say, less than
 * 100,000 items or so).
 *
 * Error handling for the hash functions is slightly different than it is for
 * the other nsort functions.  Basically, if an error occurs, the hash object
 * has an item called hashError that is set to the error.  You can get a 
 * description of this error by looking at sortErrorString[hsh->hashError].
 * This is thread safe because the sortErrorString array is read-only and
 * the hsh->hashError is local to the object.  Therefore, neither is likely
 * to be altered by another thread as you are gathering the data.
 *
 * \subsubsection{nsort_hash_create}
 * \index{nsort_hash_create}
 *
 * [Verbatim] */

nsort_hash_t *nsort_hash_create (void)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_create() function allocates an nsort hash item on the
 * heap and returns a pointer to it.  It returns NULL if it fails and you
 * can look at a description of the error with nsort_show_error().
 * The hash item doesn't have to be created on the heap.  It can be allocated
 * as an automatic variable on the stack, in which case this function needn't
 * be called.
 *
 * [EndDoc]
 */
{
  nsort_hash_t *hsh;
  hsh = malloc (sizeof (nsort_hash_t));
  if (0 == hsh) {
    set_sortError (SORT_NOMEMORY);
    return 0;
  }
  return hsh;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_hash_destroy}
 * \index{nsort_hash_destroy}
 *
 * [Verbatim] */

int nsort_hash_destroy (nsort_hash_t *hsh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_destroy() function deallocates a hash object that was
 * created with nsort_hash_create().  The only error occurs if the
 * parameter to the function is invalid.  In that case, you can get a 
 * description of the error using nsort_show_error().  nsort_hash_destroy()
 * returns _OK_ if it succeeds.
 *
 * [EndDoc]
 */
{
  if (hsh == 0) {
    set_sortError (SORT_PARAM);
    return _ERROR_;
  }
  check_pointer (hsh);
  free (hsh);
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_hash_init}
 * \index{nsort_hash_init}
 *
 * [Verbatim] */

int nsort_hash_init (nsort_hash_t *hsh,
    int (*compare) (void*,void*),
    unsigned int (*hash)(const char *))

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_init() function initializes a hash object so it can
 * be used by the hash functions.  The following are the parameters to
 * this function:
 *
 * \begin{itemize}
 *
 * \item [hsh] This is the hash object.  It can be allocated on the heap
 * with a call to nsort_hash_create() or on the stack as an automatic
 * variable.
 *
 * \item [compare] This is a pointer to a compare function.  It will be used
 * in the nsort sort buckets to determine sort order.
 *
 * \item [hash] This is the pointer to a hash function.  If it not NULL, it
 * should be a function that ``hashes well'' for the application that you are
 * implementing.  A good hash function will fairly evenly divide the elements
 * in the application domain across the buckets.  If you set this parameter to
 * NULL, a default hash function will be used which is known to be good for
 * strings.
 * 
 * \end{itemize}
 *
 * This function returns _OK_ on success or _ERROR_ on error.  If there is an
 * error, hsh->hashError will contain the error.  You can get a
 * a description of the error from the read-only string
 * sortErrorString[hsh->hashError].
 *
 * [EndDoc]
 */
{
  int i, j;
  int status;

  if (compare == 0) {
    hsh->hashError = SORT_PARAM;
    return _ERROR_;;
  }
  // Set reasonable defaults.
  memset (hsh, 0, sizeof (nsort_hash_t));
  if (hash == 0)
    hsh->hash = (unsigned int(*)(const char*))nsort_hash_function;
  else
    hsh->hash = hash;
  for (i = 0; i < NSORT_HASH_SIZE; i++) {
    hsh->srts[i] = nsort_create ();
    if (hsh->srts[i] == 0) {
      for (j = 0; j < i; j++) {
	      nsort_del (hsh->srts[j], 0);
      }
      hsh->hashError = SORT_NOMEMORY;
      return _ERROR_;;
    }
    status = nsort_init (hsh->srts[i], compare, TRUE, TRUE);
    if (status == _ERROR_) {
      if (hsh->srts[i]->sortError != SORT_NOERROR)
        hsh->hashError = hsh->srts[i]->sortError;
      else if (nsort_check_error())
        hsh->hashError = get_sortError();
      else
        // shouldn't get here, but...
        hsh->hashError = SORT_UNSPECIFIED;
      for (j = 0; j < i; j++) {
      	nsort_del (hsh->srts[j], 0);
      }
      return _ERROR_;;
    }
  }
  return _OK_;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_hash_add_item}
 * \index{nsort_hash_add_item}
 *
 * [Verbatim] */

int nsort_hash_add_item (nsort_hash_t *hsh, const char *item)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_add_item() adds the item given by ``item'' to the hash
 * managed by the hsh object.  It returns _OK_ if it succeeds and
 * _ERROR_ if not.  If there is an error, hsh->hashError will contain the
 * error.  The string that is being added to the hash is copied into
 * a buffer that is allocated on the heap and added there.  From that
 * point on, it is managed by the nsort hash functions.
 *
 * [EndDoc]
 */
{
  int status;
  unsigned int hash_offset;
  nsort_link_t *lnk;
  char *data;

  if (hsh == 0 || item == 0 || item[0] == '\0') {
    hsh->hashError = SORT_PARAM;
    return _ERROR_;
  }
  hash_offset = hsh->hash (item);
  lnk = malloc (sizeof (nsort_link_t));
  if (0 == lnk) {
    hsh->hashError = SORT_NOMEMORY;
    return _ERROR_;
  }
  data = malloc (strlen (item)+1);
  if (0 == data) {
    hsh->hashError = SORT_NOMEMORY;
    free (lnk);
    return _ERROR_;
  }
  strcpy (data, item);
  lnk->data = data;
  hsh->numCompares = 0;
  status = nsort_add_item (hsh->srts[hash_offset], lnk);
  if (status != _ERROR_) {
    hsh->number++;
    hsh->numCompares = hsh->srts[hash_offset]->numCompares;
  }
  if (status == _ERROR_) {
    if (hsh->srts[hash_offset]->sortError != SORT_NOERROR) {
      hsh->hashError = hsh->srts[hash_offset]->sortError;
      hsh->srts[hash_offset]->sortError = SORT_NOERROR;
    }
    else if (nsort_check_error())
      hsh->hashError = get_sortError();
    else
      hsh->hashError = SORT_UNSPECIFIED;
  }
  return status;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_hash_find_item}
 * \index{nsort_hash_find_item}
 *
 * [Verbatim] */

char *nsort_hash_find_item (nsort_hash_t *hsh, const char *item)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_find_item() will search the hash object for the
 * item given by ``item''.  If it finds it, it will return a read only
 * pointer to it.  Otherwise, it will return NULL.  If NULL is returned, 
 * hsh->hashError should be checked to insure there was no error.  An error
 * condition is defined by hsh->hashError > 0 (SORT_NOERROR is 0).
 *
 * [EndDoc]
 */
{
  nsort_link_t *found;
  nsort_link_t *lnk;
  register char *data;
  unsigned int hash_offset;

  if (hsh == 0 || item == 0 || item[0] == '\0') {
    hsh->hashError = SORT_PARAM;
    return 0;
  }
  hash_offset = hsh->hash (item);
  lnk = malloc (sizeof (nsort_link_t));
  if (0 == lnk) {
    hsh->hashError = SORT_NOMEMORY;
    return 0;
  }
  data = malloc (strlen (item)+1);
  if (0 == data) {
    hsh->hashError = SORT_NOMEMORY;
    free (lnk);
    return 0;
  }
  strcpy (data, item);
  lnk->data = data;
  hsh->numCompares = 0;
  found = nsort_find_item (hsh->srts[hash_offset], lnk);
  hsh->numCompares = hsh->srts[hash_offset]->numCompares;
  free (data);
  free (lnk);
  if (found != 0)
    return found->data;
  else {
    if (hsh->srts[hash_offset]->sortError != SORT_NOERROR) {
      hsh->hashError = hsh->srts[hash_offset]->sortError;
      hsh->srts[hash_offset]->sortError = SORT_NOERROR;
    }
    else if (nsort_check_error())
      hsh->hashError = get_sortError();
    else
      hsh->hashError = SORT_NOERROR;
  }
  return 0;
}

/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_hash_del}
 * \index{nsort_hash_del}
 *
 * [Verbatim] */

int nsort_hash_del (nsort_hash_t *hsh)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_hash_del() function will clear all the hash buckets and
 * hashed items and free up all the memory associated with the hash.  If
 * the hsh object was allocated with nsort_hash_create(), it will need to
 * be freed with nsort_hash_destroy().
 *
 * [EndDoc]
 */
{
  int i;
  if (hsh == 0) {
    hsh->hashError = SORT_PARAM; 
    return _ERROR_;
  }
  for (i = 0; i < NSORT_HASH_SIZE; i++) {
    nsort_del (hsh->srts[i], 0);
    nsort_destroy (hsh->srts[i]);
    hsh->srts[i] = 0;
  }
  return _OK_;
}

#ifdef DEBUG

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup
#undef print_block_list

static void add_to_list (void *p, int size, char *file, int line, int type);
static void remove_block (void *address, char *file, int line, int type);
static void alloc_error (char *mesg, char *file, int line, int type);

#ifndef MARK_INACTIVE
#define MARK_INACTIVE  0   /* 0 if don't want nodes to be retained */
  /* 1 if garbage from list nodes is ok */
#endif  /*  */

#define bool int
  enum
{ MALLOC, CALLOC, REALLOC, FREE, USER_CALL, STRDUP, NEW, DELETE };
static char *type_string[] =
  { "malloc", "calloc", "realloc", "free", "user-call", "strdup", "new",
"delete"
};

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define FILE_LEN PATH_MAX
struct node
{
  char file[FILE_LEN + 1];    /* Filename of allocation call */
  int line;                   /* Line number of allocation call */
  int size;                   /* Size of the allocated block */
  void *address;              /* Address of the real allocated block */
  void *memory;               /* Address of the memory to return to caller */
  unsigned long *header;      /* Points to the header */
  unsigned long *trailer;     /* Points to the footer */
  int type;                   /* Block from malloc, calloc or realloc? */
  bool active;                /* Flag if block currently active, or if */
                              /*   has already been freed              */
  struct node *next;          /* Linked list pointer */
};

static struct node *list = NULL;/* Head of linked list of header blocks */

void *nsort_malloc (int size, char *file, int line)
{
  void *p;
  p = malloc (size + SIZE_DEBUG);   /* call the real malloc */
  if (p != NULL)
    add_to_list (p, size, file, line, MALLOC);
  else {
    alloc_error ("malloc returned NULL", file, line, MALLOC);
    exit (EXIT_FAILURE);
  }
  return (void *) ((char *) p + SIZE_DEBUG_HEADER);
}

void *nsort_inside_malloc (int size, char *file, int line, int type)
{
  void *p;
  p = malloc (size + SIZE_DEBUG);   /* call the real malloc */
  if (p != NULL)
    add_to_list (p, size, file, line, type);
  else {
    alloc_error ("malloc returned NULL", file, line, type);
    exit (EXIT_FAILURE);
  }
  return (void *) ((char *) p + SIZE_DEBUG_HEADER);
}

void *nsort_calloc (int size, int num, char *file, int line)
{
  void *p;
  p = malloc (size * num + SIZE_DEBUG);
  if (p != NULL) {
    add_to_list (p, size * num, file, line, CALLOC);
    memset ((void *) ((char *) p + SIZE_DEBUG_HEADER), 0, num * size);
  }
  else {
    alloc_error ("calloc returned NULL", file, line, CALLOC);
    exit (EXIT_FAILURE);
  }
  return ((void *) ((char *) p + SIZE_DEBUG_HEADER));
}

void nsort_free (void *p, char *file, int line)
{
  if (p == NULL) {
    alloc_error ("free(NULL) is strange usage (although legal)", file, line,
                  FREE);
    return;                   /* do nothing - free(NULL) has no effect */
  }
  if (!check_valid (p, file, line, FREE))
    return;
  remove_block (p, file, line, FREE);
  free ((void *) ((char *) p - SIZE_DEBUG_HEADER));   /* Call the real free */
}

void nsort_internal_free (void *p, char *file, int line)
{
  if (p == NULL) {
    alloc_error ("free(NULL) is strange usage (although legal)", file, line,
                  DELETE);
    return;                   /* do nothing - free(NULL) has no effect */
  }
  if (!check_valid (p, file, line, DELETE))
    return;
  remove_block (p, file, line, DELETE);
  free ((void *) ((char *) p - SIZE_DEBUG_HEADER));   /* Call the real free */
}

void *nsort_realloc (void *p, int newsize, char *file, int line)
{
  void *n;
  if (p == NULL) {
    alloc_error ("realloc(NULL,...) is strange usage (although legal)", file,
                  line, REALLOC);
  }
  else {                        /* pointer is not NULL */
    if (!check_valid (p, file, line, REALLOC))
      return NULL;            /* return NULL for bad reallocation */
  }
  remove_block (p, file, line, REALLOC);
  nsort_free (p, NULL, 0);
  n = nsort_inside_malloc (newsize, file, line, REALLOC);
  if (n == NULL) {
    alloc_error ("realloc returned NULL", file, line, REALLOC);
    exit (EXIT_FAILURE);
  }
  return n;
}

#if STRDUP_EXISTS
char *nsort_strdup (char *s, char *file, int line)
{
  char *p;
  p = (char *) nsort_inside_malloc (strlen (s) + 1, file, line, STRDUP);
  if (p != NULL)
    strcpy (p, s);
  return p;
}
#endif  /* STRDUP_EXISTS */

static void add_to_list (void *p, int size, char *file, int line, int type)
{
  struct node *n;
  n = (struct node *) malloc (sizeof (struct node));        /* Make new header
no
de */
  if (n == NULL) {
    alloc_error
      ("(Internal) malloc returned NULL; unable to track allocation", file,
       line, type);
    return;
  }
  n->address = (void *) ((char *) p + SIZE_DEBUG_HEADER);
  n->memory = p;
  n->size = size;
  n->header = (unsigned long *) p;
  n->trailer = (unsigned long *) ((char *) p + size + SIZE_DEBUG_HEADER);
  *n->header = DBGMEM_HEADER;
  *n->trailer = DBGMEM_TRAILER;

  /* Need to be careful that __FILE__ is not too long */
  strncpy (n->file, file, FILE_LEN);
  n->file[FILE_LEN] = '\0';   /* ensure terminating zero present */
  n->line = line;
  n->type = type;
  n->active = TRUE;           /* Has not been freed yet */
  n->next = list;             /* Add new header node to front of list */
  list = n;
}

static void remove_block (void *address, char *file, int line, int type)
{
  struct node *prev, *temp;
  for (prev = NULL, temp = list; temp != NULL && temp->address != address;
         prev = temp, temp = temp->next)
    ; /* empty for loop */
  if (temp == NULL) {
      /* should always find the block on list */
      alloc_error ("Internal block list corrupted, address not found", file,
                   line, type);
    return;
  }
#if MARK_INACTIVE
    temp->active = FALSE;       /* Dont really free the block, */
#else
    if (prev == NULL)           /* Delete block from the linked list */
    list = temp->next;                /* Delete from front of linked list */
  else
    prev->next = temp->next;  /* Delete from middle or end */
  free (temp);

#endif  /*  */
}

static void alloc_error (char *mesg, char *file, int line, int type)
{
  fprintf (stdout, "DBG ALLOCATION ERROR: %s\n", mesg);
  fprintf (stdout, "                                  ");
  if (type != USER_CALL)
    fprintf (stdout, "Call to %s: ", type_string[type]);
  fprintf (stdout, "File '%s',  Line %d\n", file, line);
  print_block_list ();
  exit (EXIT_FAILURE);
}

void print_block_list (void)
{
  struct node *temp;
  int count = 0;
  fprintf (stdout, "\n              CURRENT HEAP STATUS \n");
  for (temp = list; temp != NULL; temp = temp->next) {
    if (temp->active) {
      count++;
      fprintf (stdout, "DBG: %s: Address %p, Size %d, File: %s, Line %d\n",
                type_string[temp->type], temp->address, temp->size,
                temp->file, temp->line);
    }
  }
  if (count == 0)
    fprintf (stdout, "DBG: No blocks on heap\n");
  fprintf (stdout, "\n");
}

int check_valid (void *address, char *file, int line, int type)
{
  struct node *temp;
  bool found_inactive = FALSE;
  for (temp = list; temp != NULL; temp = temp->next) {
    if (temp->address == address) {
      if (temp->active) {     /* found a valid address */

          /* but still check for mismatches */
          if (type == FREE && temp->type == NEW)
          alloc_error ("Block free'd was allocated using new", file, line,
                        type);

        else
        if (type == DELETE
               && (temp->type == MALLOC || temp->type == CALLOC
                     || temp->type == REALLOC))
          alloc_error
            (" Block delete'd was allocated using malloc/calloc/realloc", file,
             line, type);

          /*
           * Check the header, trailer values
           */
          if (*temp->header != DBGMEM_HEADER
              || *temp->trailer != DBGMEM_TRAILER)
          alloc_error (" Block overwritten during program execution", file,
                        line, type);
        return TRUE;          /* it is a valid block */
      }

      else                      /* not an active block */
        found_inactive = TRUE;
    }
  }
  if (found_inactive) {     /* Found block but already freed */
    if (type == USER_CALL)
      alloc_error ("Pointer to block already freed", file, line, type);

    else
      alloc_error ("Memory block freed twice", file, line, type);
  }

  else {                        /* Not valid (not on list of blocks) */
    if (type == USER_CALL)
      alloc_error ("Pointer not an allocated block", file, line, type);

    else
    if (type == DELETE)
      alloc_error ("Block delete'd is not allocated", file, line, type);

    else
      alloc_error ("Block free'd is not allocated", file, line, type);
  }
  return FALSE;               /* Not a legal block */
}

int check_address (void *address, char *file, int line)
{
  struct node *temp;
  for (temp = list; temp != NULL; temp = temp->next) {
    if (temp->active && address >= temp->address
         &&(char *) address < (char *) temp->address + temp->size)
      return TRUE;            /* Address is in this block */
  }

    /* Address is not valid */
    alloc_error ("Pointer not inside block", file, line, USER_CALL);
  return FALSE;
}

int nsort_debug_list_check (nsort_list_t *lh, int level, char *file, int line)
{
#ifdef DEBUG
  nsort_link_t *lnk;

  if (!check_pointer (lh->head))
    printf ("DBG: check_pointer (lh->head) failed on line %d, file %s\n", line, file);
  if (!check_pointer (lh->tail))
    printf ("DBG: check_pointer (lh->tail) failed on line %d, file %s\n", line, file);
  if (level > 0) {
    lnk = lh->head->next;
    while (lnk != lh->tail) {
      if (!check_pointer (lnk))
        printf ("DBG: check_pointer (lnk) failed on line %d, file %s\n", line, file);
      lnk = lnk->next;
    }
  }
#endif

  return _OK_;
}

int nsort_debug_sort_check (nsort_t *srt, int level, char *file, int line)
{
#ifdef DEBUG
  if (!check_pointer (srt->head)) {
    printf ("DBG: check_pointer (srt->head) failed on line %d, file %s\n", line, file);
    return _ERROR_;
  }
  if (!check_pointer (srt->tail)) {
    printf ("DBG: check_pointer (srt->head) failed on line %d, file %s\n", line, file);
    return _ERROR_;
  }
  if (check_pointer (srt->current)) {
    printf ("DBG: check_pointer (srt->head) failed on line %d, file %s\n", line, file);
    return _ERROR_;
  }
  if (FALSE == nsort_debug_list_check (srt->lh, level, file, line))
    return _ERROR_;
#endif
  return _OK_;
}

#endif /* DEBUG */


/* Copyright Abandoned 1996 TCX DataKonsult AB & Monty Program KB & Detron HB
   This file is public domain and comes with NO WARRANTY of any kind */

void returnClean (void*p);

/* Envoke the comparison function, returns either 0, < 0, or > 0. */
#define CMP(A,B) ((*cmp)((A),(B)))

#define min(a,b)        ((a)<(b)?(a):(b))

/* Byte-wise swap two items of size SIZE. */
#define SWAP(A,B,SIZE) do {int sz=(int)(SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

/* Copy SIZE bytes from item B to item A. */
#define COPY(A,B,SIZE) {int sz = (int) (SIZE); do { *(A)++ = *(B)++; } while (--sz); }

/* This should be replaced by a standard ANSI macro. */
#define BYTES_PER_WORD 8

/* The next 4 #defines implement a very fast in-line stack abstraction. */
#define STACK_SIZE (BYTES_PER_WORD * sizeof (long))
#define PUSH(LOW,HIGH) do {top->lo = LOW;top++->hi = HIGH;} while (0)
#define POP(LOW,HIGH)  do {LOW = (--top)->lo;HIGH = top->hi;} while (0)
#define STACK_NOT_EMPTY (stack < top)

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sparc SLC. */
#define MAX_THRESH 12

/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
{
  char *lo;
  char *hi;
} stack_node;

/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of MAX_INT is allocated on the
      stack.  Assuming a 32-bit integer, this needs only 32 *
      sizeof (stack_node) == 136 bits.  Pretty cheap, actually.

   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.

   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segements.

   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (n)
      stack size is needed (actually O(1) in this case)! */

#if defined(QSORT_TYPE_IS_VOID)
#define SORT_RETURN return
#else
#define SORT_RETURN return 0
#endif

int bqsort (void *base_ptr, int total_elems, int size, int(*cmp)(void*,void*))
{
  /* Allocating SIZE bytes for a pivot buffer facilitates a better
     algorithm below since we can do comparisons directly on the pivot.
     */
  int   max_thresh   = (int) (MAX_THRESH * size);
  if (total_elems <= 1)
    SORT_RETURN;                /* Crashes on MSDOS if continues */

  if (total_elems > MAX_THRESH)
  {
    char       *lo = base_ptr;
    char       *hi = lo + size * (total_elems - 1);
    stack_node stack[STACK_SIZE]; /* Largest size needed for 32-bit int!!! */
    stack_node *top = stack + 1;
    char *pivot_buffer = (char *) malloc ((int) size);

    while (STACK_NOT_EMPTY)
    {
      char *left_ptr;
      char *right_ptr;
      {
        char *pivot = pivot_buffer;
        {
          /* Select median value from among LO, MID, and HI. Rearrange
             LO and HI so the three values are sorted. This lowers the
             probability of picking a pathological pivot value and
             skips a comparison for both the LEFT_PTR and RIGHT_PTR. */

          char *mid = lo + size * (((unsigned int) (hi - lo) / (unsigned int) size) >> 1);

          if (CMP(hi,lo) < 0)
            SWAP (hi, lo, size);
          if (CMP (mid, lo) < 0)
            SWAP (mid, lo, size);
          else if (CMP (hi, mid) < 0)
            SWAP (mid, hi, size);
          COPY (pivot, mid, size);
          pivot = pivot_buffer;
        }
        left_ptr  = lo + size;
        right_ptr = hi - size;

        /* Here's the famous ``collapse the walls'' section of quicksort.
           Gotta like those tight inner loops!  They are the main reason
           that this algorithm runs much faster than others. */
        do
        {
          while (CMP (left_ptr, pivot) < 0)
            left_ptr += size;

          while (CMP (pivot, right_ptr) < 0)
            right_ptr -= size;

          if (left_ptr < right_ptr)
          {
            SWAP (left_ptr, right_ptr, size);
            left_ptr += size;
            right_ptr -= size;
          }
          else if (left_ptr == right_ptr)
          {
            left_ptr += size;
            right_ptr -= size;
            break;
          }
        }
        while (left_ptr <= right_ptr);
      }

      /* Set up pointers for next iteration.  First determine whether
         left and right partitions are below the threshold size. If so,
         ignore one or both.  Otherwise, push the larger partition's
         bounds on the stack and continue sorting the smaller one. */

      if ((right_ptr - lo) <= max_thresh)
      {
        if ((hi - left_ptr) <= max_thresh)      /* Ignore both small parts. */
          POP (lo, hi);
        else                                    /* Ignore small left part. */
          lo = left_ptr;
      }
      else if ((hi - left_ptr) <= max_thresh)   /* Ignore small right part. */
        hi = right_ptr;
      else if ((right_ptr - lo) > (hi - left_ptr)) /* Push larger left part */
      {
        PUSH (lo, right_ptr);
        lo = left_ptr;
      }
      else                                      /* Push larger right part */
      {
        PUSH (left_ptr, hi);
        hi = right_ptr;
      }
    }
    free(pivot_buffer);
  }

  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */

  {
    char *end_ptr = (char*) base_ptr + size * (total_elems - 1);
    char *run_ptr;
    char *tmp_ptr = (char*) base_ptr;
    char *thresh  = min (end_ptr, (char*) base_ptr + max_thresh);

    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */

    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if (CMP (run_ptr, tmp_ptr) < 0)
        tmp_ptr = run_ptr;

    if (tmp_ptr != (char*) base_ptr)
      SWAP (tmp_ptr, (char*) base_ptr, size);

    /* Insertion sort, running from left-hand-side up to `right-hand-side.'
       Pretty much straight out of the original GNU qsort routine. */

    for (run_ptr = (char*) base_ptr + size;
         (tmp_ptr = run_ptr += size) <= end_ptr; )
    {
      while (CMP (run_ptr, tmp_ptr -= size) < 0) ;

      if ((tmp_ptr += size) != run_ptr)
      {
        char *trav;

        for (trav = run_ptr + size; --trav >= run_ptr;)
        {
          char c = *trav;
          char *hi, *lo;

          for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
            *hi = *lo;
          *hi = c;
        }
      }

    }
  }
  return 0;
}

int nsort_qsort (nsort_t *srt, void *base, size_t numItems, size_t recSize,
    int (*qcompare) (void *, void *),
    int (*scompare) (void *, void *))
{
  unsigned int i;
  nsort_link_t *lnk;
  int status;
  void **cpp;

  bqsort (base, numItems, recSize, qcompare);

  memset (srt, 0, sizeof (nsort_t));
  srt->compare = scompare;

  srt->lh = nsort_list_create ();
  if (0 == srt->lh) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  status = nsort_list_init (srt->lh);
  if (status == _ERROR_) {
    srt->sortError = srt->lh->listError;
    return _ERROR_;
  }
  srt->head = malloc (sizeof (nsort_node_t));
  if (0 == srt->head) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->head, 0, sizeof (nsort_node_t));
  srt->tail = malloc (sizeof (nsort_node_t));
  if (0 == srt->tail) {
    free (srt->head);
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->tail, 0, sizeof (nsort_node_t));
  srt->current = srt->head;
  srt->isUnique = FALSE;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->head->prev = 0;
  srt->tail->next = 0;

  cpp = (void **) base;
  for (i = 0; i < numItems; i++) {
    if (*(cpp + i) == 0)
      break;
    lnk = malloc (sizeof (nsort_link_t));
    if (lnk == 0) {
      srt->sortError = SORT_NOMEMORY;
      free (srt->head);
      free (srt->tail);
      return _ERROR_;
    }
    lnk->data = (*(cpp + i));
    /* check_list(srt->lh, 0); */
    srt->lh->current = srt->lh->tail->prev;
    nsort_list_insert_link (srt->lh, lnk);
  }
  status = nsort_restructure_nodes (srt);
  if (_ERROR_ == status) {
    nsort_del (srt, returnClean);
    /* error is set */
    return _ERROR_;
  }
  return _OK_;
}

int nsort_list_qsort (nsort_t *srt, nsort_list_t *lh, size_t recSize,
    int (*compare) (void *, void *))
{
  char *sortArray = 0;
  nsort_link_t *lnk;
  int status;
  register int counter;

  sortArray = malloc (recSize * lh->number + 1);
  if (0 == sortArray) {
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }

  /*
   * populate sortArray
   */
  lnk = lh->head->next;
  counter = 0;
  while (lnk != lh->tail) {
    if ((size_t)counter >= lh->number) {
      srt->sortError = SORT_CORRUPT;
      free (sortArray);
      return _ERROR_;
    }
    memmove (sortArray + (counter * recSize), lnk->data, recSize);
    counter++;
    lnk = lnk->next;
  }
  bqsort ((void *) sortArray, lh->number, recSize, compare);
  /*
   * Now, put the sorted data back into the list and build the shell.
   */
  lnk = lh->head->next;
  counter = 0;
  while (lnk != lh->tail) {
    if ((size_t)counter >= lh->number) {
      srt->sortError = SORT_CORRUPT;
      free (sortArray);
      return _ERROR_;
    }
    memmove (lnk->data, sortArray + counter * recSize, recSize);
    counter++;
    lnk = lnk->next;
  }

  memset (srt, 0, sizeof (nsort_t));
  srt->compare = (int (*)(void *, void *)) compare;

  srt->lh = lh;
  srt->head = malloc (sizeof (nsort_node_t));
  if (0 == srt->head) {
    srt->sortError = SORT_NOMEMORY;
    free (sortArray);
    return _ERROR_;
  }
  memset (srt->head, 0, sizeof (nsort_node_t));
  srt->tail = malloc (sizeof (nsort_node_t));
  if (0 == srt->tail) {
    free (srt->head);
    free (sortArray);
    srt->sortError = SORT_NOMEMORY;
    return _ERROR_;
  }
  memset (srt->tail, 0, sizeof (nsort_node_t));
  srt->current = srt->head;
  srt->isUnique = FALSE;
  srt->manageAllocs = TRUE;
  srt->head->next = srt->tail;
  srt->tail->prev = srt->head;
  srt->head->prev = 0;
  srt->tail->next = 0;

  status = nsort_restructure_nodes (srt);
  if (_ERROR_ == status) {
    free (sortArray);
    nsort_del (srt, 0);
    return _ERROR_;
  }
  free (sortArray);
  return _OK_;
}

