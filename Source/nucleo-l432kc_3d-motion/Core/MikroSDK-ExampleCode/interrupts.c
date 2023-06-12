#include "app.h"
#include "main.h"

#define ONE_MS	1

extern volatile BOOL EC_DATA_AVAIL;                               // HIDI2_HOST_INT indicates EC data available
extern volatile BOOL usTimeout;
