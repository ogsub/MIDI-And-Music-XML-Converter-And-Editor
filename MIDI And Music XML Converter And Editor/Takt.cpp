#include "Takt.h"


Trajanje Takt::trajanje;
int Takt::count;

Takt::Takt(Trajanje t):zauzeto(0,t.getRazlomak().getImenilac()) {
	trajanje = t;
	vctVctSimbol.reserve(100);
	popunjen = false;
	brojTakta = ++count;
	iterVectSimb = vctVctSimbol.begin();
}


Takt::~Takt() {
}

void Takt::dodajSimbol(std::shared_ptr<Simbol> simbol_) {
	std::vector<std::shared_ptr<Simbol>> temp;
	temp.push_back(simbol_);
	vctVctSimbol.push_back(temp);
	zauzeto = zauzeto + simbol_->getTrajanje().getRazlomak();
	if (zauzeto == trajanje.getRazlomak())
		popunjen = true;
}
///////////////////////
/*void Takt::dodajSimbol(Simbol && simbol_) {
	std::vector<Simbol> temp;
	temp.push_back(simbol_);
	vctVctSimbol.push_back(temp);
	zauzeto = zauzeto + simbol_.getTrajanje().getRazlomak();
	if (zauzeto == trajanje.getRazlomak())
		popunjen = true;
}*/
///////////////////////
void Takt::dodajSimbole(std::vector<std::shared_ptr<Simbol>>& simboli_) {
	std::for_each(simboli_.begin(), simboli_.end(), [this](std::shared_ptr<Simbol>& s) {
		s->setBrojTakta(getBrTakta());
	});
	vctVctSimbol.push_back(simboli_);
	zauzeto = zauzeto + simboli_.back()->getTrajanje().getRazlomak();
	if (zauzeto == trajanje.getRazlomak())
		popunjen = true;
}

bool Takt::imaMesta(std::shared_ptr<Simbol> simbol_) {
	return trajanje.getRazlomak() > (simbol_->getTrajanje().getRazlomak() + zauzeto);
}

bool Takt::getPopunjen() {
	return popunjen;
}

int Takt::getBrTakta() {
	if (brojTakta % 2)
		return brojTakta / 2 + 1;
	else
		return brojTakta / 2;
}

int Takt::getStvarniBrTakta() {
	return brojTakta;
}

Razlomak& Takt::getZauzeto() {
	return zauzeto;
}

std::vector<std::vector<std::shared_ptr<Simbol>>>& Takt::getVctSimbola() {
	return vctVctSimbol;
}

std::vector<std::vector<std::shared_ptr<Simbol>>>::iterator & Takt::getIterVectSimb() {
	return iterVectSimb;
}

std::vector<std::shared_ptr<Simbol>>::iterator & Takt::getIterSimbol() {
	return iterSimbol;
}

void Takt::setIterVectorSimbol(std::vector<std::vector<std::shared_ptr<Simbol>>>::iterator iterator_) {
	iterVectSimb = iterator_;
}

void Takt::setIterSimbol(std::vector<std::shared_ptr<Simbol>>::iterator iterator_) {
	iterSimbol = iterator_;
}

Trajanje & Takt::getTrajanje() {
	return trajanje;
}
