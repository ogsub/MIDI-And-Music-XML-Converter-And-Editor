

#include "MXMLFormater.h"



MXMLFormater::MXMLFormater()
{
}


MXMLFormater::~MXMLFormater()
{
}

void MXMLFormater::konvertuj(Mapa & mapa, Kompozicija * kompozicija) {
	konvertujMXML(kompozicija->getOpisVektoraSimbola(), kompozicija->getRhithm(), kompozicija);
}

void MXMLFormater::konvertujMXML(std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> opisi, std::pair<std::vector<int>, std::vector<int>> rhytham, Kompozicija* kompozicija) {
	std::ofstream oFile;
	oFile.open("out.mxml");
	writeHeader(oFile);
	
	std::vector<int> trajanja = rhytham.first;
	std::vector<std::vector<std::string>> jedanOpis = opisi.first;
	bool leva = false;
	Razlomak trajanjeTakta = kompozicija->getTrajanjeTakta().getRazlomak();

	for (int j = 0; j < 2; j++) {
		Razlomak trenutno(0, 8);
		noteHeader(oFile, leva, kompozicija->getTrajanjeTakta().getRazlomak().getBrojilac(), kompozicija->getTrajanjeTakta().getRazlomak().getImenilac());
		int i = 0;
		//std::for_each(jedanOpis.begin(), jedanOpis.end(), [&oFile, &i, &trajanja, this, &trenutno, &trajanjeTakta, &jedanOpis](std::vector<std::string> sVct) {
			for (auto sVct = jedanOpis.begin(); sVct != jedanOpis.end(); sVct++) {
				bool poslednji = false;
				if (sVct == jedanOpis.end() - 1)
					poslednji = true;
				std::vector<std::string> vektorPrekinutih;
				int chordCnt = 0;
				bool prekinuta = false;
				bool PrviDrugiDeo = true; //da znam da li je prvi ili drugi deo prekinute
				//std::for_each(sVct.begin(), sVct.end(), [&vektorPrekinutih, &oFile, &chordCnt, &i, &trajanja, &prekinuta, &PrviDrugiDeo, this, &trenutno, &trajanjeTakta](std::string s) {
				for (std::vector<std::string>::iterator it = (*sVct).begin(); it != (*sVct).end(); it++) {
					bool poslednjiSimultan = false;
					if (it == (*sVct).end() - 1)
						poslednjiSimultan = true;
					dodajNotu(poslednji, poslednjiSimultan, it, oFile, chordCnt, i, trajanja, prekinuta, PrviDrugiDeo, trenutno, trajanjeTakta, vektorPrekinutih, (*sVct), jedanOpis);
				}
				//});
				if (vektorPrekinutih.size() > 0)
					i = i - 1;
				chordCnt = 0;
				for (std::vector<std::string>::iterator it = vektorPrekinutih.begin(); it != vektorPrekinutih.end(); it++) {
					bool poslednjiSimultan = false;
					if (it == vektorPrekinutih.end() - 1)
						poslednjiSimultan = true;
					dodajNotu(poslednji, poslednjiSimultan, it, oFile, chordCnt, i, trajanja, prekinuta, PrviDrugiDeo, trenutno, trajanjeTakta, vektorPrekinutih, vektorPrekinutih, jedanOpis);
				}
			}
		//});
		jedanOpis = opisi.second;
		leva = true;
		oFile << //"</measure>\n"
			"</part>\n";
	}
	oFile << "</score-partwise>\n";
	
	return;
}

