#include <iostream>
#include <fstream>

#include "layers.hpp"



BoolNN::BoolNN()
	: N_layers(0) {
	version = 1;
}



BoolNN::BoolNN(unsigned N_in)
	: N_in(N_in), N_layers(0) {
	version = 1;
}


BoolNN::~BoolNN() {

	for (LayerGeneric *l : Layers) {
		l->~LayerGeneric();
	}
}


bool BoolNN::LoadFile(const char* file) {

	std::fstream fd;
	char magic[3];
	uint32_t ver = 0, nl = 0;

	fd.open(file, std::fstream::in | std::fstream::binary);
	if (!fd.is_open()) {
		std::cerr << "failed to open " << file << std::endl;
		return false;
	}

	fd.read (magic, sizeof(magic));
	if (magic[0] != 'B' || magic[1] != 'N' || magic[2] != 'N') {
		std::cerr << "bad magic" << std::endl;
		return false;
	}

	fd.read ((char *) &ver, sizeof(uint32_t));
	if (version != ver) {
		std::cerr << "bad version "
			  << ver << std::endl;
		return false;
	}
	
	fd.read ((char *) &nl, sizeof(uint32_t));

	Layers.clear();
	N_layers = 0;		// cant set N_layers yet since they're not in!!
	for (unsigned i = 0; i < nl; i++) {
		// so we don't accidentally destruct when layer
		// goes out of scope we need it to be on heap, not
		// stack
		LayerGeneric* l = new LayerGeneric(LayerKind::BIT, 1, 1);
		if (l->Read(fd)) {
			AddLayer(l); // check return value!!!!
		} else {
			std::cerr << "failed to read layer "
				  << i << std::endl;
			return false;
		}
	}


	N_layers = nl;
	return true;
}




bool BoolNN::DumpFile(const char* file) const {

	std::fstream fd;
	uint32_t nl = N_layers;
	char magic[] = "BNN";

	fd.open (file, std::fstream::out
		 | std::fstream::binary | std::fstream::trunc);
	if (!fd.is_open()) {
		std::cerr << "failed to open " << file << std::endl;
	}

	fd.write(magic, sizeof(magic) - 1);
	fd.write((char *) &version, sizeof(uint32_t));
	fd.write((char *) &nl, sizeof(uint32_t));

	for (unsigned i = 0; i < N_layers; i++) {
		if( !Layers[i]->Write(fd) ) {
			std::cerr << "failed to dump layer "
				  << i << std::endl;
			return false;
		}
	}
	return true;
}



bool BoolNN::AddLayer(LayerGeneric* l) {

	unsigned in_size;
	if (N_layers) {
		in_size = Layers[N_layers - 1]->N_out;
	} else {
		in_size = N_in;
	}
	
	if ( in_size != l->N_in ) {
		return false;
	}
	Layers.push_back(l);
	N_layers++;
	return true;
}



bool BoolNN::AddLayer(LayerKind k, unsigned size) {

	unsigned in_size;
	if (N_layers) {
		in_size = Layers[N_layers - 1]->N_out;
	} else {
		in_size = N_in;
	}

	LayerGeneric* l = new LayerGeneric(k, in_size, size);
	Layers.push_back(l);
	N_layers++;
	return true;
}


void BoolNN::FlipBit(unsigned N) {
	// we don't expect to have more than uint32_max dof

	unsigned ndof = 0;
	unsigned n = N%GetNDOF();		// clip it
	for (LayerGeneric *l : Layers) {
		ndof += l->GetNDOF();
		if (n < ndof) {
			l->FlipBit(n);
			break;
		}
	}
	return;
}

std::vector<bool> BoolNN::Compute(const std::vector<bool>& input) const {

	std::vector<bool> r = input;

	for (const LayerGeneric *l : Layers) {
		r = l->Compute(r);
	}
	return r;
}

unsigned BoolNN::GetNDOF() const {

	unsigned ndof = 0;
	for (const LayerGeneric *l : Layers) {
		ndof += l->GetNDOF();

	}
	return ndof;
}
