#include "LinijskiSistem.h"



LinijskiSistem::LinijskiSistem() {
	vctTakt.reserve(25);
	///////////////
	iterTakt = vctTakt.begin();
	/////////////
}


LinijskiSistem::~LinijskiSistem() {
}

void LinijskiSistem::dodajTakt(Takt & t){
	vctTakt.push_back(t);
}

void LinijskiSistem::dodajTakt(Takt && t) {
	vctTakt.push_back(t);
}

void LinijskiSistem::zapocniIterNote() {
	
}

void LinijskiSistem::setIterTakt(std::vector<Takt>::iterator t) {
	iterTakt = t;
}

std::vector<Takt>& LinijskiSistem::getVctTakt() {
	return vctTakt;
}

std::vector<Takt>::iterator& LinijskiSistem::getIterTakt() {
	return iterTakt;
}
