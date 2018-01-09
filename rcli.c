/*
 * Paul Stephen Borile
 * readline based cli builder
 */

// WARNING!!!! Needed to use strdup in this code. Without it, it's not defined
#define _BSD_SOURCE

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>

#include "rcli.h"

/*
   typedef int execute_fun_type (char *command, char arg1);

   typedef struct rcli_cmds
   {
    char *command;
    int arg_required;
    execute_fun_type (*execute); // command function to call for execution
   } rcli_cmds_t;

   // rcli handle
   typedef struct _rcli_h
   {
    rcli_cmds_t *cmds;
    int cmds_len;
    char *prompt;
   } rcli_h;

 */

static int _rcli_search(rcli_h *h, char *cmd);
// GNU readline / historu function are not multi istantiable (they don'
// receive e context) so we need a pointer to the only one and global
// rcli_h handle
static rcli_h *g = NULL;

static char **_rcli_cmd_name_completion(const char *text, int start, int end);
static char *_rcli_cmd_name_generator(const char *text, int state);
char *qtok(char *str, char **next);

// init the rcli
rcli_h *rcli_create(rcli_cmds_t *c, char *cli_prompt)
{
    rcli_h *h = NULL;
    int i;
    if ((h = (rcli_h *) malloc(sizeof(rcli_h))) == NULL)
    {
        return (NULL);
    }
    // just count them
    for (i=0; c[i].command != NULL; i++) ;
    h->cmds_len = i;

    h->cmds = c;

    rl_bind_key('\t', rl_complete);

    h->prompt = strdup(cli_prompt);

    // globalize h
    g = h;

    //
    rl_attempted_completion_function = _rcli_cmd_name_completion;

    return h;
}

void rcli_destroy(rcli_h *h)
{
    free(h->prompt);
    free(h);
}

void rcli_run(rcli_h *h)
{
    char *input;
    int stay_in_loop = 1;
    int ret, which;
    char cmd[32];
    char arg1[4096];
    char *p;

    while (stay_in_loop)
    {
        input = readline(h->prompt);

        if (input == NULL)
        {
            stay_in_loop = 0;
            continue;
        }

        // parse arguments

        p = input;
        strcpy(cmd, qtok(p, &p));
        strcpy(arg1, qtok(p, &p));

        // find command index in cmds

        if (( which = _rcli_search(h, cmd)) == -1 )
        {
            printf("Unknown command \"%s\"\n", cmd);
            free(input);
            continue;
        }

        ret = h->cmds[which].execute(cmd, arg1);

        // Add input to history.
        add_history(input);
        free(input);
    }
}

// search command in table
static int _rcli_search(rcli_h *h, char *cmd)
{
    for (int i=0; i < h->cmds_len; i++)
    {
        if (strcmp(cmd, h->cmds[i].command) == 0 )
        {
            return i;
        }
    }

    return -1;
}

static char **_rcli_cmd_name_completion(const char *text, int start, int end)
{
    //printf("_rcli_cmd_name_completion() - called for %s, start %d, end %d\n", text, start, end);
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, _rcli_cmd_name_generator);
}

static char *_rcli_cmd_name_generator(const char *text, int state)
{
    static int list_index, len;
    char *name;

    //printf("_rcli_cmd_name_generator() - cmds_len = %d, called for %s, state %d\n", g->cmds_len, text, state);

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = g->cmds[list_index++].command))
    {
        if (strncmp(name, text, len) == 0 )
        {
            //printf("returning %s\n", g->cmds[list_index].command);
            return strdup(name);
        }
    }

    //printf("returning NULL\n");
    return NULL;
}
