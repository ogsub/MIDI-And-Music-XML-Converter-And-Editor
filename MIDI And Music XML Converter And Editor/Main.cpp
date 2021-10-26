
#include "Kompozicija.h"
#include "Mapa.h"
#include "MIDIFormater.h"
#include "MXMLFormater.h"
#include "BMPFormater.h"

int main(int argc, char *argv[]) {	//ime programa, putanja mape, putanja kompozicije, brojilac, imenilac
	bool konvertovano = false;
	int meni;
	bool kraj = false;
	Mapa mapa;
	Kompozicija kompozicija;

	std::vector<int> istorija;
	mapa.otvoriMapu(argc, argv);
	mapa.popuniTabeluKljucSlovo();
	std::cout << "-----------------\n";
	mapa.popuniTabeluKljucNota();
	
	bool otvorena = false;
	bool postavljenSimbol = false;

	while (!kraj) {

		std::cout << "Meni\n"
			"1. Ucitavanje kompozicije\n"
			"2. Prikazi prvi simbol\n"
			"3. Nota napred\n"
			"4. Nota nazad\n"
			"5. Takt napred\n"
			"6. Takt nazad\n"
			"7. Ispisi simbole iz violinskog i bas sistema\n"
			"8. Promeni oktavu kompozicije\n"
			"9. Promeni takt kompozicije\n"
			"10. Postavi povisilicu\n"
			"11. Promeni oktavu note\n"
			"12. Konvertuj u MIDI fajl\n"
			"13. Konvertuj u MXML fajl\n"
			"14.Ukloni povisilicu\n"
			"15 Generisi BMI fajl\n"
			"16. Izadji\n";
		
		std::cin >> meni;
		
		switch (meni) {
		case 1:
			if (!otvorena) {
				konvertovano = false;
				kompozicija.otvoriKompoziciju(argc, argv);
				kompozicija.ispisiFajl();
				kompozicija.setTrajanjeTakta(argc, argv);
				kompozicija.ucitajNote(mapa);
				kompozicija.popuniPauzama();
				otvorena = true;
			}
			else
				std::cerr << "Kompozicija je vec otvorena\n";
			break;
		case 2:
			if (otvorena) {
				istorija.push_back(2);
				kompozicija.prikaziPrviSimbol();
				postavljenSimbol = true;
			}
			else
				std::cerr << "Kompozicija nije otvorena\n";
			break;
		case 3:
			if (!postavljenSimbol && otvorena) {
				istorija.push_back(3);
				kompozicija.prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			istorija.push_back(3);
			kompozicija.nextSimbol();
			kompozicija.ispisiSimbol();
			break;
		case 4:
			if (!postavljenSimbol && otvorena) {
				istorija.push_back(4);
				kompozicija.prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if(!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			istorija.push_back(4);
			kompozicija.prewSimbol();
			kompozicija.ispisiSimbol();
			break;
		case 5:
			if (!postavljenSimbol && otvorena) {
				istorija.push_back(5);
				kompozicija.prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if(!otvorena){
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			istorija.push_back(5);
			kompozicija.nextTakt();
			kompozicija.ispisiSimbol();
			break;
		case 6:
			if (!postavljenSimbol && otvorena) {
				istorija.push_back(6);
				kompozicija.prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			istorija.push_back(6);
			kompozicija.prewTakt();
			kompozicija.ispisiSimbol();
			break;
		case 7:
			if (otvorena) {
				istorija.push_back(7);
				kompozicija.ispisLinijskihSistema();
			}
			else
				std::cerr << "Kompozicija nije otvorena\n";
			break;
		case 8:
		{
			if (otvorena) {
				istorija.push_back(8);
				konvertovano = false;
				int i;
				std::cout << "Za koliko zelis da promenis oktavu? ";
				std::cin >> i;
				istorija.push_back(i);
				kompozicija.pomeranjeKompozicije(i);
			}
			else std::cerr << "Kompozicija nije otvorena\n";
			break;
		}
		case 9:
			if (otvorena) {
				konvertovano = false;
				kompozicija.izmeniTakt(mapa, istorija, konvertovano, postavljenSimbol);
			}
			else std::cerr << "Kompozicija nije otvorena\n";
			break;
		case 10:
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				istorija.push_back(10);
				kompozicija.prikaziPrviSimbol(false);
				postavljenSimbol = true;
				konvertovano = false;
				kompozicija.setPovisilica();
			}
			else {
				istorija.push_back(10);
				konvertovano = false;
				kompozicija.setPovisilica();
			}
			break;
		case 11:
		{
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				istorija.push_back(11);
				kompozicija.prikaziPrviSimbol(false);
				postavljenSimbol = true;
				int i;
				std::cout << "Unesi broj za koji zelis da izmenis oktavu\n";
				std::cin >> i;
				istorija.push_back(i);
				kompozicija.izmeniOktavuNote(i);
			}
			break;
		}
		case 12:
		{
			if (otvorena) {
				konvertovano = true;
				std::unique_ptr<Formater> midi = std::make_unique<MIDIFormater>();
				midi->konvertuj(mapa, &kompozicija);
			}
			else std::cout << "Kompozicija nije otvorena\n";
			break;
		}
		case 13:
		{
			if (otvorena) {
				konvertovano = true;
				std::unique_ptr<Formater> mxml = std::make_unique<MXMLFormater>();
				mxml->konvertuj(mapa, &kompozicija);
			}
			else std::cout << "Kompozicija nije otvorena\n";
			break;
		}
		case 14: {
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				istorija.push_back(10);
				kompozicija.prikaziPrviSimbol(false);
				postavljenSimbol = true;
				konvertovano = false;
				kompozicija.ukliniPovisilicu();
			}
			else {
				istorija.push_back(10);
				konvertovano = false;
				kompozicija.ukliniPovisilicu();
			}
			break;
		}
		case 15:
		{
			if (otvorena) {
				int visina;
				int sirina;
				std::cout << "Unesi visinu\n";
				std::cin >> visina;
				std::cout << "Unesi sirinu\n";
				std::cin >> sirina;
				konvertovano = true;
				std::unique_ptr<BMPFormater> bmp = std::make_unique<BMPFormater>(visina, sirina);
				bmp->postaviPiksel(sirina / 2, visina / 2, 255, 255, 255);
				bmp->ispisi("out.bmp");
			}
			else std::cout << "Kompozicija nije otvorena\n";
			break;
		}
		case 16:
		{
			kraj = true;
			if (!konvertovano) {
				int optIzlaz = 0;
				while (optIzlaz != 3) {
					std::cout << "Nije nista konvertovano od zadnje promene.\n"
						"1. Konvertuj u MIDI fajl\n"
						"2. Konvertuj u MXML fajl\n"
						"3. Napusti program\n";
					std::cin >> optIzlaz;

					switch (optIzlaz)
					{
					case 1:
					{
						std::unique_ptr<Formater> midi = std::make_unique<MIDIFormater>();
						midi->konvertuj(mapa, &kompozicija);
						exit(0);
						break;
					}
					case 2:
					{
						std::unique_ptr<Formater> mxml = std::make_unique<MXMLFormater>();
						mxml->konvertuj(mapa, &kompozicija);
						exit(0);
						break;
					}
					}
				}
			}
			break;
		}
		}
	}
}
