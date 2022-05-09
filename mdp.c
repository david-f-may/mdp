/* Source File: mdp.c */

/*
 * RCS Info
 *
 * $Id: mdp.c,v 0.12 2019/06/26 21:30:53 dmay Exp dmay $
 *
 * $Log: mdp.c,v $
 * Revision 0.12  2019/06/26 21:30:53  dmay
 * Fixed crc stuff.
 *
 * Revision 0.11  2019/06/26 17:27:29  dmay
 * Took out some unneeded stuff and put in a -h switch and a help message.
 *
 * Revision 0.10  2019/06/26 15:16:38  dmay
 * I cleaned up some errors.
 *
 * Revision 0.9  2019/06/07 20:21:42  dmay
 * I implemented a sorted output function with -s on command-line.
 *
 * Revision 0.8  2019/06/07 17:44:04  dmay
 * Implemented exdir() and hashinfo.
 *
 * Revision 0.7  2019/06/06 22:04:14  dmay
 * I implemented the ex() keyword to exclude a file.
 *
 * Revision 0.6  2019/06/06 20:43:20  dmay
 * I added the default keyword, sha256 and sha512.
 *
 * Revision 0.5  2019/05/24 19:54:43  dmay
 * Added the mddir command support.
 *
 * Revision 0.4  2019/05/24 15:25:32  dmay
 * I added crc64 and made that the default.
 *
 * Revision 0.3  2019/05/23 21:02:17  dmay
 * In this version I fully supported the command-line options.
 *
 * Revision 0.2  2019/05/23 19:15:05  dmay
 * Did considerable work on formatting.
 *
 * Revision 0.1  2019/05/23 17:10:18  dmay
 * This is the initial verstion.
 *
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

#include <sys/types.h>
#include <sys/stat.h>                            // stat
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>                              // read(), getopt()
#include <stdlib.h>                              // getopt() stuff
#include <stdint.h>
#include <getopt.h>
#include <pwd.h>                                 // getpwuid()
#include <grp.h>                                 // getgrgid()
#include <dirent.h>
#include <regex.h>
#include <errno.h>
#include <err.h>
#include <linux/limits.h>                        // PATH_MAX
#include <openssl/md5.h>                         // md5sum()
#include <openssl/sha.h>                         // sha1sum()
#include <openssl/ripemd.h>                      // rmd160sum()

#include "mtoken.h"
#include "sort.h"

#define MSG_SZ 1023
#define BUFSIZE 1024*512
#define FIELD_SZ 31
#define SHA_SZ 2*SHA_DIGEST_LENGTH
#define SHA256_SZ 2*SHA256_DIGEST_LENGTH
#define SHA512_SZ 2*SHA512_DIGEST_LENGTH
#define MD5_SZ 2*MD5_DIGEST_LENGTH
#define RMD_SZ 2*RIPEMD160_DIGEST_LENGTH

static int print_errors_only = 0;
static int quiet = 0;
static int skip_directories = 0;
static int sorted = 0;

/* path    |owner   |group   |size |mode    |YYYY/MM/DD-HH:MM:SS|md5sum|sha1sum|rmd160sum */
/* PATH_MAX FIELD_SZ FIELD_SZ off_t FIELD_SZ FIELD_SZ            MD5_SZ SHA_SZ  RMD_SZ */
typedef struct _file_dat {
    unsigned char path[PATH_MAX];
    unsigned char owner[FIELD_SZ+1];
    unsigned char group[FIELD_SZ+1];
    off_t size;
    unsigned char mode[FIELD_SZ+1];
    char dt[FIELD_SZ+1];
    unsigned char md5[MD5_SZ+1];
    unsigned char sha[SHA_SZ+1];
    unsigned char sha256[SHA256_SZ+1];
    unsigned char sha512[SHA512_SZ+1];
    int no_crc;
#ifdef __CRC32__
    unsigned int crc;
#endif
#ifndef __CRC32__
    unsigned long long int crc;
#endif
    unsigned char rmd[RMD_SZ+1];
} file_dat;

/*
 **********************************************************************************
 * Function prototypes.
 **********************************************************************************
 */
static int fexists (const char *path);
int split_r (char **cpp, int maxnum, char *ln, int ch);
static int get_stat_data (const char *file, file_dat *fdp);
#ifdef __CRC32__
static unsigned crc32(unsigned val, const void *ss, int len);
static unsigned crc32sum (char *fname);
#endif
#ifndef __CRC32__
static uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l);
static uint64_t crc64sum (const unsigned char *fname);
#endif
static unsigned char *sha1sum (char *fname);
static unsigned char *sha256sum (char *fname);
static unsigned char *md5sum (char *fname);
static unsigned char *rmd160sum (char *fname);
static int get_file_data (char *fname, file_dat *fdp);
int do_md (char *fl, char *val);
int do_exclude_add (char *fl);
int do_sorted_add (char *fl);
static int process_dir(char *dname, char *pattern, char *param, int spec, int is_exclude);
static int recursive_process (char *dname, regex_t *reg, char *param, int spec, int is_exclude);
static void ProcessToken (CDBTokenType token, parseItem * pi);
static CDBTokenType ParseFile (parseItem * pi);
void usage (void);

/*
 **********************************************************************************
 * Support functions
 **********************************************************************************
 */
static int fexists (const char *path)
{
    struct stat buf;
    if (lstat (path, &buf) == 0)
        return 1;
    else
        return 0;
}

int split_r (char **cpp, int maxnum, char *ln, int ch)
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
    return counter;
}

static int get_stat_data (const char *file, file_dat *fdp)
{
    struct stat sb;
    struct passwd *pw;
    struct group *gp;
    char *cp;
    char lctime[FIELD_SZ+1];

    strncpy ((char *)fdp->path, file, PATH_MAX-1);
        if (lstat (file, &sb) == -1) {
            fprintf (stdout, "***Error in get_stat_data(), line %d, processing '%s': \n", __LINE__, file);
            fflush (stdout);
            perror (0);
            return -1;
        }
    strncpy (lctime, ctime (&sb.st_ctime), FIELD_SZ);
    lctime[FIELD_SZ] = '\0';

    /* Get the mode string */
    fdp->mode[0] = '?';
    if (S_ISBLK (sb.st_mode))
        fdp->mode[0] = 'b';
    if (S_ISCHR (sb.st_mode))
        fdp->mode[0] = 'c';
    if (S_ISDIR (sb.st_mode))
        fdp->mode[0] = 'd';
    if (S_ISREG (sb.st_mode))
        fdp->mode[0] = '-';
    if (S_ISFIFO (sb.st_mode))
        fdp->mode[0] = 'p';
    if (S_ISLNK (sb.st_mode))
        fdp->mode[0] = 'l';
    if (S_ISSOCK (sb.st_mode))
        fdp->mode[0] = 's';

    fdp->mode[1] = sb.st_mode & S_IRUSR ? 'r' : '-';
    fdp->mode[2] = sb.st_mode & S_IWUSR ? 'w' : '-';
    fdp->mode[3] = sb.st_mode & S_IXUSR ? 'x' : '-';

    fdp->mode[4] = sb.st_mode & S_IRGRP ? 'r' : '-';
    fdp->mode[5] = sb.st_mode & S_IWGRP ? 'w' : '-';
    fdp->mode[6] = sb.st_mode & S_IXGRP ? 'x' : '-';

    fdp->mode[7] = sb.st_mode & S_IROTH ? 'r' : '-';
    fdp->mode[8] = sb.st_mode & S_IWOTH ? 'w' : '-';
    fdp->mode[9] = sb.st_mode & S_IXOTH ? 'x' : '-';

    if (sb.st_mode & S_ISUID) {
        if (fdp->mode[3] != 'x')
            fdp->mode[3] = 'S';
        else
            fdp->mode[3] = 's';
    }
    if (sb.st_mode & S_ISGID) {
        if (fdp->mode[6] != 'x')
            fdp->mode[6] = 'S';
        else
            fdp->mode[6] = 's';
    }
    if (sb.st_mode & S_ISVTX) {
        if (fdp->mode[9] != 'x')
            fdp->mode[9] = 'T';
        else
            fdp->mode[9] = 't';
    }
    fdp->mode[10] = '\0';

    /* now, get the owner and group from uid, gid */
    pw = getpwuid (sb.st_uid);
    if (0 == pw)
        snprintf ((char*)fdp->owner, FIELD_SZ, "%d", sb.st_uid);
    else
        strncpy ((char*)fdp->owner, pw->pw_name, FIELD_SZ);

    gp = getgrgid (sb.st_gid);
    if (0 == gp)
        snprintf ((char*)fdp->group, FIELD_SZ, "%d", sb.st_gid);
    else
        strncpy ((char*)fdp->group, gp->gr_name, FIELD_SZ);

    /* Now, get the size and date */
    fdp->size = sb.st_size;
    /* work with lctime */
    cp = strchr (lctime, '\n');
    if (cp != 0)
        *cp = '\0';
    /*
     * * Format: YYYY/MM/DD-HH:MM:SS
     * */
    /* 
     * * Year
     * */
    fdp->dt[0] = lctime[20];
    fdp->dt[1] = lctime[21];
    fdp->dt[2] = lctime[22];
    fdp->dt[3] = lctime[23];
    fdp->dt[4] = '/';
    /* month */
    if (lctime[4] == 'A') {            /* Aug, Apr */
        if (lctime[5] == 'u') {          /* Aug */
            fdp->dt[5] = '0';
            fdp->dt[6] = '8';
        }

        else {                             /* Apr */
            fdp->dt[5] = '0';
            fdp->dt[6] = '4';
        }
    }
    if (lctime[4] == 'D') {            /* Dec */
        fdp->dt[5] = '1';
        fdp->dt[6] = '2';
    }
    if (lctime[4] == 'F') {            /* Feb */
        fdp->dt[5] = '0';
        fdp->dt[6] = '2';
    }
    if (lctime[4] == 'J') {            /* Jan, Jun, Jul */
        if (lctime[5] == 'u') {          /* Jun, Jul */
            if (lctime[6] == 'l') {        /* Jul */
                fdp->dt[5] = '0';
                fdp->dt[6] = '7';
            }

            else {                           /* Jun */
                fdp->dt[5] = '0';
                fdp->dt[6] = '6';
            }
        }

        else {                             /* Jan */
            fdp->dt[5] = '0';
            fdp->dt[6] = '1';
        }
    }
    if (lctime[4] == 'M') {            /* Mar, May */
        if (lctime[5] == 'r') {          /* Mar */
            fdp->dt[5] = '0';
            fdp->dt[6] = '3';
        }

        else {                             /* May */
            fdp->dt[5] = '0';
            fdp->dt[6] = '5';
        }
    }
    if (lctime[4] == 'N') {            /* Nov */
        fdp->dt[5] = '1';
        fdp->dt[6] = '1';
    }
    if (lctime[4] == 'O') {            /* Oct */
        fdp->dt[5] = '1';
        fdp->dt[6] = '0';
    }
    if (lctime[4] == 'S') {            /* Sep */
        fdp->dt[5] = '0';
        fdp->dt[6] = '9';
    }
    fdp->dt[7] = '/';
    /*
     * * Day
     * */
    fdp->dt[8] = lctime[8];
    fdp->dt[9] = lctime[9];
    fdp->dt[10] = '-';
    if (fdp->dt[8] == ' ')
        fdp->dt[8] = '0';
    /*
     * * Hour, Min, Sec
     * */
    fdp->dt[11] = lctime[11];
    fdp->dt[12] = lctime[12];
    fdp->dt[13] = lctime[13];
    fdp->dt[14] = lctime[14];
    fdp->dt[15] = lctime[15];
    fdp->dt[16] = lctime[16];
    fdp->dt[17] = lctime[17];
    fdp->dt[18] = lctime[18];
    fdp->dt[19] = '\0';

    /* done */
    return 0;
}

