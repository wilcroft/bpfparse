#include "rule.h"

void BpfRule::addRule(enum BPFproto p, enum BPFdir d, enum BPFtype t, int v){
	predlist.emplace_back(p,d,t,v);
}
