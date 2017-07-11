#pragma once

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
	BPFtypePortrange
};

class BpfPredicate {

public:

	enum BPFproto proto;
	enum BPFdir dir;
	enum BPFtype type;
	int value;

	BpfPredicate();
	BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v);
};
