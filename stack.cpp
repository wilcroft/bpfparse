#include "stack.h"

void stack::addpred(std::string s) {
	if (active.empty()) newpred();
	for (auto& x : active) {
		x.push_back(s);
	}
}

// Called when an OR is found
void stack::newpred() {
	preds.splice(preds.end(), active);
	active.emplace_back();
}

stacks::stacks() {
	open();
}

//Start a new stack layer
void stacks::open() {
	st.emplace_back();
}

// Close the top-most layer; apply effects downward
void stacks::close() {

	if (st.size() > 1) {

		//move the active predicate into completed for the top stack element
		st.back().newpred();

		// remove the top stack; get a reference to the new top
		struct stack pop (st.back());
		st.pop_back();
		struct stack & repl = st.back();

		// make a new stack level to now-current top (old top+new top)
		struct stack newstack;
		
		//copy the finished preds over
		for (std::list<std::list<std::string>>::iterator it = repl.preds.begin(); it != repl.preds.end(); it++) {
			newstack.preds.emplace_back(*it);
		}

		// apply the popped stack to the active predicates
		if (repl.active.size() == 0) {
			//cover the case where the now-top stack didnt have active predicates.
			for (auto & s : pop.preds) {
				newstack.active.emplace_back(s);
			}
		}
		else {
			for (auto & s : pop.preds) {
				for (auto & t : repl.active) {
					newstack.active.emplace_back(t);
					if (t.size() != 0 && ((istring::istring)newstack.active.back().back()) != "and") newstack.active.back().push_back("and");
					newstack.active.back().insert(newstack.active.back().end(), s.begin(), s.end());
				}
			}
		}
//		for (auto &s : pop.preds) {
//			//for (std::list<std::list<std::string>>::iterator it = repl.preds.begin(); it != repl.preds.end(); it++) {
//				newstack.preds.emplace_back(repl.preds.back());
//				if (((istring::istring)newstack.preds.back().back())!="and") newstack.preds.back().push_back("and");
//				newstack.preds.back().insert(newstack.preds.back().end(),s.begin(), s.end());
//			//}
//		}

		//remove the old top; push the combined version
		st.pop_back();
		st.emplace_back(newstack);
	}
}

void stacks::addpred(std::string s) {
	st.back().addpred(s);
}

void stacks::newpred() {
	st.back().newpred();
}