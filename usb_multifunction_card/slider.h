#ifndef SLIDER_H
#define SLIDER_H

typedef struct slider_position
{
    unsigned char left;
    unsigned char right;
} slider_status;

slider_status slider_get_status(void);

#endif
