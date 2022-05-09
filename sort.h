/* Header File: sort.h */

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

#ifndef __SORT_H__
#define __SORT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 * Includes
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 */
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if !defined(__CINT__)
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <errno.h>

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/*
 * @DocInclude ../include/disclaim.tex
 *
 * [BeginDoc]
 *
 * \section{The Sort Module}
 *
 * This is a generic implementation of a sort algorithm developed by
 * the author over a series of years (decades), from the middle 1980's to the present
 * date.  This sort algorithm has elements of a skip list, a jump list
 * \index{skip list} \index{jump list} \index{b+tree}
 * and a b+tree.  Using this code, you can quickly sort objects flexibly
 * according to the needs of the application.  The data is stored in a
 * doubly linked list and the ``order'' is determined by a user defined
 * \index{linked list}
 * compare function, a pointer of which is passed to the initialization
 * function.  For lack of a better name, this sort algorithm is called ``new
 * sort'', or nsort for short.  The alogrithm is unique as far as I know and
 * was developed solely by the author.
 *
 * In the recent improvements of this code, the goals were as follows (in
 * order of importance):
 *
 * \begin{enumerate}
 * \item Stability - much effort has been made to make the code as stable as possible.
 * Considerable testing has been done to flush out errors and the interface was written
 * so as to try to catch errors without compromising speed.
 * \item Speed - the sort algorithm is fast (it compares favorably with the qsort
 * \index{qsort}
 * algorithm) and much effort has been made to keep the code lean and clean.
 * \item Simplicity - the code here is written to be as easily read and maintained as
 * possible and great effort is made to keep the interface simple and logical.
 * \end{enumerate}
 *
 * Included in this code is a very fast non-recursive qsort as well as my sort
 * algorithm.  The qsort has been released into the public domain by TCX DataKonsult
 * but is under the GPL as included here.  You can find the original with older
 * distributions of MySQL source code.
 *
 * There is a file in the top directory of this distribution called README that contains
 * information that you should be familiar with about nsort.  Please read that for quick
 * instructions on building nsort and using it.  This file also tells you how to contact
 * me should you need to.
 *
 * The reader could reasonably ask the question, ``Why another sort algorithm?''  After
 * all, qsort is fast and works well.  However, it has often been the case in programs that
 * I have written that I want to sort data and then do something else with it, like
 * search through it efficiently or store it on disk.  The nsort code was written to
 * accommodate the ``something else''.  For example, nsort sorts data, of course, but then
 * it allows you to search for specific items in the data.  When you have found an
 * item, you can traverse in order forwards or backwards through the data from that point.
 * Once you have sorted the data, you can use nsort functionality to save it to disk.
 * The qsort function does not provide these capabilities inherently.  Also, qsort is
 * good for situations where you can sort the data all at once.  If, for example, you
 * have an application where you sort 50,000 records, quick sort can do that well.  However, 
 * if later you add 20,000 more records,
 * you have to sort all the records again with the quick sort.  With nsort, you can simply
 * add the 20,000 records to the original sorted set without having to duplicate the effort.
 *
 * \subsection{Outline}
 *
 * This documentation is broken down into the following sections:
 *
 * \begin{enumerate}
 *
 * \item A tutorial section, which takes actual working code and uses it to demonstrate
 * using the code in the library.
 *
 * \item A reference section, which describes the data types and the functions that are
 * part of the nsort library.
 *
 * \item An index that is, hopefully, extensive enough to be useful.
 *
 * \end{enumerate}
 *
 * \section{Tutorial Section}
 *
 * In this section, we will look at certain source files in the tests directory that are used
 * to test the nsort functionality.  We will describe basically what is happening as 
 * items are created and destroyed with the use of nsort functions.  We will not cover any
 * functionality in depth, but will briefly discuss what is happening in the context that
 * it occurs.  For in depth coverage of a function or a data type, look at the reference
 * information.  It's also important to realize that the code snippets shown here are
 * \emph{not complete}.  They are taken out of context to show how they would be used, but
 * the declaration of the C objects is often not show.  In other words, if you copy the
 * code shown here and try to compile it, it may or may not compile.  You need to pull
 * the code from the complete C files in order to use it in your own programs.
 *
 * Not all the sample programs are discussed.  The goal here is to give a user who is
 * fluent in C programming enough of an understanding of the nsort code to begin building her
 * own programs.  You may find it usefull to look at the ones that are not discussed.  You may
 * also find it useful to look through the shell scripts in the tests directory.  For example,
 * I have included full support for the Tiny C Compiler by Fabrice Bellard in the programs and
 * there is a set of shell scripts that call tcc to compile and run the programs.
 *
 * [EndDoc]
 *
 *
 * @DocInclude ../tests/floglist.c
 * @DocInclude ../tests/floglist_l.c
 * @DocInclude ../tests/flogsrt.c
 * @DocInclude ../tests/flogsrt_l.c
 * @DocInclude ../tests/flognsrt.c
 * @DocInclude ../tests/flogsrtq.c
 * @DocInclude ../tests/flogsrtq2.c
 * @DocInclude ../tests/flogsrtsys.c
 * @DocInclude ../tests/flogthrd.c
 * @DocInclude ../tests/floghash.c
 * @DocInclude ../tests/fqsort.c
 *
 * [BeginDoc]
 *
 * \section{Reference Section}
 *
 * In this section, we will describe the types and functions that make up the nsort library.
 * Our goal here is to describe each API function well enough that the use of the function
 * and associated types is clear.
 *
 * [EndDoc]
 *
 */

/*
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 * Type definitions
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 */
#if defined(__linux__) || defined(__solaris__)
#define NSORT_FILE_MODE (O_RDWR)
#define NSORT_FILE_PERMS (0644)
#endif

#if defined(__CYGWIN__) || defined(WIN32)
#define NSORT_FILE_MODE (O_RDWR|O_BINARY)
#define NSORT_FILE_PERMS (S_IREAD|S_IWRITE)
#endif

#define NSORT_FILE_BEG SEEK_SET
#define NSORT_FILE_END SEEK_END
#define NSORT_FILE_CUR SEEK_CUR

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef YES
#define YES 1
#define NO 2
#endif

#ifndef _OK_
#define _OK_ 0
#endif

#ifndef _ERROR_
#define _ERROR_ -1
#endif

#define NSORT_NODE_LEVEL 10
#define DEFAULT_MAGIC 0xea37beefUL
#define NSORT_MIDPOINT 6
#define NSORT_OUTPOINT 10
#define NSORT_RESTRUCT 5

