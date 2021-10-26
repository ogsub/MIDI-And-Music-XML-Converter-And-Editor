#ifndef MIDIFORMATER_H
#define MIDIFORMATER_H
#include "LinijskiSistem.h"
#include "Formater.h"

class MIDIFormater: public Formater {
public:
	MIDIFormater();
	~MIDIFormater();
	void konvertuj(Mapa& mapa, Kompozicija* kompozicija) override;
	//void konvertujMXML(std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> opis, std::pair<std::vector<int>, std::vector<int>> rhytham, Kompozicija* k) {}
private:
	void konvertujMIDI(std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> melody, std::pair<std::vector<int>, std::vector<int>> rhytham);
};

#endif