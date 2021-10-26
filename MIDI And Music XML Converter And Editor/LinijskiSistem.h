#ifndef LINIJSKISISTEM_H
#define LINIJSKISISTEM_H

#include "Takt.h"

class LinijskiSistem {
public:
	LinijskiSistem();
	//LinijskiSistem(const LinijskiSistem&) { std::cout << "POZVAN1"; }
	//LinijskiSistem(const LinijskiSistem&&) { std::cout << "POZVAN2"; }
	~LinijskiSistem();

	void dodajTakt(Takt& t);
	void dodajTakt(Takt && t);
	void zapocniIterNote(); //implementiraj
	void setIterTakt(std::vector<Takt>::iterator t);
	std::vector<Takt>& getVctTakt();
	std::vector<Takt>::iterator& getIterTakt();
	
private:
	std::vector<Takt> vctTakt;
	std::vector<Takt>::iterator iterTakt;
};

#endif