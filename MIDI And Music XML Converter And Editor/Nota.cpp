#include "Nota.h"
#include <iomanip>

Nota::Nota(int oktava_, Visina visina_, bool povisilica_): oktava(oktava_), visina(visina_), povisilica(povisilica_) {
	if (oktava_ < 2 || oktava_ > 6) {
		std::cerr << "Data oktava ispada iz opsega, automatski upisana oktava 2\n";
		oktava = 2;
	}
}

Nota::Nota(int oktava_, Visina visina_, Razlomak trajanje_, bool povisilica_): Simbol(trajanje_), oktava(oktava_), 
visina(visina_), povisilica(povisilica_) {
	if (oktava < 2 || oktava > 6) {
		std::cerr << "Data oktava ispada iz opsega, automatski upisana oktava 2\n";
		oktava = 2;
	}
}

Nota::~Nota() {
}

void Nota::postaviPovisilicu() {
	if (povisilica) {
		std::cerr << "Nota vec ima povisilicu\n";
		return;
	}
	povisilica = true;
}

void Nota::ukloniPovisilicu() {
	if (!povisilica) {
		std::cerr << "Povisilica nije ni postavljena.\n";
		return;
	}
	povisilica = false;
}

void Nota::setOktava(int oktava_) {
	oktava = oktava_;
}

void Nota::setVisina(Visina visina_) {
	visina = visina_;
}

int Nota::getOktava() {
	return oktava;
}

Nota::Visina Nota::getVisina() {
	return visina;
}

std::string Nota::ispisivanjePodataka() {
	std::string c;
	std::string ret;
	switch (visina)
	{
	case Nota::C:
		c ="C";
		break;
	case Nota::D:
		c = 'D';
		break;
	case Nota::E:
		c = 'E';
		break;
	case Nota::F:
		c = 'F';
		break;
	case Nota::G:
		c = 'G';
		break;
	case Nota::A:
		c = 'A';
		break;
	case Nota::B:
		c = 'B';
		break;
	}
	if (trajanje.getRazlomak() == Razlomak(1, 8))
		ret = (char)tolower(c[0]);
	else
		ret = ret + c[0];
	//std::cout << std::setw(10) <<c[0] << visina;
	ret = ret + std::to_string(oktava);
	if (povisilica) {
		ret = ret + "# ";
	}
	else {
		//std::cout << " ";
		ret = ret + " ";
	}
	return ret;
}

bool Nota::promeniOktavuZa(int i) {
	oktava += i;
	if (oktava > 6)
		oktava = 6;
	if (oktava < 2)
		oktava = 2;
	return true;
}

std::string Nota::getStr() { //format: C#3 ili C3
	std::string s;
	switch (visina)
	{
	case Nota::C:
		s.append("C");
		break;
	case Nota::D:
		s.append("D");
		break;
	case Nota::E:
		s.append("E");
		break;
	case Nota::F:
		s.append("F");
		break;
	case Nota::G:
		s.append("G");
		break;
	case Nota::A:
		s.append("A");
		break;
	case Nota::B:
		s.append("B");
		break;
	}
	if (povisilica)
		s.append("#");
	s.append(std::to_string(oktava));
	return s;
}


std::ostream& Nota::ispis(std::ostream& out) const {
	out << "Oktava: " << oktava << " Visina: "; 
	switch (visina)
	{
	case Nota::C:
		out << "C";
		break;
	case Nota::D:
		out << "D";
		break;
	case Nota::E:
		out << "E";
		break;
	case Nota::F:
		out << "F";
		break;
	case Nota::G:
		out << "G";
		break;
	case Nota::A:
		out << "A";
		break;
	case Nota::B:
		out << "B";
		break;
	}		
	return out << " Trajanje: " << trajanje << " Broj takta: " << brojTakta << " Povisilica: " << povisilica;
}
