#include <iostream>
#include <vector>

#include "layers.hpp"


static uint32_t BitPack (const std::vector<bool> v, const unsigned offset) {

	uint32_t result = 0;

	if (offset > v.size()) {
		std::cerr << "offset mismatch in BitPack" << std::endl;
		return result;
	}

	for (unsigned n = offset; (n < offset + sizeof(uint32_t))
		     && (n < v.size()) ; n++) {
		result |= v[n] << n;
	}
	return result;
}

static void BitUnpack(std::vector<bool> v, const uint32_t a, unsigned offset) {

	if (offset > v.size()) {
		std::cerr << "offset mismatch in BitUnpack"
			  << std::endl;
		return;
	}

	if (offset + sizeof(uint32_t) > v.size()) {
		std::cerr << "offset + size mismatch in BitUnpack"
			  << std::endl;
	}

	for (unsigned n = offset; (n < offset + sizeof(uint32_t))
		     && (n < v.size()) ; n++) {
		v[n] = a & (1 << (n - offset));
	}
	return;
}


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

	if (input.size() != N_in) {
		std::cerr << "size mismatch in"
			  << "LayerBIT::Compute" << std::endl;
	}

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
		unsigned i = n / W.M;
		unsigned j = n % W.M;
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

	unsigned n = N < GetNDOF() ? N : N % GetNDOF();
	unsigned k = n - W.GetNDOF();

	if (k > 0) {
		unsigned k_int = k / B.size();
		unsigned k_bit = k % sizeof(uint32_t); // change if type
						       // in the header
						       // changed
		B[k_int] ^= 1 << k_bit;
	} else {
		unsigned i = n / W.M;
		unsigned j = n % W.M;
		W[i][j] = !W[i][j];
	}
}

std::vector<bool> LayerDWORD::Compute(std::vector<bool> input) {

	std::vector<bool> result;

	if (input.size() != N_in * sizeof(uint32_t)) {
		std::cerr << "size mismatch in"
			  << "LayerDWORD::Compute" << std::endl;
		return result;
	}

	for (unsigned i = 0; i < N_in; i++) {
		// uint32_t a = BitPack(input, i);
	}
}