//poslednji sluzi da znam da li je gotov poslednji od simultanih pre nego sto treba da stavim takt
//poslednjiSimultan sluzi za slucaj kada je poslednji u taktu onaj koji je drugi deo simultanih (npr kad takt traje 1/8)
void MXMLFormater::dodajNotu(bool poslednji, bool poslednjiSimultan, std::vector<std::string>::iterator s, std::ostream & oFile, int &chordCnt, int & i, std::vector<int>& trajanja, bool prekinuta, bool& PrviDrugiDeo, Razlomak& trenutno, Razlomak& trajanjeTakta, std::vector<std::string>& vektorPrekinutih, std::vector<std::string>& sVct, std::vector<std::vector<std::string>> vektorVektora) {
	//bool prekinuta = false;
	if (!PrviDrugiDeo && s == sVct.begin()) {//prekinuta && !PrviDrugiDeo) {
		oFile << "</measure>\n"
			"<measure>\n";
		trenutno = Razlomak(0, 8);
	}
	
	oFile << "<note>\n";

	if (chordCnt) {
		oFile << "<chord/>\n";
		i--; //jer uzima trajanje od proslog
	}
	chordCnt++;
	
	if (*s != "0") {
		oFile << "<pitch>\n"
			"<step>" << (*s)[0] << "</step>\n";
		if (s->length() == 3) {
			oFile << "<octave>" << (*s)[2] << "</octave>\n";
		}
		else if (s->length() == 2) {
			oFile << "<octave>" << (*s)[1] << "</octave>\n";
		}

		if (s->length() == 3) {
			oFile << "<alter>" << 1 << "</alter>\n";
		}
		oFile << "</pitch>\n";
	}
	else {
		oFile << "<rest/>\n";
	}

	oFile << "<duration>";

	if (trajanja[i] == 3 ) { //ako je 3 znaci da je prekinuta
		oFile << 1 << "</duration>\n"; //ovde je bilo 2 za 1/4, ne znam zasto sam to stavio, ali tako je bilo, sad mi je logicnije da bude 1, za 1/8
		if (PrviDrugiDeo) {
			prekinuta = true;
			vektorPrekinutih.push_back(*s);
		}
		if(!(chordCnt - 1))
			trenutno = trenutno + Razlomak(1, 8);
	}
	else if (trajanja[i] == 2) {
		oFile << trajanja[i] << "</duration>\n";
		if(!(chordCnt - 1))
			trenutno = trenutno + Razlomak(1, 4);
	}
	else if (trajanja[i] == 1) {
		oFile << trajanja[i] << "</duration>\n";
		if(!(chordCnt - 1))
			trenutno = trenutno + Razlomak(1, 8);
	}
	/*
	if (prekinuta && !PrviDrugiDeo) {
		oFile << "</measure>\n"
			"<measure>";
		trenutno = Razlomak(0, 8);
	}
	*/
	i++;
	if (prekinuta) { //&& PrviDrugiDeo) {
		oFile << "<tie type=\"start\"/>\n";
		oFile << "</note>\n";
		if(s == (sVct.end() - 1))
			PrviDrugiDeo = false;
		//dodajNotu(s, oFile, --chordCnt, --i, trajanja, prekinuta, PrviDrugiDeo, trenutno, trajanjeTakta);
		return;
	}
	else if (!PrviDrugiDeo) { //&& s == (sVct.end() - 1)) {//prekinuta && !PrviDrugiDeo) {
		oFile << "<tie type=\"end\"/>\n";
		//PrviDrugiDeo = true;
	}
	
	oFile << "</note>\n";

	if ((trenutno == trajanjeTakta && sVct.size() == 1)) {
		oFile << "</measure>\n";
		if(!poslednji)	//find(vektorVektora.begin(), vektorVektora.end(), sVct) != (vektorVektora.end() - 1))					//if(sVct != *(vektorVektora.end() - 1)) // ovde moze potencijalno da dodje do greske ali su male sanse
			oFile << "<measure>\n";
		trenutno = Razlomak(0, 8);
	}
	else if (trenutno == trajanjeTakta && poslednjiSimultan) {
		oFile << "</measure>\n";
		if (!poslednji)	//find(vektorVektora.begin(), vektorVektora.end(), sVct) != (vektorVektora.end() - 1))					//if(sVct != *(vektorVektora.end() - 1)) // ovde moze potencijalno da dodje do greske ali su male sanse
			oFile << "<measure>\n";
		trenutno = Razlomak(0, 8);
	}
	/*else if(trenutno == trajanjeTakta && PrviDrugiDeo) {
		oFile << "</measure>\n";
		if (!poslednji)	//find(vektorVektora.begin(), vektorVektora.end(), sVct) != (vektorVektora.end() - 1))					//if(sVct != *(vektorVektora.end() - 1)) // ovde moze potencijalno da dodje do greske ali su male sanse
			oFile << "<measure>\n";
		trenutno = Razlomak(0, 8);
	}*/
}

void MXMLFormater::noteHeader(std::ostream & file, bool DesnaLeva, int brojilac, int imenilac) {
	file<<"<part id=";
	if (DesnaLeva)
		file << "\"Right\"";
	else
		file << "\"Left\"";
	file << ">\n"
		"<measure>\n"
		"<attributes>\n"
		"<divisions>2</divisions>\n"
		"<time>\n"
		"<beats>"; 
	file << brojilac;
	file<< "</beats>\n"
		"<beat-type>";
	file << imenilac;
	file << "</beat-type>\n"
		"</time>\n"
		"<clef>\n"
		"<sign>";
	if (DesnaLeva) {
		file << "G</sign>\n"
			"<line>2</line >\n";
	}
	else {
		file <<"F</sign>\n"
			"<line>4</line >\n";
	}
		file << "</clef>\n"
		"</attributes>\n";
}

void MXMLFormater::writeHeader(std::ofstream& file) {
	file << header;
}
