#ifndef NOTA_H
#define NOTA_H
#include "Simbol.h"
#include <vector>
#include <string>

class Nota : public Simbol {
public:
	enum Visina { C, D, E, F, G, A, B };

	Nota(int oktava_, Visina visina_, bool povisilica_ = false);
	Nota(int oktava_, Visina visina_ ,Razlomak trajanje_, bool povisilica_ = false);
	
	~Nota();
	
	void postaviPovisilicu() override;
	void ukloniPovisilicu() override;
	void setOktava(int oktava_);
	void setVisina(Visina visina_);
	int getOktava() override;
	Visina getVisina();
	std::string ispisivanjePodataka() override;
	bool promeniOktavuZa(int i) override;
	std::string getStr() override;

private:
	int oktava;
	Visina visina;
	bool povisilica;

	std::ostream& ispis(std::ostream& out) const override;
};

#endif