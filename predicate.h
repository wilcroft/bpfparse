#pragma once
#include <vector>
#include <bitset>
#include "ternary.h"
#include "eth_constants.h"
#include "istring.h"

using  tern::ternary;

enum BPFprimatives {
	BPFprimDir,
	BPFprimProto,
	BPFprimType
};

enum BPFdir {
	BPFdirOR,
	BPFdirAND,
	BPFdirSRC,
	BPFdirDEST
};

enum BPFproto {
	BPFprotoAll,
	BPFprotoEther,
//	BPFprotoFddi,
//	BPFprotoTr,
	BPFprotoWlan,
	BPFprotoIP,
	BPFprotoIP6,
	BPFprotoARP,
	BPFprotoRARP,
	BPFprotoDECNET,
	BPFprotoTCP,
	BPFprotoUDP
};

enum BPFtype {
	BPFtypeHost,
	BPFtypeNet,
	BPFtypePort,
	BPFtypePortrange,
	BPFtypeLength
};

class BpfPredicate {

public:

	enum BPFproto proto;
	enum BPFdir dir;
	enum BPFtype type;
	int value;
	std::vector<ternary> src_mac;
	std::vector<ternary> dest_mac;
	std::vector<ternary> vlan_mac;
	std::vector<ternary> ethertype;
	std::vector<ternary> ether_proto;
	std::vector<ternary> src_ip_addr;
	std::vector<ternary> dest_ip_addr;
	std::vector<ternary> ip_proto;
	std::vector<ternary> src_port;
	std::vector<ternary> dest_port;
	short length;



	BpfPredicate();
	//BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v);
	void addDestIP4addr(std::string& s);
	void addDestIP4mask(std::string& s);
	void addDestPort(std::string& s);
	void addSrcIP4addr(std::string& s);
	void addSrcIP4mask(std::string& s);
	void addSrcPort(std::string& s);
	void addIP4addr(std::string& s);
	void addIP4mask(std::string& s);
	void addPort(std::string& s);
	void addEProto(std::string& s);
	void addIProto(std::string& s);

	std::string tobits();

private:
	void addIP4addr(std::string& s, std::vector<ternary>& addr);
	void addIP4mask(std::string& s, std::vector<ternary>& addr);
	void addPort(std::string& s, std::vector<ternary>& port);
};