/*
 * [BeginDoc]
 *
 * \subsection{Error handling}
 *
 * Occasionally errors occur. When they occur, they need to handled by the
 * programmer.  The nsort routines use a consistent error handling scheme
 * designed to be as simple as possible as well as thread safe.  There is an
 * enumerated type (nsort_error_t) and two global variables, sortError and
 * sortErrorString.  The global variables are such that, if there is a global
 * error, sortError will contain a value and sortErrorString[sortError] will
 * point to a string that describes the error.
 * \index{nsort_error_t} \index{sortError} \index{sortErrorString}
 *
 * Also, within each primary data structure, like nsort_list_t
 * for lists and nsort_t for sort objects, there is an error variable.  The global
 * variables should be used for errors in which there is no ``local'' way to handle
 * them.  For example, if we are working with a list object, any errors should be
 * handled by setting the variable in the nsort_list_t object.
 *
 * To maintain thread safety, there is a specific interface provided for setting
 * or retrieving the global sortError variable that will be described later.
 * The global variable should not be set or read directly by the application as this
 * has the potential of making the application and/or the nsort shared object
 * non-reentrant.
 *
 * [Verbatim] */

typedef enum _nsort_error_t {
  SORT_NOERROR,              /* no error */
  SORT_NOMEMORY,             /* memory allocation error */
  SORT_ERRNO,                /* look at C standard errno */
  SORT_FDENIED,              /* file permission denied - EACCES */
  SORT_FTOOMANY,             /* too many open files - EMFILE */
  SORT_FNOFILE,              /* file does not exist - ENOENT */
  SORT_FEBADF,               /* bad file descriptor - EBADF */
  SORT_FPROHIB,              /* operation prohibited - EAGAIN */
  SORT_FDEADLK,              /* operation would cause a deadlk - EDEADLK */
  SORT_FNOLOCK,              /* locking failed - ENOLCK */
  SORT_FRDWR,                /* file write or read was incomplete */
  SORT_PARAM,                /* bad function parameter */
  SORT_LIST_INIT,            /* list initialization failed */
  SORT_LIST_INVALID,         /* invalid list used */
  SORT_LIST_NOTEMPTY,        /* cannot delete a populated list */
  SORT_LIST_BADFILE,         /* bad list file */
  SORT_UNIQUE,               /* unique constraint violated */
  SORT_CORRUPT,              /* corrupt data detected */
  /* Add new ones here */
  SORT_UNSPECIFIED           /* unspecified error */
} nsort_error_t;

extern nsort_error_t sortError;
extern char *sortErrorString[];

/* [EndDoc] */

/*
 * [BeginDoc]
 *
 * \subsection{Data Types}
 *
 * There are several data types that are used to manage data in the nsort library.  Quickly
 * enumerated, they are as follows:
 *
 * \begin{enumerate}
 *
 * \item nsort_link_t
 * \item nsort_list_t
 * \item nsort_node_t
 * \item nsort_store_t
 * \item nsort_node_level_t
 * \item nsort_t
 * \item nsort_hash_t
 *
 * \end{enumerate}
 *
 * \subsubsection{nsort_link_t}
 * \index{nsort_link_t}
 *
 * The first data type is the nsort_link_t type, which is defined as follows:
 * [Verbatim] */

typedef struct _nsort_link_t {
  struct _nsort_link_t *next;
  struct _nsort_link_t *prev;
  int number;
  void *data;
} nsort_link_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_link_t type is used to create objects that provide the generic links in a doubly-linked list.
 * Typically, when you build a linked list, you create the data structure that you want
 * to be part of the list with next and prev pointers and then use those to link the
 * data objects together.  Here, we create generic objects called links that themselves
 * point to user data.
 *
 * The following are descriptions of each of the elements of the nsort_link_t object:
 *
 * \begin{itemize}
 *
 * \item [next, prev]  The next and prev items are in the nsort_link_t object to
 * allow each item of nsort_link_t type to be linked into a doubly-linked list.
 * 
 * \item [number] The number item is application specific and is present so that
 * applications which might have use for it can use it for tracking links in lists
 * or nsort objects.
 * 
 * \item [data] the data item will point to the user's data and must be set before
 * the nsort_link_t object is passed to any of the nsort API functions.
 *
 * \end{itemize}
 *
 * The nsort routines will not touch
 * the ``number'' member or the ``data'' member of nsort_link_t objects.  These need
 * to be set for each link by the application.
 *
 * \subsubsection{nsort_list_t}
 * \index{nsort_list_t}
 *
 * The next data type is the nsort_list_t type and it is defined as follows:
 * [Verbatim] */

typedef struct _nsort_list_t {
  nsort_error_t listError;
  struct _nsort_link_t *head;
  struct _nsort_link_t *tail;
  struct _nsort_link_t *current;
  size_t number;
  struct _nsort_list_t *__n;
  struct _nsort_list_t *__p;
} nsort_list_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_t object is a list header object.  It contains the data needed to
 * manage lists, including markers and currency pointers.
 *
 * The following are descriptions of each of the elements of the nsort_list_t object:
 *
 * \begin{itemize}
 *
 * \item [head, tail] The head and tail items serve as markers for the list.  The first
 * element of the list is head->next and the last element of the list is tail->prev.
 *
 * \item [current] The current item serves as a marker for location.  Certain functions,
 * like nsort_list_remove_link() use the current link to determine what to act on.  The
 * functions that use or set the current item will be specified in the documentation.
 * Your application can freely set this according to need.
 *
 * \item [number] The number item is the number of links that are contained by the list.
 * This should only be read by the application; if it is altered, it will result in
 * corruption errors.
 *
 * \item [__n, __p] The __n and __p items are used to link the list into a global list
 * so they can be managed as part of the system.  These are only set when the list is
 * created or deleted and should not be changed by your application.
 *
 * \end{itemize}
 * 
 * [Verbatim] */

typedef struct _nsort_node_t {
  struct _nsort_node_t *prev;
  struct _nsort_node_t *next;
  nsort_link_t *here;
  struct _nsort_node_t *level[NSORT_NODE_LEVEL];
} nsort_node_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_node_t are data objects that represent nodes.  A node in the nsort world
 * is a device that is used to create the indexing for nsorts.  Nodes are linked
 * together into a list of nodes and each of them point to a link.  Basically, these
 * index into the list and serve as the bottom level index.  Additionally, the level
 * member of the nsort_node_t object is used to create higher level indexes into the
 * list.
 *
 * The following are descriptions of each of the elements of the nsort_node_t object:
 *
 * \begin{itemize}
 *
 * \item [prev,next] The prev and next items are used to link nodes together into a list
 * of nodes.  The prev pointer is not used in this implementation of the algorithm, but
 * there are plans to possibly do so at some point.
 *
 * \item [here] The here item points to a link in the doubly-linked list.
 *
 * \item [level] The level item is an array of node pointers used to provide levels
 * of indexing.  See the source code to understand how this is accomplished.
 *
 * \end{itemize}
 *
 * [Verbatim] */

