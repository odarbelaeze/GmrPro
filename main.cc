#include "Gmr.h"

#include <iostream>

template<typename T>
std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
{
	for (auto&& i : vr)
		os << i << " ";

	return os;
}

std::ostream& operator<< (std::ostream& os, gmr::Spin spin)
{
	os << (spin == gmr::Spin::Up? "Up" : "Down");
	return os;
}


int main(int argc, char const *argv[])
{
	gmr::particles_t particles;
	gmr::insertParticles (particles, gmr::Specie::Ion, gmr::Lattice::sc, { 0, 0, 0 });
	return 0;
}
