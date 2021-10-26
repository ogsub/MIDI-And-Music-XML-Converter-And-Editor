#ifndef KOMPOZICIJA_H
#define KOMPOZICIJA_H
#include "LinijskiSistem.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include "Mapa.h"
#include "Pauza.h"
#include "Nota.h"
#include "Simbol.h"
#include <iomanip>
#include <memory>

class Kompozicija {
public:
	Kompozicija();
	~Kompozicija();
	std::ifstream& getKompozicija();
	void otvoriKompoziciju(int argc, char* argv[]);
	void ispisiFajl();
	void setTrajanjeTakta(int argc, char* argv[]);
	void setTrajanjeTakta();
	std::vector<LinijskiSistem>& getVctLinSis();
	Trajanje& getTrajanjeTakta();
	void ucitajNote(Mapa& mapa, int menjajOktavu = 0);
	void prikaziPrviSimbol(bool ispis = true);
	void prikaziZadnjiSimbol(bool ispis = true);
	void nextSimbol();
	void prewSimbol();
	void nextTakt();
	void prewTakt();
	void ispisLinijskihSistema();
	void ispisiSimbol();
	void pomeranjeKompozicije(int i);
	void setPovisilica();
	void ukliniPovisilicu();
	void izmeniTakt(Mapa& mapa, std::vector<int>& istorija_, bool& konvertovano_, bool& postavljenSimbol_);
	void izmeniOktavuNote(int i);
	void popuniPauzama();
	std::pair<std::vector<std::string>, std::vector<std::string>> getOpisSimbola();
	std::pair < std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> getOpisVektoraSimbola();
	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> getMelody(Mapa* mapa_);
	std::pair<std::vector<int>, std::vector<int>> getRhithm();
private:
	std::vector<LinijskiSistem> vctLinSis;
	std::string putanja;
	std::ifstream kompozicija;
	Trajanje trajanjeTakta;

	void pomocSlovo(Mapa* mapa_, LinijskiSistem &linijskiSis, char slovo_, bool nota_, bool kratka);
	void pomocSlova(Mapa * mapa_, std::vector<std::shared_ptr<Simbol>>& leva, std::vector<std::shared_ptr<Simbol>>& desna);
	void uvecajOktavu(int i); //MOZDA TREBA DA SE BRISE
};

#endif