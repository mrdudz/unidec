#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MAXFILELENGTH   (0x10000 * 1024) // 1Mb

#define MAXADDRLINES    16
#define MAXDATALINES     8

#define WINDOWWIDTH     1300
#define WINDOWHEIGHT    750

#define ADDRLABELYPOS   600
#define ADDRCOMBOYPOS   620
#define DATALABELYPOS   660
#define DATACOMBOYPOS   680

#define BOTTOMHEIGHT    ((WINDOWHEIGHT)-(ADDRLABELYPOS))
#define TEXTBOXHEIGHT   ((WINDOWHEIGHT)-(BOTTOMHEIGHT))


GtkWidget *combobox_addrline[MAXADDRLINES];
GtkWidget *combobox_dataline[MAXADDRLINES];
gulong combobox_dataline_signal_id[MAXDATALINES];
gulong combobox_addrline_signal_id[MAXADDRLINES];

GtkWidget *scrolled;
GtkWidget *textbox;

unsigned char rawbuffer[MAXFILELENGTH];
unsigned char decodedbuffer[MAXFILELENGTH];

unsigned char hexdumpbuffer[MAXFILELENGTH*5];

#define PREVIEW_CSS \
    "label {\n" \
    "    font-family: \"Monospace\";\n" \
    "    background-color: black;\n" \
    "    color: limegreen;\n" \
    "}\n"

/** \brief  Create a new CSS provider and set it to \a css
 *
 * Use this function if you need a CSS provider you'll apply multiple times,
 * if you need to apply CSS to a widget only once, there's a wrapper function
 * #gtk3_css_add().
 *
 * \param[in]   css CSS string
 *
 * \return  new provider or NULL on error
 */
GtkCssProvider *gtk3_css_provider_new(const char *css)
{
    GtkCssProvider *provider;
    GError *err = NULL;

    /* instanciate CSS provider */
    provider = gtk_css_provider_new();
    /* attempt to load CSS from string */
    gtk_css_provider_load_from_data(provider, css, -1, &err);
    if (err != NULL) {
        fprintf(stderr, "CSS error: %s\n", err->message);
        g_error_free(err);
        return NULL;
    }
    return provider;
}


/** \brief  Add CSS \a provider to \a widget
 *
 * \param[in,out]   widget      widget to add \a provider to
 * \param[in]       provider    CSS provider
 *
 * \return  bool
 */
