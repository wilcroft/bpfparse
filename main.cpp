#include "rule.h"
#include "predicate.h"
#include <string>

int main(void){

	BpfRule rule;
	rule.addRule("A and BC AND d and EFG");
	std::cout << std::endl;
	rule.addRule("A and ( B or C )");
	std::cout << std::endl;
	rule.addRule("A or 1 (B and C)");
	std::cout << std::endl;
	rule.addRule("B or (A and C) and F or G");
	std::cout << std::endl;
	rule.addRule("(A or B) and (C or D)");
	std::cout << std::endl;
	rule.addRule("(A or B) (C or D)");
	std::cout << std::endl;
	rule.addRule("A or (B and (G or H))");
	std::cout << std::endl;
	rule.addRule("(((A or B)))");

	return 0;
}