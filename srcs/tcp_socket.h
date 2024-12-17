/*
    socket object for TCP/IP protocols

        encapsulation of sockfd and sockaddr_in

        issues
        - Do we have to handle IPv6? further argument needed.
                -> dual stack socket, handle IPv4 using IPv6.
*/

#ifndef JUST1RCE_SRCS_TCP_SOCKET_H
#define JUST1RCE_SRCS_TCP_SOCKET_H

#define JUST1RCE_SRCS_SOCKET_REQUEST_QUEUE_SIZE 50

#define JUST1RCE_SRCS_SOCKET_CREATION_ERROR "Socket : socket failed."
#define JUST1RCE_SRCS_SOCKET_SET_OPTION_ERROR "Socket : setsocketopt failed."
#define JUST1RCE_SRCS_SOCKET_BIND_ERROR "Socket : bind failed."
#define JUST1RCE_SRCS_SOCKET_LISTEN_ERROR "Socket : listen failed."
#define JUST1RCE_SRCS_ACCEPT_ERROR "Socket : accept failed."
#define JUST1RCE_SRCS_FCNTL_GETFL_ERROR "Socket : fcntl, get flag failed."
#define JUST1RCE_SRCS_FCNTL_SETFL_ERROR "Socket : fcntl, set flag failed."

extern "C" {
#include <netinet/in.h>
}

#include <string>

namespace Just1RCe {

typedef struct sockaddr_in InetSocketAddress;

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
  explicit TcpSocket(
      std::string const &port_number);  // server socket, getaddrinfo
  explicit TcpSocket(
      int const listen_sock_fd);  // client socket, accepted socket
  ~TcpSocket();

  int socket_fd();
  bool is_listen_only();

  in_port_t GetPortNum();
  in_addr_t GetIPAddress();
};

}  // namespace Just1RCe

#endif
