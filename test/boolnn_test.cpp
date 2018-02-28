#include <iostream>
#include <vector>
#include <cstdlib>

#include "layers.hpp"

void dump_vec(std::vector<bool> r_1) {
	for (unsigned i = 0; i < r_1.size(); i++) {
		std::cout << r_1[i] << ' ';
	}
	std::cout << std::endl;
}

int main (int argc, char **argv) {

	BoolNN bnn(32), bnn_2;
	std::vector<bool> v;
	std::vector<bool> r_1, r_2;
	char fname[] = "test.bnn";

	bnn.AddLayer(LayerKind::BIT_BIAS, 128);
	bnn.AddLayer(LayerKind::BIT, 128);
	bnn.AddLayer(LayerKind::DWORD, 128);
	bnn.AddLayer(LayerKind::BIT_BIAS, 16);
	

	for (unsigned i = 0; i < 32; i++) {
		v.push_back(!(rand() % 2));
	}

	std::cout << bnn.GetNDOF() << " nn dof" << std::endl;

	for (unsigned i = 0; i < bnn.GetNDOF() * 4; i++) {
		bnn.FlipBit(rand());
		r_1 = bnn.Compute(v);
		// dump_vec(r_1);
	}

	r_1 = bnn.Compute(v);
	bnn.DumpFile(fname);
	bnn_2.LoadFile(fname);
	r_2 = bnn.Compute(v);

	if (r_1.size() != r_2.size()) {
		std::cerr << "size mismatch in results" << std::endl;
	}

	for (unsigned i = 0; i < r_1.size(); i++) {
		std::cout << r_1[i] << '\t' << r_2[i] <<  std::endl;
		if (r_1[i] != r_2[i]) {
			std::cerr << "wrong results" << std::endl;
		}
	}

	return 0;
}
