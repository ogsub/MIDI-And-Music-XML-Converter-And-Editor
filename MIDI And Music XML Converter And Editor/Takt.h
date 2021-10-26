#ifndef TAKT_H
#define TAKT_H

#include "Simbol.h"
#include <vector>
#include "Nota.h"
#include <algorithm>
#include <memory>


class Takt
{
public:
	Takt(Trajanje t);
	~Takt();

	void dodajSimbol(std::shared_ptr<Simbol> simbol_);
	//void dodajSimbol(Simbol && simbol_);
	void dodajSimbole(std::vector<std::shared_ptr<Simbol>>& simboli_); //koristimo ih za preklopljene
	bool imaMesta(std::shared_ptr<Simbol> simbol_);
	bool getPopunjen();
	int getBrTakta();
	int getStvarniBrTakta();
	Razlomak& getZauzeto();
	std::vector<std::vector<std::shared_ptr<Simbol>>>& getVctSimbola();
	std::vector<std::vector<std::shared_ptr<Simbol>>>::iterator& getIterVectSimb();
	std::vector<std::shared_ptr<Simbol>>::iterator& getIterSimbol();
	void setIterVectorSimbol(std::vector<std::vector<std::shared_ptr<Simbol>>>::iterator iterator_);
	void setIterSimbol(std::vector<std::shared_ptr<Simbol>>::iterator iterator_);
	Trajanje& getTrajanje();
	//void ucitajNote;


private:
	static int count;
	
	static Trajanje trajanje;
	Razlomak zauzeto; //kolka je trenutna popunjenost
	bool popunjen;
	int brojTakta;
	std::vector<std::vector<std::shared_ptr<Simbol>>> vctVctSimbol;
	std::vector<std::vector<std::shared_ptr<Simbol>>>::iterator iterVectSimb;
	std::vector<std::shared_ptr<Simbol>>::iterator iterSimbol;
	//implementiraj prelazak na sledecu notu i prelazak na prethodnu notu.
};


#endif
