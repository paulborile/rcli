# rcli
readline based library for writing cli applications in C

Just set the list of commands and develop a function for each command : all the rest is done by rcli.
Uses GNU readline and GNU history libraries
Example code :

```

int terminate(char *command, char *arg1);
int get_api_version(char *command, char *arg1);
int create(char *command, char *arg1);
int set(char *command, char *arg1);
int add(char *command, char *arg1);
int delete(char *command, char *arg1);


static rcli_cmds_t commands[] = {
    { "quit", no_argument, terminate },
    { "get_api_version", no_argument, get_api_version },
    { "create", no_argument, create },
    { "set", required_argument, set },
    { "add", required_argument, add },
    { "delete", required_argument, delete },
    {0,0,0}
};


int main(int argc, char **argv)
{

    // main loop

    rcli_h *r = rcli_create(commands, "cli-> ");

    rcli_run(r);

    exit(EXIT_SUCCESS);
}

```

Still heavily in progress ...
