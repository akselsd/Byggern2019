#ifndef rightUTTONS_H
#define rightUTTONS_H

#include <stdbool.h>

typedef struct button_clicked
{
    bool left;
    bool right; 
} buttons_status;

buttons_status buttons_get_status(void);

#endif
