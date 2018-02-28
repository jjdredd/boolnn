#include <iostream>
#include <vector>
#include <cstdlib>

#include "layers.hpp"

int main (int argc, char **argv) {

	BoolNN bnn(32), bnn_2;
	std::vector<bool> v;
	std::vector<bool> r_1, r_2;
	char fname[] = "test.bnn";

	bnn.AddLayer(LayerKind::BIT_BIAS, 32);
	// bnn.AddLayer(LayerKind::BIT, 32);

	for (unsigned i = 0; i < 16; i++) {
		v.push_back(true);
	}
	for (unsigned i = 16; i < 32; i++) {
		v.push_back(true);
	}


	std::cout << bnn.GetNDOF() << " nn dof" << std::endl;

	for (unsigned i = 0; i < 3200; i++) {
		bnn.FlipBit(i);
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
