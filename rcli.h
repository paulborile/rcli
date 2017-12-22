/*
 * Paul Stephen Borile
 * readline based cli builder
 */


#include <readline/readline.h>
#include <readline/history.h>


#define no_argument 0
#define required_argument 1

typedef int execute_fun_type (char *command, char *arg1);
// typedef int vcap_as_bool_compute_type (vcap_t *, vcap_error *error_code);

// list of commands
typedef struct rcli_cmds
{
  char *command;
  int arg_required;
  execute_fun_type (*execute); // command function to call for execution
  //    vcap_as_bool_compute_type (*vcap_as_bool_compute);
} rcli_cmds_t;

// rcli handle
typedef struct _rcli_h
{
  rcli_cmds_t *cmds;
  int cmds_len;
  char *prompt;
} rcli_h;

// init the rcli
rcli_h *rcli_create(rcli_cmds_t *c, char *cli_prompt);
void rcli_destroy(rcli_h *h);

// read_execute commands
void rcli_run(rcli_h *h);
