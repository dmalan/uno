#ifndef UNO_HDR
#define UNO_HDR

#include <stdint.h>

enum card_type
{
    NUMERIC,
    WILD,
    NUM_TYPES
};

enum card_color
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    NUM_COLORS
};

// struct representing a single card
struct card_t
{
    uint8_t type;
    uint8_t color;
    uint8_t value;
};

#define UNO_PORT 50501
#define UNO_SERVER "cloud.cs50.net"



#endif
