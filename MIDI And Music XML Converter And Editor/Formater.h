#ifndef FORMATER_H
#define FORMATER_H
#include "MidiFile.h"
#include "Kompozicija.h"

class Formater
{
public:
	Formater();
	virtual ~Formater();
	virtual void konvertuj(Mapa& mapa, Kompozicija* kompozicija) = 0;
	//virtual void konvertujMIDI(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> melody, std::pair<std::vector<int>, std::vector<int>> rhytham) = 0;
	//virtual void konvertujMXML(std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> opis, std::pair<std::vector<int>, std::vector<int>> rhytham, Kompozicija* k) = 0;

};

#endif