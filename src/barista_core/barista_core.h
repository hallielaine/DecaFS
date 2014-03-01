#ifndef __BARISTA_CORE_H__
#define __BARISTA_CORE_H__

#include <stdio.h>
#include <stdlib.h>

#include "messages.h"
#include "volatile_metadata.h"

void process_arguments (int argc, char *argv[]);
void exit_failure (const char *message);
#endif
