#pragma once

#ifdef DEBUG_LOGGING
#define D(args...)       printf(args); printf("\n");
#define D_HEX(num)       printf("\n"); printf("0x%x", num)
#else
#define D(args...)
#define D_HEX(num)
#endif
