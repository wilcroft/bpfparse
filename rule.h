#pragma once
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "predicate.h"
#include "istring.h"
#include "stack.h"
#include "pegtlparse.h"

class MatchTreeNode;

class BpfRule{

	friend MatchTreeNode;

	std::list<BpfPredicate> predlist;

public:
	BpfRule();
	BpfRule(std::list<BpfPredicate *>);

	void addRule(enum BPFproto p, enum BPFdir d, enum BPFtype type, int value);
	void addRule(istring::istring s);
	void writeRulesToFile(std::string fname);
};
