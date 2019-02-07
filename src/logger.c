/*
 * Copyright (C) 2018 Kamil Lorenc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "logger.h"

levelopt_t levelopts[] = {
  LEVELOPT(ERROR), LEVELOPT(WARNING), LEVELOPT(INFO), LEVELOPT(DEBUG)
};

static level_t log_level = LEVEL_ERROR;

int set_numeric_log_level(level_t level)
{
  if (level >= LEVEL_MAX)
  {
    return 1;
  }
  log_level = level;
  INFO("log level set to %s(%d)", get_log_level(), log_level);
  return 0;
}

int set_log_level(const char *level)
{
  int i;
  if (isdigit(level[0]))
  {
    /* TODO: use strtol instead of atol */
    if (set_numeric_log_level(atol(level)))
    {
      return 1;
    }
  }
  else
  {
    log_level = LEVEL_MAX;
    for (i = 0; i < LEVEL_MAX; i++)
    {
      if (strcmp(levelopts[i].text, level) == 0)
      {
        if (set_numeric_log_level(i))
        {
          return 1;
        }
        break;
      }
    }
    if (log_level == LEVEL_MAX)
    {
      return 1;
    }
  }

  return 0;
}

level_t get_numeric_log_level()
{
  return log_level;
}

char *get_log_level()
{
  level_t level = get_numeric_log_level();
  return levelopts[level].text;
}

void LOG(level_t level, char *format, ...)
{
  va_list va;
  if (level > get_numeric_log_level())
  {
    return;
  }

  fprintf(stderr, "[%s] ", levelopts[level].text);

  va_start(va, format);
  vfprintf(stderr, format, va);
  va_end(va);

  fprintf(stderr, "\n");
}
