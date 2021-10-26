#include "Mapa.h"
#include <iostream>;


Mapa::Mapa()
{
}


Mapa::~Mapa() {
	mapCSV.close();
}

void Mapa::otvoriMapu(int argc, char *argv[]) {
	if (argc == 1) {
		std::cerr << "Putanja do mape nije navedena, unesite je naknadno(zajedno sa ekstenzijom).\n";
		//std::getline(std::cin, putanjaMap); //ovo sam zakomentirao dok testiram da imam uvek tacnu putanju
		//putanjaMap = "C:\\Users\\Ognjen\\Desktop\\POOPzad\\map.csv";
		putanjaMap = argv[1];
		//putanjaMap = "C:\\Users\\Ognjen\\Desktop\\New Text Document.txt";
	}
	else
		putanjaMap = argv[1];

	mapCSV.open(putanjaMap);

	while (mapCSV.fail()) {
		std::cerr << "Losa putanja do mape.\n";
		std::cout << "Upisati putanju do mape.\n";
		std::getline(std::cin, putanjaMap);
		mapCSV.open(putanjaMap);
	}
}

std::ifstream& Mapa::getMapFile() {
	return mapCSV;
}

void Mapa::popuniTabeluKljucSlovo () {
	mapCSV.clear();                
	mapCSV.seekg(0, std::ios::beg);
	std::string s;
	std::regex rex("([0-9a-zA-Z]?[@$%^*(.!?-]?),([CDEFGAB]#?[0-9]),([0-9][0-9])");
	while (std::getline(mapCSV, s)) {
		std::smatch match;
		if (std::regex_match(s, match, rex)) {
			////////////////////////////////////////////////////
			std::cout<<match.str(1) << " ";
			std::cout << match.str(2) << " ";
			std::cout << match.str(3) << " ";
			std::cout << "\n";
			////////////////////////////////////////////////////
			notaMidi pomocna = notaMidi(match.str(2), std::stoi(match.str(3)));
			hashKljucSlovo.insert(std::pair<char, notaMidi>(match.str(1)[0], pomocna));
		}
	}
}

void Mapa::popuniTabeluKljucNota() {
	mapCSV.clear();
	mapCSV.seekg(0, std::ios::beg);
	std::string s;
	std::regex rex("([0-9a-zA-Z]?[@$%^*(.!?-]?),([CDEFGAB]#?[0-9]),([0-9][0-9])"); //
	while (std::getline(mapCSV, s)) {
		std::smatch match;
		if (std::regex_match(s, match, rex)) {
			////////////////////////////////////////////////////
			std::cout << match.str(1) << " ";
			std::cout << match.str(2) << " ";
			std::cout << match.str(3) << " ";
			std::cout << "\n";
			////////////////////////////////////////////////////
			slovoMidi pomocna = slovoMidi(match.str(1)[0], std::stoi(match.str(3))); //match.str(1)[0] je prvi match(slovo koje matchuje kao str, pa zato uzimamo sa [0] prvo slovo
			hashKljucNota.insert(std::pair<std::string, slovoMidi>(match.str(2), pomocna));
		}
	}
}

void Mapa::ispisHash() {
	for (auto it = hashKljucSlovo.begin(); it != hashKljucSlovo.end(); it++) { //auto je tipa std::map<char, notaMidi>::iterator
		std::cout << " first " << it->first << " second " << it->second.opisNote << " " << it->second.midiBroj << std::endl;
	}
}

std::map<char, Mapa::notaMidi>& Mapa::getHash() {
	return hashKljucSlovo;
}

std::string& Mapa::getOpisNote(char kljuc_)
{
	if (hashKljucSlovo.find(kljuc_) != hashKljucSlovo.end())
		return hashKljucSlovo.find(kljuc_)->second.opisNote;
	else
		return hashKljucSlovo.find('f')->second.opisNote;
}

Nota::Visina Mapa::getVisina(const std::string& opisNote_) {
	switch (opisNote_[0]) {
	case 'C':
		return Nota::C;
	case 'D':
		return Nota::D;
	case 'E':
		return Nota::E;
	case 'F':
		return Nota::F;
	case 'G':
		return Nota::G;
	case 'A':
		return Nota::A;
	case 'B':
		return Nota::B;
	default:
		std::cerr << "Kriticna greska. Mapa nije validna.";
		exit(-1);
	}
}

int Mapa::getOktava(const std::string& opisNote_) {
	if (opisNote_.length() == 3)
		return (int)opisNote_[2] - 48;
	else
		return (int)opisNote_[1] - 48;
}

bool Mapa::getPovisilica(const std::string& opisNote_) {
	if (opisNote_.length() == 3 && opisNote_[1] == '#')
		return true;
	return false;
}

int Mapa::getMidiBroj(char kljucSlovo_) {
	return hashKljucSlovo.find(kljucSlovo_)->second.midiBroj;
}

int Mapa::getMidiBroj(std::string kljucStr_) {
	return hashKljucNota.find(kljucStr_)->second.midiBroj;
}

int Mapa::getSlovo(std::string kljucStr_) {
	return hashKljucNota.find(kljucStr_)->second.slovo;
}
