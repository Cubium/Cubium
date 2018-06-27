#ifndef OP_CODES
#define OP_CODES

#include <stdint.h>

/*
 * Op-codes for each type of SpaMessage
 */

const uint8_t op_LOCAL_HELLO              = 0x20; /* 32 */
const uint8_t op_LOCAL_ACK                = 0x21; /* 33 */
const uint8_t op_ALL_REGISTERED           = 0x22; /* 34 */
const uint8_t op_ALL_SUBSCRIBED           = 0x23; /* 35 */

const uint8_t op_SPA_SUBSCRIPTION_REQUEST = 0x46; /* 70 */
const uint8_t op_SPA_SUBSCRIPTION_REPLY   = 0x47; /* 71 */

const uint8_t op_SPA_DATA                 = 0x74; /* 116 */
const uint8_t op_SPA_COURIER              = 0x75; /* 117 MASTER CHIEF */
const uint8_t op_SPA_STRING               = 0x90; /* 144 */

#endif
