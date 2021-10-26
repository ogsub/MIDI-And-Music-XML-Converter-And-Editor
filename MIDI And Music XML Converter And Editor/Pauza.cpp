#include "Pauza.h"

Pauza::Pauza(Razlomak trajanje_): Simbol(trajanje_) {}


Pauza::~Pauza() {
}

bool Pauza::promeniOktavuZa(int i)
{
	return false;
}

std::string Pauza::ispisivanjePodataka() {
	if (trajanje.getRazlomak() == Razlomak(1, 4))
		return "!";
		//std::cout << "!";
	else
		return "_";
		//std::cout << "_";
}

std::string Pauza::getStr() {
	return "0";
}


std::ostream& Pauza::ispis(std::ostream& out) const {
	return out << "Trajanje: " << trajanje << " Broj takta: " << brojTakta;
}
