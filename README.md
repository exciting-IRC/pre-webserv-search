#  <arpa/inet.h>

h = host(99% little endian)<br />
n = network(big endian)<br />
l = long(32byte)<br />
s = short(16byte)<br />

네트워크 - 호스트 간의 엔디안을 변환한다

```c
uint16_t htons(uint16_t hostshort);

uint32_t htonl(uint32_t hostlong);

uint16_t ntohs(uint16_t netshort);

uint32_t ntohl(uint32_t netlong);
```

'.'으로 구분된 ip 문자열을 32비트 정수로 변환한다.
변환할 수 없을 경우에는 INADDR_NONE을 반환한다
```c
in_addr_t inet_addr(const char *cp);
```

# <sys/event.h>

i/o 멀티플렉싱 라이브러리

kqueue를 위한 fd를 할당한다

실패시 -1 반환
```c
int kqueue(void);
```

새로운 이벤트를 등록하거나, 특정 이벤트를 풀링한다
```c
int kevent(
    int kq, 
    const struct kevent *changelist,
    int nchanges,
    struct kevent *eventlist,
    int nevents,
    const struct timespec *timeout
);
```

```c
 struct kevent {
    /* identifier for this event */
    uintptr_t  ident;
    /* filter for event */
    int16_t    filter;
    /* general flags */
    uint16_t   flags;
    /* filter-specific flags */
    uint32_t   fflags;
    /* filter-specific data */
    intptr_t   data;
    /* opaque user data identifier */
    void       *udata;
 };
```
# <sys/socket.h>

## sockaddr
여러 소켓 관련 함수에서 주소를 특정하기 위해 사용하는 구조체.<br>
`sa_data`필드의 길이가 고정되어 있지 않기 때문에,<br>
`sockaddr`을 이용하는 함수는 전체 구조체의 크기를 인자로 받는다.<br>
또한 이용하는 프로토콜에 따라서 `sockaddr`과 호환되지만 조금 더 사용하기 편한 형태의 구조체를 제공할 수 있다.
`sa_data` 필드는 `네트워크 엔디안`으로 저장되어야 한다
```c
struct sockaddr {
  // Address family
  sa_family_t sa_family;
  // socket length (variable length array)
  char        sa_data[?];
};
```

```c
socklen_t address_len;
```

### <sys/in.h> - sockaddr

`AF_INET` (IPV4) 를 위한 sockaddr
```c
struct sockaddr_in {
  sa_family_t     sin_family;   // always AF_INET 
  in_port_t       sin_port;     // Port number 
  struct in_addr  sin_addr;     // IP address 
};
```

`AF_INET6` (IPV6) 를 위한 sockaddr
```c
struct sockaddr_in6 {
  sa_family_t      sin6_family;
  in_port_t        sin6_port; 
  uint32_t         sin6_flowinfo;
  struct in6_addr  sin6_addr;
  uint32_t         sin6_scope_id;
};
```

### <sys/un.h> - sockaddr

`AF_UNIX` (유닉스 소켓)을 위한 sockaddr
```c
strut sockaddr_un {
  sa_family_t  sun_family //  Address family. (AF_UNIX) 
  char         sun_path[_POSIX_PATH_MAX ?] // Socket pathname.
};
```

---

## Socket(2)

소켓 통신을 위한 종단점을 생성하고, 그 식별자를 반환한다

```c
int socket(int domain, int type, int protocol);
```

`domain` :  통신할 영역

- `PF_LOCAL`  : 유닉스 소켓
- `PF_INET`   : IPV4
- `PF_INET6`  : IPV6
- 이외에 PF_UNIX, PF_ROUTE, PF_KEY, PF_SYSTEM, PF_NDRV가 있다.

`type` : 통신 방법

- `SOCK_STREAM` : 신뢰성 있는 통신 방법
- `SOCK_DGRAM`  : 신뢰성 없는 통신 방법
- `SOCK_RAW`    : 직접 네트워크 인터페이스에 접근 (super user only)

`protocol` : 실제 프로토콜을 지정한다

- see getprotoent(3)

`반환값` : fd (실패시 -1)

---

## accept(2)

```c
int accept (
    int                         socket,
    struct sockaddr *restrict   address,
    socklen_t *restrict         address_len
);
```

`socket`      : 소켓의 fd

`address`     : 호출 결과를 담을 구조체

`address_len` : a

---

## listen(2)

연결 지향 소켓 descriptor `socket`을 연결을 대기하는 상태로 만든다.

`backlog` : 한번에 연결을 허용할 클라이언트 수
- 만약 값이 SOMAXCONN 보다 크면 값이 SOMAXCONN으로 조정된다.
- 만약 값이 0보다 작으면 값이 0으로 조정된다.
- 만약 값이 0이면 값은 적당한 최소 크기로 조정된다.

`return` : 성공한 경우 0 / 실패한 경우 -1

```c
int listen(int socket, int backlog);
```

---

## send(2)
```c
ssize_t send(int socket, const void *buffer, 
             size_t length, int flags);
```

---

## recv(2)
```c
ssize_t recv(int socket, void *buffer,
             size_t length, int flags);
```

---

## bind(2)
할당된 명명되어있지 않은 소켓 `socket`에 이름을 할당한다 
```c
int bind(int socket,
         const struct sockaddr *address, 
         socklen_t address_len);
```

`socket`   : 명명할 소켓 fd

`sockaddr`, `address_len` : see [sockaddr](#sockaddr)

---

## getsockname(2)

```c
int getsockname(int socket, 
                struct sockaddr *restrict address,
                socklen_t *restrict address_len);
```

---

int connect(int socket, const struct sockaddr *address, socklen_t address_len);

int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);


# <fcntl.h>

fcntl(fd, F_SETFL, O_NONBLOCK);
