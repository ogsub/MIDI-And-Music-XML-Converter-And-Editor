#include "Simbol.h"

Simbol::Simbol(): trajanje(Trajanje(Razlomak(3, 4))), prekinuta(false) {}

Simbol::Simbol(const Trajanje& trajanje_): trajanje(trajanje_), prekinuta(false) {}

Simbol::~Simbol() {}

void Simbol::setPrekinuta(bool prekinuta_) {
	prekinuta = prekinuta_;
}

bool Simbol::getPrekinuta() {
	return prekinuta;
}

void Simbol::setTrajanje(Trajanje trajanje_) {
	trajanje = trajanje_;
}

void Simbol::setBrojTakta(int brojTakta_) {
	brojTakta = brojTakta_;
}

Trajanje& Simbol::getTrajanje() {
	return trajanje;
}

int Simbol::getBrojTakta() {
	return brojTakta;
}

std::ostream & operator<<(std::ostream & out, const Simbol & simbol) {
	return simbol.ispis(out);
}
