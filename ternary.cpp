#include "ternary.h"

ternary::ternary() {
	d = TERN_DC;
}
ternary::ternary(const enum ternary_value& t) {
	d = t;
}

bool ternary::operator==(const enum ternary_value& t) {
	if (t == TERN_DC) return true;
	if (d == TERN_DC) return true;
	return d = t;
}
bool ternary::operator==(const ternary& t) {
	if (t.d == TERN_DC) return true;
	if (d == TERN_DC) return true;
	return d = t.d;
}