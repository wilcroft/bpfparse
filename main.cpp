#include "rule.h"
#include "predicate.h"
#include "matchtreenode.h"
#include <string>
#include <fstream>

int main(void){

	std::ifstream ifs;
	BpfRule rule;

	//std::list<BpfRule> rules;
	//rule.addRule("A and BC AND d and EFG");
	//std::cout << std::endl;
	//rule.addRule("A and ( B or C )");
	//std::cout << std::endl;
	//rule.addRule("A or 1 (B and C)");
	//std::cout << std::endl;
	//rule.addRule("B or (A and C) and F or G");
	//std::cout << std::endl;
	//rule.addRule("(A or B) and (C or D)");
	//std::cout << std::endl;
	//rule.addRule("(A or B) (C or D)");
	//std::cout << std::endl;
	//rule.addRule("A or (B and (G or H))");
	//std::cout << std::endl;
	//rule.addRule("(((A or B)))");
	//std::cout << std::endl;
	//rule.addRule("UDP port 5060 and ip and (dst net 160.96.0.0/12)");
	//std::cout << std::endl;
	//rule.addRule("UDP port 5060 and ip and (net 160.96.0.0/12)");
	//rule.addRule("UDP port 5060 and ip and (net 160.96.0.0/12)");

	ifs.open("../rules", std::ifstream::in);

	if (!ifs.is_open()) return -1;
	std::string s;
	std::getline(ifs, s);
	long i = 0;
	while (ifs.good()) {
		rule.addRule(s);
		std::getline(ifs, s);
		i++;
		double d = (double)i / 73352;
		if (d == 0.25) std::cout << "25%" <<std::endl;
		if (d == 0.5) std::cout << "50%" <<std::endl;
	}

	MatchTreeNode treenode(&rule);

	std::cout << treenode.getDepth() << std::endl;
	std::cout << treenode.nodeCount() << std::endl;
	//rule.writeRulesToFile("../allrules.pla");

	ifs.close();
	return 0;
}