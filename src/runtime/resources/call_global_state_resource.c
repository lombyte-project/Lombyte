/* Pass the shared state object and caller arguments to the resource hook. */

#include "rnc/runtime/core_state.h"

extern int state_resource_call(int resource, int first, int second,
                             int third) __asm__("func_00116A38");

int call_global_state_resource(int resource, int first, int second) __asm__("CallGlobalStateResource");

int call_global_state_resource(int resource, int first, int second) {
    return state_resource_call(GlobalStateResource, resource, first, second);
}
