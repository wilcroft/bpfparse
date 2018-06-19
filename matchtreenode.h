#pragma once

#include "ternary.h"
#include <vector>
#include "rule.h"

typedef struct {
	bool valid;
	int output;
} MatchResult_t;

class MatchTreeNode {
public:
	bool matchNotAddr;
	int allCompareStart;
	int matchCompareSize;
	std::vector<bool> matchMask;
	MatchResult_t falseMatch;
	MatchResult_t trueMatch;
	MatchTreeNode* addrNext[4];
	
	MatchTreeNode();
	MatchTreeNode(BpfRule * rules);
	MatchTreeNode(BpfRule * rules, int foo);
	~MatchTreeNode();
	
	MatchTreeNode * next(int value);
	MatchTreeNode * next();
	int getOutput(bool m);
	unsigned int getDepth();
	unsigned int nodeCount();
	//std::vector<std::pair<std::list<BpfPredicate *>, std::list<BpfPredicate *>>> splitlist;

};