#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

// Strips backslashes from quotes
static char *unescapeToken(char *token)
{
    char *in = token;
    char *out = token;

    while (*in)
    {
        assert(in >= out);

        if ((in[0] == '\\') && (in[1] == '"'))
        {
            *out = in[1];
            out++;
            in += 2;
        }
        else
        {
            *out = *in;
            out++;
            in++;
        }
    }
    *out = 0;
    return token;
}

// Returns the end of the token, without chaning it.
char *qtok(char *str, char **next)
{
    char *current = str;
    char *start = str;
    int isQuoted = 0;

    // Eat beginning whitespace.
    while (*current && isspace(*current)) current++;
    start = current;

    if (*current == '"')
    {
        isQuoted = 1;
        // Quoted token
        current++; // Skip the beginning quote.
        start = current;
        for (;; )
        {
            // Go till we find a quote or the end of string.
            while (*current && (*current != '"')) current++;
            if (!*current)
            {
                // Reached the end of the string.
                goto finalize;
            }
            if (*(current - 1) == '\\')
            {
                // Escaped quote keep going.
                current++;
                continue;
            }
            // Reached the ending quote.
            goto finalize;
        }
    }
    // Not quoted so run till we see a space.
    while (*current && !isspace(*current)) current++;
finalize:
    if (*current)
    {
        // Close token if not closed already.
        *current = 0;
        current++;
        // Eat trailing whitespace.
        while (*current && isspace(*current)) current++;
    }
    *next = current;

    return isQuoted ? unescapeToken(start) : start;
}

#ifdef TEST
int main()
{
    char buffer[1024];
    char *finger;
    char *pText = buffer;

    while (1)
    {
        fgets(buffer, 1024, stdin);
        if ((finger = (char *) strchr(buffer, '\n')) != NULL)
        {
            *finger = '\0';
        }

        printf("Original: '%s'\n", buffer);
        char *pText = buffer;
        while (*pText)
        {
            printf("'%s'\n", qtok(pText, &pText));
        }
    }

}
#endif
