#ifndef OP_CODES
#define OP_CODES

#include <stdint.h>

const uint8_t op_LOCAL_HELLO              = 0x20; /* 32 */
const uint8_t op_LOCAL_ACK                = 0x21; /* 33 */

const uint8_t op_SPA_SUBSCRIPTION_REQUEST = 0x46; /* 70 */
const uint8_t op_SPA_SUBSCRIPTION_REPLY   = 0x47; /* 71 */

const uint8_t op_SPA_DATA                 = 0x74; /* 116 */
const uint8_t op_SPA_COURIER              = 0x75; /* 117 MASTER CHIEF */

#endif
