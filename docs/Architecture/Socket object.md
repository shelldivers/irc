---
Date: 2024-11-29
---

# Description
유닉스 소켓 프로그래밍을 위해서 추상화 한 TCP 소켓 객체

# Attributes
## Member Variable

### 1. kInetSocketAddrLen - private
소켓 객체에 예비된 InetSocketAddress타입 구조체의 크기.

### 2. cur_inet_sock_addr_len_ - private
소켓 객체에 write될 sockaddr타입 구조체의 크기. 앞서 살펴본 kInetSocketAddrLen와의 비교를 통해서
accept된 sockaddr의 문제를 확인할 수 있음. 예비한 공간보다 큰 sockaddr을 가질 경우 sockaddr은 truncated되며, 문제 발생.

### 3. socket_fd_ - private
소켓의 file descriptor. systemcall에 전달되는 핸들. RAII.

### 4. is_listen_only - private
소켓의 listening 여부를 위한 필드. RAII.

### 5. sockaddr_in
해당 소켓의 바인드 정보를 저장하는 구조체.
프로토콜 정보, 포트 번호, IP 주소를 저장함.

## constructor & destructor
디폴트 생성, 복사, 복사 할당 불가능함.

### 1. construct with std::string
인자로 포트 번호를 std::string으로 받아 listening 소켓을 생성함.

- reuse address 설정됨.
- listen 설정됨.

### 2. construct with fd
인자로 listenting socket의 fd를 int로 받아서 client 소켓을 생성함.

- reuse address 설정됨.

## Member Function

### 1. int socket_fd();
소켓의 fd에 대한 getter

### 2. bool is_listen_only();
getter, 소켓의 리스닝 설정 여부

### 3. in_port_t GetPortNum();
bind 및 accept된 소켓의 포트 번호, host byte order로 반환됨.

### 4. in_addr_t GetIPAddress();
bind 및 accept된 소켓의 IP 주소, host byte order로  반환됨.