/*
 **********************************************************************************
 * crc32 functions
 **********************************************************************************
 */
#ifdef __CRC32__
const unsigned crc32_table[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
};

static unsigned crc32(unsigned val, const void *ss, int len)
{
    const unsigned char *s = (const unsigned char *)ss;
    while (--len >= 0)
        val = crc32_table[(val ^ *s++) & 0xff] ^ (val >> 8);
    return val;
}

static unsigned crc32sum (char *fname)
{
    FILE *fp;
    char *filebuf;
    int fd, i;
    struct stat statbuf;
    unsigned crc = 0;
    char msg[FIELD_SZ+1];

    if (lstat (fname, &statbuf) != 0) {
        fprintf (stdout, "***Error in crc32sum(), line %d, processing %s: \n", __LINE__, fname);
        fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) )
        return 0;
    fp = fopen (fname, "r");
    if (0 == fp) {
            fprintf (stdout, "***Error in crc32sum(), line %d, processing %s: \n", __LINE__, fname);
        fflush(stdout);
        perror (0);
        return 0;
    }
    filebuf = malloc ((size_t) (statbuf.st_size + 1));
    if (0 == filebuf) {
            fprintf (stdout, "***Error in crc32sum(), line %d: fatal memory error allocating %d bytes\n", __LINE__, (int) statbuf.st_size+1);
        return 0;
    }
    fd = fileno (fp);
    i = read (fd, filebuf, statbuf.st_size);
    if (i <= 0) {
            fprintf (stdout, "***Error in crc32sum(), line %d: read of '%s' failed\n", __LINE__, fname);
        return 0;
    }
    fclose (fp);
    crc = crc32 (crc, filebuf, statbuf.st_size);
    crc &= 0xffffffff;                   /* otherwise, won't work on 64-bit systems. */
    free (filebuf);
    return crc;
}
#endif /* __CRC32__ */

/*
 **********************************************************************************
 * crc64 functions
 **********************************************************************************
 */
