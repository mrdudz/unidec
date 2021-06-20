
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFILELENGTH   0x10000

char *infilename = NULL, *outfilename = NULL;
unsigned char inbuf[MAXFILELENGTH];
unsigned char outbuf[MAXFILELENGTH];

int verbose = 0;

#include "decodefunc.c"
#include "filefunc.c"

static void usage(void)
{
    printf("unidec - swap address- and databits in a binary dump\n"
           "usage: unidec <options> -i <input file> -o <output file>\n"
           "-a <old> <new>  swap address line (port->eprom)\n"
           "-d <old> <new>  swap data line (port->eprom)\n"
           "-i <file>       name input file\n"
           "-o <file>       name output file\n"
           "--verbose       increase verbose level\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    int i, oldline, newline;

    decodeinit();

    // process commandline
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-a")) {
            newline = strtoul(argv[i+1], NULL, 0);
            oldline = strtoul(argv[i+2], NULL, 0);
            if (verbose > 1) { printf("addr port %2d -> eprom %2d\n", newline, oldline); }
            addrshift[oldline] = newline;
            i += 2;
        } else if (!strcmp(argv[i], "-d")) {
            newline = strtoul(argv[i+1], NULL, 0);
            oldline = strtoul(argv[i+2], NULL, 0);
            if (verbose > 1) { printf("data port %2d -> eprom %2d\n", newline, oldline); }
            datashift[oldline] = newline;
            i += 2;
        } else if (!strcmp(argv[i], "-i")) {
            infilename = argv[i+1];
            i += 1;
        } else if (!strcmp(argv[i], "-o")) {
            outfilename = argv[i+1];
            i += 1;
        } else if (!strcmp(argv[i], "--verbose")) {
            verbose++;
        } else {
            usage();
        }
    }

    if (infilename == NULL) {
        fprintf(stderr, "error: input filename missing.\n");
        exit(-1);
    }
    if (outfilename == NULL) {
        fprintf(stderr, "error: output filename missing.\n");
        exit(-1);
    }

    loadinputfile(inbuf, infilename);

    updatereverselookup();

    if (verbose) {
        printf("filelength: %04lx\n", filelength);
        printf("addresslines: %d\n", addrlines);

        printf("port:  ");
        for (i = 0; i < addrlines; i++) {
            printf("%2d ", i);
        }
        printf("\neprom: ");
        for (i = 0; i < addrlines; i++) {
            printf("%2d ", addrshiftreverse[i]);
        }
        printf("\ndatalines: %d\nport:  ", datalines);
        for (i = 0; i < datalines; i++) {
            printf("%2d ", i);
        }
        printf("\neprom: ");
        for (i = 0; i < datalines; i++) {
            printf("%2d ", datashiftreverse[i]);
        }
        printf("\n");
    }

    decode(outbuf, inbuf, filelength);

    saveoutputfile(outbuf, outfilename);
}
