#pragma once
#include <list>
#include <string>
#include "predicate.cpp"

class BpfRule{
	std::list<BpfPredicate> predlist;

public:
	void addRule(enum BPFproto p, enum BPFdir d, enum BPFtype type, int value);
//	addRule(std::string s);
};
