/* Initialization script parser version 1.0 (c) 2001 Kostas Michalopoulos */

#ifndef __CMDPARSE_H_INCLUDED__
#define __CMDPARSE_H_INCLUDED__

/* use this to set a variable */
void set_var_value(char *name, char *value);

/* use this to get a variable */
char *get_var_value(char *name);

/* use this to parse a command (line) */
void parse_line(char *line);

/* use this to parse a text file */
int parse_file(char *fname);

/* use this to free variables after use */
void free_vars();

#endif

