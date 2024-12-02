
extern "C" {
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>

#include "tcp_socket.h"

socklen_t const TcpSocket::kInetSocketAddrLen = sizeof(InetSocketAddress);

TcpSocket::TcpSocket(std::string const &port_number) try
    : cur_inet_sock_addr_len_(kInetSocketAddrLen),
      socket_fd_(socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)),
      is_listen_only_(true) {
  if (socket_fd_ == -1) throw SocketCreationFailError();

  inet_sock_address_.sin6_family = AF_INET6;
  inet_sock_address_.sin6_flowinfo = 0;
  inet_sock_address_.sin6_port = htons(atoi(port_number.c_str()));
  inet_sock_address_.sin6_addr = in6addr_any;
  if (bind(socket_fd_, reinterpret_cast<struct sockaddr *>(&inet_sock_address_),
           kInetSocketAddrLen) == -1)
    throw SocketCreationFailError();

  // reuse addr and port
  const int sock_reuse_opt = 1;
  if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &sock_reuse_opt,
                 sizeof(sock_reuse_opt)) == -1)
    throw SocketCreationFailError();

  // enable dual stack socket
  const int disable_v6_only_opt = 0;
  if (setsockopt(socket_fd_, IPPROTO_IPV6, IPV6_V6ONLY, &disable_v6_only_opt,
                 sizeof(disable_v6_only_opt)) == -1)
    throw SocketCreationFailError();

  if (listen(socket_fd_, FTIRC_SRCS_SOCKET_REQUEST_QUEUE_SIZE) == -1)
    throw SocketCreationFailError();

} catch (const std::exception &e) {
  std::cerr << e.what() << '\n';
  throw;
}

TcpSocket::TcpSocket(int const listen_sock_fd) try
    : cur_inet_sock_addr_len_(
          kInetSocketAddrLen),  // this field will be overlapped by accept
      socket_fd_(
          accept(listen_sock_fd,
                 reinterpret_cast<struct sockaddr *>(&inet_sock_address_),
                 &cur_inet_sock_addr_len_)),
      is_listen_only_(false) {
  if (socket_fd_ == -1) throw SocketCreationFailError();

  if (cur_inet_sock_addr_len_ >
      kInetSocketAddrLen)  // accept error, truncated, when?
    throw SocketCreationFailError();

  // reuse addr and port
  const int sock_reuse_opt = 1;
  if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &sock_reuse_opt,
                 sizeof(sock_reuse_opt)) == -1)
    throw SocketCreationFailError();

} catch (const std::exception &e) {
  std::cerr << e.what() << '\n';
}

TcpSocket::~TcpSocket() { close(socket_fd_); }
