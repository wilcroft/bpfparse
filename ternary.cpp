#include "ternary.h"
namespace tern {
	ternary::ternary() {
		d = TERN_DC;
	}
	ternary::ternary(const enum ternary_value& t) {
		d = t;
	}
	ternary::ternary(const bool& b) {
		d = b ? TERN_1 : TERN_0;
	}

	bool ternary::operator==(const enum ternary_value& t) {
		if (t == TERN_DC) return true;
		if (d == TERN_DC) return true;
		return d == t;
	}
	bool ternary::operator==(const ternary& t) const {
		if (t.d == TERN_DC) return true;
		if (d == TERN_DC) return true;
		return d == t.d;
	}
	bool ternary::operator==(const bool& b) {
		if (d == TERN_DC) return true;
		if (d == TERN_1) return b;
		return !b;
	}
	bool ternary::operator!=(const enum ternary_value& t) {
		if (t == TERN_DC) return true;
		if (d == TERN_DC) return true;
		return d != t;
	}
	bool ternary::operator!=(const ternary& t) {
		if (t.d == TERN_DC) return true;
		if (d == TERN_DC) return true;
		return d != t.d;
	}
	bool ternary::operator!=(const bool& b) {
		if (d == TERN_DC) return true;
		if (d == TERN_1) return !b;
		return b;
	}

	void setVector(std::vector<ternary>& v, int i) {
		std::bitset<MAX_VECTOR_SIZE> b = i;
		for (unsigned int idx = 0; idx < v.size(); idx++) {
			v[idx] = (bool)b[idx];
		}
	}
}