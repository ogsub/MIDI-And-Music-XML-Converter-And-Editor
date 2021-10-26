#ifndef PAUZA_H
#define PAUZA_H
#include "Simbol.h"
class Pauza : public Simbol {
public:
	Pauza() = default;
	Pauza(Razlomak trajanje);
	~Pauza();
	
	void ukloniPovisilicu() override {}
	bool promeniOktavuZa(int i) override;
	std::string ispisivanjePodataka() override;
	std::string getStr() override;
private:
	std::ostream& ispis(std::ostream& out) const override;
};

#endif