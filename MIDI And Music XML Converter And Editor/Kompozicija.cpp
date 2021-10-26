#include "Kompozicija.h"
#include "Takt.h"

Kompozicija::Kompozicija() {
	vctLinSis.reserve(2);
	vctLinSis.emplace_back();
	vctLinSis.emplace_back();
}


Kompozicija::~Kompozicija() {
}

std::ifstream& Kompozicija::getKompozicija() {
	return kompozicija;
}

void Kompozicija::otvoriKompoziciju(int argc, char* argv[]) {
	if (kompozicija.is_open()) {
		std::cerr << "Postojeca kompozicija nije zatvorena";
		return;
	}
	bool tacnaPutanja = false;
	while (!tacnaPutanja) {		
		if (argc > 2)
			putanja = argv[2];

		kompozicija.open(putanja);

		if (!kompozicija.fail()) {
			tacnaPutanja = true;
		}
		else {
			std::cerr << "Uneta putanja kompozicije nije validna, pokusaj ponovo.\n";
			std::cin.clear(); // reset failbit
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::getline(std::cin, putanja);
			kompozicija.open(putanja);
			if (!kompozicija.fail()) {
				tacnaPutanja = true;
			}
		}
	}
}

void Kompozicija::ispisiFajl() {
	std::string s;
	
	while (!kompozicija.eof()) {
		kompozicija >> s;
		std::cout << s << " ";
	}

	kompozicija.seekg(0, std::ios::beg);
}

void Kompozicija::setTrajanjeTakta(int argc, char* argv[]) {
	int brojilac_;
	int imenilac_;

	if (argc > 4) {
		brojilac_ = (int)(*argv[3]) - 48;
		imenilac_ = (int)(*argv[4]) - 48;
	}
	else {
		std::cout << "\nUneti zeljenu vrednost takta\n"
			"Brojilac: ";

		std::cin >> brojilac_;
		std::cout << "Imenilac: ";
		
		std::cin >> imenilac_;
	}
	trajanjeTakta = Trajanje(Razlomak(brojilac_, imenilac_));
}

void Kompozicija::setTrajanjeTakta() {
	int brojilac_;
	int imenilac_;

	std::cout << "\nUneti zeljenu vrednost takta\n"
		"Brojilac: ";
	std::cin >> brojilac_;
	std::cout << "Imenilac: ";
	std::cin >> imenilac_;
	
	trajanjeTakta = Trajanje(Razlomak(brojilac_, imenilac_));
}

std::vector<LinijskiSistem>& Kompozicija::getVctLinSis() {
	return vctLinSis;
}

Trajanje& Kompozicija::getTrajanjeTakta() {
	return trajanjeTakta;
}

void Kompozicija::ucitajNote(Mapa& mapa, int menjajOktavu) {
	char slovo;
	std::string opis;
	kompozicija.clear();
	kompozicija.seekg(0, std::ios::beg);
	kompozicija >> std::noskipws;
	while (kompozicija >> slovo) {
		if (slovo == '\n') continue;
		switch (slovo) {
		case '[': //oznaka za vise nota [abS] 1/4 istovremeno ili [a b S] 1/8 svi jedno za drugim
		{
			bool razmak = false;
			std::vector<char> vctSlova;
			while (kompozicija >> slovo) {
				if (slovo == ']')
					break;
				vctSlova.push_back(slovo);
			}
			auto found = std::find(vctSlova.begin(), vctSlova.end(), ' ');
			if (found != vctSlova.end()) { //ako su nadjeni razmaci, upisujemo lepo note, kao kod normalnih slova, samo skracene(1/8) pa je flag skracene u pomocSLovo = true
				std::for_each(vctSlova.begin(), vctSlova.end(), [this, &mapa, menjajOktavu](char slovo) {
					if (slovo == ' ')
						return;
					if (mapa.getOktava(mapa.getOpisNote(slovo)) + menjajOktavu <= 3) {
						pomocSlovo(&mapa, *getVctLinSis().begin(), slovo, true, true); //upisemo ovde notu, skracenu(1/8)
						pomocSlovo(&mapa, getVctLinSis().back(), slovo, false, true); //upisemo ovde pauzu, skracenu(1/8)
					}
					else {
						pomocSlovo(&mapa, *getVctLinSis().begin(), slovo, false, true);
						pomocSlovo(&mapa, getVctLinSis().back(), slovo, true, true);
					}
				});
			}
			else { //slucaj kada se vise znakova svira u isto vreme(nema razmaka u vctSlova).
				std::vector<std::shared_ptr<Simbol>> simboliLeva;
				std::vector<std::shared_ptr<Simbol>> simboliDesna;

				std::for_each(vctSlova.begin(), vctSlova.end(), [this, &mapa, &simboliLeva, &simboliDesna, menjajOktavu](char slovo) {
					std::string opisNote = mapa.getOpisNote(slovo);
					//sva slova smo stavili u vektor nota za levu i za desnu ruku.
					if (mapa.getOktava(opisNote) + menjajOktavu <= 3) {
						std::shared_ptr<Nota> n = std::make_shared<Nota>(mapa.getOktava(opisNote), mapa.getVisina(opisNote), Razlomak(1, 4), mapa.getPovisilica(opisNote));
						simboliLeva.push_back(n);
					}
					else {
						std::shared_ptr<Nota> n = std::make_shared<Nota>(mapa.getOktava(opisNote), mapa.getVisina(opisNote), Razlomak(1, 4), mapa.getPovisilica(opisNote));
						simboliDesna.push_back(n);
					}
				});
				pomocSlova(&mapa, simboliLeva, simboliDesna);
			}
		}
			break;
		case ' ': //pauza 1/8
			std::for_each(vctLinSis.begin(), vctLinSis.end(), [this](LinijskiSistem& linijskiSis) {
				//Pauza p(Razlomak(1, 8));
				std::shared_ptr<Simbol> p = std::make_shared<Pauza>(Razlomak(1, 8));
				if (linijskiSis.getVctTakt().empty() || !linijskiSis.getVctTakt().back().imaMesta(p)) {
					Takt t1 = Takt(this->getTrajanjeTakta());
					linijskiSis.dodajTakt(t1);//ako je vektor taktova prazan, ili je poslednji vektor pun, dodamo novi takt u vctTakt;
				}
				p->setBrojTakta(linijskiSis.getVctTakt().back().getBrTakta());
				linijskiSis.getVctTakt().back().dodajSimbol(p);
			});
			break;
		case '|': //pauza 1/4

			std::for_each(vctLinSis.begin(), vctLinSis.end(), [this, &mapa](LinijskiSistem& linijskiSis) {
				pomocSlovo(&mapa, linijskiSis, '|', false, false);
			});

			break;
		default: //trazi notu i bradjuje je
			if (mapa.getOktava(mapa.getOpisNote(slovo)) + menjajOktavu <= 3) {
				pomocSlovo(&mapa, *getVctLinSis().begin(), slovo, true, false); //upisemo ovde notu, ne skracenu(1/4)
				pomocSlovo(&mapa, getVctLinSis().back(), slovo, false, false); //upisemo ovde pauzu, ne skracenu(1/4)
			}
			else {
				pomocSlovo(&mapa, *getVctLinSis().begin(), slovo, false, false);
				pomocSlovo(&mapa, getVctLinSis().back(), slovo, true, false);
			}
			break;
		}
	}
}

