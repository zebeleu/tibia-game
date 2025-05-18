#ifndef TIBIA_VECTOR_HH_
#define TIBIA_VECTOR_HH_ 1

#include "main.hh"

// NOTE(fusion): What the actual fuck. This is an ever growing dynamic array
// with each access through `operator()` growing it to make sure the requested
// index is valid, even for negative indices.
template<typename T>
struct vector{
	// REGULAR FUNCTIONS
	// =========================================================================
	vector(int min, int max, int block){
		int space = (max - min) + 1;
		if(space < 1){
			error("vector: Ungueltige Feldgroesse %d bis %d.\n", min, max);
			space = 1;
		}

		if(block < 0){
			error("vector: Ungueltige Blockgroesse %d.\n", block);
			block = 0;
		}

		this->min = min;
		this->max = max;
		this->start = min;
		this->space = space;
		this->block = block;
		this->initialized = false;
		this->entry = new T[this->space];
	}

	vector(int min, int max, int block, T init) : vector(min, max, block) {
		this->initialized = true;
		this->init = init;
		for(int i = 0; i < this->space; i += 1){
			this->entry[i] = init;
		}
	}

	// TODO(fusion): Probably missing some inlined destructor?

	T *at(int index){
		// TODO(fusion): This is probably not the best way to achieve this.

		while(index < this->start){
			int increment = this->block;
			if(increment == 0){
				increment = this->space;
			}

			T *entry = new T[this->space + increment];
			for(int i = this->min; i <= this->max; i += 1){
				int old_index = i - this->start;
				int new_index = old_index + increment;

				// TODO(fusion): Do we actually need to swap elements here? I'm
				// assuming some non-trivial structures that would invoke their
				// destructors when `this->entry` gets deleted just below.
				std::swap(entry[new_index], this->entry[old_index]);
			}

			if(this->entry != NULL){
				delete[] this->entry;
			}
			this->entry = entry;
			this->start -= increment;
			this->space += increment;
		}

		while(index >= (this->start + this->space)){
			int increment = this->block;
			if(increment == 0){
				increment = this->space;
			}

			T *entry = new[this->space + increment];
			for(int i = this->min; i <= this->max; i += 1){
				int old_index = i - this->start;
				int new_index = old_index;

				// TODO(fusion): Same as above.
				std::swap(entry[new_index], this->entry[old_index]);
			}

			if(this->entry != NULL){
				delete[] this->entry;
			}
			this->entry = entry;
			this->space += increment;
		}

		while(index < this->min){
			this->min -= 1;
			if(this->initialized){
				this->entry[this->min - this->start] = this->init;
			}
		}

		while(index > this->max){
			this->max += 1;
			if(this->initialized){
				this->entry[this->max - this->start] = this->init;
			}
		}

		return &this->entry[index - this->start];
	}

	// DATA
	// =========================================================================
	int min;
	int max;
	int start;
	int space;
	int block;
	bool initialized;
	T init;
	T *entry;
};

#endif //TIBIA_VECTOR_HH_