typedef struct _nsort_store_t {
  unsigned long thisMagic;
  char timeStamp[27];
  char description[128];
  int isUnique;
  int manageAllocs;
  int isCase;
  int splitVal;
  int number;
  int size;
} nsort_store_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_store_t object is used when storing the contents of a list or sort item.
 * The contents of this item gets stamped into the file as a header and is read back
 * to allow the user to save the list contents.
 *
 * The following are descriptions of each of the elements of the nsort_store_t object:
 *
 * \begin{itemize}
 *
 * \item [thisMagic] This is a special number that gets stamped into the file
 * and can be used to determine what kind of file it is.  Applications can have
 * their own magic number and use that to determine if the file being opened is
 * correct for the application.
 *
 * \item [timeStamp] This is a time stamp of the form 'Mon Oct 31 14:32:00 2005'.  This
 * will reflect the time the file was saved (within a few milliseconds).
 *
 * \item [description] This is a string that can be populated by the application.  It
 * is completely application dependent and can be left empty.
 *
 * \item [isUnique] This only has meaning for sorted objects.  If it is TRUE (1), then
 * unique constraints are applied to the 
 *
 * \item [manageAllocs] This item only has meaning for sorted objects.  Basically, if this
 * is true, then the nsort routines take responsibility for memory resources used by the
 * components of the nsort_t data object.  This further implies that all nsort_link_t items,
 * all link->data, all nsort_list_t and all nsort_t items are allocated on the heap using
 * malloc or calloc.  Bad things will happen if this is not the case.
 *
 * \item [isCase] This will determine whether a case-insensitive sort is required.  This is
 * not set or read by the nsort routines and is only there for the application to use if
 * required.
 *
 * \item [splitVal] This variable is provided to allow the application to specify thresholds
 * at which splits should occur in blocks.  This is not used by the nsort routines and is provided
 * for the application.
 *
 * \item [number] This is the number of items stored from the list in the file.
 *
 * \item [size] This item reflects the size of each item from the list stored in the file.
 * If you store a list in a file, the items must be of equal length, although this is not a
 * requirement of lists that are in memory.
 *
 * \end{itemize}
 *
 * There are elements of the nsort_store_t object that are not used by the nsort routines but
 * are provided for higher-level applications to use.
 *
 *
 * [Verbatim] */

typedef struct _nsort_node_level_t {
  int lvl[NSORT_NODE_LEVEL];
} nsort_node_level_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_node_level_t}
 * \index{nsort_node_level_t}
 *
 * The nsort_node_level_t data type is used by the nsort_node_levels() function to
 * \index{nsort_node_levels}
 * communicate information about an nsort_t object to the programmer.  For example, you
 * may want to get a snap shot of how many levels are currently populated in an nsort and
 * how many nodes there are on each level.  You would need to use an object of type
 * nsort_node_level_t to get that information.  Look at the flogsrt.c and flognsrt.c programs
 * in the tests directory to get a feel for how to use this information.
 *
 * The following are descriptions of each of the elements of the nsort_node_level_t object:
 *
 * \begin{itemize}
 *
 * \item [lvl] This is the only member of the nsort_node_level_t data type.  It is basically an
 * array of integers.  It is populated by a call to nsort_node_levels().
 * 
 * \end{itemize}
 *
 * [Verbatim] */

typedef struct _nsort_t {
  nsort_error_t sortError;
  nsort_node_t *head;
  nsort_node_t *tail;
  nsort_node_t *current;
  size_t numNodes;
  int nodeLevel;
  int isUnique;
  int manageAllocs;
  nsort_list_t *lh;
  size_t numCompares;
  int thresh;
  size_t numRestruct;
  int (*compare)(void*,void*);
#ifdef NSORT_STATS
  double traversal_time;
#endif
} nsort_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * \subsubsection{nsort_t}
 * \index{nsort_t}
 *
 * The following are descriptions of each of the elements of the nsort_t object:
 *
 * \begin{itemize}
 *
 * \item [sortError] The sortError item is used to communicate diagnostics in the event
 * of an error.
 *
 * \item [head,tail] The head and tail items are the head and tail of the node list and
 * server the same purpose as head and tail on a linked list.  The head->next item is
 * the first item in the node list and the tail->prev item is the last item in the list.
 *
 * \item [current] The current item is a pointer to the ``current'' member of the list.
 * Certain functions act on the node list based on the value of the current member.
 *
 * \item [numNodes] This is the number of nodes that are in the node list.
 *
 * \item [nodeLevel] This item is not used by the nsort routines and is provided for
 * higher level applications to use.
 *
 * \item [isUnique] This item will determine if unique constraints are attached to
 * elements of the list.  If this is true, trying to add an element that is ``equal'' to
 * something that is already in the list will result in an error.
 *
 * \item [manageAllocs] This item will determine whether the nsort routines are responsible
 * to manage allocated items or not.
 *
 * \item [lh] The lh item is the list header, or the pointer to a nsort_list_t object.
 * This is where the actual sorted data is managed.  Any valid operation permitted
 * on a list is permitted on this item.  The first member of the list is lh->head->next
 * and the last member is lh->tail->prev.
 *
 * \item [numCompares] This item is a statistical gathering tool used to try to keep the
 * index information balanced.  Basically, as an nsort object is being populated, the
 * index is built in place as items are added.  The end result is that is is possible, and
 * depending on the randomness of the data, probable, that the distance in terms of links
 * between any two nodes may become large.  The numCompares item will indicate this and it
 * is used as a tool to indicate a possible need for an index restructure.
 *
 * \item [thresh] This item is used in conjunction with the numCompares item.  If the
 * numCompares item is greater than a certain threshold, thresh is incremented.  If it
 * is incremented past another threshold, then a restructure is initiated.  These tests
 * are done upon entering an add or a search (either a find or a query), after which the
 * numCompares item is reset to zero.
 *
 * \item [numRestruct] This item shows how many times the sort item has been restructured.
 * The restructure may have occured in the course of populating the object, searching for
 * an item, or as a result of an explicit call to nsort_restructure_nodes ().
 *
 * \item [compare] This item will contain a pointer to a user defined compare function.
 * The compare function is very important and will determine how the data elements in
 * the list are ``sorted''.
 *
 * \item [traversal_time] This item is only defined if NSORT_STATS is defined.  You
 * should be advised that #defining NSORT_STATS slows down the performance of the sort
 * routines greatly, almost to the point where it becomes unuseable.
 *
 * \end{itemize}
 *
 *
 * The nsort_t item is the primary data item of the nsort sort.  Much of the complexity
 * is hidden in the list header lh and processing the
 * levels array.  Overall, the nsort object is
 * fairly simple to use, although how the indexing is done during adds is not at all
 * trivial and I don't make an attempt to explain it in detail.  You can figure that
 * out by reading the source.
 *
 * \subsubsection{nsort_hash_t}
 * \index{nsort_hash_t}
 *
 * The nsort_hash_t data type is used to create a high-performance hash.  Basically,
 * you can add several hundred thousand items to an nsort_hash_t object (of length
 * 5-40 bytes) in about half a second and search for each of those items in the hash
 * in under a second.  This is probably overkill in an application where you only have
 * a few thousand items to hash; however, it is robust enough to do the job.
 *
 * [Verbatim] */

