unsigned int addrshift[32];
unsigned int addrshiftreverse[32];
unsigned int datashift[8];
unsigned int datashiftreverse[8];
int addrlines = 0;
int datalines = 8;

static void decodeinit(void)
{
    int i;
    // init the mapping
    for (i = 0; i < 8; i++) {
        datashift[i] = i;
    }
    for (i = 0; i < 32; i++) {
        addrshift[i] = i;
    }
}

static void updatereverselookup(void)
{
    int i;
    for (i = 0; i < addrlines; i++) {
        addrshiftreverse[addrshift[i]] = i;
    }
    for (i = 0; i < datalines; i++) {
        datashiftreverse[datashift[i]] = i;
    }
}

static unsigned char makedata(unsigned int data)
{
    unsigned int outdata = 0;
    int i;
    for (i = 0; i < 8; i++) {
        outdata |= ((data & (1 << i)) >> i) << datashift[i];
    }
    return outdata;
}

static unsigned int makeaddr(unsigned int addr)
{
    unsigned int outaddr = 0;
    int i;
    for (i = 0; i < addrlines; i++) {
        outaddr |= ((addr & (1 << i)) >> i) << addrshift[i];
    }
    return outaddr;
}

static int decode(unsigned char *out, unsigned char *in, unsigned int len)
{
    unsigned int addr;
    unsigned char data;
    int i;

    for (i = 0; i < len; i++) {
        data = makedata(in[i]);
        addr = makeaddr(i);
        out[addr] = data;
    }
}

static unsigned int getnumbits(unsigned int num)
{
    unsigned int test = 1;
    int count = 1;
    while ((num & test) != num) {
        test = (test << 1) | 1;
        count++;
    }
    return count;
}

static int dumpdatalines(void)
{
    int i;
    printf("datalines: ");
    for (i = 0; i < datalines; i++) {
        printf("%02x ", datashift[i]);
    }
    printf("\n");
    printf("reverse:   ");
    for (i = 0; i < datalines; i++) {
        printf("%02x ", datashiftreverse[i]);
    }
    printf("\n");
}

static int dumpaddrlines(void)
{
    int i;
    printf("addrlines: ");
    for (i = 0; i < addrlines; i++) {
        printf("%02x ", addrshift[i]);
    }
    printf("\n");
    printf("reverse:   ");
    for (i = 0; i < addrlines; i++) {
        printf("%02x ", addrshiftreverse[i]);
    }
    printf("\n");
}
