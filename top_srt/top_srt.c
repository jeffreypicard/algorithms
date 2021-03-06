/*
 * top_srt.c
 *
 * Academic implementation of topological sort on a graph.
 *
 * Author: Jeffrey Picard (jeff@jeffreypicard.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Data structures
 */

struct node {
    int num_deps;
    struct node **deps;
    int v; /* visited */
    char *data;
};

/*
 * Utility functions
 */

/*
 * Graph utility functions
 *
 * FIXME: Move to separate file
 */
void
print_graph(struct node **l, int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        if (l[i]->data) {
            fprintf(stderr, "%s (%p), ", l[i]->data, l[i]);
            fprintf(stderr, "%d\n", l[i]->num_deps);
            for (j = 0; j < l[i]->num_deps; j++) {
                fprintf(stderr, "%s (%p) ", l[i]->deps[j]->data, l[i]->deps[j]);
            }
            fprintf(stderr, "\n");
        } else {
            fprintf(stderr, "(null)\n");
        }
    }
}

/*
 * find_node
 *
 * FIXME: slow
 */
struct node **
find_node(struct node **l, int n, char *needle)
{
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(l[i]->data, needle) == 0) return &l[i];
    }

    return 0;
}

/*
 * resize
 *
 * Resize the buffer array by doubling.
 *
 * FIXME: Implement.
 */
char *
resize(char *x)
{
    return x;
}

/*
 * readline
 *
 * Role your own for pwnage.
 * FIXME: Rolling your own readline is stupid, bug prone and inefficient.
 */
char *
readline(FILE *f)
{
    char *buf;
    int i;
    char c;
    /* FIXME: *NEVER* use magic constants. */
    int buf_siz = 256;

    if (!f) return 0;

    buf = calloc(buf_siz, sizeof *buf);   
    if (!buf) return 0;

    /* INIT! */
    i = 0;

    while ((c = fgetc(f)) != EOF) {
        buf[i] = c;
        /* fprintf(stderr, "'%c'\n", c); */
        i++;
        if (c == '\n') break;
        if (i >= buf_siz) {
            /* FIXME: resize and stuff */
        }
    }
    
    if (i > 0) {
        return buf;
    } else {
        /* 
         * Only happens if we get an EOF and no chars in buffer, so free and
         * return null
         */
        free(buf);
        return 0;
    }
}

/*
 * parse
 *
 * Parse input file describing the graph
 * 
 * FIXME: The *** seems weird, maybe rethink this
 * Also have an error return value
 */
void
parse(FILE *f, struct node ***out_l, int *out_n)
{
    struct node **l;
    /* FIXME can be done with one buf */
    char *buf;
    char buf2[256];
    int n, m, o;
    int num_nodes;
    int i, j, k;

    buf = readline(f);
    if (!buf) goto fail; 

    sscanf(buf, "%d", &num_nodes);
    /* fprintf(stderr, "num_nodes: %d\n", num_nodes); */
    free(buf);

    l = calloc(num_nodes, sizeof *l);
    if (!l) goto fail; 
    for (i = 0; i < num_nodes; i++) {
        l[i] = calloc(1, sizeof **l);
        if (!l[i]) goto fail; 
    }

    buf = readline(f);
    /* fprintf(stderr, "buf: %s\n", buf); */
    if (!buf) goto fail; 

    /* FIXME: assume stuff about readline */
    m = strlen(buf);
    
    i = 0;
    j = 0;
    while (i < num_nodes && j < m) {
        /* zero the buffer */
        memset(buf2, 0, 256);

        o = sscanf(buf + j, "%s ", buf2);
        j += o + 1; /* +1 for the space */
        l[i]->data = calloc(o + 1, sizeof(char));
        if (!l[i]->data) goto fail;
        memcpy(l[i]->data, buf2, o);

        i++;
    }

    /* print_graph(l, num_nodes); */

    i = 0;
    while ((buf = readline(f))) {
        struct node **links;
        int num_links;

        m = strlen(buf);

        j = sscanf(buf, "%d ", &num_links);

        l[i]->deps = calloc(num_links, sizeof(struct node *));
        if (!l[i]->deps) goto fail;

        l[i]->num_deps = num_links;

        /* print_graph(l, num_nodes); */

        /*
         * Fill in the adj matrix with pointers to the associated nodes
         */
        k = 0;
        while (j < m && k < num_links) {
            /* zero the buffer */
            memset(buf2, 0, 256);

            o = sscanf(buf + j, "%s ", buf2);
            j += o + 1; /* +1 for the space */

            struct node **dep_node = find_node(l, num_nodes, buf2);
            /* l[i]->deps[k] = *find_node(l, n, buf2); */
            if (!dep_node) {
                /* fprintf(stderr, "No node found!\n"); */
                goto fail;
            }
            l[i]->deps[k] = *dep_node;
            k++;
        }

        i++;
        free(buf);
    }

    *out_l = l;
    *out_n = num_nodes;

    fail:
    /* return (void *) 0; */
    return;
}

/*
 * sort
 *
 * DO IT!
 *
 * Do the sorting.
 * Returns failure or success.
 */
void
sort(struct node **l, int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        if (l[i]->v) {
            continue;
        } else {
            l[i]->v = 1;
        }
        sort(l[i]->deps, l[i]->num_deps);
        if (l[i]->data) {
            fprintf(stderr, "%s\n", l[i]->data);
        } else {
            fprintf(stderr, "(null)\n");
        }
    }
}

/*
 * Main program
 */

int
main(int argc, char **argv)
{
    struct node **l;
    int n;
    char *buf;
    FILE *f;

    /* Exit w/ error code if not enough args */
    if (argc < 2) goto fail;

    /* use first arg as param w/ out checking FIXME */
    f = fopen(argv[1], "r");

    parse(f, &l, &n);
    /* print_graph(l, n); */
    sort(l, n);

    return 0;
    fail:
    return -1;
}
