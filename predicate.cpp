#include "predicate.h"

BpfPredicate::BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	proto = p;
	dir = d;
	type = t;
	value = v;
}

BpfPredicate::BpfPredicate() {
	proto = BPFprotoAll;
	dir = BPFdirOR;
	type = BPFtypeHost;
	value = 0;

	src_mac.resize(SIZE_MAC);
	dest_mac.resize(SIZE_MAC);
	vlan_mac.resize(SIZE_VLAN);
	ether_proto.resize(SIZE_ETHER_PROTO);
	src_ip_addr.resize(SIZE_IPV4_ADDR);
	dest_ip_addr.resize(SIZE_IPV4_ADDR);
	ip_proto.resize(SIZE_IPV_PROTO);
}
