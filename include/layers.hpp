#ifndef _BOOLNN_LAYERS_
#define _BOOLNN_LAYERS_

#include <vector>
#include <fstream>

#include <cstdint>


// 
// boolean matrix class
// 
class BoolMat {

	friend class LayerGeneric;
	
public:
	BoolMat(unsigned, unsigned);
	virtual ~BoolMat();
	
	// operators
	// not sure how to name and what to overload
	// BoolMat operator* (BoolMat);
	// std::vector<bool> operator* (std::vector<bool>);
	BoolMat operator& (const BoolMat&) const;
	std::vector<bool> operator& (const std::vector<bool>&) const;
	BoolMat operator^ (const BoolMat&) const;

	// io
	bool Write (std::fstream&) const;
	bool Read (std::fstream&);


	unsigned GetNDOF() const;
	// idk about this
	// std::vector<bool> operator[] (unsigned);

private:
	unsigned M, N;

	std::vector<bool> *W; 	// or bool **W ???? one vector saves space
	// use boost bit field?
};

bool operator& (const std::vector<bool>&, const std::vector<bool>&);
std::vector<bool> operator^ (const std::vector<bool>&, const std::vector<bool>&);




//
// Layer types
// 
enum class LayerKind : char {
	BIT = 1,
	DWORD = 2,
	BIT_BIAS = 3,
	DWORD_BIAS = 4,
	ADD = 5
};


inline bool is_Biased (LayerKind k) {
	return (k == LayerKind::BIT_BIAS) || (k == LayerKind::DWORD_BIAS);
}

inline bool is_BIT (LayerKind k) {
	return (k == LayerKind::BIT) || (k == LayerKind::BIT_BIAS);
}


// 
// layer for all bits
// 
class LayerGeneric {

public:
	LayerGeneric(LayerKind, unsigned, unsigned); // input, output dim
	std::vector<bool> Compute(const std::vector<bool>&) const;
	unsigned GetNDOF() const;	// DOF (number of parameters)
	void FlipBit(unsigned);

	// io
	bool Write (std::fstream&) const;
	bool Read (std::fstream&);

private:
	// std::vector<bool *> weights();
	// retreive pointer to bits for external
	// manipulation. needed???

	LayerKind kind;
	unsigned N_in, N_out, N_bits;

	BoolMat W;		// weight matrix
	std::vector<bool> B;	// biases
};




//
// Boolean Neural Network
// 
class BoolNN {

public:
	BoolNN();

	// file i/o
	bool LoadFile(const std::string&);
	bool DumpFile(const std::string&);

	// construction/modification
	bool AddLayer(LayerGeneric);

	// simulation
	void FlipBit(unsigned);
	std::vector<bool> Compute(const std::vector<bool>&);

private:
	std::vector<LayerGeneric> layers;
	// make parallel layers????

};





#endif	// _BOOLNN_LAYERS_
