#include "Razlomak.h"

Razlomak::Razlomak(int brojilac_, int imenilac_) : brojilac(brojilac_), imenilac(imenilac_) {
	if (imenilac != 4 && imenilac != 8) {
		std::cout << "Imenilac je bio postavljen na vrednost koja nije ni 4 ni 8, automatska promena u 8\n";
		imenilac = 8;
	}
	while (brojilac > imenilac) {
		std::cerr << "Brojilac ne moze biti veci od imenioca\n Postavljena default vrednost brojioca na 3\n";
		brojilac = 3;
	}
}

Razlomak::Razlomak(int brojilac_, int imenilac_, bool t) : brojilac(brojilac_), imenilac(imenilac_) {
	if (imenilac != 4 && imenilac != 8) {
		std::cout << "Imenilac je bio postavljen na vrednost koja nije ni 4 ni 8, automatska promena u 8\n";
		imenilac = 8;
	}
}
//////////////////////
Razlomak::Razlomak(const Razlomak& r2) {
	imenilac = r2.imenilac;
	brojilac = r2.brojilac;
}
bool Razlomak::operator==(const Razlomak & r) {
	return brojilac*r.imenilac == r.brojilac*imenilac;
}
bool Razlomak::operator==(const Razlomak & r) const {
	return brojilac * r.imenilac == r.brojilac*imenilac;
}
int Razlomak::getImenilac() {
	return imenilac;
}
int Razlomak::getBrojilac() {
	return brojilac;
}
//////////////////////
Razlomak operator+(const Razlomak & r1, const Razlomak & r2) {
	if (r1.imenilac > r2.imenilac)
		return Razlomak(r1.brojilac + r2.brojilac * 2, r1.imenilac, true);
	else if(r1.imenilac < r2.imenilac)
		return Razlomak(r1.brojilac*2 + r2.brojilac, r2.imenilac, true);
	return Razlomak(r1.brojilac + r2.brojilac, r1.imenilac, true);
}

bool operator>(const Razlomak& r1, const Razlomak& r2) {
	return r1.brojilac*r2.imenilac >= r2.brojilac*r1.imenilac;
}

std::ostream& operator<<(std::ostream& out, const Razlomak& r) {
	return out << r.brojilac << "/" << r.imenilac;
}