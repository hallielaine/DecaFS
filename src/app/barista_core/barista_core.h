#ifndef __BARISTA_CORE_H__
#define __BARISTA_CORE_H__

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <map>

#include "limits.h"
#include "messages.h"
#include "volatile_metadata.h"
#include "../../lib/network_core/bar_clnt.h"

void process_arguments (int argc, char *argv[]);
void exit_failure (const char *message);

const char *get_size_error_message (const char *type, const char *value);
#endif
