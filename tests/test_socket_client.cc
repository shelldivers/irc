
/*
    test server for TcpSocket

   c++ -Wall -Wextra -Werror -std=c++98 test_socket_client.cc -o
   test_echo_client
*/

extern "C" {
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
}

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Error : wrong number of argument\n";
    exit(1);
  }
  struct sockaddr_in addr_server = {};

  memset(&addr_server, 0, sizeof(addr_server));
  addr_server.sin_family = AF_INET;                  // TCP protocol
  addr_server.sin_addr.s_addr = inet_addr(argv[1]);  // server IP,
  addr_server.sin_port = htons(6667);  // server port number, 6667 for IRC

  std::string w_buff;
  char r_buff[BUFF_SIZE];

  int sock_client =
      socket(AF_INET, SOCK_STREAM, 0);  // 소켓 생성(IPv4, TCP, 기본프로토콜)
  if (sock_client == -1) {
    std::cout << "socket error" << std::endl;
    close(sock_client);
    exit(1);
  }

  if (connect(sock_client, (struct sockaddr *)&addr_server,
              sizeof(addr_server)) == -1) {  // 연결 요청
    std::cout << "connect error" << std::endl;
    close(sock_client);
    exit(1);
  }

  while (1) {
    memset(r_buff, 0, BUFF_SIZE);

    std::cout << "to server : ";
    std::getline(std::cin, w_buff);

    if (w_buff.size() >= BUFF_SIZE) break;
    int write_chk = write(sock_client, w_buff.c_str(),
                          w_buff.size());  // 작성 길이만큼 write(전송)
    if (write_chk == -1) {
      std::cout << "write error" << std::endl;
      break;
    }

    // receiving
    int read_chk = read(sock_client, r_buff,
                        BUFF_SIZE - 1);  // 읽기 버퍼사이즈-1 만큼 read(읽기)
    if (read_chk == -1) {
      std::cout << "read error" << std::endl;
      break;
    } else {
      r_buff[strlen(r_buff)] = '\n';
      std::cout << "from server : " << r_buff;  // 버퍼 출력
    }
  }
  close(sock_client);  // 연결 종료
  return 0;
}
