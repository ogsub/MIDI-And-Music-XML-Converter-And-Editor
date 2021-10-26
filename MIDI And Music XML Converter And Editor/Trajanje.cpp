#include "Trajanje.h"



Trajanje::Trajanje() {
	razlomak = Razlomak();
}

Trajanje::Trajanje(Razlomak razlomak_): razlomak(razlomak_) {}


Trajanje::~Trajanje() {
}

void Trajanje::setRazlomak(Razlomak razlomak_) {
	razlomak = razlomak_;
}

Razlomak& Trajanje::getRazlomak() {
	return razlomak;
}

std::ostream & operator<<(std::ostream& out, const Trajanje& trajanje) {
	return out << trajanje.razlomak;
}
