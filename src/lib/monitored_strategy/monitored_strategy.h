#ifndef __MONITORED_STRATEGY_H__
#define __MONITORED_STRATEGY_H__

#include <stdint.h>

#include "decafs_types/file_types.h"
#include "decafs_types/ip_address.h"

/*
 * Called during DecaFS statup process. This function needs to initiate all
 *   module-defined startup activities and register custom modules with
 *   Barista Core.
 */
extern "C" void strategy_startup();

/*
 * Monitoring function. When registered, this function will allow students
 *   to check system health and implement recovery strategies.
 */
extern "C" void monitor_func ();

/*
 * When registered, this function is called when a node goes down.
 */
extern "C" void node_failure_handler_func (uint32_t node_number);

/*
 * When registered, this function is called when a node comes up.
 */
extern "C" void node_up_handler_func (uint32_t node_number);
#endif
