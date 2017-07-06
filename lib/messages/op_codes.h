#ifndef OP_CODES
#define OP_CODES

#include <stdint.h>

const uint8_t op_LOCAL_HELLO = 0x20;
const uint8_t op_LOCAL_ACK = 0x21;

const uint8_t op_SPA_SUBSCRIPTION_REQUEST = 0x46;
const uint8_t op_SPA_SUBSCRIPTION_REPLY = 0x47;

const uint8_t op_SPA_DATA = 0x74;

#endif
