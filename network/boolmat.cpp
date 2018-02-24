#include <iostream>
#include <vector>

#include "layers.hpp"



//
// boolean vector products
// 
bool operator& (const std::vector<bool>& first,
		const std::vector<bool>& second) {

	bool res = false;
	unsigned N = first.size();

	if ( N != second.size() ) {
		std::cerr << "vector operator& size mismatch" << std::endl;
		return res;
	}

	for (unsigned i = 0; i < N; i++) {
		res ^= first[i] & second[i];
	}
		
	return res;
}


std::vector<bool> operator^ (const std::vector<bool>& first,
			     const std::vector<bool>& second) {

	std::vector<bool> res;
	unsigned N = first.size();

	res.reserve(N);
	if ( N != second.size() ) {
		std::cerr << "vector operator^ size mismatch" << std::endl;
		return res;
	}

	for (unsigned i = 0; i < N; i++) {
		res[i] = first[i] ^ second[i];
	}
	
	return res;
}




//
// BoolMat
// 
BoolMat::BoolMat (unsigned M, unsigned N)
	: M(M), N(N) {

	W = new std::vector<bool>[M];
	for (unsigned i = 0; i < M; i++) {
		W[i].reserve(N);
	}
}

BoolMat::~BoolMat () {

	delete[] W;
}

BoolMat BoolMat::operator& (const BoolMat& rhs) const {

	BoolMat res(rhs.M, rhs.N);

	if (N != rhs.M) {
		std::cerr << "size mismatch in BoolMat::operator&(const BoolMat)"
			  << std::endl;
		return res;
	}
	
	for (unsigned i = 0; i < M; i++) {
		for (unsigned j = 0; j < rhs.N; j++) {
			for (unsigned k = 0; k < N; k++) {
				res.W[i][j] = W[i][k] & rhs.W[k][j];
			}
		}
	}

	return res;
}

std::vector<bool> BoolMat::operator& (const std::vector<bool>& rhs) const {

	std::vector<bool> res;

	if (N != rhs.size()) {
		std::cerr << "size mismatch in"
			  << " BoolMat::operator& (const std::vector<bool>"
			  << std::endl;
		return res;
	}

	res.reserve(rhs.size());

	for (unsigned i = 0; i < M; i++) {
		res[i] = W[i] & rhs;
	}

	return res;
}

BoolMat BoolMat::operator^ (const BoolMat& rhs) const {

	BoolMat res(M, N);

	if (rhs.M != M || rhs.N != N) {
		std::cerr << "size mismatch in"
			  << "BoolMat BoolMat::operator^ (const BoolMat)"
			  << std::endl;
		return res;
	}

	for (unsigned i = 0; i < M; i++) {
		for (unsigned j = 0; j < N; j++) {
			res.W[i][j] = W[i][j] ^ rhs.W[i][j];
		}
	}

	return res;
}

unsigned BoolMat::GetNDOF() const {
	return M*N;
}
