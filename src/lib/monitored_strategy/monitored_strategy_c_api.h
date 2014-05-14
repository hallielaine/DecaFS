#ifndef __MONITORED_STRATEGY_C_API_H__
#define __MONITORED_STRATEGY_C_API_H__

#include <stdint.h>

/*
 *	Register a module to be called with a specific timeout, 
 *	  repeatedly throughout DecaFS execution.
 *
 * If this function is called MORE THAN ONCE the last monitor will be the
 *   monitor in effect.
 */
extern "C" void register_monitor_module (void (*monitor_module)(), 
                                         struct timeval timeout);

/*
 *	Register a function to be called on node failure.
 *
 * If this function is called MORE THAN ONCE the last handler will be the
 *   handler in effect.
 */
extern "C" void register_node_failure_handler (void (*failure_handler)(uint32_t node_number));

/*
 *	Register a function to be called on node coming online.
 *
 * If this function is called MORE THAN ONCE the last handler will be the
 *   handler in effect.
 */
extern "C" void register_node_up_handler (void (*up_handler)(uint32_t node_number));

/*
 * Call a previously registed node failure handler.
 */
extern "C" void run_node_failure_handler (uint32_t node_number);

/*
 * Call a previously registed node up handler.
 */
extern "C" void run_node_up_handler (uint32_t node_number);

#endif
