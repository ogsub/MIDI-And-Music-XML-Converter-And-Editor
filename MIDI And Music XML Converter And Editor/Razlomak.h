#pragma once

#include <iostream>

class Razlomak {
public:
	Razlomak(int brojilac_ = 3, int imenilac_ = 4);
	Razlomak(int brojilac_, int imenilac_, bool t);
	Razlomak(const Razlomak& r2);
	friend Razlomak operator+(const Razlomak& r1, const Razlomak& r2);
	friend bool operator>(const Razlomak& r1, const Razlomak& r2);//vece ili jednako
	bool operator==(const Razlomak&r);
	bool operator==(const Razlomak & r) const;
	friend std::ostream& operator<<(std::ostream& out, const Razlomak& r);
	int getImenilac();
	int getBrojilac();

private:
	   int imenilac;
	   int brojilac;

};