void Kompozicija::prikaziPrviSimbol(bool ispisi) {
	//inicijalizujemo iterator taktova
	vctLinSis.begin()->setIterTakt(vctLinSis.begin()->getVctTakt().begin());
	vctLinSis.back().setIterTakt(vctLinSis.back().getVctTakt().begin());
	//inicijalizujemo iterator vektora simbola
	vctLinSis.begin()->getIterTakt()->setIterVectorSimbol(vctLinSis.begin()->getVctTakt().begin()->getVctSimbola().begin());
	vctLinSis.back().getIterTakt()->setIterVectorSimbol(vctLinSis.back().getVctTakt().begin()->getVctSimbola().begin());
	//inicijalizovali smo iterator simbola
	vctLinSis.begin()->getIterTakt()->setIterSimbol(vctLinSis.begin()->getVctTakt().begin()->getVctSimbola().begin()->begin());
	vctLinSis.back().getIterTakt()->setIterSimbol(vctLinSis.back().getVctTakt().begin()->getVctSimbola().begin()->begin());
	//ispis tog simbola
	if(ispisi)
		ispisiSimbol();
}
void Kompozicija::prikaziZadnjiSimbol(bool ispisi) {
	//inicijalizujemo iterator taktova
	vctLinSis.front().setIterTakt(--vctLinSis.begin()->getVctTakt().end());
	vctLinSis.back().setIterTakt(--vctLinSis.back().getVctTakt().end());
	//inicijalizujemo iterator vektora simbola
	vctLinSis.begin()->getIterTakt()->setIterVectorSimbol(--((--vctLinSis.begin()->getVctTakt().end())->getVctSimbola().end()));
	vctLinSis.back().getIterTakt()->setIterVectorSimbol(--((--vctLinSis.back().getVctTakt().end())->getVctSimbola().end()));
	//inicijalizovali smo iterator simbola
	vctLinSis.begin()->getIterTakt()->setIterSimbol(vctLinSis.begin()->getVctTakt().begin()->getVctSimbola().begin()->begin());
	vctLinSis.back().getIterTakt()->setIterSimbol(vctLinSis.back().getVctTakt().begin()->getVctSimbola().begin()->begin());

	//ispis tog simbola
	if(ispisi)
		ispisiSimbol();
}

