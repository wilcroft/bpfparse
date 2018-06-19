#pragma once
//#include "istring.h"
#include <vector>
#include <map>

#define SIZE_IFACE_INDEX 2

// ether, fddi, tr, wlan, ip, ip6, arp, rarp, decnet, tcp and udp
// Ethernet Protocol reference numbers
#define ETHER_IPV4	0x0800
#define ETHER_IPV6	0x86DD
#define ETHER_ARP	0x0806
#define ETHER_RARP	0x8035
#define ETHER_DECNET	0x6003
#define ETHER_VLAN	0x8100
#define ETHER_VLAN_DOUBLE	0x9100
#define ETHER_MPLS_UNI	0x8847
#define ETHER_MPLS_MULTI	0x8848
#define ETHER_PPPOE_DISC	0x8863
#define ETHER_PPPOR_SESS	0x8864

// IP Protocol reference numbers
#define IP_TCP	17
#define IP_UDP	6
#define IP_ICMP	1

// Matchable field sizes
#define SIZE_MAC 48
#define SIZE_VLAN 32
#define SIZE_ETHER_PROTO 16
#define SIZE_IPV_PROTO 8
#define SIZE_IPV4_ADDR 32
#define SIZE_IPV_LEN 16
#define SIZE_IPV6_ADDR 128
#define SIZE_PORT 16

/////////////////////////////////
// Ethernet Frame Bits
// 0-47: Dest MAC (hwaddr)
// 48-95: Src MAC (hwaddr)
// (opt) 96-127: VLAN
// 96-111/128-143: EtherType

////////////////////////////////
// IP4 Bits (Start at 112/144)
// 128-143/160-175: Length
// 184-191/216-223: Protocol

////////////////////////////////
// IP6 Bits (Starts at 112/144)
// 144-159/176-191: Length
// 160-168/192-199: Protocol

// Valid keywords

/*const std::vector<istring::istring> primatives = {
	"dst",
	"src",
	"host",
	"ether",
	"gateway",
	"net",
	"port",
	"portrange",
	"less",
	"greater",
	"ip",
	"proto",
	"ip6",
	"arp",
	"rarp",
	"mask",
	"icmp",
	"icmp6",
	"igrp",
	"igrp6",
	//pim, ah,esp, vrrp,
	"tcp",
	"udp",
	"protochain",
	"broadcast"


};*/