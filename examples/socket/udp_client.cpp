/*
    Simple udp client
*/
#include "../../lib/platform_abstraction/socket/socket.hpp"
#define SERVER "127.0.0.1"
#define BUFLEN 512 //Max length of buffer
#define PORT 8888  //The port on which to send data

void die(char *s)
{
  perror(s);
  exit(1);
}

int main(void)
{
  // Socket is an abstraction class that we have built. This makes it so Unix
  // specific code can live in only one place. As well as allowing us to not
  // litter gross C code everywhere. You can find the source in
  // lib/PA_Socket/socket.hpp
  Socket s;

  while (true)
  {
    std::string message;
    std::cout << "Enter message to send: " << std::endl;
    std::cin >> message;
    bool result = s.send(SERVER, PORT, (uint8_t *)message.c_str(), strlen(message.c_str()));
    if (result)
    {
      std::cout << "Message successfully sent" << std::endl;
      std::cout << "Message: " << message << std::endl;
    }
    else
    {
      std::cout << "Message was not sent..." << std::endl;
    }
  }

  return 0;
}

/*
    Simple udp client example code from the interwebs
*/
/**
 * Our code basically wraps this socket API
 */

// #include<stdio.h> //printf
// #include<string.h> //memset
// #include<stdlib.h> //exit(0);
// #include<arpa/inet.h>
// #include<sys/socket.h>
//
// #define SERVER "127.0.0.1"
// #define BUFLEN 512  //Max length of buffer
// #define PORT 8888   //The port on which to send data
//
// void die(char *s)
// {
//     perror(s);
//     exit(1);
// }
//
// int main(void)
// {
//     struct sockaddr_in si_other;
//     int s, i, slen=sizeof(si_other);
//     char buf[BUFLEN];
//     char message[BUFLEN];
//
//     if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
//     {
//         die("socket");
//     }
//
//     memset((char *) &si_other, 0, sizeof(si_other));
//     si_other.sin_family = AF_INET;
//     si_other.sin_port = htons(PORT);
//
//     if (inet_aton(SERVER , &si_other.sin_addr) == 0)
//     {
//         fprintf(stderr, "inet_aton() failed\n");
//         exit(1);
//     }
//
//     while(1)
//     {
//         printf("Enter message : ");
//         gets(message);
//
//         //send the message
//         if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
//         {
//             die("sendto()");
//         }
//
//         //receive a reply and print it
//         //clear the buffer by filling null, it might have previously received data
//         memset(buf,'\0', BUFLEN);
//         //try to receive some data, this is a blocking call
//         if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
//         {
//             die("recvfrom()");
//         }
//
//         puts(buf);
//     }
//
//     close(s);
//     return 0;
// }
