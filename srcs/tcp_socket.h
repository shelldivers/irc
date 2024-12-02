/*
    socket object for TCP/IP protocols

        encapsulation of sockfd and sockaddr_in

        issues
        - Do we have to handle IPv6? further argument needed.
                -> dual stack socket, handle IPv4 using IPv6.
*/

#ifndef FTIRC_SRCS_TCP_SOCKET_H
#define FTIRC_SRCS_TCP_SOCKET_H

#define FTIRC_SRCS_SOCKET_REQUEST_QUEUE_SIZE 50

extern "C" {
#include <netinet/in.h>
}

#include <stdexcept>
#include <string>

typedef struct sockaddr_in6 InetSocketAddress;

class TcpSocket {
 private:
  TcpSocket();
  TcpSocket(TcpSocket const &);
  TcpSocket &operator=(TcpSocket const &);

  static socklen_t const kInetSocketAddrLen;

  socklen_t cur_inet_sock_addr_len_;
  int const socket_fd_;
  bool const is_listen_only_;
  InetSocketAddress inet_sock_address_;

 public:
  TcpSocket(std::string const &port_number);  // server socket, getaddrinfo
  TcpSocket(int const listen_sock_fd);        // client socket, accepted socket
  ~TcpSocket();

  int socket_fd();
  bool is_listen_only();

  in_port_t GetPortNum();
  char const *GetIPAddress();

  class SocketCreationFailError : public std::runtime_error {
   public:
    const char *what();
  };
};

#endif