void Kompozicija::nextSimbol() {
	bool ispisi = true;
	std::for_each(vctLinSis.begin(), vctLinSis.end(), [this, &ispisi](LinijskiSistem& linSis) {		
		if (!ispisi) //znaci ako je usao tamo da treba da vrati na pocetak, ne treba da odradi jos jedno za sledeci linijski sistem(povratak na pocetak vraca iteratore i jednog i drugog linijskog sistema)
			return;
		int brPonavljanja = 1;
		for (int i = 0; i < brPonavljanja && brPonavljanja < 3; i++) {
			if (++linSis.getIterTakt()->getIterVectSimb() == linSis.getIterTakt()->getVctSimbola().end()) {
				if (++linSis.getIterTakt() == linSis.getVctTakt().end()) {
					std::cout << "Kretanje ispocetka\n" << std::endl;
					prikaziPrviSimbol(false);
					ispisi = false;
					return;
				}
				else {
					linSis.getIterTakt()->setIterVectorSimbol(linSis.getIterTakt()->getVctSimbola().begin());
					linSis.getIterTakt()->setIterSimbol(linSis.getIterTakt()->getIterVectSimb()->begin());
					
					if ((*linSis.getIterTakt()->getIterSimbol())->getPrekinuta())
						brPonavljanja++;
				}
			}
			else {
				linSis.getIterTakt()->setIterSimbol(linSis.getIterTakt()->getIterVectSimb()->begin()); // ne treba ++u argumentu jer se gore u ifu proverava moze li da se vektor simbola mrda i to ima bocni efekat
				if ((*linSis.getIterTakt()->getIterSimbol())->getPrekinuta())
					brPonavljanja++;
			}
		}
		
	});
}

void Kompozicija::prewSimbol() {
	bool ispisi = true;
	std::for_each(vctLinSis.begin(), vctLinSis.end(), [this, &ispisi](LinijskiSistem& linSis) {
		if (!ispisi) //znaci ako je usao tamo da treba da vrati na pocetak, ne treba da odradi jos jedno za sledeci linijski sistem(povratak na pocetak vraca iteratore i jednog i drugog linijskog sistema)
			return;
		int brPonavljanja = 1;
		for (int i = 0; i < brPonavljanja && brPonavljanja < 3; i++) {
			if (linSis.getIterTakt()->getIterVectSimb() == linSis.getIterTakt()->getVctSimbola().begin()) {
				if (linSis.getIterTakt() == linSis.getVctTakt().begin()) {
					std::cout << "Kretanje ispocetka\n" << std::endl;
					prikaziZadnjiSimbol(false);
					ispisi = false;
					return;
				}
				else {
					linSis.getIterTakt()->setIterVectorSimbol(--(--linSis.getIterTakt())->getVctSimbola().end());
					linSis.getIterTakt()->setIterSimbol(linSis.getIterTakt()->getIterVectSimb()->begin());
				}
			}
			else {
				linSis.getIterTakt()->setIterSimbol((--linSis.getIterTakt()->getIterVectSimb())->begin()); // ne treba ++u argumentu jer se gore u ifu proverava moze li da se vektor simbola mrda i to ima bocni efekat
				if ((*linSis.getIterTakt()->getIterSimbol())->getPrekinuta())
					brPonavljanja++;
			}
		}
		//}
	});
}

void Kompozicija::nextTakt() {
	bool ispisi = true;
	std::for_each(vctLinSis.begin(), vctLinSis.end(), [this, &ispisi](LinijskiSistem& linSis) {
		if (!ispisi) //znaci ako je usao tamo da treba da vrati na pocetak, ne treba da odradi jos jedno za sledeci linijski sistem(povratak na pocetak vraca iteratore i jednog i drugog linijskog sistema)
			return;			
		if (++linSis.getIterTakt() == linSis.getVctTakt().end()) {
			std::cout << "Kretanje ispocetka\n" << std::endl;
			prikaziPrviSimbol(false);
			ispisi = false;
			return;
		}
		else {
			linSis.getIterTakt()->setIterVectorSimbol(linSis.getIterTakt()->getVctSimbola().begin());
			linSis.getIterTakt()->setIterSimbol(linSis.getIterTakt()->getIterVectSimb()->begin());
		}	
	});
}

void Kompozicija::prewTakt() {
	bool ispisi = true;
	std::for_each(vctLinSis.begin(), vctLinSis.end(), [this, &ispisi](LinijskiSistem& linSis) {
		if (!ispisi) //znaci ako je usao tamo da treba da vrati na pocetak, ne treba da odradi jos jedno za sledeci linijski sistem(povratak na pocetak vraca iteratore i jednog i drugog linijskog sistema)
			return;			
		if (linSis.getIterTakt() == linSis.getVctTakt().begin()) {
			std::cout << "Kretanje ispocetka\n" << std::endl;
			vctLinSis.front().setIterTakt(--vctLinSis.begin()->getVctTakt().end());
			vctLinSis.back().setIterTakt(--vctLinSis.back().getVctTakt().end());
			//inicijalizujemo iterator vektora simbola
			vctLinSis.begin()->getIterTakt()->setIterVectorSimbol((--vctLinSis.begin()->getVctTakt().end())->getVctSimbola().begin());
			vctLinSis.back().getIterTakt()->setIterVectorSimbol((--vctLinSis.back().getVctTakt().end())->getVctSimbola().begin());
			//inicijalizovali smo iterator simbola
			vctLinSis.begin()->getIterTakt()->setIterSimbol(vctLinSis.begin()->getVctTakt().begin()->getVctSimbola().begin()->begin());
			vctLinSis.back().getIterTakt()->setIterSimbol(vctLinSis.back().getVctTakt().begin()->getVctSimbola().begin()->begin());

			ispisi = false;
			return;
		}
		else {
			linSis.getIterTakt()->setIterVectorSimbol(--(--linSis.getIterTakt())->getVctSimbola().end());
			linSis.getIterTakt()->setIterSimbol(linSis.getIterTakt()->getIterVectSimb()->begin());
		}
	});
}

