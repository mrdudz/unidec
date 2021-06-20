
size_t filelength = 0;

int loadinputfile(unsigned char *inbuf, char *filename)
{
    FILE *in;

    in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "error: could not open '%s' for reading.\n", filename);
        exit(-1);
    }
    filelength = fread(inbuf, 1, MAXFILELENGTH, in);
    fclose(in);

    addrlines = getnumbits(filelength-1);
    if ((1 << addrlines) != filelength) {
        fprintf(stderr, "error: input file length should be a power of two.\n");
    }
} 

int saveoutputfile(unsigned char *outbuf, char *filename)
{
    FILE *out;
    out = fopen(outfilename, "wb");
    if (out == NULL) {
        fprintf(stderr, "error: could not open '%s' for writing.\n", outfilename);
        exit(-1);
    }
    fwrite(outbuf, 1, filelength, out);
    fclose(out);
}