#ifndef __CRC32__
static const uint64_t crc64_tab[256] = {
    UINT64_C(0x0000000000000000), UINT64_C(0x7ad870c830358979),
    UINT64_C(0xf5b0e190606b12f2), UINT64_C(0x8f689158505e9b8b),
    UINT64_C(0xc038e5739841b68f), UINT64_C(0xbae095bba8743ff6),
    UINT64_C(0x358804e3f82aa47d), UINT64_C(0x4f50742bc81f2d04),
    UINT64_C(0xab28ecb46814fe75), UINT64_C(0xd1f09c7c5821770c),
    UINT64_C(0x5e980d24087fec87), UINT64_C(0x24407dec384a65fe),
    UINT64_C(0x6b1009c7f05548fa), UINT64_C(0x11c8790fc060c183),
    UINT64_C(0x9ea0e857903e5a08), UINT64_C(0xe478989fa00bd371),
    UINT64_C(0x7d08ff3b88be6f81), UINT64_C(0x07d08ff3b88be6f8),
    UINT64_C(0x88b81eabe8d57d73), UINT64_C(0xf2606e63d8e0f40a),
    UINT64_C(0xbd301a4810ffd90e), UINT64_C(0xc7e86a8020ca5077),
    UINT64_C(0x4880fbd87094cbfc), UINT64_C(0x32588b1040a14285),
    UINT64_C(0xd620138fe0aa91f4), UINT64_C(0xacf86347d09f188d),
    UINT64_C(0x2390f21f80c18306), UINT64_C(0x594882d7b0f40a7f),
    UINT64_C(0x1618f6fc78eb277b), UINT64_C(0x6cc0863448deae02),
    UINT64_C(0xe3a8176c18803589), UINT64_C(0x997067a428b5bcf0),
    UINT64_C(0xfa11fe77117cdf02), UINT64_C(0x80c98ebf2149567b),
    UINT64_C(0x0fa11fe77117cdf0), UINT64_C(0x75796f2f41224489),
    UINT64_C(0x3a291b04893d698d), UINT64_C(0x40f16bccb908e0f4),
    UINT64_C(0xcf99fa94e9567b7f), UINT64_C(0xb5418a5cd963f206),
    UINT64_C(0x513912c379682177), UINT64_C(0x2be1620b495da80e),
    UINT64_C(0xa489f35319033385), UINT64_C(0xde51839b2936bafc),
    UINT64_C(0x9101f7b0e12997f8), UINT64_C(0xebd98778d11c1e81),
    UINT64_C(0x64b116208142850a), UINT64_C(0x1e6966e8b1770c73),
    UINT64_C(0x8719014c99c2b083), UINT64_C(0xfdc17184a9f739fa),
    UINT64_C(0x72a9e0dcf9a9a271), UINT64_C(0x08719014c99c2b08),
    UINT64_C(0x4721e43f0183060c), UINT64_C(0x3df994f731b68f75),
    UINT64_C(0xb29105af61e814fe), UINT64_C(0xc849756751dd9d87),
    UINT64_C(0x2c31edf8f1d64ef6), UINT64_C(0x56e99d30c1e3c78f),
    UINT64_C(0xd9810c6891bd5c04), UINT64_C(0xa3597ca0a188d57d),
    UINT64_C(0xec09088b6997f879), UINT64_C(0x96d1784359a27100),
    UINT64_C(0x19b9e91b09fcea8b), UINT64_C(0x636199d339c963f2),
    UINT64_C(0xdf7adabd7a6e2d6f), UINT64_C(0xa5a2aa754a5ba416),
    UINT64_C(0x2aca3b2d1a053f9d), UINT64_C(0x50124be52a30b6e4),
    UINT64_C(0x1f423fcee22f9be0), UINT64_C(0x659a4f06d21a1299),
    UINT64_C(0xeaf2de5e82448912), UINT64_C(0x902aae96b271006b),
    UINT64_C(0x74523609127ad31a), UINT64_C(0x0e8a46c1224f5a63),
    UINT64_C(0x81e2d7997211c1e8), UINT64_C(0xfb3aa75142244891),
    UINT64_C(0xb46ad37a8a3b6595), UINT64_C(0xceb2a3b2ba0eecec),
    UINT64_C(0x41da32eaea507767), UINT64_C(0x3b024222da65fe1e),
    UINT64_C(0xa2722586f2d042ee), UINT64_C(0xd8aa554ec2e5cb97),
    UINT64_C(0x57c2c41692bb501c), UINT64_C(0x2d1ab4dea28ed965),
    UINT64_C(0x624ac0f56a91f461), UINT64_C(0x1892b03d5aa47d18),
    UINT64_C(0x97fa21650afae693), UINT64_C(0xed2251ad3acf6fea),
    UINT64_C(0x095ac9329ac4bc9b), UINT64_C(0x7382b9faaaf135e2),
    UINT64_C(0xfcea28a2faafae69), UINT64_C(0x8632586aca9a2710),
    UINT64_C(0xc9622c4102850a14), UINT64_C(0xb3ba5c8932b0836d),
    UINT64_C(0x3cd2cdd162ee18e6), UINT64_C(0x460abd1952db919f),
    UINT64_C(0x256b24ca6b12f26d), UINT64_C(0x5fb354025b277b14),
    UINT64_C(0xd0dbc55a0b79e09f), UINT64_C(0xaa03b5923b4c69e6),
    UINT64_C(0xe553c1b9f35344e2), UINT64_C(0x9f8bb171c366cd9b),
    UINT64_C(0x10e3202993385610), UINT64_C(0x6a3b50e1a30ddf69),
    UINT64_C(0x8e43c87e03060c18), UINT64_C(0xf49bb8b633338561),
    UINT64_C(0x7bf329ee636d1eea), UINT64_C(0x012b592653589793),
    UINT64_C(0x4e7b2d0d9b47ba97), UINT64_C(0x34a35dc5ab7233ee),
    UINT64_C(0xbbcbcc9dfb2ca865), UINT64_C(0xc113bc55cb19211c),
    UINT64_C(0x5863dbf1e3ac9dec), UINT64_C(0x22bbab39d3991495),
    UINT64_C(0xadd33a6183c78f1e), UINT64_C(0xd70b4aa9b3f20667),
    UINT64_C(0x985b3e827bed2b63), UINT64_C(0xe2834e4a4bd8a21a),
    UINT64_C(0x6debdf121b863991), UINT64_C(0x1733afda2bb3b0e8),
    UINT64_C(0xf34b37458bb86399), UINT64_C(0x8993478dbb8deae0),
    UINT64_C(0x06fbd6d5ebd3716b), UINT64_C(0x7c23a61ddbe6f812),
    UINT64_C(0x3373d23613f9d516), UINT64_C(0x49aba2fe23cc5c6f),
    UINT64_C(0xc6c333a67392c7e4), UINT64_C(0xbc1b436e43a74e9d),
    UINT64_C(0x95ac9329ac4bc9b5), UINT64_C(0xef74e3e19c7e40cc),
    UINT64_C(0x601c72b9cc20db47), UINT64_C(0x1ac40271fc15523e),
    UINT64_C(0x5594765a340a7f3a), UINT64_C(0x2f4c0692043ff643),
    UINT64_C(0xa02497ca54616dc8), UINT64_C(0xdafce7026454e4b1),
    UINT64_C(0x3e847f9dc45f37c0), UINT64_C(0x445c0f55f46abeb9),
    UINT64_C(0xcb349e0da4342532), UINT64_C(0xb1eceec59401ac4b),
    UINT64_C(0xfebc9aee5c1e814f), UINT64_C(0x8464ea266c2b0836),
    UINT64_C(0x0b0c7b7e3c7593bd), UINT64_C(0x71d40bb60c401ac4),
    UINT64_C(0xe8a46c1224f5a634), UINT64_C(0x927c1cda14c02f4d),
    UINT64_C(0x1d148d82449eb4c6), UINT64_C(0x67ccfd4a74ab3dbf),
    UINT64_C(0x289c8961bcb410bb), UINT64_C(0x5244f9a98c8199c2),
    UINT64_C(0xdd2c68f1dcdf0249), UINT64_C(0xa7f41839ecea8b30),
    UINT64_C(0x438c80a64ce15841), UINT64_C(0x3954f06e7cd4d138),
    UINT64_C(0xb63c61362c8a4ab3), UINT64_C(0xcce411fe1cbfc3ca),
    UINT64_C(0x83b465d5d4a0eece), UINT64_C(0xf96c151de49567b7),
    UINT64_C(0x76048445b4cbfc3c), UINT64_C(0x0cdcf48d84fe7545),
    UINT64_C(0x6fbd6d5ebd3716b7), UINT64_C(0x15651d968d029fce),
    UINT64_C(0x9a0d8ccedd5c0445), UINT64_C(0xe0d5fc06ed698d3c),
    UINT64_C(0xaf85882d2576a038), UINT64_C(0xd55df8e515432941),
    UINT64_C(0x5a3569bd451db2ca), UINT64_C(0x20ed197575283bb3),
    UINT64_C(0xc49581ead523e8c2), UINT64_C(0xbe4df122e51661bb),
    UINT64_C(0x3125607ab548fa30), UINT64_C(0x4bfd10b2857d7349),
    UINT64_C(0x04ad64994d625e4d), UINT64_C(0x7e7514517d57d734),
    UINT64_C(0xf11d85092d094cbf), UINT64_C(0x8bc5f5c11d3cc5c6),
    UINT64_C(0x12b5926535897936), UINT64_C(0x686de2ad05bcf04f),
    UINT64_C(0xe70573f555e26bc4), UINT64_C(0x9ddd033d65d7e2bd),
    UINT64_C(0xd28d7716adc8cfb9), UINT64_C(0xa85507de9dfd46c0),
    UINT64_C(0x273d9686cda3dd4b), UINT64_C(0x5de5e64efd965432),
    UINT64_C(0xb99d7ed15d9d8743), UINT64_C(0xc3450e196da80e3a),
    UINT64_C(0x4c2d9f413df695b1), UINT64_C(0x36f5ef890dc31cc8),
    UINT64_C(0x79a59ba2c5dc31cc), UINT64_C(0x037deb6af5e9b8b5),
    UINT64_C(0x8c157a32a5b7233e), UINT64_C(0xf6cd0afa9582aa47),
    UINT64_C(0x4ad64994d625e4da), UINT64_C(0x300e395ce6106da3),
    UINT64_C(0xbf66a804b64ef628), UINT64_C(0xc5bed8cc867b7f51),
    UINT64_C(0x8aeeace74e645255), UINT64_C(0xf036dc2f7e51db2c),
    UINT64_C(0x7f5e4d772e0f40a7), UINT64_C(0x05863dbf1e3ac9de),
    UINT64_C(0xe1fea520be311aaf), UINT64_C(0x9b26d5e88e0493d6),
    UINT64_C(0x144e44b0de5a085d), UINT64_C(0x6e963478ee6f8124),
    UINT64_C(0x21c640532670ac20), UINT64_C(0x5b1e309b16452559),
    UINT64_C(0xd476a1c3461bbed2), UINT64_C(0xaeaed10b762e37ab),
    UINT64_C(0x37deb6af5e9b8b5b), UINT64_C(0x4d06c6676eae0222),
    UINT64_C(0xc26e573f3ef099a9), UINT64_C(0xb8b627f70ec510d0),
    UINT64_C(0xf7e653dcc6da3dd4), UINT64_C(0x8d3e2314f6efb4ad),
    UINT64_C(0x0256b24ca6b12f26), UINT64_C(0x788ec2849684a65f),
    UINT64_C(0x9cf65a1b368f752e), UINT64_C(0xe62e2ad306bafc57),
    UINT64_C(0x6946bb8b56e467dc), UINT64_C(0x139ecb4366d1eea5),
    UINT64_C(0x5ccebf68aecec3a1), UINT64_C(0x2616cfa09efb4ad8),
    UINT64_C(0xa97e5ef8cea5d153), UINT64_C(0xd3a62e30fe90582a),
    UINT64_C(0xb0c7b7e3c7593bd8), UINT64_C(0xca1fc72bf76cb2a1),
    UINT64_C(0x45775673a732292a), UINT64_C(0x3faf26bb9707a053),
    UINT64_C(0x70ff52905f188d57), UINT64_C(0x0a2722586f2d042e),
    UINT64_C(0x854fb3003f739fa5), UINT64_C(0xff97c3c80f4616dc),
    UINT64_C(0x1bef5b57af4dc5ad), UINT64_C(0x61372b9f9f784cd4),
    UINT64_C(0xee5fbac7cf26d75f), UINT64_C(0x9487ca0fff135e26),
    UINT64_C(0xdbd7be24370c7322), UINT64_C(0xa10fceec0739fa5b),
    UINT64_C(0x2e675fb4576761d0), UINT64_C(0x54bf2f7c6752e8a9),
    UINT64_C(0xcdcf48d84fe75459), UINT64_C(0xb71738107fd2dd20),
    UINT64_C(0x387fa9482f8c46ab), UINT64_C(0x42a7d9801fb9cfd2),
    UINT64_C(0x0df7adabd7a6e2d6), UINT64_C(0x772fdd63e7936baf),
    UINT64_C(0xf8474c3bb7cdf024), UINT64_C(0x829f3cf387f8795d),
    UINT64_C(0x66e7a46c27f3aa2c), UINT64_C(0x1c3fd4a417c62355),
    UINT64_C(0x935745fc4798b8de), UINT64_C(0xe98f353477ad31a7),
    UINT64_C(0xa6df411fbfb21ca3), UINT64_C(0xdc0731d78f8795da),
    UINT64_C(0x536fa08fdfd90e51), UINT64_C(0x29b7d047efec8728),
};

static uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l)
{
    uint64_t j;

    for (j = 0; j < l; j++) {
        uint8_t byte = s[j];
        crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
    }
    return crc;
}

static uint64_t crc64sum (const unsigned char *fname)
{
    FILE *fp;
    char *filebuf;
    int fd, i;
    struct stat statbuf;
    unsigned crc = 0;

    if (lstat ((const char *)fname, &statbuf) != 0) {
        fprintf (stdout, "***Error in main(), line %d, processing %s: \n", __LINE__, fname);
        fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) )
    return 0;
    fp = fopen ((const char *)fname, "r");
    if (0 == fp) {
        fprintf (stdout, "***Error in main(), line %d, processing %s: \n", __LINE__, fname);
        fflush(stdout);
        perror (0);
        return 0;
    }
    filebuf = malloc ((size_t) (statbuf.st_size + 1));
    if (0 == filebuf) {
        fprintf (stdout, "***Error in main(), line %d: fatal memory error allocating %d bytes\n", __LINE__, (int) statbuf.st_size+1);
        return 0;
    }
    fd = fileno (fp);
    i = read (fd, filebuf, statbuf.st_size);
    if (i <= 0) {
        fprintf (stdout, "***Error in main(), line %d: read of '%s' failed\n", __LINE__, fname);
        return 0;
    }
    fclose (fp);
    crc = crc64 (crc, (const unsigned char *)filebuf, statbuf.st_size);
    free (filebuf);
    return crc;
}
#endif /* ! __CRC32__ */

/*
 **********************************************************************************
 * sha1sum()
 **********************************************************************************
 */