void Kompozicija::ispisLinijskihSistema() {	
	std::shared_ptr<std::string>st = std::make_shared<std::string>();
	
	std::ofstream ispis;
	bool leva = true;
	std::for_each(vctLinSis.begin(), vctLinSis.end(), [st, &ispis, &leva](LinijskiSistem itLinSis) {
		if (leva)
			ispis.open("ispisLeva.txt");
		else
			ispis.open("ispisDesna.txt");
		leva = false;
		std::for_each(itLinSis.getVctTakt().begin(), itLinSis.getVctTakt().end(), [st, &ispis, &leva](Takt it1) {

			std::for_each(it1.getVctSimbola().begin(), it1.getVctSimbola().end(), [st, &ispis](std::vector<std::shared_ptr<Simbol>> it2) {
				*st = *st + "(";

				std::for_each(it2.begin(), it2.end(), [st](std::shared_ptr<Simbol> s) {
					*st = *st + s->ispisivanjePodataka();
				});

				*st = *st + ")";
			});

			ispis << *st << "|\n";
			st->clear();
		});

		ispis.close();
	});
	
	std::string s;
	std::ifstream levaTxt;
	levaTxt.open("ispisLeva.txt");
	std::ifstream desnaTxt;
	desnaTxt.open("ispisDesna.txt");
	while (std::getline(levaTxt, s)) {
		std::cout << std::setw(40) << s << "\n";
		std::getline(desnaTxt, s);
		std::cout << std::setw(40) << s << "\n";
		std::cout << "\n";
	}
}

void Kompozicija::pomocSlovo(Mapa* mapa_, LinijskiSistem& linijskiSis, char slovo_, bool nota_, bool kratka) {
	//Simbol p;
	std::shared_ptr<Simbol> p;
	if (!nota_) {
		if(!kratka)
		p = std::make_shared<Pauza>(Razlomak(1, 4));
		else {
			p = std::make_shared<Pauza>(Razlomak(1, 8));
		}
	}
	else {
		std::string opisNote = mapa_->getOpisNote(slovo_);
		if (kratka)
			//p = Nota(mapa_->getOktava(opisNote), mapa_->getVisina(opisNote), Razlomak(1, 8), mapa_->getPovisilica(opisNote));
			p = std::make_shared<Nota>(mapa_->getOktava(opisNote), mapa_->getVisina(opisNote), Razlomak(1, 8), mapa_->getPovisilica(opisNote));
		else
			//p = Nota(mapa_->getOktava(opisNote), mapa_->getVisina(opisNote), Razlomak(1, 4), mapa_->getPovisilica(opisNote));
			p = std::make_shared<Nota>(mapa_->getOktava(opisNote), mapa_->getVisina(opisNote), Razlomak(1, 4), mapa_->getPovisilica(opisNote));
	}
	
	if (linijskiSis.getVctTakt().empty() || linijskiSis.getVctTakt().back().getPopunjen()) {
		Takt t1 = Takt(this->getTrajanjeTakta());
		linijskiSis.dodajTakt(t1);//ako je vektor taktova prazan, ili je poslednji vektor pun, dodamo novi takt u vctTakt;
		p->setBrojTakta(linijskiSis.getVctTakt().back().getBrTakta());
		if(linijskiSis.getVctTakt().back().imaMesta(p))										///
			linijskiSis.getVctTakt().back().dodajSimbol(p);// samo ovo je bilo pre ovoga	///
		else {
			p->getTrajanje().setRazlomak(Razlomak(1, 8));
			p->setPrekinuta(); // da bi znao da mora da se spoji sa prvom iz sledeceg takta.
			p->setBrojTakta(linijskiSis.getVctTakt().back().getBrTakta());
			linijskiSis.getVctTakt().back().dodajSimbol(p);
			linijskiSis.dodajTakt(Takt(this->getTrajanjeTakta()));
			linijskiSis.getVctTakt().back().dodajSimbol(p);
		}
	}																						///
	else if (!linijskiSis.getVctTakt().back().getPopunjen() && !linijskiSis.getVctTakt().back().imaMesta(p)) {//slucaj kada nije popunjena i nema mesta, mora da se deli na 2 dela i flag podeljen da se postavi na true
		p->getTrajanje().setRazlomak(Razlomak(1, 8));
		p->setPrekinuta(); // da bi znao da mora da se spoji sa prvom iz sledeceg takta.
		p->setBrojTakta(linijskiSis.getVctTakt().back().getBrTakta());
		linijskiSis.getVctTakt().back().dodajSimbol(p);
		linijskiSis.dodajTakt(Takt(this->getTrajanjeTakta()));
		linijskiSis.getVctTakt().back().dodajSimbol(p);
	}
	else {
		p->setBrojTakta(linijskiSis.getVctTakt().back().getBrTakta());
		linijskiSis.getVctTakt().back().dodajSimbol(p);
	}
}

