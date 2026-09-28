#ifndef RNC_SDK_LIBRARY_CHECKMODELVERSION_TYPES_H
#define RNC_SDK_LIBRARY_CHECKMODELVERSION_TYPES_H

#include "types.h"

typedef struct {
	char	pad[0x24];
	int	server;		/* 0x24 */
} Cd;

#endif /* RNC_SDK_LIBRARY_CHECKMODELVERSION_TYPES_H */