static unsigned char *sha1sum (char *fname)
{
    SHA_CTX c;
    int fd, i;
    unsigned char buf[BUFSIZE];
    FILE *fp;
    unsigned char sha1[SHA_DIGEST_LENGTH];
    static char sha1out[2 * SHA_DIGEST_LENGTH + 1];
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in sha1sum(), line %d, processing '%s': \n", __LINE__, fname);
            fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
        return 0;
    fp = fopen (fname, "r");
    if (fp == 0) {
        return 0;
    }
    fd = fileno (fp);
    SHA1_Init (&c);
    for (;;) {
        i = read (fd, buf, BUFSIZE);
        if (i <= 0)
            break;
        SHA1_Update (&c, buf, (unsigned long) i);
    }
    SHA1_Final (&(sha1[0]), &c);
    fclose (fp);
    memset (sha1out, 0, 2 * SHA_DIGEST_LENGTH + 1);
    for (i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf (&(sha1out[2 * i]), "%02x", sha1[i]);
    return (unsigned char *) sha1out;
}

/*
 **********************************************************************************
 * sha256sum()
 **********************************************************************************
 */
static unsigned char *sha256sum (char *fname)
{
  SHA256_CTX c;
  int fd, i;
  unsigned char buf[BUFSIZE];
  FILE *fp;
  unsigned char sha1[SHA256_DIGEST_LENGTH];
  static char sha1out[2 * SHA256_DIGEST_LENGTH + 1];
  struct stat statbuf;

  if (lstat (fname, &statbuf) != 0) {
    fprintf (stdout, "***Error in sha256sum(), line %d, processing '%s': \n", __LINE__, fname);
    fflush(stdout);
    perror (0);
    return 0;
  }
  if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
    return 0;
  fp = fopen (fname, "r");
  if (fp == 0) {
    return 0;
  }
  fd = fileno (fp);
  SHA256_Init (&c);
  for (;;) {
    i = read (fd, buf, BUFSIZE);
    if (i <= 0)
      break;
    SHA256_Update (&c, buf, (unsigned long) i);
  }
  SHA256_Final (&(sha1[0]), &c);
  fclose (fp);
  memset (sha1out, 0, 2 * SHA256_DIGEST_LENGTH + 1);
  for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
    sprintf (&(sha1out[2 * i]), "%02x", sha1[i]);
  return (unsigned char *) sha1out;
}


/*
 **********************************************************************************
 * sha512sum()
 **********************************************************************************
 */
static unsigned char *sha512sum (char *fname)
{
  SHA512_CTX c;
  int fd, i;
  unsigned char buf[BUFSIZE];
  FILE *fp;
  unsigned char sha1[SHA512_DIGEST_LENGTH];
  static char sha1out[2 * SHA512_DIGEST_LENGTH + 1];
  struct stat statbuf;

  if (lstat (fname, &statbuf) != 0) {
    fprintf (stdout, "***Error in sha512sum(), line %d, processing '%s': \n", __LINE__, fname);
    fflush(stdout);
    perror (0);
    return 0;
  }
  if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
    return 0;
  fp = fopen (fname, "r");
  if (fp == 0) {
    return 0;
  }
  fd = fileno (fp);
  SHA512_Init (&c);
  for (;;) {
    i = read (fd, buf, BUFSIZE);
    if (i <= 0)
      break;
    SHA512_Update (&c, buf, (unsigned long) i);
  }
  SHA512_Final (&(sha1[0]), &c);
  fclose (fp);
  memset (sha1out, 0, 2 * SHA512_DIGEST_LENGTH + 1);
  for (i = 0; i < SHA512_DIGEST_LENGTH; i++)
    sprintf (&(sha1out[2 * i]), "%02x", sha1[i]);
  return (unsigned char *) sha1out;
}


/*
 **********************************************************************************
 * md5sum()
 **********************************************************************************
 */
static unsigned char *md5sum (char *fname)
{
    FILE *fp;
    static unsigned char buf[BUFSIZE];
    int fd, i;
    MD5_CTX c;
    unsigned char md5[MD5_DIGEST_LENGTH];
    static char md5out[2 * MD5_DIGEST_LENGTH + 1];
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in sha1sum(), line %d, processing '%s': \n", __LINE__, fname);
            fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
        return 0;

    fp = fopen (fname, "r");
    if (0 == fp) {
        return 0;
    }
    fd = fileno (fp);
    MD5_Init (&c);
    for (;;) {
        i = read (fd, buf, BUFSIZE);
        if (i <= 0)
            break;
        MD5_Update (&c, buf, (unsigned long) i);
    }
    MD5_Final (&(md5[0]), &c);
    fclose (fp);
    memset (md5out, 0, 2 * MD5_DIGEST_LENGTH + 1);
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf (&(md5out[2 * i]), "%02x", md5[i]);
    return (unsigned char *) md5out;
}

/*
 **********************************************************************************
 * rmd160sum()
 **********************************************************************************
 */
static unsigned char *rmd160sum (char *fname)
{
    RIPEMD160_CTX c;
    FILE *fp;
    int fd, i;
    unsigned char buf[BUFSIZE];
    unsigned char rmd160[RIPEMD160_DIGEST_LENGTH];
    static char rmd160out[2 * RIPEMD160_DIGEST_LENGTH + 1];
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in rmd160sum(), line %d, processing '%s': \n", __LINE__, fname);
            fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
        return 0;

    fp = fopen (fname, "r");
    if (0 == fp) {
        return 0;
    }
    fd = fileno (fp);
    RIPEMD160_Init (&c);
    for (;;) {
        i = read (fd, buf, BUFSIZE);
        if (i <= 0)
            break;
        RIPEMD160_Update (&c, buf, (unsigned long) i);
    }
    RIPEMD160_Final (&(rmd160[0]), &c);
    fclose (fp);
    memset (rmd160out, 0, 2 * RIPEMD160_DIGEST_LENGTH + 1);
    for (i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
        sprintf (&(rmd160out[2 * i]), "%02x", rmd160[i]);
    return (unsigned char *) rmd160out;
}

/*
 **********************************************************************************
 * get_file_data()
 **********************************************************************************
 */
static int get_file_data (char *fname, file_dat *fdp)
{
    int ret;
    unsigned char *shsum;
    unsigned char *mdsum;
    unsigned char *rmsum;
    struct stat statbuf;

    if (lstat (fname, &statbuf) != 0) {
            fprintf (stdout, "***Error in get_file_data(), in line %d, processing '%s': \n", __LINE__, fname);
            fflush(stdout);
        perror (0);
        return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) )
        return 0;

    ret = get_stat_data (fname, fdp);
    if (ret)
        return -1;

    if (fdp->size == 0) {
        /* Do not process files with 0 size */
        fdp->crc = 0;
        strcpy ((char *)fdp->sha, "NONE");
        strcpy ((char *)fdp->sha256, "NONE");
        strcpy ((char *)fdp->sha512, "NONE");
        strcpy ((char *)fdp->md5, "NONE");
        strcpy ((char *)fdp->rmd, "NONE");
        return 0;
    }
    if (fdp->size > (off_t)(250*1024*1024)) {
        /* file larger than 250 Megs - don't process */
        fdp->crc = 0;
        strcpy ((char *)fdp->sha, "NONE");
        strcpy ((char *)fdp->sha256, "NONE");
        strcpy ((char *)fdp->sha512, "NONE");
        strcpy ((char *)fdp->md5, "NONE");
        strcpy ((char *)fdp->rmd, "NONE");
        return 0;
    }
    if (S_ISDIR(statbuf.st_mode)) {
        /* file larger than 250 Megs - don't process */
        fdp->crc = 0;
        strcpy ((char *)fdp->sha, "DIRECTORY");
        strcpy ((char *)fdp->sha256, "DIRECTORY");
        strcpy ((char *)fdp->sha512, "DIRECTORY");
        strcpy ((char *)fdp->md5, "DIRECTORY");
        strcpy ((char *)fdp->rmd, "DIRECTORY");
        return 0;
    }
    if (fdp->mode[0] != 'd' && fdp->no_crc == 0) {
#ifdef __CRC32__
        fdp->crc  = crc32sum (fname);
#endif /* __CRC32 */
#ifndef __CRC32__
        fdp->crc  = crc64sum ((const unsigned char *)fname);
#endif /* ! __CRC32 */
        if (fdp->crc == 0)
            fdp->no_crc = 1;
    }
    else
        fdp->crc = 0;

    shsum = sha1sum(fname);
    if (shsum == 0)
        return -1;
    strncpy ((char*)fdp->sha, (char*)shsum, 2*SHA_DIGEST_LENGTH);

    shsum = sha256sum(fname);
    if (shsum == 0)
        return -1;
    strncpy ((char*)fdp->sha256, (char*)shsum, 2*SHA256_DIGEST_LENGTH);

    shsum = sha512sum(fname);
    if (shsum == 0)
        return -1;
    strncpy ((char*)fdp->sha512, (char*)shsum, 2*SHA512_DIGEST_LENGTH);

    mdsum = md5sum (fname);
    if (mdsum == 0)
        return -1;
    strncpy ((char*)fdp->md5, (char*)mdsum, 2*MD5_DIGEST_LENGTH);

    rmsum = rmd160sum (fname);
    if (rmsum == 0)
        return -1;
    strncpy ((char *)fdp->rmd, (char *)rmsum, 2*RIPEMD160_DIGEST_LENGTH);

    return 0;
}

#define NUM_ITEMS 20

static file_dat _fd;

int do_md (char *fl, char *val)
{
    int ret;
    int i;
    struct stat statbuf;
    char *cp = 0;
    char *cp1 = 0;
    int print_outstr;
    file_dat *fdp;
    char *items[NUM_ITEMS+1];
    char outstr[PATH_MAX];
    char size[PATH_MAX];
    char param[PATH_MAX+1];

    print_outstr = 0;
    // Make sure file exists and get statbuf data.
    if (lstat (fl, &statbuf) != 0) {
        if (! quiet) {
            fprintf (stderr, "***Warning in do_md(), line %d: File '%s' does not exist.\n", __LINE__, fl);
        }
        return -1;
    }
    if (skip_directories) {
        if (S_ISDIR (statbuf.st_mode))
            return 0;
    }
    if (! (S_ISREG(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) ) {
        if (! quiet) {
            fprintf (stderr, "***Warning in do_md(), line %d: File '%s' is not a regular file or a directory.\n", __LINE__, fl);
        }
        return 0;
    }
    if (val == 0 || val[0] == '\0') {
            fprintf (stderr, "\n***Error in do_md(), line %d: val \"%s\" is invalid\n", __LINE__, val);
        return -1;
    }

    strcpy (param, val);
    fdp = &_fd;
    memset (fdp, 0, sizeof(file_dat));
    // See if val (param) has '|', so it is like, "mode|owner|group|size|dt|md5|sha" (default), for example.
    cp1 = param;
    cp = strchr (param, (int) '|');
    if (cp != 0) {
        for (i = 0; i < NUM_ITEMS; i++) {
            *cp = '\0';
            items[i] = malloc (strlen (cp1) + 1);
            if (items[i] == 0) {
                goto ProgramDone;
            }
            strcpy (items[i], cp1);
            items[i+1] = 0;
            cp1 = cp+1;
            if (*cp1 == '\0')
                break;
            cp = strchr (cp1, (int) '|');
            if (cp == 0) {
                i++;
                items[i] = malloc (strlen (cp1) + 1);
                if (items[i] == 0) {
                    goto ProgramDone;
                }
                strcpy (items[i], cp1);
                items[i+1] = 0;
                break;
            }
        }
    }
    else {
        items[0] = malloc (strlen (param)+1);
        memset (items[0], 0, strlen (param) + 1);
        strcpy (items[0], param);
        items[1] = 0;
    }

    if (S_ISDIR(statbuf.st_mode))
        fdp->no_crc = 1;
    ret = get_file_data (fl, fdp);
    if (ret)
        goto ProgramDone;

    if (! strcmp (items[0], "all")) {
        if (fdp->path[0] != '\0') {
            if (!print_errors_only) {
                if (!sorted) {
#ifdef __CRC32__
                    printf ("%s|%s|%s|%s|%lld|%s|%d|%s|%s|%s|%s|%s\n",
#endif
#ifndef __CRC32__
                    printf ("%s|%s|%s|%s|%lld|%s|%llx|%s|%s|%s|%s|%s\n",
#endif
                            fdp->path,
                            fdp->mode,
                            fdp->owner,
                            fdp->group,
                            (long long) fdp->size,
                            fdp->dt,
                            fdp->crc,
                            fdp->md5,
                            fdp->sha,
                            fdp->sha256,
                            fdp->sha512,
                            fdp->rmd);
                }
                else {
#ifdef __CRC32__
                    snprintf (outstr, PATH_MAX, "%s|%s|%s|%s|%lld|%s|%d|%s|%s|%s|%s|%s\n",
#endif
#ifndef __CRC32__
                    snprintf (outstr, PATH_MAX, "%s|%s|%s|%s|%lld|%s|%llx|%s|%s|%s|%s|%s\n",
#endif
                            fdp->path,
                            fdp->mode,
                            fdp->owner,
                            fdp->group,
                            (long long) fdp->size,
                            fdp->dt,
                            fdp->crc,
                            fdp->md5,
                            fdp->sha,
                            fdp->sha256,
                            fdp->sha512,
                            fdp->rmd);
                    ret = do_sorted_add (outstr);
                }
            }
        }
        goto ProgramDone;
    }
    if (! strcmp (items[0], "default")) {
        if (fdp->path[0] != '\0') {
            if (!print_errors_only) {
                if (! sorted) {
#ifdef __CRC32__
                    printf ("%s|%s|%s|%s|%lld|%s|%d|%s|%s\n",
#endif
#ifndef __CRC32__
                    printf ("%s|%s|%s|%s|%lld|%s|%llx|%s|%s\n",
#endif
                            fdp->path,
                            fdp->mode,
                            fdp->owner,
                            fdp->group,
                            (long long) fdp->size,
                            fdp->dt, 
                            fdp->crc,
                            fdp->md5,
                            fdp->sha256);
                }
                else {
#ifdef __CRC32__
                    snprintf (outstr, PATH_MAX, "%s|%s|%s|%s|%lld|%s|%d|%s|%s\n",
#endif
#ifndef __CRC32__
                    snprintf (outstr, PATH_MAX, "%s|%s|%s|%s|%lld|%s|%llx|%s|%s\n",
#endif
                            fdp->path,
                            fdp->mode,
                            fdp->owner,
                            fdp->group,
                            (long long) fdp->size,
                            fdp->dt,
                            fdp->crc,
                            fdp->md5,
                            fdp->sha256);
                    ret = do_sorted_add (outstr);
                }
            }
        }
        goto ProgramDone;
    }
   for (i = 0; i < NUM_ITEMS; i++) {
        if (!strcmp (items[i], "mode")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->mode);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->mode);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "owner")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->owner);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->owner);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "group")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->group);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->group);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "size")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%lld ", fdp->path, (long long) fdp->size);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                snprintf (size, 50, "|%lld", (long long)fdp->size);
                strcat (outstr, (const char *)size);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "dt")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->dt);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->dt);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "crc")) {
            if (i == 0) {
#ifdef __CRC32__
                snprintf (outstr, PATH_MAX-1, "%s|%d", fdp->path, fdp->crc);
#endif
#ifndef __CRC32__
                snprintf (outstr, PATH_MAX-1, "%s|%llx", fdp->path, fdp->crc);
#endif
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
#ifdef __CRC32__
                snprintf (size, 50, "|%d", fdp->crc);
#endif
#ifndef __CRC32__
                snprintf (size, 50, "|%llx", fdp->crc);
#endif
                strcat (outstr, (const char *)size);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "md5")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->md5);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->md5);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "sha")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->sha);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->sha);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "sha256")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->sha256);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->sha256);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "sha512")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->sha512);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->sha512);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
        if (!strcmp (items[i], "rmd")) {
            if (i == 0) {
                snprintf (outstr, PATH_MAX-1, "%s|%s", fdp->path, fdp->rmd);
                print_outstr = 1;
                goto ProcessResults;
            }
            else {
                strcat (outstr, "|");
                strcat (outstr, (const char *)fdp->rmd);
                print_outstr = 1;
                goto ProcessResults;
            }
        }
