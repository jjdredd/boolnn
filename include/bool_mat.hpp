#ifndef _BOOL_MAT_
#define _BOOL_MAT_

#include <vector>

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
	bool * operator[] (unsigned);

private:
	std::vector<bool> *W; 	// or bool **W ????

};

bool operator& (const std::vector<bool>,const std::vector<bool>);
std::vector<bool> operator^ (const std::vector<bool>, const std::vector<bool>);


#endif _BOOL_MAT_
