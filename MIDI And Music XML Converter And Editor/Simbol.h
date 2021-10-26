#ifndef SIMBOL_H
#define SIMBOL_H

#include "Trajanje.h"

class Simbol {
public:
	Simbol();
	Simbol(const Trajanje& trajanje_);
	virtual ~Simbol();

	void setPrekinuta(bool prekinuta_ = true);
	bool getPrekinuta();
	void setTrajanje(Trajanje);
	void setBrojTakta(int brojTakta_);
	Trajanje& getTrajanje();
	int getBrojTakta();
	virtual std::ostream& ispis(std::ostream& out) const { return out; }
	virtual std::string ispisivanjePodataka() { return ""; }
	virtual bool promeniOktavuZa(int i) { return true; }
	virtual void postaviPovisilicu() {}
	virtual void ukloniPovisilicu() {}
	virtual int getOktava() { return 0; }
	virtual std::string getStr() { return ""; }

	friend std::ostream& operator<<(std::ostream& out, const Simbol& simbol);
protected:
	Trajanje trajanje;
	bool prekinuta;
	int brojTakta;
	
	
	
};

#endif