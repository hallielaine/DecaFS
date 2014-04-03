#ifndef __BARISTA_CORE_H__
#define __BARISTA_CORE_H__

#include <stdio.h>
#include <stdlib.h>

#include "limits.h"
#include "file_types.h"
#include "messages.h"
#include "io_manager.h"
#include "persistent_metadata.h"
#include "volatile_metadata.h"
#include "network_core/bar_clnt.h"
#include "access/access.h"

void process_arguments (int argc, char *argv[]);
void exit_failure (const char *message);

const char *get_size_error_message (const char *type, const char *value);
#endif
