#ifndef TRAJANJE_H
#define TRAJANJE_H
#include "Razlomak.h"

class Trajanje {
public:
	Trajanje();
	Trajanje(Razlomak);
	~Trajanje();

	void setRazlomak(Razlomak trajanje_);
	Razlomak& getRazlomak();
	friend std::ostream& operator<<(std::ostream& out, const Trajanje& trajanje);

private:
	Razlomak razlomak;
};

#endif