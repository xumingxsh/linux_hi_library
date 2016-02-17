#pragma once

#include <netinet/in.h>
#include "pthread.h"

namespace Hi
{
namespace Net
{
unsigned long long get_cur_time();
sockaddr* convert_sock_in(sockaddr_in& addr);
template<typename To, typename From>
static inline To implicit_cast(From const &f) 
{
  return f;
}
}
}