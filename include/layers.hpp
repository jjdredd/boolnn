#ifndef _BOOLNN_LAYERS_
#define _BOOLNN_LAYERS_

#include <vector>
#include <cstdint>

#include "bool_mat.hpp"


// layer for all bits
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


// layer for dword-wise operations
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
