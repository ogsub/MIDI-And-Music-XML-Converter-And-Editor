
#ifndef MXMLFormater_H
#define MXMLFormater_H
#include "Formater.h"
#include <fstream>
#include <algorithm>

class MXMLFormater : public Formater {
public:
	MXMLFormater();
	~MXMLFormater();

	void konvertuj(Mapa& mapa, Kompozicija* kompozicija) override;
	
private:
	void konvertujMXML(std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> opisi, std::pair<std::vector<int>, std::vector<int>> rhytham, Kompozicija* k);
	void dodajNotu(bool poslednji, bool poslednjiSimultan, std::vector<std::string>::iterator s, std::ostream& oFile, int& cnt, int& i, std::vector<int>& trajanja, bool prekinuta, bool& PrviDrugiDeo, Razlomak& trenutno, Razlomak& trajanjeTakta, std::vector<std::string>& vectorPrekinutih, std::vector<std::string>& sVct, std::vector<std::vector<std::string>>);
	void noteHeader(std::ostream& file, bool DesnaLeva, int brojilac, int imenilac);
	void writeHeader(std::ofstream& file);
	const std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
		"<!DOCTYPE score-partwise PUBLIC\n"
		"\"-//Recordare//DTD MusicXML 3.1 Partwise//EN\"\n"
		"\"http://www.musicxml.org/dtds/partwise.dtd \">\n"
		"<score-partwise version=\"3.1\">\n"
		"<part-list>\n"
		"<score-part id =\"Right\"></score-part>\n"
		"<score-part id =\"Left\"></score-part>\n"
		"</part-list>\n";
};

#endif