ProcessResults:
        if (i < NUM_ITEMS && items[i+1] == 0) {
            if (print_outstr == 1) {
                if (! print_errors_only) {
                    if (! sorted)
                        printf ("%s\n", outstr);
                    else {
                        ret = do_sorted_add (outstr);
                        goto ProgramDone;
                    }
                }
            }
            else {
                if (! print_errors_only) {
                    printf ("\n");
                }
            }
            break;
        }
    }

    ret = 0;

ProgramDone: 
ExitDeleteItems:
    for (i = 0; i < NUM_ITEMS; i++) {
        if (items[i] != 0) {
            free (items[i]);
            items[i] = 0;
        }
        else break;
    }
    return ret;
}

#define ERROR_LEN 512
static nsort_hash_t *hsh = 0;
static nsort_t *srt = 0;
static nsort_link_t hsh_link;

#define HASH_STR(x)  (((x)[0]<<15)+((x)[1]<<10)+((x)[2]<<5)+(x)[3])

static int sortCompare (void *p1, void *p2)
{
  unsigned int v1 = HASH_STR((char*)p1),
               v2 = HASH_STR((char*)p2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return (strcmp ((char *) p1, (char *) p2));
}

int do_exclude_add (char *fl)
{
    int ret;
    char err_str[ERROR_LEN+1];

    if (hsh == 0) {
        hsh = nsort_hash_create ();
        if (hsh == 0) {
            nsort_show_error (err_str, ERROR_LEN);
            printf ("***ERROR: Function do_exclude_add: Line %d: %s\n", __LINE__, err_str);
            return -1;
        }
        ret = nsort_hash_init (hsh, sortCompare, 0);
        if (ret == _ERROR_) {
            printf ("***ERROR: Function do_exclude_add: Line %d: %s\n", __LINE__, sortErrorString[hsh->hashError]);
            return -1;
        }
    }
    ret = nsort_hash_add_item (hsh, fl);
    if (ret == _ERROR_) {
        if (hsh->hashError == SORT_UNIQUE) {
            hsh->hashError = SORT_NOERROR;
        } else {
            printf ("***ERROR: Function do_exclude_add: Line %d: %s\n", __LINE__, sortErrorString[hsh->hashError]);
            return -1;
        }
    }
    return 0;
}

int do_sorted_add (char *fl)
{
    nsort_link_t *lnk;
    char *data;
    char str[ERROR_LEN+1];
    int ret;

    if (srt == 0) {
        srt = nsort_create();
        if (srt == 0) {
            nsort_show_error (str, ERROR_LEN);
            printf ("***Error in do_sorted_add (), line %d: nsort_create (): %s\n", __LINE__, str);
            return -1;
        }
        ret = nsort_init (srt, sortCompare, TRUE, TRUE);
        if (ret == _ERROR_) {
            nsort_show_sort_error (srt, str, ERROR_LEN);
            printf ("***Error in do_sorted_add (), line %d: nsort_init(): %s\n", __LINE__, str);
            nsort_destroy (srt);
            return -1;
        }
    }
    lnk = malloc (sizeof (nsort_link_t));
    if (lnk == 0) {
        printf ("***Error in do_sorted_add (), line %d: malloc (sizeof (nsort_link_t))\n", __LINE__); 
        nsort_del (srt, 0);
        nsort_destroy (srt);
        return -1;
    }
    data = malloc (strlen (fl)+1);
    if (data == 0) {
        printf ("***Error in do_sorted_add (), line %d: malloc (sizeof (%ld))\n", __LINE__, strlen (fl)); 
        free (lnk);
        nsort_del (srt, 0);
        nsort_destroy (srt);
        return -1;
    }
    strcpy (data, fl);
    lnk->data = data;
    ret = nsort_add_item (srt, lnk);
    if (ret == _ERROR_) {
        nsort_show_sort_error (srt, str, ERROR_LEN);
        printf ("***Error in do_sorted_add (), line %d: nsort_add_item (): %s\n", __LINE__, str); 
        free (data);
        free (lnk);
        nsort_del (srt, 0);
        nsort_destroy (srt);
        return -1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// ParseMD()
// Parse and process an "md ..." line.
////////////////////////////////////////////////////////////////////////////////

#define ERROR(f,t_str) printf ("***ERROR: Function %s: File %s, line %d, id = \"%s\" unexpected: \"%s\"\n", f, pi->tokenFile, pi->lnctr, pi->id, t_str)

static CDBTokenType ParseMD (parseItem *pi)
{
    CDBTokenType tkn = TOKTRUE;
    static char fn[PATH_MAX];
    static char param[PATH_MAX];
    static char *cp;
    nsort_link_t *lnk = &hsh_link;
    int ret;

    // Expecting "("
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, "(")) {
        ERROR ("ParseMD()", "Expected OPERATOR '('");
        return TOKFALSE;
    }
    // Expecting STRING (file name)
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != STRING) {
        ERROR ("ParseMD()", "Expected a STRING, the filename of a file to process");
        return TOKFALSE;
    }
    strcpy (fn, pi->id);
    // Expecting ","
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ",")) {
        ERROR ("ParseMD()", "Expected OPERATOR ','");
        return TOKFALSE;
    }
    // Expecting STRING (file name)
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != STRING) {
        ERROR ("ParseMD()", "Expected a STRING, a parameter string like \"all\" or \"mode|owner|group\"");
        return TOKFALSE;
    }
    strcpy (param, pi->id);
    // Expecting ")"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ")")) {
        ERROR ("ParseMD()", "Expected OPERATOR ')'");
        return TOKFALSE;
    }
    // Expecting ";"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ";")) {
        ERROR ("ParseMD()", "Expected OPERATOR ';'");
        return TOKFALSE;
    }
    
    // Make sure the file is not in the exclude list.
    if (hsh != 0) {
        lnk->data = fn;
        cp = nsort_hash_find_item (hsh, (const char *)lnk->data);
        if (cp != 0)
            return TOKTRUE;
    }

    // Okay, we have what we need. Process it.
    ret = do_md (fn, param);
    if (ret == 0)
        return TOKTRUE;
    return TOKFALSE;
}
////////////////////////////////////////////////////////////////////////////////
// ParseMD()
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// ParseEX()
// Parse an exclude line.
////////////////////////////////////////////////////////////////////////////////

#define ERROR(f,t_str) printf ("***ERROR: Function %s: File %s, line %d, id = \"%s\" unexpected: \"%s\"\n", f, pi->tokenFile, pi->lnctr, pi->id, t_str)

static CDBTokenType ParseEX (parseItem *pi)
{
    CDBTokenType tkn = TOKTRUE;
    static char fn[PATH_MAX];
    int ret;

    // Expecting "("
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, "(")) {
        ERROR ("ParseEX()", "Expected OPERATOR '('");
        return TOKFALSE;
    }
    // Expecting STRING (file name)
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != STRING) {
        ERROR ("ParseEX()", "Expected a STRING, the filename of a file to process");
        return TOKFALSE;
    }
    strcpy (fn, pi->id);
    // Expecting ")"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ")")) {
        ERROR ("ParseEX()", "Expected OPERATOR ')'");
        return TOKFALSE;
    }
    // Expecting ";"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ";")) {
        ERROR ("ParseEX()", "Expected OPERATOR ';'");
        return TOKFALSE;
    }
    
    // Okay, we have what we need. Process it.
    ret = do_exclude_add (fn);
    // BUGBUG
    //printf ("HashInfo: %d items in hash\n", hsh->number);
    // BUGBUG
    if (ret == 0)
        return TOKTRUE;
    return TOKFALSE;
}
////////////////////////////////////////////////////////////////////////////////
// ParseEX ()
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// ParseDir()
// Parse and process an "mddir ..." line.
// Traverse the directory and process each item there.
////////////////////////////////////////////////////////////////////////////////
enum {
        WALK_OK = 0,
        WALK_BADPATTERN,
        WALK_NAMETOOLONG,
        WALK_BADIO,
};
 