gboolean gtk3_css_provider_add(GtkWidget *widget,
                                    GtkCssProvider *provider)
{
    GtkStyleContext *context;

    /* try to get style context */
    context = gtk_widget_get_style_context(widget);
    if (context == NULL) {
        fprintf(stderr, "CSS error\n");
        /* don't free the context, it's owned by the widget */
        return FALSE;
    }

    /* add provider */
    gtk_style_context_add_provider(context,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
    return TRUE;
}


/** \brief  Remove \a provider from \a widget
 *
 * \param[in,out]   widget      gtk widget
 * \param[in]       provider    gtk css provider to remove
 *
 * \return  gboolean
 */
gboolean gtk3_css_provider_remove(GtkWidget *widget,
                                       GtkCssProvider *provider)
{
    GtkStyleContext *context;

    /* try to get style context */
    context = gtk_widget_get_style_context(widget);
    if (context == NULL) {
        fprintf(stderr, "Couldn't get style context of widget\n");
        /* don't free the context, it's owned by the widget */
        return FALSE;
    }

    gtk_style_context_remove_provider(context, GTK_STYLE_PROVIDER(provider));
    return TRUE;
}



/** \brief  Add CSS string \a css to \a widget
 *
 * Only use this when adding CSS to a single widget, if you need to add the
 * same CSS to multiple widgets use #gtk3_css_provider_new() to create
 * a CSS provider once and #gtk3_css_provider_add() to add it multiple
 * times.
 *
 * \param[in,out]   widget  widget to add CSS to
 * \param[in]       css     CSS string
 *
 * \return  bool
 */
gboolean gtk3_css_add(GtkWidget *widget, const char *css)
{
    GtkCssProvider *provider;

    provider = gtk3_css_provider_new(css);
    if (provider != NULL) {
        return gtk3_css_provider_add(widget, provider);
    }
    return FALSE;
}


char *infilename = NULL, *outfilename = NULL;
int verbose = 0;

#include "decodefunc.c"
#include "filefunc.c"

static void destroy(GtkWidget *widget, gpointer data)
{
    printf("saving '%s'\n", outfilename);
    saveoutputfile(decodedbuffer, outfilename);
    gtk_main_quit();
}

static char *scrtab =
    "@abcdefghijklmno" // 00
    "pqrstuvwxyz[.]^_" // 10
    " !\"#$%&'()*+,-./" // 20
    "0123456789:;<=>?" // 30
    ".ABCDEFGHIJKLMNO" // 40
    "PQRSTUVWXYZ....." // 50
    "................"
    "................"
    "................"
    "................"
    "................"
    "................"
    "................"
    "................"
    "................"
    "................";

static char *pettab =
    "................"  // 00
    "................"  // 10
    " !\"#$%&'()*+,-./" // 20
    "0123456789:;<=>?" // 30
    "@abcdefghijklmno" // 40
    "pqrstuvwxyz[.]^_" // 50
    ".ABCDEFGHIJKLMNO" // 60
    "PQRSTUVWXYZ....." // 70
    "................"
    "................"
    "................"
    "................"
    ".ABCDEFGHIJKLMNO" // C0
    "PQRSTUVWXYZ....." // D0
    "................"
    "................";

static void update_hexdump(void)
{
    int i, n = 0;
    char *txt = hexdumpbuffer;
    for (i = 0; i < filelength; i += 0x20) {
        txt += sprintf(txt, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "
                            "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "
                            " %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
                            "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
                            " %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
                            "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n"
                       ,i,
                       decodedbuffer[i], decodedbuffer[i+1], decodedbuffer[i+2], decodedbuffer[i+3],
                       decodedbuffer[i+4], decodedbuffer[i+5], decodedbuffer[i+6], decodedbuffer[i+7],
                       decodedbuffer[i+8], decodedbuffer[i+9], decodedbuffer[i+10], decodedbuffer[i+11],
                       decodedbuffer[i+12], decodedbuffer[i+13], decodedbuffer[i+14], decodedbuffer[i+15],
                       decodedbuffer[i+16], decodedbuffer[i+17], decodedbuffer[i+18], decodedbuffer[i+19],
                       decodedbuffer[i+20], decodedbuffer[i+21], decodedbuffer[i+22], decodedbuffer[i+23],
                       decodedbuffer[i+24], decodedbuffer[i+25], decodedbuffer[i+26], decodedbuffer[i+27],
                       decodedbuffer[i+28], decodedbuffer[i+29], decodedbuffer[i+30], decodedbuffer[i+31],
                       pettab[decodedbuffer[i]], pettab[decodedbuffer[i+1]], pettab[decodedbuffer[i+2]], pettab[decodedbuffer[i+3]],
                       pettab[decodedbuffer[i+4]], pettab[decodedbuffer[i+5]], pettab[decodedbuffer[i+6]], pettab[decodedbuffer[i+7]],
                       pettab[decodedbuffer[i+8]], pettab[decodedbuffer[i+9]], pettab[decodedbuffer[i+10]], pettab[decodedbuffer[i+11]],
                       pettab[decodedbuffer[i+12]], pettab[decodedbuffer[i+13]], pettab[decodedbuffer[i+14]], pettab[decodedbuffer[i+15]],
                       pettab[decodedbuffer[i+16]], pettab[decodedbuffer[i+17]], pettab[decodedbuffer[i+18]], pettab[decodedbuffer[i+19]],
                       pettab[decodedbuffer[i+20]], pettab[decodedbuffer[i+21]], pettab[decodedbuffer[i+22]], pettab[decodedbuffer[i+23]],
                       pettab[decodedbuffer[i+24]], pettab[decodedbuffer[i+25]], pettab[decodedbuffer[i+26]], pettab[decodedbuffer[i+27]],
                       pettab[decodedbuffer[i+28]], pettab[decodedbuffer[i+29]], pettab[decodedbuffer[i+30]], pettab[decodedbuffer[i+31]],
                       scrtab[decodedbuffer[i]], scrtab[decodedbuffer[i+1]], scrtab[decodedbuffer[i+2]], scrtab[decodedbuffer[i+3]],
                       scrtab[decodedbuffer[i+4]], scrtab[decodedbuffer[i+5]], scrtab[decodedbuffer[i+6]], scrtab[decodedbuffer[i+7]],
                       scrtab[decodedbuffer[i+8]], scrtab[decodedbuffer[i+9]], scrtab[decodedbuffer[i+10]], scrtab[decodedbuffer[i+11]],
                       scrtab[decodedbuffer[i+12]], scrtab[decodedbuffer[i+13]], scrtab[decodedbuffer[i+14]], scrtab[decodedbuffer[i+15]],
                       scrtab[decodedbuffer[i+16]], scrtab[decodedbuffer[i+17]], scrtab[decodedbuffer[i+18]], scrtab[decodedbuffer[i+19]],
                       scrtab[decodedbuffer[i+20]], scrtab[decodedbuffer[i+21]], scrtab[decodedbuffer[i+22]], scrtab[decodedbuffer[i+23]],
                       scrtab[decodedbuffer[i+24]], scrtab[decodedbuffer[i+25]], scrtab[decodedbuffer[i+26]], scrtab[decodedbuffer[i+27]],
                       scrtab[decodedbuffer[i+28]], scrtab[decodedbuffer[i+29]], scrtab[decodedbuffer[i+30]], scrtab[decodedbuffer[i+31]]
               );
    }
//    gtk_text_buffer_set_text (textbuffer, hexdumpbuffer, -1);
    gtk_label_set_text(GTK_LABEL(textbox), hexdumpbuffer);
}

static void update_addr_comboboxes(void)
{
    int i;
    for (i = 0; i < MAXADDRLINES; i++) {
        unsigned char combotext[5];
        // we need to block the signal handler so we dont trigger ours
        g_signal_handler_block (combobox_addrline[i], combobox_addrline_signal_id[i]);
        sprintf(combotext, "%d", addrshift[i]);
        gtk_combo_box_set_active_id (GTK_COMBO_BOX(combobox_addrline[i]), combotext);
        // unblock signal handler
        g_signal_handler_unblock (combobox_addrline[i], combobox_addrline_signal_id[i]);
    }
}

static void update_data_comboboxes(void)
{
    int i;
    for (i = 0; i < MAXDATALINES; i++) {
        unsigned char combotext[5];
        // we need to block the signal handler so we dont trigger ours
        g_signal_handler_block (combobox_dataline[i], combobox_dataline_signal_id[i]);
        sprintf(combotext, "%d", datashift[i]);
        gtk_combo_box_set_active_id (GTK_COMBO_BOX(combobox_dataline[i]), combotext);
        // unblock signal handler
        g_signal_handler_unblock (combobox_dataline[i], combobox_dataline_signal_id[i]);
    }
}

static int lastaddrline = -1;
static unsigned int lastaddrshift[MAXADDRLINES];

static void addrline_changed(GtkWidget *widget, gpointer data)
{
    ptrdiff_t line = (ptrdiff_t)data;
    const gchar *valstr = gtk_combo_box_get_active_id (GTK_COMBO_BOX(widget));
    unsigned long val, oldval;
    val = strtoul(valstr, NULL, 0);

    if (lastaddrline != line) {
        printf("new addrline %ld\n", line);
        memcpy(&lastaddrshift[0], &addrshift[0], MAXADDRLINES * sizeof(int));
        lastaddrline = line;
    } else {
        printf("same addrline %ld\n", line);
        memcpy(&addrshift[0], &lastaddrshift[0], MAXADDRLINES * sizeof(int));
    }
    updatereverselookup();
    dumpaddrlines();

    oldval = addrshift[line];
    addrshift[line] = val;
    printf("addr line %ld to %ld, line %d to %ld\n", line, val, addrshiftreverse[val], oldval);
    addrshift[addrshiftreverse[val]] = oldval;

    updatereverselookup();
    dumpaddrlines();

    update_addr_comboboxes();
    decode(decodedbuffer, rawbuffer, filelength);
    update_hexdump();
}

static int lastdataline = -1;
static unsigned int lastdatashift[MAXDATALINES];

static void dataline_changed(GtkWidget *widget, gpointer data)
{
    ptrdiff_t line = (ptrdiff_t)data;
    const gchar *valstr = gtk_combo_box_get_active_id (GTK_COMBO_BOX(widget));
    unsigned long val, oldval;
    val = strtoul(valstr, NULL, 0);

    if (lastdataline != line) {
        printf("new dataline %ld\n", line);
        memcpy(&lastdatashift[0], &datashift[0], MAXDATALINES * sizeof(int));
        lastdataline = line;
    } else {
        printf("same dataline %ld\n", line);
        memcpy(&datashift[0], &lastdatashift[0], MAXDATALINES * sizeof(int));
    }
    updatereverselookup();
    dumpdatalines();

    oldval = datashift[line];
    datashift[line] = val;
    printf("data line %ld to %ld, line %d to %ld\n", line, val, datashiftreverse[val], oldval);
    datashift[datashiftreverse[val]] = oldval;

    updatereverselookup();
    dumpdatalines();

    update_data_comboboxes();
    decode(decodedbuffer, rawbuffer, filelength);
    update_hexdump();
}

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

    loadinputfile(rawbuffer, infilename);
    updatereverselookup();
    decode(decodedbuffer, rawbuffer, filelength);

    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK unidec");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

    GtkWidget *k;
    k= gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), k);

    // create the textbox
    {
        textbox = gtk_label_new(NULL);
        gtk_label_set_single_line_mode(GTK_LABEL(textbox), FALSE);        
        gtk3_css_add(textbox, PREVIEW_CSS);

        scrolled = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_set_size_request(scrolled, WINDOWWIDTH, TEXTBOXHEIGHT);
        gtk_container_add(GTK_CONTAINER(scrolled), textbox);

        gtk_fixed_put (GTK_FIXED (k), scrolled, 0, 0);

        update_hexdump();
    }

    // create combo boxes for the address lines
    {
        int i = 0, n = 0;
        GtkWidget *label;
        label = gtk_label_new("Bus:");
        gtk_fixed_put (GTK_FIXED (k), label, 10, ADDRLABELYPOS);
        label = gtk_label_new("Chip:");
        gtk_fixed_put (GTK_FIXED (k), label, 10, ADDRCOMBOYPOS);

        for (i = 0; i < MAXADDRLINES; i++) {
            unsigned char combotext[5];
            sprintf(combotext, "A%d", i);
            label = gtk_label_new(combotext);
            gtk_fixed_put (GTK_FIXED (k), label, 15 + (MAXADDRLINES - i) * 60, ADDRLABELYPOS);

            combobox_addrline[i] = gtk_combo_box_text_new();
            gtk_fixed_put (GTK_FIXED (k), combobox_addrline[i], (MAXADDRLINES - i) * 60, ADDRCOMBOYPOS);
            gtk_widget_set_size_request(combobox_addrline[i], 10, 10);
            for (n = 0; n < MAXADDRLINES; n++) {
                sprintf(combotext, "%d", n);
                if ((addrlines <= i) || (n < addrlines)) {
                    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_addrline[i]), combotext, combotext);
                }
            }
            sprintf(combotext, "%d", addrshift[i]);
            gtk_combo_box_set_active_id (GTK_COMBO_BOX(combobox_addrline[i]), combotext);
            combobox_addrline_signal_id[i] = g_signal_connect(combobox_addrline[i], "changed", G_CALLBACK(addrline_changed), (gpointer)(ptrdiff_t)i);
            if (addrlines <= i) {
                gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(combobox_addrline[i]), GTK_SENSITIVITY_OFF);
            }
        }
    }

    // create combo boxes for the data lines
    {
        int i = 0, n = 0;
        GtkWidget *label;
        label = gtk_label_new("Bus:");
        gtk_fixed_put (GTK_FIXED (k), label, 10, DATALABELYPOS);
        label = gtk_label_new("Chip:");
        gtk_fixed_put (GTK_FIXED (k), label, 10, DATACOMBOYPOS);

        for (i = 0; i < MAXDATALINES; i++) {
            unsigned char combotext[5];
            sprintf(combotext, "D%d", i);
            label = gtk_label_new(combotext);
            gtk_fixed_put (GTK_FIXED (k), label, 15 + (MAXDATALINES - i) * 60, DATALABELYPOS);

            combobox_dataline[i] = gtk_combo_box_text_new();
            gtk_fixed_put (GTK_FIXED (k), combobox_dataline[i], (MAXDATALINES - i) * 60, DATACOMBOYPOS);
            gtk_widget_set_size_request(combobox_dataline[i], 10, 10);
            for (n = 0; n < MAXDATALINES; n++) {
                sprintf(combotext, "%d", n);
                gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combobox_dataline[i]), combotext, combotext);
            }
            sprintf(combotext, "%d", datashift[i]);
            gtk_combo_box_set_active_id (GTK_COMBO_BOX(combobox_dataline[i]), combotext);
            combobox_dataline_signal_id[i] = g_signal_connect(combobox_dataline[i], "changed", G_CALLBACK(dataline_changed), (gpointer)(ptrdiff_t)i);
        }
    }

    gtk_widget_set_size_request(GTK_WIDGET(window),WINDOWWIDTH, WINDOWHEIGHT);
    gtk_widget_show_all(GTK_WIDGET(window));

    gtk_main();
    return 0;
}
