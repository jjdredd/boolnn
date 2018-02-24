#include <iostream>
#include <vector>

#include "layers.hpp"


static uint32_t BitPack (const std::vector<bool>& v, const unsigned offset) {

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

static void BitUnpack(std::vector<bool>& v, const uint32_t a,
		      const unsigned offset) {

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
// LayerGeneric class
// 
LayerGeneric::LayerGeneric(LayerKind kind, unsigned N_in, unsigned N_out)
	: kind(kind), N_in(N_in), N_out(N_out), W(N_in, 2*N_out), B(2*N_out) {


	N_bits = N_in * N_out;
	
	if ( !is_BIT(kind) ) {
		N_bits /= sizeof(uint32_t);
	}

	if ( is_Biased(kind) ) {
		N_bits += 2 * N_out;
	}
	
}

unsigned LayerGeneric::GetNDOF() const {
	return N_bits;
}

std::vector<bool> LayerGeneric::Compute(const std::vector<bool>& input) const {

	std::vector<bool> result(N_out), a = W & input;

	if (input.size() != N_in) {
		std::cerr << "size mismatch in"
			  << "LayerGeneric::Compute" << std::endl;
	}

	if (kind == LayerKind::ADD) {
		// just add and be done with it
		for (unsigned i = 0; i < N_out; i += sizeof(uint32_t)) {
			uint32_t s_1 = BitPack(a, 2*i);
			uint32_t s_2 = BitPack(a, 2*i + 1);
			// add mod 2^32
			BitUnpack(result, s_1 + s_2, i);
		}
		return result;
	}

	if ( is_Biased(kind) ) {
		a = a ^ B;
	}

	// optimize for DWORD ???
	for (unsigned i = 0; i < N_out; i++) {
		result[i] = a[2*i] & a[2*i + 1];
	}

	return result;
}

void LayerGeneric::FlipBit(unsigned N) {
	
	unsigned n = N < GetNDOF() ? N : N%GetNDOF(); // clip it
	unsigned k = n - W.GetNDOF();

	if (k > 0) {
		// if it's an unbiased layer, we will have k = 0,
		// so this this shouldn't be reachable
		// in the unbiased case
		B[k] = !B[k];
	} else {
		// not using k here
		unsigned i = n / W.M;
		unsigned j = n % W.M;
		W.W[i][j] = !W.W[i][j];
	}
}
