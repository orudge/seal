/* Initialization script parser version 1.0 (c) 2001 Kostas Michalopoulos */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmdparse.h"

typedef struct t_var *p_var;
typedef struct t_var
{
  char  *name;
  char  *value;
  p_var next;
} t_var;

p_var vars = NULL, last = NULL;

p_var new_var(char *name, char *value)
{
  p_var var = (p_var) malloc(sizeof(t_var));
  var->name = strdup(name);
  var->value = strdup(value);
  var->next = NULL;
  if (vars) last->next = var; else vars = var;
  last = var;

  return var;
}

p_var find_var(char *name)
{
  p_var v = vars;
  if (!v) return NULL;
  while (v) {
    if (!stricmp(v->name, name+1)) return v;
    v = v->next;
  };
  return NULL;
}

char *get_var(char *name)
{
  char *res;
  int slen = strlen(name);

  if (name[0] == '"' && name[slen-1] == '"') {
    res = (char *) malloc(slen);
    strcpy(res, name+1);
    res[strlen(res)-1] = 0;
    res = realloc(res, strlen(res)+1);

    return res;
  };
  if (name[0] == '$') {
    p_var v = vars;
    if (!v) return strdup("");
    while (v) {
      if (!stricmp(v->name, name+1)) return strdup(v->value);
      v = v->next;
    };

    return strdup("");
  };
  res = (char *) malloc(50);
  itoa(atoi(name), res, 10);
  res = (char *) realloc(res, strlen(res)+1);

  return res;
}

void set_var(char *name, char *value)
{
  p_var v = find_var(name);
  if (!v) {
    new_var(name, value);

    return;
  };

  v->value = value;
}

void set_var_value(char *name, char *value)
{
  set_var(name, value);
}

char *get_var_value(char *name)
{
  p_var v = vars;
  if (!v) return strdup("");
  while (v) {
    if (!stricmp(v->name, name)) return strdup(v->value);
    v = v->next;
  };

  return strdup("");
}

void parse_line(char *line)
{
  char **arg = malloc(4);
  int args = 0, i, c = 0, us = 0;

  arg[0] = strdup("");

  for (i=0;i<strlen(line);i++) switch (line[i]) {
    case ' ': if (us) {
      arg[args] = realloc(arg[args], strlen(arg[args])+2);
      arg[args][c++] = ' ';
      arg[args][c] = 0;
    } else if (arg[args][0]) {
      args++;
      c = 0;
      arg = realloc(arg, args*4+4);
      arg[args] = strdup("");
    }; break;
    case '"': {
      us = !us;
      arg[args] = realloc(arg[args], strlen(arg[args])+2);
      arg[args][c++] = '"';
      arg[args][c] = 0;
    }; break;
    default: {
      arg[args] = realloc(arg[args], strlen(arg[args])+2);
      arg[args][c++] = line[i];
      arg[args][c] = 0;
    };
  };
  strlwr(arg[0]);

  if (!strcmp(arg[0], "parse")) {
    if (args >= 1 && arg[1][0]) parse_file(arg[1]);
  } else
  if (arg[0][strlen(arg[0])-1] == ':') {
    char *buff = strdup("");
    for (i=1;i<=args;i++) {
      char *a = get_var(arg[i]);
      buff = realloc(buff, strlen(buff)+strlen(a)+1);
      strcat(buff, a);
      free(a);
    };
    arg[0][strlen(arg[0])-1] = 0;
    set_var(arg[0], buff);
    free(buff);
  };

  for (i=0;i<=args;i++) free(arg[i]);
  free(arg);
}

int parse_file(char *fname)
{
  FILE *f = fopen(fname, "rt");
  if (!f) return 0;

  while (!feof(f)) {
    char buff[1024];
    fgets(buff, 1024, f);

    if (buff[strlen(buff)-1] == '\n') buff[strlen(buff)-1] = 0;
    if (buff[0] && buff[0] != ';') parse_line(buff);
  };

  fclose(f);

  return 1;
}

void free_vars()
{
  p_var v = vars;
  if (!v) return;
  do {
    p_var next = v->next;
    free(v->name);
    free(v->value);
    free(v);
    v = next;
  } while (v);
}

