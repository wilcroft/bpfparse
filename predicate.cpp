#include "predicate.h"

//BpfPredicate::BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
//	proto = p;
//	dir = d;
//	type = t;
//	value = v;
//}

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
	src_port.resize(SIZE_PORT);
	dest_port.resize(SIZE_PORT);
}

void BpfPredicate::addDestIP4addr(std::string& s) {
	addIP4addr(s, dest_ip_addr);
}
void BpfPredicate::addDestIP4mask(std::string& s) {
	addIP4mask(s, dest_ip_addr);
}
void BpfPredicate::addDestPort(std::string& s) {
	addPort(s, dest_port);
}
void BpfPredicate::addSrcIP4addr(std::string& s) {
	addIP4addr(s, src_ip_addr);
}
void BpfPredicate::addSrcIP4mask(std::string& s) {
	addIP4mask(s, src_ip_addr);
}
void BpfPredicate::addSrcPort(std::string& s) {
	addPort(s, src_port);
}
void BpfPredicate::addIP4addr(std::string& s) {
	addIP4addr(s, src_ip_addr);
	addIP4addr(s, dest_ip_addr);
}
void BpfPredicate::addIP4mask(std::string& s) {
	addIP4mask(s, src_ip_addr);
	addIP4mask(s, dest_ip_addr);
}
void BpfPredicate::addPort(std::string& s) {
	addPort(s, dest_port);
	addPort(s, src_port);
}
void BpfPredicate::addIP4addr(std::string& s, std::vector<ternary>& addr) {
	std::list<istring::istring> lst = istring::tokenize(s, ".");

	int idx = 0;
	for (std::list<istring::istring>::reverse_iterator it = lst.rbegin(); it != lst.rend(); it++) {
		if (*it == "*") {
			for (int i = 0; i < SIZE_IPV4_ADDR / 4; i++)
				addr[idx++] = ternary::TERN_DC;
		}
		else {
			std::bitset<SIZE_IPV4_ADDR / 4> b = std::stoi(*it);
			for (int i = 0; i < SIZE_IPV4_ADDR / 4; i++)
				addr[idx++] = (bool)b[i];
		}
	}
}
void BpfPredicate::addIP4mask(std::string& s, std::vector<ternary>& addr) {
	int m = SIZE_IPV4_ADDR - std::stoi(s);

	for (int i = 0; i < m; i++) {
		addr[i] = ternary::TERN_DC;
	}
}

void BpfPredicate::addPort(std::string& s, std::vector<ternary>& port) {
	std::bitset<SIZE_PORT> b = std::stoi(s);
	for (int i = 0; i < SIZE_PORT; i++) {
		port[i] = (bool)b[i];
	}
}
void BpfPredicate::addEProto(std::string& s) {
	if ((istring::istring)s == "IP") tern::setVector(ether_proto, ETHER_IPV4);
	else if ((istring::istring)s == "IP6") tern::setVector(ether_proto, ETHER_IPV6);
	else if ((istring::istring)s == "ARP") tern::setVector(ether_proto, ETHER_ARP);
	else if ((istring::istring)s == "RARP") tern::setVector(ether_proto, ETHER_RARP);
	else if ((istring::istring)s == "MPLS") tern::setVector(ether_proto, ETHER_MPLS_UNI);
	else if ((istring::istring)s == "VLAN") tern::setVector(ether_proto, ETHER_VLAN);
	else {
		std::string msg = "Invalid Ether Protocol " + s;
		throw std::exception(msg.c_str());
	}

}
void BpfPredicate::addIProto(std::string& s) {
	if ((istring::istring)s == "TCP") tern::setVector(ip_proto, IP_TCP);
	else if ((istring::istring)s == "UDP") tern::setVector(ip_proto, IP_UDP);
	else if ((istring::istring)s == "ICMP") tern::setVector(ip_proto, IP_ICMP);
	else {
		std::string msg = "Invalid IP Protocol " + s;
		throw std::exception(msg.c_str());
	}
}