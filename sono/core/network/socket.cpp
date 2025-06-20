#include "socket.h"

b8 gSocketInitialized = false;

b8 InitSocket() {
#ifdef SONO_PLATFORM_WINDOWS
  WSAData data;
  gSocketInitialized = WSAStartup(MAKEWORD(2, 2), &data) == NO_ERROR;
  return gSocketInitialized;
#else
  return true;
#endif
}

void ShutdownSocket() {
#ifdef SONO_PLATFORM_WINDOWS
  WSACleanup();
#endif
}

Socket::Socket() {}

void Socket::Open(u16 port) {}

void Socket::Close() {}

b8 Socket::IsOpen() const { return true; }

i32 Socket::Recv(const IPAddr &dest, void *data, i32 size) { return 0; }

b8 Socket::Send(const IPAddr &dest, const void *data, i32 size) { return false; }

Socket::~Socket() {}
