#include "predicate.h"

BpfPredicate::BpfPredicate(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	proto = p;
	dir = d;
	type = t;
	value = v;
}

