#include <iostream>
#include <vector>

#include "layers.hpp"


//
// LayerBIT class
// 
LayerBIT::LayerBIT(unsigned N_in, unsigned N_out)
	: N_in(N_in), N_out(N_out), W(N_in, 2*N_out), B(2*N_out) {

	N_bits = N_in * N_out + 2 * N_out;
}

unsigned LayerBIT::GetNDOF() const {
	return N_bits;
}

std::vector<bool> LayerBIT::Compute(std::vector<bool> input) const {

	std::vector<bool> result(N_out), a = W & input;

	for (unsigned i = 0; i < N_out; i++) {
		result[i] = a[2*i] & a[2*i + 1];
	}

	return result;
}

void LayerBIT::FlipBit(unsigned N) {
	
	unsigned n = N < GetNDOF() ? N : N%GetNDOF();
	unsigned k = n - W.GetNDOF();

	if (k > 0) {
		B[k] = !B[k];
	} else {
		unsigned i = n / W.N;
		unsigned j = n % W.N;
		W[i][j] = !W[i][j];
	}
}



//
// LayerDWORD
// 
LayerDWORD::LayerDWORD(unsigned N_in, unsigned N_out)
	: N_in(N_in), N_out(N_out), W(N_in, 2*N_out), B(2*N_out) {
	
	N_bits = N_in * N_out + 2*N_out;
}

unsigned LayerDWORD::GetNDOF() {
	return N_bits;
}

void LayerDWORD::FlipBit(unsigned N) {

	unsigned n = N < GetNDOF() ? N : N%GetNDOF();
	unsigned k = n - W.GetNDOF();

	if (k > 0) {
		B[k] = !B[k];
	} else {
		unsigned i = n / W.N;
		unsigned j = n % W.N;
		W[i][j] = !W[i][j];
	}
}
