#include <regex.h>
#include <stdlib.h>
#include "../include/eregex.h"

int match(const char *string, char *pattern, int ignore_case){
  int status;
  regex_t re;

  if(ignore_case){
    if (regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
      return 0;      
    }
  }else
    if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0) {
      return 0;      
    }

  status = regexec(&re, string, (size_t) 0, NULL, 0);
  regfree(&re);
  if (status != 0) {
    return 0;
  }
  return 1;
}
