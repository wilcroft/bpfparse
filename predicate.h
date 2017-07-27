#pragma once
#include <vector>
#include "ternary.h"
#include "eth_constants.h"

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
	short length;



	BpfPredicate();
	BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v);
};
