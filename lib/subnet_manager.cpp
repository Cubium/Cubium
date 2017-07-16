#include <iostream>
#include <spa_message.h>
#include <subnet_manager.hpp>
#include <messages/op_codes.h>
#include <messages/local/local_ack.h>
#include <memory>
#include <messages/local/local_spa_message.h>
#include <messages/local/local_hello.h>

std::shared_ptr<SpaCommunicator> SubnetManager::communicator;

/*
void SubnetManager::messageCallback(void * buff, uint32_t len)
{
  LocalSpaMessage* message = (LocalSpaMessage*)buff;

  printf("Opcode: %d\n", message->spaLocalHeader.opcode);
  
  if(op_LOCAL_HELLO == message->spaLocalHeader.opcode)
  {
	LocalHello* castMessage = (LocalHello*)message; 
	communicator->getLocalCommunicator()->insertToRoutingTable(castMessage->localSpaMessage.spaMessage.spaHeader.source, castMessage->localSpaMessage.spaLocalHeader.sourcePort);		

	  LocalAck* msg = new LocalAck(
	  0,
	  0,
	  castMessage->localSpaMessage.spaMessage.spaHeader.destination,
	  castMessage->localSpaMessage.spaMessage.spaHeader.source,
	  0,
	  8888,
	  0);
	 
	 
	  if(msg == nullptr || communicator == nullptr)
	  {
		  std::cout << "bad things, aborting." << std::endl;
	  }
	  else
	  {
		  std::cout << "Sending message." << std::endl;
	      if(communicator->send((SpaMessage*)msg)) std::cout << "Message SENT!" << std::endl;
		
	  }

  }
  
  
  return;



}
*/
