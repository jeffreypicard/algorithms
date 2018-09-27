/*
 * top_srt.c
 *
 * Academic implementation of topological sort on a graph.
 *
 * Author: Jeffrey Picard (jeff@jeffreypicard.com)
 */

/*
 * Data structures
 */

struct node {
    int num_deps;
    struct node* deps;
    int v; /* visited */
};

/*
 * Utility functions
 */

/*
 * Main program
 */

int
main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    return -1;
}
