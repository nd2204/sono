#ifndef SN_SOCKET_H
#define SN_SOCKET_H

#include "net_types.h"

#ifdef SONO_PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")

#elif defined(SONO_PLATFORM_MACOS) || defined(SONO_PLATFORM_UNIX)

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

extern b8 gSocketInitialized;

b8 InitSocket();
void ShutdownSocket();

class Socket {
public:
  Socket();

  void Open(u16 port);

  void Close();

  b8 IsOpen() const;

  b8 Send(const IPAddr &dest, const void *data, i32 size);

  i32 Recv(const IPAddr &sender, void *data, i32 size);

  ~Socket();

private:
  i32 mHandle;
};

#endif // !SN_SOCKET_H
