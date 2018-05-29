#ifndef ALL_SUBSCRIBED_H
#define ALL_SUBSCRIBED_H

#include <messages/local/local_spa_message.h>
#include <messages/op_codes.h>
#include <stdint.h>

struct AllSubscribed
{
  AllSubscribed(
      LogicalAddress destination,
      LogicalAddress source) : localSpaMessage(0,
                                               0,
                                               72,
                                               destination,
                                               source,
                                               0,
                                               op_ALL_SUBSCRIBED,
                                               3500) {}
  LocalSpaMessage localSpaMessage;
};

#endif
