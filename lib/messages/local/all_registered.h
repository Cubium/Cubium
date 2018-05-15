#ifndef ALL_REGISTERED_H
#define ALL_REGISTERED_H

#include <messages/local/local_spa_message.h>
#include <messages/op_codes.h>
#include <stdint.h>

struct AllRegistered
{
  AllRegistered(
      LogicalAddress destination,
      LogicalAddress source) : localSpaMessage(0,
                                               0,
                                               72,
                                               destination,
                                               source,
                                               0,
                                               op_ALL_REGISTERED,
                                               3500) {}
  LocalSpaMessage localSpaMessage;
};

#endif