#define NSORT_HASH_SIZE 251

typedef struct _nsort_hash_t {
  nsort_error_t hashError;
  int number;
  int numCompares;
  unsigned int (*hash) (const char *);
  nsort_t *srts[NSORT_HASH_SIZE];
} nsort_hash_t;

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The following are descriptions of each of the elements of the nsort_hash_t object:
 *
 * \begin{itemize}
 *
 * \item [hashError] This item is an error indicator.  It is of type nsort_error_t
 * and therefore is one of the enumerated items above.  If an error occurs in one
 * of the nsort items, the error is transferred to here and cleared from the sort
 * object.
 *
 * \item [number] This item is the number of items that are currently stored in
 * the hash object.
 *
 * \item [numCompares] This item reflects the number of compares that were needed
 * for the most recent search or insert.
 *
 * \item [hash] This is a pointer to a hash function.  There is one provided that
 * is good with strings; however, if you want to use another, you can.
 *
 * \item [srts] This is the array of nsort objects that are the actual buckets for
 * the hash.  NSORT_HASH_SIZE is the number of these that will be allocated and it
 * should be a prime number.
 *
 * \end{itemize}
 *
 * [EndDoc]
 */

/*
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 * Macros
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 */
#ifdef nsort_file_create
#undef nsort_file_create
#endif

/*
 * [BeginDoc]
 *
 * \subsection{File Routines}
 *
 * There are certain functions that are simply wrappers over system calls. 
 * You have to change certain parameters to accommodate system differences
 * and you have to deal with errors if they happen.  Often, the functionality
 * required is trivial enough that you can create a preprocessor macro
 * to accomplish the task and then call it in a similar way to a function.
 * \footnote{It should be pointed out that you can't do this all the time.
 * For example, the nsort routines require a pointer to a compare function
 * in the nsort_init() call.  This cannot be accomplished with a preprocessor
 * macro, even though the compare function is fairly trivial.}
 * This is precisely the situation with the file related functions. They are
 * really just wrappers around very portable C library calls.  The wrappers are
 * needed because you have slight differences with different systems and you have
 * to handle error conditions.  There is no real need to have separate functions
 * to accomplish these tasks.  Also, macros don't have the overhead of functions.
 * A big downer to macros is that they can be tough to debug.  Also, things can
 * be changed in macros where with function calls, things aren't changed unless
 * pointers are passed.  Unless otherwise specified in the documentation, the
 * programmer can assume that a parameter \emph{will not} be changed by a macro.
 *
 * \subsubsection{nsort_file_create}
 * \index{nsort_file_create}
 *
 * The first file creation macro is nsort_file_create.  This is defined as
 * follows:
 * [Verbatim] */
