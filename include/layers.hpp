#ifndef _BOOLNN_LAYERS_
#define _BOOLNN_LAYERS_

#include <vector>
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
	std::vector<bool> *W; 	// or bool **W ????
	// use boost bitfield?
};

bool operator& (const std::vector<bool>,const std::vector<bool>);
std::vector<bool> operator^ (const std::vector<bool>, const std::vector<bool>);



// 
// layer for all bits
// 
class LayerBIT {

public:
	LayerBIT(unsigned, unsigned);
	virtual ~LayerBIT();
	std::vector<bool> Compute(std::vector<bool>);

private:
	std::vector<bool *> weights(); // retreive pointer to bits for external
				       // manipulation
	
	BoolMat W;		// weight matrix
	std::vector<bool> B;	// biases

};



// 
// layer for dword-wise operations
// 
class LayerDWORD {

public:
	LayerDWORD(unsigned, unsigned);
	virtual ~LayerDWORD(unsigned, unsigned);
	std::vector<bool> Compute(std::vector<bool>);

private:
	
	std::vector<BoolMat> W;	// weights (Dword-wise)
	std::vector<uint32_t> B; // biases (Dword-wise)

};

#endif	// _BOOLNN_LAYERS_
