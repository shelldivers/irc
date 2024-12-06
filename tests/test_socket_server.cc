
/*
    test server for TcpSocket

    c++ -Wall -Wextra -Werror -std=c++98 test_socket_server.cc
   ../srcs/tcp_socket.cc -o test_echo_server
*/
extern "C" {
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
}

#include <cstdio>
#include <iostream>
#include <string>

#include "../srcs/tcp_socket.h"

#define BUFF_SIZE 1024

int main() {
  char buff_rcv[BUFF_SIZE + 5];
  std::string buff_snd;

  try {
    TcpSocket server("6667");  // server socket, need port number
    TcpSocket client(
        server.socket_fd());  // client socket, blocking, waiting for connection

    while (1) {
      // receiving message
      const int readCnt = read(client.socket_fd(), buff_rcv, BUFF_SIZE - 1);
      buff_rcv[readCnt] = '\0';  // null termination
      std::cout << "from client : " << buff_rcv << std::endl;

      // sending message
      std::cout << "to client : ";
      std::getline(std::cin, buff_snd);
      write(client.socket_fd(), buff_snd.c_str(), buff_snd.size());
    }
  } catch (const std::exception& e) {
    perror("Socket error : ");
    std::cerr << e.what() << '\n';
  }
}