void Kompozicija::pomocSlova(Mapa* mapa_, std::vector<std::shared_ptr<Simbol>>& leva, std::vector<std::shared_ptr<Simbol>>&desna) {
	std::vector<std::shared_ptr<Simbol>> simboli;
	bool jednomLeva = false;
	for (auto it = getVctLinSis().begin(); it != getVctLinSis().end(); it++) {
		if (!jednomLeva) { //da simboli budu prvi prolaz leva ruka, a drugi prolaz desna
			simboli = leva;
			jednomLeva = true;
		}
		else
			simboli = desna;
		if (leva.empty() && simboli == leva) {
			pomocSlovo(mapa_, *getVctLinSis().begin(), '|', false, false);
			continue;
		}
		else if (desna.empty() && simboli == desna) {
			pomocSlovo(mapa_, getVctLinSis().back(), '|', false, false);
			continue;
		}
		if (it->getVctTakt().empty() || it->getVctTakt().back().getPopunjen()) {

			Takt t1 = Takt(this->getTrajanjeTakta());
			it->dodajTakt(t1);//ako je vektor taktova prazan, ili je poslednji vektor pun, dodamo novi takt u vctTakt;
			if(it->getVctTakt().back().imaMesta(simboli.back()))
				it->getVctTakt().back().dodajSimbole(simboli);// dodaj simbole im dodeljuje i BrTakta               
			else {
				std::for_each(simboli.begin(), simboli.end(), [](std::shared_ptr<Simbol> nota) {
					nota->getTrajanje().setRazlomak(Razlomak(1, 8));
					nota->setPrekinuta(); // da bi znao da mora da se spoji sa prvom iz sledeceg takta.
				});
				it->getVctTakt().back().dodajSimbole(simboli);
				it->dodajTakt(Takt(this->getTrajanjeTakta()));
				it->getVctTakt().back().dodajSimbole(simboli);
			}
		}
		else if (!it->getVctTakt().back().getPopunjen() && !it->getVctTakt().back().imaMesta(simboli.back())) {
			std::for_each(simboli.begin(), simboli.end(), [](std::shared_ptr<Simbol> nota) {
				nota->getTrajanje().setRazlomak(Razlomak(1, 8));
				nota->setPrekinuta(); // da bi znao da mora da se spoji sa prvom iz sledeceg takta.
			});
			it->getVctTakt().back().dodajSimbole(simboli);
			it->dodajTakt(Takt(this->getTrajanjeTakta()));
			it->getVctTakt().back().dodajSimbole(simboli);
		}
		else
			it->getVctTakt().back().dodajSimbole(simboli);
	}
}

void Kompozicija::ispisiSimbol() {
	std::for_each(vctLinSis.begin()->getIterTakt()->getIterVectSimb()->begin(), vctLinSis.begin()->getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> ptrSimbol) {
		std::cout<<*ptrSimbol << "\n";
	});
	std::for_each(vctLinSis.back().getIterTakt()->getIterVectSimb()->begin(), vctLinSis.back().getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> ptrSimbol) {
		std::cout << *ptrSimbol << "\n";
	});
	
}

void Kompozicija::pomeranjeKompozicije(int i) {
	uvecajOktavu(i);
}

void Kompozicija::uvecajOktavu(int i) {
	bool sem = true;
	prikaziPrviSimbol();
	std::vector<std::shared_ptr<Simbol>>& pocetniLevaSimboli = *getVctLinSis().front().getIterTakt()->getIterVectSimb(); //ovo koristimo samo da znamo kada da prestanemo sa do while loop-om
	Takt pocetniLevaTakt = *getVctLinSis().front().getIterTakt();
	Takt pomoc = *getVctLinSis().front().getIterTakt();
	do {
		this->izmeniOktavuNote(i);
		nextSimbol();
		pomoc = *getVctLinSis().front().getIterTakt();
	} while (pocetniLevaTakt.getStvarniBrTakta() != pomoc.getStvarniBrTakta() || *getVctLinSis().front().getIterTakt()->getIterVectSimb() != pocetniLevaSimboli);
}

void Kompozicija::setPovisilica() {
	std::for_each(getVctLinSis().front().getIterTakt()->getIterVectSimb()->begin(), getVctLinSis().front().getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> s) {
		s->postaviPovisilicu();
	});
	std::for_each(getVctLinSis().back().getIterTakt()->getIterVectSimb()->begin(), getVctLinSis().back().getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> s) {
		s->postaviPovisilicu();
	});
}

void Kompozicija::ukliniPovisilicu() {
	std::for_each(getVctLinSis().front().getIterTakt()->getIterVectSimb()->begin(), getVctLinSis().front().getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> s) {
		s->ukloniPovisilicu();
	});
	std::for_each(getVctLinSis().back().getIterTakt()->getIterVectSimb()->begin(), getVctLinSis().back().getIterTakt()->getIterVectSimb()->end(), [](std::shared_ptr<Simbol> s) {
		s->ukloniPovisilicu();
	});
}