#define WS_NONE         0
#define WS_RECURSIVE    (1 << 0)
#define WS_DEFAULT      WS_RECURSIVE
#define WS_FOLLOWLINK   (1 << 1)        /* follow symlinks */
#define WS_DOTFILES     (1 << 2)        /* per unix convention, .file is hidden */
#define WS_MATCHDIRS    (1 << 3)        /* if pattern is used on dir names too */
 
static int recursive_process (char *dname, regex_t *reg, char *param, int spec, int is_exclude)
{
        struct dirent *dent;
        DIR *dir;
        struct stat st;
        char fn[FILENAME_MAX];
        int res = WALK_OK;
        int len = strlen(dname);
        nsort_link_t *lnk = &hsh_link;
        char *this_cp;

        if (len >= FILENAME_MAX - 1)
                return WALK_NAMETOOLONG;
 
        strcpy(fn, dname);
        fn[len++] = '/';
 
        if (!(dir = opendir(dname))) {
                warn("can't open %s", dname);
                return WALK_BADIO;
        }
 
        errno = 0;
        while ((dent = readdir(dir))) {
//                if (!(spec & WS_DOTFILES) && dent->d_name[0] == '.')
//                        continue;
                if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
                        continue;
 
                strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
                if (lstat(fn, &st) == -1) {
                        warn("Can't stat %s", fn);
                        res = TOKFALSE;
                        continue;
                }
 
                /* don't follow symlink unless told so */
                if (S_ISLNK(st.st_mode) && !(spec & WS_FOLLOWLINK))
                        continue;
 
                /* will be false for symlinked dirs */
                if (S_ISDIR(st.st_mode)) {
                        /* recursively follow dirs */
                        if ((spec & WS_RECURSIVE))
                                recursive_process (fn, reg, param, spec, is_exclude);
 
                        if (!(spec & WS_MATCHDIRS)) continue;
                }
 
                /* pattern match */
                if (!regexec(reg, fn, 0, 0, 0)) {
                    // Make sure the file is not in the exclude list.
                    if (hsh != 0) {
                        lnk->data = fn;
                        this_cp = nsort_hash_find_item (hsh, (const char *)lnk->data);
                        if (this_cp != 0)
                            continue;
                    }
                    // Process it as a do_md() or as an exclude.
                    if (is_exclude == 0) {
                        res = do_md (fn, param);
                        if (res == -1) {
                            /* ignore this item but continue */
                            fprintf (stderr, "***Warning in recursive_process(), line %d: Unspecified do_md() error.\n", __LINE__);
                            continue;
                        }
                    }
                    else {
                        /* is_exclude is lit up - let's add these to the hash */
                        res = do_exclude_add (fn);
                        if (res == -1) {
                            /* ignore this item but continue */
                            fprintf (stderr, "***Warning in recursive_process(), line %d: Unspecified do_exclude_add() error.\n", __LINE__);
                            continue;
                        }
                    }
                }
        }
 
        if (dir) closedir(dir);
        // BUGBUG
        //if (is_exclude == 1) {
        //    printf ("HashInfo: %d items in hash\n", hsh->number);
        //}
        // BUGBUG

        if (errno)  {
            warn ("errno set");
            return TOKFALSE;
        }
        if (res) {
            warn ("res set");
            return TOKFALSE;
        }
        return TOKTRUE;
}
 
static int process_dir(char *dname, char *pattern, char *param, int spec, int is_exclude)
{
        regex_t r;
        int res;
        if (regcomp(&r, pattern, REG_EXTENDED | REG_NOSUB))
                return WALK_BADPATTERN;
        res = recursive_process (dname, &r, param, spec, is_exclude);
        regfree(&r);
 
        return res;
}

static CDBTokenType ParseDir (parseItem *pi, int is_exclude)
{
    CDBTokenType tkn = TOKTRUE;
    static char fn[PATH_MAX];
    static char param[PATH_MAX];
    int ret;

    // Expecting "("
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, "(")) {
        ERROR ("ParseDir()", "Expected OPERATOR '('");
        return TOKFALSE;
    }
    // Expecting STRING (file name)
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != STRING) {
        ERROR ("ParseDir()", "Expected a STRING, the directory to process");
        return TOKFALSE;
    }
    strcpy (fn, pi->id);
    if (is_exclude == 0) {
        // Expecting ","
        tkn = getToken (pi);
        if (tkn == DONE)
            return DONE;
        if (tkn != OPERATOR && strcmp (pi->id, ",")) {
            ERROR ("ParseDir()", "Expected OPERATOR ','");
            return TOKFALSE;
        }
        // Expecting STRING (mode)
        tkn = getToken (pi);
        if (tkn == DONE)
            return DONE;
        if (tkn != STRING) {
            ERROR ("ParseDir()", "Expected a STRING, a parameter string like \"all\" or \"default\" or \"mode|owner|group\"");
            return TOKFALSE;
        }
        strcpy (param, pi->id);
    }
    else {
        ret = do_exclude_add (fn);
        if (ret == -1) {
            /* ignore this item but continue */
            fprintf (stderr, "***Warning in ParseDir(), line %d: Couldn't add %s to exclude list.\n", __LINE__, fn);
        }
    }
    // Expecting ")"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ")")) {
        ERROR ("ParseDir()", "Expected OPERATOR ')'");
        return TOKFALSE;
    }
    // Expecting ";"
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn != OPERATOR && strcmp (pi->id, ";")) {
        ERROR ("ParseDir()", "Expected OPERATOR ';'");
        return TOKFALSE;
    }
    
    // Okay, we have what we need. Process it.
    ret = process_dir (fn, ".*", param, REG_EXTENDED | REG_NOSUB, is_exclude);
    if (ret == TOKTRUE)
        return TOKTRUE;
    return TOKFALSE;
}
////////////////////////////////////////////////////////////////////////////////
// ParseDir()
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ParseFile()
// Top level parser.
////////////////////////////////////////////////////////////////////////////////
static CDBTokenType ParseFile (parseItem *pi)
{
    CDBTokenType tkn;
    tkn = getToken (pi);
    if (tkn == DONE)
        return DONE;
    if (tkn == IDENT && ! strcmp (pi->id, "md"))
        return (ParseMD (pi));
    else if (tkn == IDENT && ! strcmp (pi->id, "ex"))
        return (ParseEX (pi));
    else if (tkn == IDENT && ! strcmp (pi->id, "mddir"))
        return (ParseDir (pi, 0));
    else if (tkn == IDENT && ! strcmp (pi->id, "exdir"))
        return (ParseDir (pi, 1));
    else if (tkn == IDENT && ! strcmp (pi->id, "hashinfo")) {
        tkn = getToken (pi);
        if (tkn != OPERATOR && strcmp (pi->id, ";")) {
            ERROR ("ParseDir()", "Expected OPERATOR ';'");
            tkn = TOKFALSE;
            goto ParseFileDone;
        }
        if (hsh == 0) {
            printf ("HashInfo: The hash is empty\n");
            tkn = DONE;
            goto ParseFileDone;
        }
        printf ("HashInfo: %d items in exclude hash\n", hsh->number);
    }
    else {
        tkn = TOKFALSE;
        goto ParseFileDone;
    }
ParseFileDone:
    return tkn;
}

    /********************************************************************************/
