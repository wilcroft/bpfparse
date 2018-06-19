#include "matchtreenode.h"
#define uint unsigned int


MatchTreeNode::MatchTreeNode() {
	for (int i = 0; i < 4; i++)
		addrNext[i] = nullptr;

}

typedef struct {
	std::list<BpfPredicate *> br[4];
} quad;

MatchTreeNode::MatchTreeNode(BpfRule * rules) {
	std::vector<quad> splitlist;

	if (rules->predlist.size()==0) {
		matchNotAddr = true;
		return;
	}

	splitlist.resize(rules->predlist.front().concat().size()-1);
	for (auto &rule : rules->predlist) {
		std::vector<ternary> v = rule.concat();
		for (unsigned int i = 0; i < splitlist.size(); i++) {
			if (v[i] == false && v[i+1] == false)
				splitlist[i].br[0].push_back(&rule);
			if (v[i] == true && v[i+1] == false)
				splitlist[i].br[1].push_back(&rule);
			if (v[i] == false && v[i + 1] == true)
				splitlist[i].br[2].push_back(&rule);
			if (v[i] == true && v[i + 1] == true)
				splitlist[i].br[3].push_back(&rule);
		}
	}
	int idx = -1;
	uint min = 2 * rules->predlist.size();
	for (uint i = 0; i < splitlist.size(); i++) {
		uint newmin = splitlist[i].br[0].size() + splitlist[i].br[1].size() 
				+ splitlist[i].br[2].size() + splitlist[i].br[3].size()
				+ abs((int)splitlist[i].br[0].size() - (int)splitlist[i].br[3].size())
				+ abs((int)splitlist[i].br[1].size() - (int)splitlist[i].br[2].size());
		if (newmin < min) {
			min = newmin;
			idx = i;
		}
	}

	if (idx == -1) {
		matchNotAddr = true;
		return;
	}

	//BpfRule l(splitlist[idx].first);
	//BpfRule r(splitlist[idx].second);
	for (uint i=0; i<4; i++){
		BpfRule * p = new BpfRule(splitlist[idx].br[i]);
		addrNext[i] = new MatchTreeNode(p);
		delete p;
	}

	return;
}

MatchTreeNode::MatchTreeNode(BpfRule * rules, int foo) {
	std::vector<std::pair<std::list<BpfPredicate *>, std::list<BpfPredicate *>>> splitlist;

	splitlist.resize(rules->predlist.front().concat().size());
	for (auto &rule : rules->predlist) {
		std::vector<ternary> v = rule.concat();
		for (unsigned int i = 0; i < splitlist.size(); i++) {
			if (v[i] == true)
				splitlist[i].second.push_back(&rule);
			if (v[i] == false)
				splitlist[i].first.push_back(&rule);
		}
	}

	int idx = -1;
	uint min = 2*rules->predlist.size();
	for (uint i = 0; i < splitlist.size(); i++) {
		uint newmin = splitlist[i].first.size() + splitlist[i].second.size() + abs((int)splitlist[i].first.size() - (int)splitlist[i].second.size());
		if (newmin < min) {
			min = newmin;
			idx = i;
		}
	}

	if (idx == -1) {
		matchNotAddr = true;
		return;
	}

	//BpfRule l(splitlist[idx].first);
	//BpfRule r(splitlist[idx].second);
	BpfRule * p = new BpfRule(splitlist[idx].first);
	addrNext[0] = new MatchTreeNode(p);
	delete p;
	p = new BpfRule(splitlist[idx].second);
	addrNext[1] = new MatchTreeNode(p);
	delete p;
	return;
				
}

MatchTreeNode::~MatchTreeNode() {
	for (int i = 0; i < 4; i++)
		if (addrNext[i] != nullptr) delete(addrNext[i]);
}


MatchTreeNode * MatchTreeNode::next(int value) {
	if (matchNotAddr) return nullptr;
	return addrNext[value];
}

int MatchTreeNode::getOutput(bool m) {
	if (m)
		return trueMatch.valid ? trueMatch.output : -1;
	else
		return falseMatch.valid ? falseMatch.output : -1;
}

unsigned int MatchTreeNode::getDepth() {
	uint max = 0;
	for (uint i = 0; i < 4; i++)
		if (addrNext[i] != nullptr && addrNext[i]->getDepth() > max)
			max = addrNext[i]->getDepth();
	return max + 1;
 }

unsigned int MatchTreeNode::nodeCount() {
	uint max = 0;
	for (uint i = 0; i < 4; i++)
		if (addrNext[i] != nullptr)
			max += addrNext[i]->nodeCount();
	return max + 1;
}