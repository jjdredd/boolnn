#include <iostream>
#include <fstream>

#include "layers.hpp"



BoolNN::BoolNN() {
	version = 1;
}



BoolNN::BoolNN(unsigned N_in) : N_in(N_in) {
	version = 1;
}



bool BoolNN::LoadFile(const char* file) {

	std::fstream fd(file);
	char magic[3];
	uint32_t version, nl;

	if (!fd.is_open()) {
		std::cerr << "failed to open " << file << std::endl;
		return false;
	}

	fd.read (magic, sizeof(magic));
	if (magic[0] != 'B' || magic[1] != 'N' || magic[2] != 'N') {
		std::cerr << "bad magic" << std::endl;
		return false;
	}

	fd.read ((char *) &version, sizeof(uint32_t));
	if (version != 1) {
		std::cerr << "bad version" << std::endl;
		return false;
	}
	
	fd.read ((char *) &version, sizeof(uint32_t));
	fd.read ((char *) &nl, sizeof(uint32_t));
	N_layers = nl;

	for (unsigned i = 0; i < N_layers; i++) {
		// so we don't accidentally destruct when layer
		// goes out of scope we need it to be on heap, not
		// stack
		LayerGeneric* layer = new LayerGeneric(LayerKind::BIT, 1, 1);
		if (layer->Read(fd)) {
			AddLayer(layer); // check return value!!!!
		} else {
			std::cerr << "failed to read layer "
				  << i << std::endl;
			return false;
		}
	}

	return true;
}




bool BoolNN::DumpFile(const char* file) {

	std::fstream fd(file);
	uint32_t version, nl = N_layers;
	char magic[] = "BNN";

	if (!fd.is_open()) {
		std::cerr << "failed to open " << file << std::endl;
	}

	fd.write(magic, sizeof(magic));
	fd.write((char *) &version, sizeof(uint32_t));
	fd.write((char *) &nl, sizeof(uint32_t));

	for (unsigned i = 0; i < N_layers; i++) {
		if( !Layers[i].Write(fd) ) {
			std::cerr << "failed to dump layer "
				  << i << std::endl;
			return false;
		}
	}
	return true;
}



bool BoolNN::AddLayer(LayerGeneric* l) {
	
	if ( Layers.end()->N_out != l->N_in ) {
		return false;
	}
	Layers.push_back(*l);
	return true;
}



bool BoolNN::AddLayer(LayerKind k, unsigned size) {

	LayerGeneric* l = new LayerGeneric(k, Layers.end()->N_out, size);
	AddLayer(l);
	return true;
}


