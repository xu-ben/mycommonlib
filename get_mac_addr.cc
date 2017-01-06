#include <stdio.h>              /* Standard I/O */
#include <stdlib.h>             /* Standard Library */
#include <errno.h>              /* Error number and related */
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/socket.h>
#include <net/route.h>
#include <net/if.h>
#include <features.h>           /* for the glibc version number */

#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>       /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>     /* The L2 protocols */
#endif

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netdb.h>

char buf[1024];

int64_t mac_address_to_int(const unsigned char *mac_address) {
	int64_t ret = 0, weight = 1;
	for (int i = 0; i < IFHWADDRLEN; i++) {
		ret += mac_address[i] * weight;
		weight *= 256;
	}
	return ret;
}

bool get_mac_address(unsigned char *mac_address) {
	struct ifreq ifr;
	struct ifconf ifc;
	bool success = false;

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock == -1) { /* handle error*/
		return false;
	};

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */
		return false;
	}

	struct ifreq* it = ifc.ifc_req;
	const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

	for (; it != end; ++it) {
		strcpy(ifr.ifr_name, it->ifr_name);
		if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
			if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
				if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
					success = true;
					break;
				}
			}
		} else { /* handle error */
			return false;
		}
	}

	if (success) {
		memcpy(mac_address, ifr.ifr_hwaddr.sa_data, IFHWADDRLEN);
	}
	return success;
}

int main() {
	unsigned char mac[IFHWADDRLEN];
	if (!get_mac_address(mac)) {
		puts("error!");
		return 1;
	}
	for (int i = 0; i < IFHWADDRLEN - 1; i++) {
		printf("%02X:", (unsigned int) (mac[i]));
	}
	printf("%02X\n", (unsigned int) (mac[IFHWADDRLEN - 1]));
	printf("%lld\n", mac_address_to_int(mac));
}

