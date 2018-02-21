#include <iostream>
#include <cstdint>
#include <cstdio>

int main(int argc, char **argv) {

	std::cout << UINT32_MAX << std::endl;
	printf("0x%x\n", UINT32_MAX);

	bool a = false, b = true;

	std::cout << (a ^ b) << " " << (a & b)
		  << " " << (a && b) << std::endl;

	return 0;
}
