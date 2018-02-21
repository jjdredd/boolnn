#ifndef _BOOLNN_LAYERS_
#define _BOOLNN_LAYERS_

#include <vector>
#include <list>
#include <cstdint>


// 
// boolean matrix class
// 
class BoolMat {
	
public:
	BoolMat(unsigned, unsigned);
	virtual ~BoolMat();
	
	// operators
	// not sure how to name and what to overload
	// BoolMat operator* (BoolMat);
	// std::vector<bool> operator* (std::vector<bool>);
	BoolMat operator& (const BoolMat) const;
	std::vector<bool> operator& (const std::vector<bool>) const;
	BoolMat operator^ (const BoolMat) const;
	// idk about this
	// std::vector<bool> operator[] (unsigned);

private:
	std::vector<bool> *W; 	// or bool **W ???? one vector saves space
	// use boost bit field?
	unsigned M, N;
};

bool operator& (const std::vector<bool>, const std::vector<bool>);
std::vector<bool> operator^ (const std::vector<bool>, const std::vector<bool>);



// 
// layer for all bits
// 
class LayerBIT {

public:
	LayerBIT(unsigned, unsigned); // input, output dim
	virtual ~LayerBIT();
	std::vector<bool> Compute(std::vector<bool>);
	unsigned GetSize();
	void FlipRandom(unsigned);

private:
	std::vector<bool *> weights(); // retreive pointer to bits for external
				       // manipulation. needed???
	
	BoolMat W;		// weight matrix
	std::vector<bool> B;	// biases

	unsigned N_in, N_out, N_bits;

};



// 
// layer for dword-wise operations
// 
class LayerDWORD {

public:
	LayerDWORD(unsigned, unsigned);
	virtual ~LayerDWORD();
	std::vector<bool> Compute(std::vector<bool>);
	unsigned GetSize();
	void FlipRandom(unsigned);

private:
	
	std::vector<BoolMat> W;	// weights (Dword-wise)
	std::vector<uint32_t> B; // biases (Dword-wise)

	unsigned N_in, N_out, N_bits;
};

// LayerADD??


//
// Layer types
// 
enum class LayerKind {
	BIT,
	DWORD,
	ADD
};



//
// Boolean Neural Network
// 
class BoolNN {

public:
	BoolNN();
	virtual ~BoolNN();

	// file i/o
	bool LoadFile(std::string);
	bool DumpFile(std::string);

	// construction/modification
	bool AddLayer(LayerDWORD *);
	bool AddLayer(LayerBIT *);

	// simulation
	void FlipRandom();
	std::vector<bool> Compute(std::vector<bool>);

private:
	std::list< std::pair<void *, unsigned> > layers;
	// make parallel layers????

};





#endif	// _BOOLNN_LAYERS_