void Kompozicija::izmeniTakt(Mapa& mapa, std::vector<int>& istorija_, bool& konvertovano, bool& postavljenSimbol) {
	vctLinSis.clear();
	vctLinSis.emplace_back();
	vctLinSis.emplace_back();
	setTrajanjeTakta();
	ucitajNote(mapa);
	popuniPauzama();

	bool otvorena = true;
	konvertovano = false;
	postavljenSimbol = false;
	int meni;
	for (auto it = istorija_.begin(); it != istorija_.end(); it++) {
		meni = *it;

		switch (meni) {
		case 2:
			if (otvorena) {
				prikaziPrviSimbol();
				postavljenSimbol = true;
			}
			else
				std::cerr << "Kompozicija nije otvorena\n";
			break;
		case 3:
			if (!postavljenSimbol && otvorena) {
				prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			nextSimbol();
			ispisiSimbol();
			break;
		case 4:
			if (!postavljenSimbol && otvorena) {
				prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			prewSimbol();
			ispisiSimbol();
			break;
		case 5:
			if (!postavljenSimbol && otvorena) {
				prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			nextTakt();
			ispisiSimbol();
			break;
		case 6:
			if (!postavljenSimbol && otvorena) {
				prikaziPrviSimbol();
				postavljenSimbol = true;
				break;
			}
			else if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			prewTakt();
			ispisiSimbol();
			break;
		case 7:
			if (otvorena) {
				ispisLinijskihSistema();
			}
			else
				std::cerr << "Kompozicija nije otvorena\n";
			break;
		case 8:
		{
			if (otvorena) {
				konvertovano = false;
				int i;
				std::cout << "Za koliko zelis da promenis oktavu? ";
				it++;
				i = *it;
				pomeranjeKompozicije(i);
			}
			else std::cerr << "Kompozicija nije otvorena\n";
			break;
		}
		case 10:
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				prikaziPrviSimbol(false);
				postavljenSimbol = true;
				konvertovano = false;
				setPovisilica();
			}
			else {
				konvertovano = false;
				setPovisilica();
			}
			break;
		case 11:
		{
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				prikaziPrviSimbol(false);
				postavljenSimbol = true;
				int i;
				std::cout << "Unesi broj za koji zelis da izmenis oktavu\n";
				//std::cin >> i;
				it++;
				i = *it;
				izmeniOktavuNote(i);
			}
			break;
		}		
		case 14:
			if (!otvorena) {
				std::cerr << "Kompozicija nije otvorena\n";
				break;
			}
			if (!postavljenSimbol) {
				prikaziPrviSimbol(false);
				postavljenSimbol = true;
				konvertovano = false;
				ukliniPovisilicu();
			}
			else {
				konvertovano = false;
				ukliniPovisilicu();
			}
			break;
		}
	}

}

void Kompozicija::izmeniOktavuNote(int i) {
	std::vector<std::shared_ptr<Simbol>>& levaSimboli = *getVctLinSis().front().getIterTakt()->getIterVectSimb();
	std::vector<std::shared_ptr<Simbol>>& desnaSimboli = *getVctLinSis().back().getIterTakt()->getIterVectSimb();
	std::vector<std::shared_ptr<Simbol>> buffer;

	bool pauzaLeva = true; //ulazimo sa predpostavkom da ima pauze
	bool pauzaDesna = true;

	bool prethodna = false;
	bool sledeca = false;

	for (auto it = levaSimboli.begin(); it != levaSimboli.end(); it++) {
		if ((*it)->promeniOktavuZa(i)) {
			buffer.push_back(*it);
			pauzaLeva = false;
		}
	}
	for (auto it = desnaSimboli.begin(); it != desnaSimboli.end(); it++) {
		if ((*it)->promeniOktavuZa(i)) {
			buffer.push_back(*it);
			pauzaDesna = false;
		}
	}
	if (pauzaDesna && pauzaLeva)
		return;

	std::vector<std::shared_ptr<Simbol>>* prethodniSimboliLeva = nullptr;
	std::vector<std::shared_ptr<Simbol>>* prethodniSimboliDesna = nullptr;
	std::vector<std::shared_ptr<Simbol>>* sledeciSimboliLeva = nullptr;
	std::vector<std::shared_ptr<Simbol>>* sledeciSimboliDesna = nullptr;

	if (levaSimboli.front()->getPrekinuta()) {
		if (getVctLinSis().back().getIterTakt() != getVctLinSis().back().getVctTakt().begin()) {
			std::vector<Takt>::iterator itTaktLeva = getVctLinSis().front().getIterTakt();
			std::vector<Takt>::iterator itTaktDesna = getVctLinSis().back().getIterTakt();
			prethodniSimboliLeva = &(itTaktLeva-1)->getVctSimbola().back();
			prethodniSimboliDesna = &(itTaktDesna-1)->getVctSimbola().back();
		
			if (find(levaSimboli.begin(), levaSimboli.end(), prethodniSimboliLeva->front()) != levaSimboli.end()) {
				prethodna = true;
			}
		}
		if (getVctLinSis().back().getIterTakt() != --getVctLinSis().back().getVctTakt().end()) {
			std::vector<Takt>::iterator itTaktLeva = getVctLinSis().front().getIterTakt();
			std::vector<Takt>::iterator itTaktDesna = getVctLinSis().back().getIterTakt();
			sledeciSimboliLeva = &(itTaktLeva+1)->getVctSimbola().front();
			sledeciSimboliDesna = &(itTaktDesna+1)->getVctSimbola().front();
		
			if ((find(levaSimboli.begin(), levaSimboli.end(), sledeciSimboliLeva->front()) != levaSimboli.end())) {
				sledeca = true;
			}
		}
		
		

		if (prethodna) {
			prethodniSimboliLeva->clear();
			prethodniSimboliDesna->clear();
		}
		else if (sledeca) {
			sledeciSimboliLeva->clear();
			sledeciSimboliDesna->clear();
		}
	}

	levaSimboli.clear();
	desnaSimboli.clear();

	std::for_each(buffer.begin(), buffer.end(), [&levaSimboli, &desnaSimboli, prethodna, sledeca, &prethodniSimboliDesna, &prethodniSimboliLeva, &sledeciSimboliDesna, &sledeciSimboliLeva](std::shared_ptr<Simbol> s) {
		if (s->getOktava() <= 3) {
			levaSimboli.push_back(s);
			if (prethodna)
				prethodniSimboliLeva->push_back(s);
			if (sledeca)
				sledeciSimboliLeva->push_back(s);
		}
		else {
			desnaSimboli.push_back(s);
			if (prethodna)
				prethodniSimboliDesna->push_back(s);
			if (sledeca)
				sledeciSimboliDesna->push_back(s);
		}
	});
	if (levaSimboli.empty()) {
		std::shared_ptr<Pauza> p = std::make_shared<Pauza>(desnaSimboli.front()->getTrajanje().getRazlomak());
		p->setBrojTakta(desnaSimboli.front()->getBrojTakta());
		p->setPrekinuta(desnaSimboli.front()->getPrekinuta());
		levaSimboli.push_back(p);
		if (prethodna)
			prethodniSimboliLeva->push_back(p);
		if (sledeca)
			sledeciSimboliLeva->push_back(p);
	}
	if (desnaSimboli.empty()) {
		std::shared_ptr<Pauza> p = std::make_shared<Pauza>(levaSimboli.front()->getTrajanje().getRazlomak());
		p->setBrojTakta(levaSimboli.front()->getBrojTakta());
		p->setPrekinuta(levaSimboli.front()->getPrekinuta());
		desnaSimboli.push_back(p);
		if (prethodna)
			prethodniSimboliDesna->push_back(p);
		if (sledeca)
			sledeciSimboliDesna->push_back(p);
	}
}

void Kompozicija::popuniPauzama() {
	while (!vctLinSis.front().getVctTakt().back().getPopunjen()) {
		std::shared_ptr<Pauza> p = std::make_shared<Pauza>(Razlomak(1, 8));
		p->setBrojTakta(vctLinSis.front().getVctTakt().back().getBrTakta());
		//vctLinSis.front().getVctTakt().back().dodajSimbol(std::make_shared<Pauza>(Razlomak(1, 8)));
		vctLinSis.front().getVctTakt().back().dodajSimbol(p);
	}
	while (!vctLinSis.back().getVctTakt().back().getPopunjen()) {
		std::shared_ptr<Pauza> p = std::make_shared<Pauza>(Razlomak(1, 8));
		p->setBrojTakta(vctLinSis.back().getVctTakt().back().getBrTakta());
		//vctLinSis.back().getVctTakt().back().dodajSimbol(std::make_shared<Pauza>(Razlomak(1, 8)));
		vctLinSis.back().getVctTakt().back().dodajSimbol(p);
	}
}


std::pair<std::vector<std::string>, std::vector<std::string>> Kompozicija::getOpisSimbola() {
	std::pair<std::vector<std::string>, std::vector<std::string>> opisi;
	std::for_each(vctLinSis.front().getIterTakt()->getIterVectSimb()->begin(), vctLinSis.front().getIterTakt()->getIterVectSimb()->end(),
	[&opisi](std::shared_ptr<Simbol> ptrSimbol) {
		opisi.first.push_back(ptrSimbol->getStr());
	});
	
	std::for_each(vctLinSis.back().getIterTakt()->getIterVectSimb()->begin(), vctLinSis.back().getIterTakt()->getIterVectSimb()->end(),
	[&opisi](std::shared_ptr<Simbol> ptrSimbol) {
		opisi.second.push_back(ptrSimbol->getStr());
	});
	
	return opisi;
}


std::pair < std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> Kompozicija::getOpisVektoraSimbola() {
	std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> opisi;
	std::vector<std::vector<std::string>> opisiLeve;
	std::vector<std::vector<std::string>> opisiDesne;

	prikaziPrviSimbol(false);
	std::vector<std::shared_ptr<Simbol>>& pocetniLevaSimboli = *getVctLinSis().front().getIterTakt()->getIterVectSimb(); //ovo koristimo samo da znamo kada da prestanemo sa do while loop-om
	Takt pocetniLevaTakt = *getVctLinSis().front().getIterTakt();
	Takt pomoc = *getVctLinSis().front().getIterTakt();
	do {
		std::pair<std::vector<std::string>, std::vector<std::string>> pomocna = getOpisSimbola(); 

		opisiLeve.push_back(pomocna.first);
		opisiDesne.push_back(pomocna.second);

		nextSimbol();
		pomoc = *getVctLinSis().front().getIterTakt();
	
	} while (pocetniLevaTakt.getStvarniBrTakta() != pomoc.getStvarniBrTakta() || *getVctLinSis().front().getIterTakt()->getIterVectSimb() != pocetniLevaSimboli);

	opisi.first = opisiLeve;
	opisi.second = opisiDesne;

	return opisi;
}


std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> Kompozicija::getMelody(Mapa* mapa_) {

	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> melodije;
	std::vector<std::vector<int>> melodijaLeve;
	std::vector<std::vector<int>> melodijaDesne;

	prikaziPrviSimbol(false);
	std::vector<std::shared_ptr<Simbol>>& pocetniLevaSimboli = *getVctLinSis().front().getIterTakt()->getIterVectSimb(); //ovo koristimo samo da znamo kada da prestanemo sa do while loop-om

	Takt pocetniLevaTakt = *getVctLinSis().front().getIterTakt();
	Takt pomoc = *getVctLinSis().front().getIterTakt();

	do {
		std::pair<std::vector<std::string>, std::vector<std::string>> pomocna = getOpisSimbola();
		
		//sad ubacujemo o ova dva niza vektora<int>
		std::vector<int> jedinicaSimbolaLeva;
		std::vector<int> jedinicaSimbolaDesna;

		std::for_each(pomocna.first.begin(), pomocna.first.end(), [&jedinicaSimbolaLeva, mapa_](std::string& s) {
			if (s == "0")//nulu vraca opis pauze
				jedinicaSimbolaLeva.push_back(999); //ovim cu oznacavati pauzu
			else
				jedinicaSimbolaLeva.push_back(mapa_->getMidiBroj(s));
		});
		std::for_each(pomocna.second.begin(), pomocna.second.end(), [&jedinicaSimbolaDesna, mapa_](std::string& s) {
			if (s == "0")
				jedinicaSimbolaDesna.push_back(999); //ovim cu oznacavati pauzu
			else
				jedinicaSimbolaDesna.push_back(mapa_->getMidiBroj(s));
		});

		melodijaLeve.push_back(jedinicaSimbolaLeva);
		melodijaDesne.push_back(jedinicaSimbolaDesna);

		nextSimbol();
		pomoc = *getVctLinSis().front().getIterTakt();
	} while (pocetniLevaTakt.getStvarniBrTakta() != pomoc.getStvarniBrTakta() || *getVctLinSis().front().getIterTakt()->getIterVectSimb() != pocetniLevaSimboli);
	melodije.first = melodijaLeve;
	melodije.second = melodijaDesne;
	return melodije;
}

std::pair<std::vector<int>, std::vector<int>> Kompozicija::getRhithm() {
	std::pair<std::vector<int>, std::vector<int>> ritam;
	std::vector<int> ritamLeve;
	std::vector<int> ritamDesne;

	prikaziPrviSimbol(false);

	std::vector<std::shared_ptr<Simbol>>& pocetniLevaSimboli = *getVctLinSis().front().getIterTakt()->getIterVectSimb(); //ovo koristimo samo da znamo kada da prestanemo sa do while loop-om
	Takt pocetniLevaTakt = *getVctLinSis().front().getIterTakt();
	Takt pomoc = *getVctLinSis().front().getIterTakt();

	do {
		if ((*vctLinSis.front().getIterTakt()->getIterSimbol())->getPrekinuta()) //ako je prekinuta, automatski znaci da traje 1/4
			ritamLeve.push_back(3); //ipak smo stavili da je 3, da bi koristili to da znamo gde je prekid tacno
		else if ((*vctLinSis.front().getIterTakt()->getIterSimbol())->getTrajanje().getRazlomak() == Razlomak(1, 8))
			ritamLeve.push_back(1);
		else if ((*vctLinSis.front().getIterTakt()->getIterSimbol())->getTrajanje().getRazlomak() == Razlomak(1, 4))
			ritamLeve.push_back(2);

		if ((*vctLinSis.back().getIterTakt()->getIterSimbol())->getPrekinuta())
			ritamDesne.push_back(3);////ipak smo stavili da je 3, da bi koristili to da znamo gde je prekid tacno
		else if ((*vctLinSis.back().getIterTakt()->getIterSimbol())->getTrajanje().getRazlomak() == Razlomak(1, 8))
			ritamDesne.push_back(1);
		else if ((*vctLinSis.back().getIterTakt()->getIterSimbol())->getTrajanje().getRazlomak() == Razlomak(1, 4))
			ritamDesne.push_back(2);

		nextSimbol();
		pomoc = *getVctLinSis().front().getIterTakt();
	} while (pocetniLevaTakt.getStvarniBrTakta() != pomoc.getStvarniBrTakta() || *getVctLinSis().front().getIterTakt()->getIterVectSimb() != pocetniLevaSimboli);

	ritam.first = ritamLeve;	
	ritam.second = ritamDesne;

	return ritam;
}
