#include <iostream>

#include "layers.hpp"


// pack a vector of bits in uint32_t starting from offset
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

// unpack a vector of bits from uint32_t and write to v starting from offset
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

		if ( is_BIT(kind) ) {
			W.W[i][j] = !W.W[i][j];
		} else {
			bool val = !W.W[i][j];
			for (unsigned l = 0; l < sizeof(uint32_t); l++) {
				W.W[l][l] = val; // diagonal elements

			}
		}
	}
}


static LayerKind KindCode(uint32_t code) {

	switch (code) {
	case 1:
		return LayerKind::BIT;
	case 2:
		return LayerKind::DWORD;
	case 3:
		return LayerKind::BIT_BIAS;
	case 4:
		return LayerKind::DWORD_BIAS;
	case 5:
		return LayerKind::ADD;
	default:
		std::cerr << "wrong layer kind in file" << std::endl;
		return LayerKind::BIT;
	}
}


bool LayerGeneric::Write (std::fstream& out) const {

	uint32_t o_kind = static_cast<uint32_t> (kind),
		o_N_in =  static_cast<uint32_t> (N_in),
		o_N_out = static_cast<uint32_t> (N_out),
		o_N_bits = static_cast<uint32_t> (N_bits);

	// write header
	out.write((char *) &o_kind, sizeof(uint32_t))
		.write((char *) &o_N_in, sizeof(uint32_t))
		.write((char *) &o_N_out, sizeof(uint32_t))
		.write((char *) &o_N_bits, sizeof(uint32_t));

	// write weights
	W.Write(out);

	if ( !is_Biased(kind) ) return true; // no biases, we're done here

	// write biases
	for (unsigned i = 0; i < 2*N_out; i++) {
		uint8_t val;
		val = static_cast<uint8_t> (B[i]);
		out.write((char *) &val, sizeof(uint8_t));
	}

	return true;
}

bool LayerGeneric::Read (std::fstream& in) {

	uint32_t r_kind, r_N_in, r_N_out, r_N_bits;

	in.read((char *) &r_kind, sizeof(uint32_t))
		.read((char *) &r_N_in, sizeof(uint32_t))
		.read((char *) &r_N_out, sizeof(uint32_t))
		.read((char *) &r_N_bits, sizeof(uint32_t));

	// read header
	kind = KindCode (r_kind);
	N_in =  static_cast<unsigned> (r_N_in);
	N_out = static_cast<unsigned> (r_N_out);
	N_bits = static_cast<unsigned> (r_N_bits);

	// read weights
	W.Read(in);

	if ( !is_Biased(kind) )	return true; // no biases, we're done here

	// read biases
	for (unsigned i = 0; i < 2*N_out; i++) {
		uint8_t val;
		in.read((char *) &val, sizeof(uint8_t));
		B[i] =  static_cast<bool> (val);
	}

	return true;
}

