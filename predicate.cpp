#include "predicate.h"

//BpfPredicate::BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
//	proto = p;
//	dir = d;
//	type = t;
//	value = v;
//}

BpfPredicate::BpfPredicate() {
	proto = BPFprotoAll;
	dir = BPFdirAND;
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
	out_iface.resize(SIZE_IFACE_INDEX);
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
	if (s == "*") {
		for (int i = 0; i < SIZE_PORT; i++) {
			port[i] = ternary::TERN_DC;
		}
	}
	else {
		std::bitset<SIZE_PORT> b = std::stoi(s);
		for (int i = 0; i < SIZE_PORT; i++) {
			port[i] = (bool)b[i];
		}
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

std::string BpfPredicate::tobits_old() {
	//need to do src IP, dest IP, sport, dport, eproto, irpoto,
	std::string out;
	std::vector<ternary>::reverse_iterator it;
	for (it = src_ip_addr.rbegin(); it != src_ip_addr.rend(); it++) {
		out += it->ch();
	}
	for (it = dest_ip_addr.rbegin(); it != dest_ip_addr.rend(); it++) {
		out += it->ch();
	}
	for (it = src_port.rbegin(); it != src_port.rend(); it++) {
		out += it->ch();
	}
	for (it = dest_port.rbegin(); it != dest_port.rend(); it++) {
		out += it->ch();
	}
	for (it = ether_proto.rbegin(); it != ether_proto.rend(); it++) {
		out += it->ch();
	}
	for (it = ip_proto.rbegin(); it != ip_proto.rend(); it++) {
		out += it->ch();
	}
	return out;
}

std::string BpfPredicate::tobits() {
	std::vector<ternary> c = concat();
	std::vector<ternary>::reverse_iterator it;
	std::string out;
	for (it = c.rbegin(); it != c.rend(); it++)
		out += it->ch();
	return out;
}

std::vector<ternary> BpfPredicate::concat() {
	std::vector<ternary> out;
	std::vector<ternary>::reverse_iterator it;
	out.insert(out.end(), ip_proto.begin(), ip_proto.end());
	out.insert(out.end(), ether_proto.begin(), ether_proto.end());
	out.insert(out.end(), dest_port.begin(), dest_port.end());
	out.insert(out.end(), src_port.begin(), src_port.end());
	out.insert(out.end(), dest_ip_addr.begin(), dest_ip_addr.end());
	out.insert(out.end(), src_ip_addr.begin(), src_ip_addr.end());
	return out;
}