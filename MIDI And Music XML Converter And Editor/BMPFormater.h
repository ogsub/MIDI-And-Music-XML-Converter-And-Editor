/*
#pragma once
#include "Formater.h"
#include <string>
#include <ctype.h>
#include <cstdint>

#pragma pack(2)
struct BMPzaglavlje {
	char ID[2] = { 'B', 'M' };
	int32_t velicina;
	int32_t rezervisano = 0;
	int32_t dataOffset;
};

struct DIB{
	int32_t velicinaDBI = 40;
	int32_t sirina;
	int32_t visina;
	int16_t jednaKarataBoja = 1;
	int16_t brojBitaPoPikselu = 24;
	int32_t BI_RGB = 0;
	int32_t velicinaBitmape = 0;
	int32_t fiksna1 = 2835;
	int32_t fiksna2 = 2835;
	int32_t fiksna3 = 0;
	int32_t fiksna4 = 0;
};

class BMPFormater :public Formater {
public:
	BMPFormater(int visina_, int sirina_);
	~BMPFormater();
	void konvertuj(Mapa& mapa, Kompozicija* kompozicija) {};
	void postaviPiksel(int x, int y, uint8_t crvena, uint8_t zelena, uint8_t plava);
	bool ispisi(std::string ime);

private:
	int sirina;
	int visina;
	std::unique_ptr<uint8_t[]> memorijaPiksela = nullptr;
};

*/