#ifndef DEBUG
#define nsort_file_create(fname,fd) \
{ \
  fd = creat(fname,NSORT_FILE_PERMS); \
  if (fd < 0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EMFILE == errno) \
      set_sortError(SORT_FTOOMANY); \
    else if (ENOENT == errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}
#else
#define nsort_file_create(fname,fd) \
{ \
  fd = creat(fname,NSORT_FILE_PERMS); \
  if (fd < 0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EMFILE == errno) \
      set_sortError(SORT_FTOOMANY); \
    else if (ENOENT == errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  printf ("DBG: %s created and opened from %s, line %d\n", \
      fname, __FILE__, __LINE__); \
}
#endif

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The only difference between the debug version and the non-debug version
 * is that the debug version prints a line every time it opens 
 * a file and every time it closes a file (a file is left open after the call to
 * creat(2)).  There are certain environments
 * where a file that is opened and not closed can cause problems.
 *
 * The following are descriptions of the parameters to the nsort_file_create
 * macro:
 *
 * \begin{itemize}
 *
 * \item [fname] This is the name of the file to open.  It can be a complete
 * path with a name, a relative path with a name or just the name of a file.
 * This is not changed, so it can be a constant char string.
 *
 * \item [fd] This is the file descriptor for the file.  It should be an
 * integer value, and after the nsort_file_create call it will be set
 * to the open file descriptor, or something less than 0 if there is an
 * error.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_open}
 * \index{nsort_file_open}
 *
 * The nsort_file_open() macro opens a file.  It is defined as follows:
 * [Verbatim] */

#ifndef DEBUG
#define nsort_file_open(fname,fd) \
{ \
  fd = open(fname,NSORT_FILE_MODE,NSORT_FILE_PERMS); \
  if (fd < 0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EMFILE == errno) \
      set_sortError(SORT_FTOOMANY); \
    else if (ENOENT == errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}
#else
#define nsort_file_open(fname,fd) \
{ \
  fd = open(fname,NSORT_FILE_MODE,NSORT_FILE_PERMS); \
  if (fd < 0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EMFILE == errno) \
      set_sortError(SORT_FTOOMANY); \
    else if (ENOENT == errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  printf ("DBG: %s opened from %s, line %d\n", \
      fname, __FILE__, __LINE__); \
}
#endif

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * There is a debug version and a non-debug version.  The parameters to the
 * nsort_file_open macro are as follows:
 *
 * \begin{itemize}
 *
 * \item [fname] This is the name of the file to open.  It can be a complete
 * path with a name, a relative path with a name or just the name of a file.
 * This is not changed, so it can be a constant char string.
 *
 * \item [fd] This is the file descriptor for the file.  It should be an
 * integer value, and after the nsort_file_create call it will be set
 * to the open file descriptor, or something less than 0 if there is an
 * error.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_close}
 * \index{nsort_file_close}
 *
 * The nsort_file_close() macro is defined as follows:
 * [Verbatim] */

#ifndef DEBUG
#define nsort_file_close(fd) \
{ \
  int nsort_file_close_st; \
  nsort_file_close_st=close(fd); \
  if (nsort_file_close_st < 0) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}
#else
#define nsort_file_close(fd) \
{ \
  int nsort_file_close_st; \
  nsort_file_close_st=close(fd); \
  if (nsort_file_close_st < 0) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  printf ("DBG: %s closed from %s, line %d\n", \
      fname, __FILE__, __LINE__); \
}
#endif

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The parameter fd should be a file descriptor for an open file.
 * After this macro is executed, the file will be closed and cached
 * contents will be flushed to file.
 *
 * \subsubsection{nsort_file_seek_begin}
 * \index{nsort_file_seek_begin}
 *
 * The nsort_file_seek_begin function is defined as follows:
 * [Verbatim] */

#define nsort_file_seek_begin(fd,offset,status) \
{ \
  off_t nsort_st; \
  nsort_st=lseek(fd,offset,NSORT_FILE_BEG); \
  if ((long signed int)nsort_st<0) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  status=nsort_st; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_seek_begin() macro will seek offset bytes from the
 * beginning of the file given by fd and will return status information
 * in the status variable.  The following are descriptions of the
 * parameters to this macro:
 *
 * \begin{itemize}
 *
 * \item [fd] The fd parameter is the file descriptor for an open file.
 *
 * \item [offset] The offset parameter is the offset from the beginning of
 * the file that the programmer wants to seek.
 *
 * \item [status] The status parameter recieves the status of the lseek
 * call, which is essentially the offset in bytes from the beginning of
 * the file that the file pointer is set to.  It should be considered an
 * error if status is not equal to offset at the end of this macro.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_seek_end}
 * \index{nsort_file_seek_end}
 *
 * The nsort_file_seek_end() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_seek_end(fd,offset,status) \
{ \
  off_t nsort_file_seek_end_st; \
  nsort_st=lseek(fd,offset,NSORT_FILE_END); \
  if ((long signed int)nsort_st<-1L) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  status=nsort_st; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_seek_end() macro will seek offset bytes from the
 * end of the file given by fd and will return status information
 * in the status variable.  The following are descriptions of the
 * parameters to this macro:
 *
 * \begin{itemize}
 *
 * \item [fd] The fd parameter is the file descriptor for an open file.
 *
 * \item [offset] The offset parameter is the offset from the end of
 * the file that the programmer wants to seek.
 *
 * \item [status] The status parameter recieves the status of the lseek
 * call, which is essentially the offset in bytes from the end of
 * the file that the file pointer is set to.  It should be considered an
 * error if status is not equal to offset at the end of this macro.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_seek_current}
 * \index{nsort_file_seek_current}
 *
 * The nsort_file_seek_current() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_seek_current(fd,offset,status) \
{ \
  off_t nsort_st; \
  nsort_st=lseek(fd,offset,NSORT_FILE_CUR); \
  if ((long signed int)nsort_st<-1L) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  status=nsort_st; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_seek_current() macro will seek offset bytes from the
 * current point of the file given by fd and will return status information
 * in the status variable.  The following are descriptions of the
 * parameters to this macro:
 *
 * \begin{itemize}
 *
 * \item [fd] The fd parameter is the file descriptor for an open file.  This
 * parameter remains unchanged.
 *
 * \item [offset] The offset parameter is the offset from the current location of
 * the file that the programmer wants to seek.  This parameter remains unchanged.
 *
 * \item [status] The status parameter recieves the status of the lseek
 * call, which is essentially the offset in bytes from the current position of
 * the file that the file pointer is set to.  It should be considered an
 * error if status is not equal to offset at the end of this macro.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_write}
 * \index{nsort_file_write}
 *
 * The nsort_file_write() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_write(fd,buf,len,status) \
{ \
  off_t nsort_st; \
  nsort_st=(off_t)write(fd,buf,len); \
  if((long signed int)nsort_st<0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  if (nsort_st != -1 && nsort_st<(off_t)len) \
    set_sortError(SORT_FRDWR); \
  status=nsort_st; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_write macro writes len bytes from buf to the open file
 * given by fd.  The number of bytes actually written is returned in the
 * status variable, which is the only variable that is changed by this
 * macro.  The following are descriptions of parameters to this macro:
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is a file descriptor for an open file.  It will not
 * be changed by the macro.
 *
 * \item [buf] This is the buffer that contains the data that needs to be written.
 * This parameter is unchanged by the macro.
 *
 * \item [len] This is the length in bytes of the buffer of data pointed to by buf.
 * This is of type size_t in the write function call.  This parameter is unchanged
 * by the macro.
 * 
 * \item [status] This parameter is the result of the write library call.  It should
 * be the same as len or there is an error (i.e., fewer bytes got written than the
 * programmer requested).
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_read}
 * \index{nsort_file_read}
 *
 * The nsort_file_read() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_read(fd,buf,len,status) \
{ \
  off_t nsort_st; \
  nsort_st=(off_t)read(fd,buf,len); \
  if((long signed int)nsort_st<0) { \
    if (EACCES == errno) \
      set_sortError(SORT_FDENIED); \
    else if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
  if (nsort_st != -1 && nsort_st<(off_t)len) \
    set_sortError(SORT_FRDWR); \
  status=nsort_st; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_read() macro will read len bytes from the file given by fd into
 * the buffer buf.  Dire things will happen if buf is not big enough for len bytes.
 * The return value of the read call will be provded in the status parameter.
 *
 * \subsubsection{nsort_file_flush}
 * \index{nsort_file_flush}
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is the file descriptor for the open file that the
 * program wants to read from.  This parameter is unchanged by the macro.
 *
 * \item [buf] This parameter is the buffer that the data is read into from the
 * file if the call is successful.  This parameter is unchanged by the macro.
 *
 * \item [len] This parameter is the length of data to be read from the file.
 * This parameter is unchanged by the macro.
 * 
 * \item [status] This parameter will contain the return value of the read function
 * after the macro call.
 *
 * \end{itemize}
 *
 * The nsort_file_flush() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_flush(fd) \
{ \
  int nsort_st; \
  nsort_st=close(dup(fd)); \
  if (nsort_st<0) { \
    if (EBADF == errno) \
      set_sortError(SORT_FEBADF); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_flush macro forces a flush of the file to disk.  Needless
 * to say, it can slow things down, depending on how often it is
 * called.  The file is left open and accessible after the call to the macro
 * and has the same file descriptor.
 *
 * \subsubsection{nsort_file_lock}
 * \index{nsort_file_lock}
 *
 * The nsort_file_lock() macro is defined as follows:
 * [Verbatim] */

#define nsort_file_lock(fd,type,offset,whence,len,nflr_stat) \
{ \
  struct flock lock; \
  int nsort_st; \
  lock.l_type=type; \
  lock.l_start=offset; \
  lock.l_whence=whence; \
  lock.l_len=len; \
  nsort_st=fcntl(fd,F_SETLK,&lock); \
  if(nsort_st<0) { \
    if(EACCES==errno) \
      set_sortError(SORT_FDENIED); \
    else if(EMFILE==errno) \
      set_sortError(SORT_FTOOMANY); \
    else if(ENOENT==errno) \
      set_sortError(SORT_FNOFILE); \
    else if(EBADF==errno) \
      set_sortError(SORT_FEBADF); \
    else if(EAGAIN==errno) \
      set_sortError(SORT_FPROHIB); \
    else if(EDEADLK==errno) \
      set_sortError(SORT_FDEADLK); \
    else if(ENOLCK == errno) \
      set_sortError(SORT_FNOLOCK); \
    else \
      set_sortError(SORT_ERRNO); \
    nflr_stat=nsort_st; \
  } \
  else { \
    nflr_stat=_OK_; \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_lock macro attempts to lock a region of a file len bytes
 * in length, offset bytes from whence in the file (whence is beginning, current
 * or end).
 *
 * The following are the parameters and what values they can have:
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is the file descriptor for the open file.  It
 * will not be changed by the macro call.
 *
 * \item [type] This parameter is the type of lock that you are requesting.
 * It can be one of F_RDLCK, F_WRLCK or F_UNLCK.  See the man page for
 * fcntl(2) to find out what these types mean.  This parameter is not changed
 * by the call to the macro.
 *
 * \item [offset] This parameter is the offset in bytes from whence that
 * you want the lock installed.  This parameter is unchanged by the call
 * to the macro.
 *
 * \item [whence] This parameter is one of NSORT_FILE_BEG, NSORT_FILE_CUR or
 * NSORT_FILE_END, which means the beginning of the file, the current
 * location of the file pointer, or the end of the file, respectively.
 * This parameter is unchanged by the call to the macro.
 *
 * \item [len] This parameter is the length of the region in the file to lock.
 * This parameter is unchanged by the macro.
 *
 * \item [nflr_stat] This parameter will be set to the error condition if there
 * is an error, or to _OK_ if the call was successful.
 *
 * \end{itemize}
 *
 * [Verbatim] */

#define nsort_file_lockw(fd,type,offset,whence,len,nflr_stat) \
{ \
  struct flock lock; \
  int nsort_st; \
  lock.l_type=type; \
  lock.l_start=offset; \
  lock.l_whence=whence; \
  lock.l_len=len; \
  nsort_st=fcntl(fd,F_SETLKW,&lock); \
  if(nsort_st<0) { \
    if(EACCES==errno) \
      set_sortError(SORT_FDENIED); \
    else if(EMFILE==errno) \
      set_sortError(SORT_FTOOMANY); \
    else if(ENOENT==errno) \
      set_sortError(SORT_FNOFILE); \
    else if(EBADF==errno) \
      set_sortError(SORT_FEBADF); \
    else if(EAGAIN==errno) \
      set_sortError(SORT_FPROHIB); \
    else if(EDEADLK==errno) \
      set_sortError(SORT_FDEADLK); \
    else if(ENOLCK == errno) \
      set_sortError(SORT_FNOLOCK); \
    else \
      set_sortError(SORT_ERRNO); \
    nflr_stat=get_sortError(); \
  } \
  else { \
    nflr_stat=lock.l_pid; \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_lockw macro attempts to lock a region of a file len bytes
 * in length, offset bytes from whence in the file (whence is beginning, current
 * or end).  If it can't get a lock immediately, it waits for the process that owns
 * the lock to release it and then gets the lock.
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is the file descriptor for the open file.  It
 * will not be changed by the macro call.
 *
 * \item [type] This parameter is the type of lock that you are requesting.
 * It can be one of F_RDLCK, F_WRLCK or F_UNLCK.  See the man page for
 * fcntl(2) to find out what these types mean.  This parameter is not changed
 * by the call to the macro.
 *
 * \item [offset] This parameter is the offset in bytes from whence that
 * you want the lock installed.  This parameter is unchanged by the call
 * to the macro.
 *
 * \item [whence] This parameter is one of NSORT_FILE_BEG, NSORT_FILE_CUR or
 * NSORT_FILE_END, which means the beginning of the file, the current
 * location of the file pointer, or the end of the file, respectively.
 * This parameter is unchanged by the call to the macro.
 *
 * \item [len] This parameter is the length of the region in the file to lock.
 * This parameter is unchanged by the macro.
 *
 * \item [nflr_stat] This parameter will be set to the error condition if there
 * is an error, or to _OK_ if the call was successful.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_lock_test}
 * \index{nsort_file_lock_test}
 *
 * The following is how the nsort_file_lock_test() macro is defined:
 * [Verbatim] */

#define  nsort_file_lock_test(fd,type,offset,whence,len,nflt_stat) \
{ \
  struct flock lock; \
  int nsort_st; \
  lock.l_type=type; \
  lock.l_start=offset; \
  lock.l_whence=whence; \
  lock.l_len=len; \
  nsort_st=fcntl (fd,F_GETLK,&lock); \
  if (nsort_st<0) { \
    if (EACCES==errno) \
      set_sortError(SORT_FDENIED); \
    else if (EMFILE==errno) \
      set_sortError(SORT_FTOOMANY); \
    else if (ENOENT==errno) \
      set_sortError(SORT_FNOFILE); \
    else if (EBADF==errno) \
      set_sortError(SORT_FEBADF); \
    else if (EAGAIN==errno) \
      set_sortError(SORT_FPROHIB); \
    else if (EDEADLK==errno) \
      set_sortError(SORT_FDEADLK); \
    else if (ENOLCK==errno) \
      set_sortError(SORT_FNOLOCK); \
    else \
      set_sortError (SORT_ERRNO); \
    nflt_stat=_ERROR_; \
  } \
  if (lock.l_type == F_UNLCK) \
    nflt_status=FALSE; \
  nflt_stat=TRUE; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_lock_test() macro tests to see if it is possible to set a
 * lock on the range of len bytes beginning offset bytes from whence in the file
 * fd.  The following are descriptions of the parameters for the macro call:
 *
 * \begin{itemize}
 *
 * \item [fd] This parameter is the file descriptor for the open file.  It
 * will not be changed by the macro call.
 *
 * \item [type] This parameter is the type of lock that you are interested in.
 * It can be one of F_RDLCK, F_WRLCK or F_UNLCK.  See the man page for
 * fcntl(2) to find out what these types mean.  This parameter is not changed
 * by the call to the macro.
 *
 * \item [offset] This parameter is the offset in bytes from whence that
 * you want the lock tested.  This parameter is unchanged by the call
 * to the macro.
 *
 * \item [whence] This parameter is one of NSORT_FILE_BEG, NSORT_FILE_CUR or
 * NSORT_FILE_END, which means the beginning of the file, the current
 * location of the file pointer, or the end of the file, respectively.
 * This parameter is unchanged by the call to the macro.
 *
 * \item [len] This parameter is the length of the region in the file to lock.
 * This parameter is unchanged by the macro.
 *
 * \item [nflt_stat] This parameter will be set to the error condition if there
 * is an error, or to _OK_ if the call was successful.
 *
 * \end{itemize}
 *
 * \subsubsection{nsort_file_remove}
 * \index{nsort_file_remove}
 *
 * The following is how the nsort_file_remove() macro is defined:
 * [Verbatim] */

#define nsort_file_remove(fname) \
{ \
  int nsort_st=remove(fname); \
  if(nsort_st<0){ \
    if(EACCES==errno) \
      set_sortError(SORT_FDENIED); \
    else if(ENOENT==errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_remove() macro will remove the file given by fname.  The
 * fname parameter can be a string constant or a C string in a character array.
 * The name can be a full path name, a relative path name or just the name of
 * a file.
 *
 * \subsubsection{nsort_file_rename}
 * \index{nsort_file_rename}
 *
 * The following is how the nsort_file_rename() macro is defined:
 * [Verbatim] */

#define nsort_file_rename(oldname,newname) \
{ \
  int nsort_st=rename(oldname,newname); \
  if(nsort_st<0) { \
    if(EACCES==errno) \
      set_sortError(SORT_FDENIED); \
    else if(ENOENT==errno) \
      set_sortError(SORT_FNOFILE); \
    else \
      set_sortError(SORT_ERRNO); \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_file_rename() macro will remove the file given by oldname
 * to newname.  The
 * name parameters can be a string constant or a C string in a character array.
 * The names can be full path names, relative path names or just the names of
 * the files.
 *
 * \subsubsection{nsort_list_is_at_tail}
 * \index{nsort_list_is_at_tail}
 *
 * The following is how the nsort_list_is_at_tail() macro is defined:
 * [Verbatim] */

#define nsort_list_is_at_tail(lh) \
  ((lh)!=NULL && (lh)->current==(lh)->tail)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_is_at_tail() macro returns true if the current pointer of the
 * list given by lh points to the tail pointer.
 *
 * \subsubsection{nsort_list_is_at_head}
 * \index{nsort_list_is_at_head}
 *
 * The following is how the nsort_list_is_at_head() macro is defined:
 * [Verbatim] */

#define nsort_list_is_at_head(lh) \
  ((lh)!=NULL && (lh)->current==(lh)->head)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_is_at_head() macro returns true if the current pointer of the
 * list given by lh points to the head pointer.
 *
 * \subsubsection{nsort_list_is_empty}
 * \index{nsort_list_is_empty}
 *
 * The following is how the nsort_list_is_empty() macro is defined:
 * [Verbatim] */

#define nsort_list_is_empty(lh) \
  ((lh)!=NULL && (lh)->head->next==(lh)->tail && (lh)->tail->prev==(lh)->head)

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_is_empty() macro returns true if the list given by lh is empty.
 *
 * \subsubsection{nsort_list_next_link}
 * \index{nsort_list_next_link}
 *
 * The following is how the nsort_list_next_link() macro is defined:
 * [Verbatim] */

#define nsort_list_next_link(lh) \
{ \
  if ((lh)->current != (lh)->tail) { \
    (lh)->current = (lh)->current->next; \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_next_link() will set the current pointer in the list given
 * by lh to current->next.
 *
 * \subsubsection{nsort_list_prev_link}
 * \index{nsort_list_prev_link}
 *
 * The following is how the nsort_list_prev_link() macro is defined:
 * [Verbatim] */

#define nsort_list_prev_link(lh) \
{ \
  if ((lh)->current != (lh)->head) { \
    (lh)->current = (lh)->current->prev; \
  } \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_prev_link() will set the current pointer in the list given
 * by lh to current->prev.
 *
 * \subsubsection{nsort_list_first_link}
 * \index{nsort_list_first_link}
 *
 * The following is how the nsort_list_first_link() macro is defined:
 * [Verbatim] */

#define nsort_list_first_link(lh) \
{ \
  (lh)->current = (lh)->head->next; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_first_link() will set the current pointer in the list given
 * by lh to current->head->next.
 *
 * \subsubsection{nsort_list_last_link}
 * \index{nsort_list_last_link}
 *
 * The following is how the nsort_list_last_link() macro is defined:
 * [Verbatim] */

#define nsort_list_last_link(lh) \
{ \
  (lh)->current = (lh)->tail->prev; \
}

/* [EndDoc] */
/*
 * [BeginDoc]
 *
 * The nsort_list_last_link() will set the current pointer in the list given
 * by lh to current->tail->prev.
 *
 * [EndDoc]
 */

#ifdef  DEBUG
#define Assert(b)       {if(!(b)) \
  printf ("Assert failed in file %s, line %d\n", __FILE__, __LINE__);}
#else
#define Assert(b)
#endif

#ifdef  DEBUG
#define dbg(str)        str
#else
#define dbg(str)
#endif

#if     (DEBUG > 1)
#define dbg2(str)       str
#else
#define dbg2(str)
#endif

/*
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 * Prototypes
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 */
/*
 * @DocInclude ../lib/nsort.c
 */
void set_sortError (nsort_error_t er);
nsort_error_t get_sortError(void);
int nsort_elapsed (double *sec);
int nsort_check_error (void);
int nsort_check_list_error (nsort_list_t *lh);
int nsort_check_sort_error (nsort_t *srt);
int nsort_show_error (char *str, size_t len);
int nsort_show_error_num (nsort_error_t nsort_error, char *str, size_t len);
int nsort_show_list_error (nsort_list_t *lh, char *str, size_t len);
int nsort_show_sort_error (nsort_t *srt, char *str, size_t len);
int nsort_text_file_split (char **cpp, int maxnum, char *ln, int ch);
nsort_list_t *nsort_list_create (void);
int nsort_list_destroy (nsort_list_t *lh);
int nsort_list_init (nsort_list_t *nlh);
int nsort_list_del (nsort_list_t *lh);
int nsort_list_insert_link (nsort_list_t *lh, nsort_link_t *lnk);
nsort_link_t *nsort_list_remove_link (nsort_list_t *lh);
int nsort_list_clear (nsort_list_t *lh);
size_t nsort_list_write_block (int fd, off_t where, size_t reclen,
    nsort_list_t *lh);
int nsort_list_read_block (nsort_list_t *lh, int fd, off_t where, size_t reclen,
    size_t numrecs);
int nsort_list_save (nsort_list_t *lh, const char *desc, int size,
    char *file);
int nsort_list_get (nsort_list_t *lh, char *file);
int nsort_list_get_all (nsort_list_t *lh, char *file, char *desc, char *timestamp);
int nsort_list_merge_sorted (nsort_list_t *mrg, nsort_list_t *l1, nsort_list_t *l2,
    int (*compare)(void*, void*));
nsort_t *nsort_create (void);
int nsort_destroy (nsort_t *srt);
int nsort_init (nsort_t *srtp, int (*compare)(void*,void*),
    int isUnique, int manageAllocs);
int nsort_del (nsort_t *srt, void (*delFunc)(void*));
int nsort_new_node (nsort_t *srt, nsort_node_t *prevNode,
    nsort_node_t *nextNode, nsort_link_t *here);
int nsort_add_item (nsort_t *srt, nsort_link_t *lnk);
int nsort_restructure_nodes (nsort_t *srt);
nsort_link_t *nsort_find_item (nsort_t *srt, nsort_link_t *lnk);
nsort_link_t *nsort_query_item (nsort_t *srt, nsort_link_t *lnk);
int nsort_store (nsort_t *srt, nsort_store_t *ts,
    int reclen, char *fname);
int nsort_save (nsort_t *srt, const char *desc, int reclen,
    char *fname);
int nsort_retrieve (nsort_t *srt, nsort_store_t *ts,
    const char *fname, long magic);
int nsort_get (nsort_t *srt, int (*compare)(void*,void*), const char *fname);
int nsort_get_all (nsort_t *srt, int (*compare)(void*,void*), 
    const char *fname, char *desc, char *timestamp);
nsort_link_t *nsort_remove_item (nsort_t *srt, nsort_link_t *lnk);
int nsort_node_levels (nsort_t *srt, nsort_node_level_t *lvl);
nsort_list_t *nsort_sort_to_list (nsort_t *srt);
int nsort_list_to_sort (nsort_t *srt, nsort_list_t *lh, int (*cmp)(void*,void*));
unsigned long nsort_hash_function(register const unsigned char *key);
int nsort_show_hash_error (nsort_hash_t *hsh, char *str, size_t len);
nsort_hash_t *nsort_hash_create (void);
int nsort_hash_destroy (nsort_hash_t *hsh);
int nsort_hash_init (nsort_hash_t *hsh,
    int (*compare) (void*,void*),
    unsigned int (*hash)(const char *));
int nsort_hash_add_item (nsort_hash_t *hsh, const char *item);
char *nsort_hash_find_item (nsort_hash_t *hsh, const char *item);
int nsort_hash_del (nsort_hash_t *hsh);

/*
 * @DocInclude ../lib/qsort.c
 */
extern int bsort_num_compare;
int bqsort (void *base_ptr, int total_elems, int size, int(*cmp)(void*,void*));
int nsort_qsort (nsort_t *srt, void *base, size_t numItems, size_t recSize,
    int (*qcompare) (void *, void *),
    int (*scompare) (void *, void *));
int nsort_list_qsort (nsort_t *srt, nsort_list_t *lh, size_t recSize,
    int (*compare) (void *, void *));

/*
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 * Debug Malloc Code
 * <<<<<<<<<<<<<<<<<<<<====================>>>>>>>>>>>>>>>>>>>  
 */

/*
 * I've decided not to document this stuff.  I only included it because I couldn't
 * find an alternative in a mature library or system (like valgrind) that
 * did the job the way I needed it done.  Basically, I included it because I know
 * of nothing better.  When I find something better, I will replace this.  
 * It is not thread safe and, therefore, not the final solution.
 */
#ifdef DEBUG

#define DBGMEM_HEADER           0xcefaedfeUL
#define DBGMEM_TRAILER          0xfecaefbeUL
#define SIZE_DEBUG_HEADER       (sizeof(DBGMEM_HEADER))
#define SIZE_DEBUG_TRAILER      (sizeof(DBGMEM_TRAILER))
#define SIZE_DEBUG              (SIZE_DEBUG_HEADER+SIZE_DEBUG_TRAILER)

#undef malloc   /* in case they are system macros */
#undef calloc
#undef free
#undef realloc
#undef check_reference
#undef check_pointer
#undef print_block_list
#undef strdup

#define malloc(size)            nsort_malloc(size,__FILE__,__LINE__)
#define calloc(size,num)        nsort_calloc(size,num,__FILE__,__LINE__)
#define free(p)                 nsort_free(p,__FILE__,__LINE__)
#define realloc(p,size)         nsort_realloc(p,size,__FILE__,__LINE__)
#define strdup(s)               nsort_strdup(s,__FILE__, __LINE__)

/* check that p is pointing inside some block */
#define check_reference(p)      check_address(p,__FILE__,__LINE__)

/* check that p is pointing to a block */
#define check_pointer(p)        check_valid(p,__FILE__,__LINE__,4)

#define check_list(lh,lvl)      nsort_debug_list_check(lh,lvl,__FILE__,__LINE__)
#define check_sort(st,lvl)      nsort_debug_sort_check(st,lvl,__FILE__,__LINE__)

void *nsort_malloc(int size,char *filename,int line);
void *nsort_calloc(int size,int num,char *filename,int line);
void nsort_free(void *p,char *filename,int line);
void *nsort_realloc(void *p,int newsize,char *filename,int line);
void print_block_list(void);
int check_valid(void *address,char *filename,int line,int type);
int check_address(void *address,char *filename,int line);

int nsort_debug_list_check (nsort_list_t *lh, int level, char *file, int line);

#else  /* not debugging; change some functions to nothing */

#define check_reference(p)      /* nothing */
#define check_pointer(p)        /* nothing */
#define print_block_list()      /* nothing */
#define check_list(lh,lvl)      /* nothing */
#define check_sort(st,lvl)      /* nothing */

#endif  /* ifdef DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* __SORT_H__ */
