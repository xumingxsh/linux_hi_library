#include "netThreadCommon.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>

namespace Hi
{
namespace Net
{
unsigned long long get_cur_time()
{
	timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec * 1000 * 1000 + now.tv_usec;
}
sockaddr* convert_sock_in(sockaddr_in& addr)
{
	return static_cast<sockaddr*>(Hi::Net::implicit_cast<void*>(&addr));
}
}
}