char *mdp_usage_msg = 
    "********************************************************************************\n"
    "Usage: mdp mdfile\n"
    "Where mdfile is a file which dictates how to process md items. An md item is a\n"
    "file or directory to process md(s) for. What md is processed is dependent on the\n"
    "parameter string and may include md5, sha1, sha256, sha512 or rmd160. The\n"
    "following are the possible commands you can use in an md file:\n"
    "********************************************************************************\n"
    "\n"
    "md() command\n"
    "md stands for \"message digest\". This command will get md information for one\n"
    "object (file or directory).\n"
    "\n"
    "md ( \"/file/spec\", \"parameter string\" );\n"
    "\n"
    "/file/spec is a file specification, like /bin/su, or /usr/bin/sudo.\n"
    "\"parameter string\" is a string which contains one of the following:\n"
    "\"default\": same as \"mode|owner|group|size|dt|md5|sha256\"\n"
    "\"all\": same as \"mode|owner|group|size|dt|md5|sha|sha256|sha512\"\n"
    "or any combination of mode, owner, group, size, dt, md5, sha, sha256, or sha512.\n"
    "\n"
    "mode - a permissions string, like -rwxr-xr-x, that you might see with 'ls -l'\n"
    "owner - the owner of each object (file or directory)\n"
    "group - the group for each object (file or directory)\n"
    "size - the size in bytes of the object (file or directory)\n"
    "dt - the date the objects (file or directory) was last touched\n"
    "md5 - md5 sum of the object\n"
    "sha - sha1 sum of the object\n"
    "sha256 - sha256 sum of the object\n"
    "sha512 - sha512 sum of the object\n"
    "rmd - rmd160 sum of the object\n"
    "\n"
    "mddir() command\n"
    "mddir stands for \"message digest of a directory\". The mddir() command causes\n"
    "the directory specified by /dir/spec to be recursively traversed. The command\n"
    "looks like the following:\n"
    "\n"
    "mddir ( \"/dir/spec\", \"parameter string\" );\n"
    "\n"
    "/dir/spec is a directory which should be traversed for each file and directory in\n"
    "the tree.\n"
    "\n"
    "ex() command\n"
    "ex stands for exclude. The ex command allows you to create a hash of file and directory\n"
    "names to exclude from processing. Say, for example, you want to traverse a directory\n"
    "and you want to exclude a particular file because it is huge. You could include the\n"
    "full path of the file in an ex() command to exclude it.\n"
    "The ex() and exdir() commands cause a hash to be built which then works to exclude\n"
    "the specified files throughout processing.\n"
    "\n"
    "ex ( \"/file/or/dir/spec\");\n"
    "\n"
    "exdir () command\n"
    "The exdir() command is similar to the ex() command, except that the spec provided is a\n"
    "file spec, not a directory spec.\n"
    "\n"
    "exdir ( \"/dir/spec\");\n"
    "\n"
    "hashinfo command\n"
    "The hashinfo command prints the number of items in the exclude hash, if one exists.\n"
    "\n"
    "hashinfo;\n"
    "\n"
    "\n"
    "Exambles\n"
    "// Exclude the file /sbin/dmsetup\n"
    "ex (\"/sbin/dmsetup\");\n"
    "// exclude the /etc/firefox directory\n"
    "exdir (\"/etc/firefox\");\n"
    "// exclude the /usr/src directory\n"
    "exdir (\"/usr/src\");\n"
    "// exclude the /usr/share directory\n"
    "exdir (\"/usr/share\");\n"
    "// exclude the /usr/include directory\n"
    "exdir (\"/usr/include\");\n"
    "// exclude the /usr/NX directory\n"
    "exdir (\"/usr/NX\");\n"
    "// show how many items are in the excludes hash\n"
    "hashinfo;\n"
    "// traverse every file in /usr and process the default md sums\n"
    "mddir (\"/usr\", \"default\");\n"
    "// traverse every file in /sbin and process the default md sums\n"
    "mddir (\"/sbin\", \"default\");\n"
    "// get the md5, sha256 and sha512 sums for /bin/su\n"
    "md ( \"/bin/su\", \"md5|sha256|sha512\" );\n"
    "// get the rmd160 sum of /usr/bin/sudo\n"
    "md ( \"/usr/bin/sudo\", \"rmd\" );\n"

    "The following switches are honored on the mdp command-line:\n"
    "\n"
    "-c 'command' - execute a command\n"
    "This command saves the argument provided to the -c switch to a file and runs that.\n"
    "\n"
    "-l 'filename' - treat the file 'filename' as containing a list of files to process with\n"
    "a parameter string of \"default\".\n"
    "This command saves the argument provided to the -c switch to a file and runs that.\n"
    "\n"
    "-e - print errors only\n"
    "This muffles the output of mdp to only include errors. It is a troubleshooting capability.\n"
    "\n"
    "-q - quiet\n"
    "This *only* provides the message digest lines for files and directories.\n"
    "\n"
    "-d - skip directories\n"
    "This switch will skip output of directory information.\n"
    "\n"
    "-s - sorted\n"
    "This switch will cause the output to be sorted by the file/directory specification.\n"
    "\n"
    "-h - help\n"
    "This switch will cause the usage message (what you are reading now) to be printed on the\n"
    "screen.\n"
    "********************************************************************************\n"
    ;
void usage (void)
{
    printf ("%s\n", mdp_usage_msg);
    return;
}

int main (int argc, char *argv[])
{
    int opt;
    parseItem *pi;
    CDBTokenType tkn = TOKTRUE;
    nsort_link_t *lnk;
    char fltmplt[PATH_MAX];
    char ln[PATH_MAX];
    char param[PATH_MAX];
    char *cp;
    FILE *fp;
    int len;

    while ( (opt = getopt (argc, argv, "dehqc:l:s")) != -1 ) {
        switch (opt) {
            case 'e':
                print_errors_only = 1;
                quiet = 0;
                break;
            case 'q':
                quiet = 1;
                print_errors_only = 0;
                break;
            case 'd':
                skip_directories = 1;
                break;
            case 'h':
                usage ();
                return 0;
            case 's':
                sorted = 1;
                break;
            case 'c':
                strcpy (fltmplt, "MDPXXXXXX");
                mkstemp (fltmplt);
                fp = fopen (fltmplt, "w");
                if (fp == 0) {
                    fprintf (stderr, "*** Error in main(), line %d: Could not open temporary file:", __LINE__);
                    err (1, "%s", fltmplt);
                    return -1;
                }
                // Write optarg to the file.
                len = strlen (optarg);
                if (len == 0) {
                    fprintf (stderr, "*** Error in main(), line %d: No argument for '-c' switch\n", __LINE__);
                    return -1;
                }
                fwrite (optarg, len, 1, fp);
                fclose (fp);
                pi = initParser (fltmplt);
                if (pi == 0) {
                    if (pi == 0) {
                            fprintf (stderr, "\n\n***Error: couldn't create parser, %s\n",
                                    parseErrorString[parseError]);
                        return -1;
                    }
                }
                len = remove (fltmplt);
                if (len != 0) {
                    fprintf (stderr, "*** Error in main(), line %d: Could not remove temporary file:", __LINE__);
                    err (1, "%s", fltmplt);
                    return -1;
                }
                while (tkn != DONE) {
                    tkn = ParseFile (pi);
                    if (tkn == DONE)
                        goto mdpDone;
                    if (tkn == TOKFALSE)
                        goto mdpDone;
                }
                break;
            case 'l':
                fp = fopen (optarg, "r");
                if (fp == 0) {
                    fprintf (stderr, "*** Error in main(), line %d: Could not open file:", __LINE__);
                    err (1, "%s", optarg);
                    return -1;
                }
                while (1 == 1) {
                    if (fgets (ln, PATH_MAX-1, fp) == 0) {
                        fclose (fp);
                        return 0;
                    }
                    // support # comments at start of line.
                    if (ln[0] == '#')
                        continue;
                    cp = strchr (ln, '\n');
                    if (cp != 0)
                        *cp = '\0';
                    strcpy (param, "default");
                    len = do_md (ln, param);
                    if (len == -1) {
                        fprintf (stderr, "***Error in main(), line %d: do_md() error for file \"%s\"\n", __LINE__, ln);
                        fclose (fp);
                        return -1;
                    }
                }
                break;
            default:
                fprintf (stderr, "*** Usage: [ -e ] [ -q ] [ -v ] md_file\n");
                fprintf (stderr, "\t -e = print errors only\n");
                fprintf (stderr, "\t -q = quiet = print hard errors, md outputs of md_file parsing and processing only\n");
                fprintf (stderr, "\t -d = don't process directories\n");
                fprintf (stderr, "\t md_file is the file to parse and process.\n");
                return -1;
        }
    }
    if (optind == 0 || argv[optind] == 0) {
        fprintf (stderr, "*** Usage: [ -e ] [ -q ] [ -v ] md_file\n");
        fprintf (stderr, "\t -e = print errors only\n");
        fprintf (stderr, "\t -q = quiet = print hard errors, md outputs of md_file parsing and processing only\n");
        fprintf (stderr, "\t md_file is the file to parse and process.\n");
        return 0;
    }
    pi = initParser (argv[optind]);
    if (pi == 0) {
            printf ("\n\n***Error: couldn't create parser, %s\n",
                    parseErrorString[parseError]);
        return -1;
    }
    while (tkn != DONE) {
        tkn = ParseFile (pi);
        if (tkn == DONE)
            goto mdpDone;
        if (tkn == TOKFALSE)
            goto mdpDone;
    }

mdpDone:

    if (tkn == TOKFALSE) {
        ERROR ("main()", "Parse error");
    }
    if (tkn == DONE) {
        if (! quiet) {
            printf ("*** Parsed to line %d\n", pi->lnctr);
        }
    }
    if (hsh != 0) {
        nsort_hash_del (hsh);
        nsort_hash_destroy (hsh);
    }
    if (srt != 0 && srt->lh->head->next != srt->lh->tail) {
        // print the output and remove the item.
        lnk = srt->lh->head->next;
        while (lnk != srt->lh->tail) {
            printf ("%s", (char *)lnk->data);
            lnk = lnk->next;
        }
        nsort_del (srt, 0);
        nsort_destroy (srt);
    }
    deleteParser (pi);
    return 0;
}

