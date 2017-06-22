#pragma once

enum BPFdir {
	BPFdirOR,
	BPFdirAND,
	BPFdirSRC,
	BPFdirDEST
};

enum BPFproto {
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
	BPFtypePortrange
};

class BpfPredicate {

	enum BPFproto proto;
	enum BPFdir dir;
	enum BPFtype type;
	int value;

public:
	BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v);
};
