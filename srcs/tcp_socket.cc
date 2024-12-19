
extern "C" {
#include <fcntl.h>
#include <unistd.h>
}

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "tcp_socket.h"

namespace Just1RCe {

socklen_t const Just1RCe::TcpSocket::kInetSocketAddrLen =
    sizeof(InetSocketAddress);

TcpSocket::TcpSocket(std::string const &port_number) try
    : cur_inet_sock_addr_len_(kInetSocketAddrLen),
      socket_fd_(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
      is_listen_only_(true) {
  if (socket_fd_ == -1)
    throw std::runtime_error(JUST1RCE_SRCS_SOCKET_CREATION_ERROR);

  memset(&inet_sock_address_, 0, sizeof(InetSocketAddress));
  inet_sock_address_.sin_family = AF_INET;
  inet_sock_address_.sin_port = htons(atoi(port_number.c_str()));
  inet_sock_address_.sin_addr.s_addr = htonl(INADDR_ANY);

  // reuse addr and port
  const int sock_reuse_opt = 1;
  if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &sock_reuse_opt,
                 sizeof(sock_reuse_opt)) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_SOCKET_SET_OPTION_ERROR);

  if (bind(socket_fd_, reinterpret_cast<struct sockaddr *>(&inet_sock_address_),
           kInetSocketAddrLen) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_SOCKET_BIND_ERROR);

  if (listen(socket_fd_, JUST1RCE_SRCS_SOCKET_REQUEST_QUEUE_SIZE) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_SOCKET_LISTEN_ERROR);

  // non-block socket
  int flag = fcntl(socket_fd_, F_GETFL, 0);
  if (flag == -1) throw std::runtime_error(JUST1RCE_SRCS_FCNTL_GETFL_ERROR);
  if (fcntl(socket_fd_, F_SETFL, flag | O_NONBLOCK) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_FCNTL_SETFL_ERROR);

} catch (const std::exception &e) {
  std::cerr << e.what() << '\n';
  throw;
}

TcpSocket::TcpSocket(int const listen_sock_fd) try
    : cur_inet_sock_addr_len_(kInetSocketAddrLen),
      socket_fd_(
          accept(listen_sock_fd,
                 reinterpret_cast<struct sockaddr *>(&inet_sock_address_),
                 &cur_inet_sock_addr_len_)),
      is_listen_only_(false) {
  if (socket_fd_ == -1) throw std::runtime_error(JUST1RCE_SRCS_ACCEPT_ERROR);

  if (cur_inet_sock_addr_len_ > kInetSocketAddrLen)
    throw std::runtime_error(JUST1RCE_SRCS_ACCEPT_ERROR);

  // reuse addr and port
  const int sock_reuse_opt = 1;
  if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &sock_reuse_opt,
                 sizeof(sock_reuse_opt)) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_SOCKET_SET_OPTION_ERROR);

  // non-block socket
  int flag = fcntl(socket_fd_, F_GETFL, 0);
  if (flag == -1) throw std::runtime_error(JUST1RCE_SRCS_FCNTL_GETFL_ERROR);
  if (fcntl(socket_fd_, F_SETFL, flag | O_NONBLOCK) == -1)
    throw std::runtime_error(JUST1RCE_SRCS_FCNTL_SETFL_ERROR);

} catch (const std::exception &e) {
  std::cerr << e.what() << '\n';
  throw;
}

TcpSocket::~TcpSocket() { close(socket_fd_); }

int TcpSocket::socket_fd() { return socket_fd_; }

bool TcpSocket::is_listen_only() { return is_listen_only_; }

// return port number in host order
in_port_t TcpSocket::GetPortNum() { return ntohs(inet_sock_address_.sin_port); }

in_addr_t TcpSocket::GetIPAddress() {
  return ntohl(inet_sock_address_.sin_addr.s_addr);
}

}  // namespace Just1RCe
