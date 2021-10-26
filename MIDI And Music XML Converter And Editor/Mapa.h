#ifndef MAPA_H
#define MAPA_H
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include "Nota.h"

class Mapa
{
private:
	struct notaMidi {
		std::string opisNote;
		int midiBroj;

		notaMidi(std::string opis_, int midi_) {
			opisNote = opis_;
			midiBroj = midi_;
		}
	};
	
	struct slovoMidi {
		char slovo;
		int midiBroj;

		slovoMidi(char slovo_, int midiBroj_) {
			slovo = slovo_;
			midiBroj = midiBroj_;
		}
	};
public:
	Mapa();
	~Mapa();

	void otvoriMapu(int argc, char *argv[]);
	std::ifstream& getMapFile();
	void popuniTabeluKljucSlovo();
	void popuniTabeluKljucNota();
	void ispisHash();
	std::map<char, notaMidi>& getHash();
	std::string& getOpisNote(char kljuc_);
	Nota::Visina getVisina(const std::string& opisNote_);
	int getOktava(const std::string& opisNote_);
	bool getPovisilica(const std::string& opisNote_);
	int getMidiBroj(char kljucSlovo_);
	int getMidiBroj(std::string kljucStr);
	int getSlovo(std::string kljucStr);

private:
	std::ifstream mapCSV;
	std::string putanjaMap;
	std::map<char, notaMidi> hashKljucSlovo;
	std::map<std::string, slovoMidi> hashKljucNota;
};

#endif