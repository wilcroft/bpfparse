#pragma once
#include <list>
#include <string>
#include <iostream>
#include "predicate.h"
#include "istring.h"
#include "stack.h"


class BpfRule{
	std::list<BpfPredicate> predlist;

public:
	void addRule(enum BPFproto p, enum BPFdir d, enum BPFtype type, int value);
	void addRule(istring::istring s);
};
