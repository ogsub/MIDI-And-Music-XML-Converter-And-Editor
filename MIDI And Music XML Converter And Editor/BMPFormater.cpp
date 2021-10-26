/*
#include "BMPFormater.h"




BMPFormater::BMPFormater(int visina_, int sirina_) : visina(visina_), sirina(sirina_), memorijaPiksela(new uint8_t[sirina*visina]{}) {
}


BMPFormater::~BMPFormater()
{
}

void BMPFormater::postaviPiksel(int x, int y, uint8_t crvena, uint8_t zelena, uint8_t plava) {
	uint8_t* pPiksel = memorijaPiksela.get();	//pokazuje na blok memorije koja je ceo ekran
	
	pPiksel[0] = plava; //ne ide rgb nego obrnuto, zato sto je little endian
	pPiksel[1] = zelena;
	pPiksel[2] = crvena;
}

bool BMPFormater::ispisi(std::string ime) {
	BMPzaglavlje zaglavlje;
	DIB DIBzaglavlje;

	zaglavlje.velicina = sizeof(BMPzaglavlje) + sizeof(DIB) + sirina * visina * 3;
	zaglavlje.dataOffset = sizeof(BMPzaglavlje) + sizeof(DIB);
	DIBzaglavlje.sirina = sirina;
	DIBzaglavlje.visina = visina;
	std::ofstream fajl;
	fajl.open(ime, std::ios::out | std::ios::binary);
	if (fajl.fail()) {
		return false;
	}
	fajl.write((char*)&zaglavlje, sizeof(zaglavlje));
	fajl.write((char*)&DIBzaglavlje, sizeof(DIBzaglavlje));
	fajl.write((char*)memorijaPiksela.get(), sirina*visina * 3);
	fajl.close();
	
	return true;
}
*/