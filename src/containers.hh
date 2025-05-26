#ifndef TIBIA_CONTAINERS_HH_
#define TIBIA_CONTAINERS_HH_ 1

#include "common.hh"

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

			T *entry = new T[this->space + increment];
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

template<typename K, typename T>
struct priority_queue_entry{
	K Key;
	T Data;
};

template<typename K, typename T>
struct priority_queue{
	// REGULAR FUNCTIONS
	// =========================================================================
	priority_queue(int capacity, int increment){
		Entry = new vector<priority_queue_entry<K, T>>(1, capacity, increment);
		Entries = 0;
	}

	// TODO(fusion): Probably missing some inlined destructor?

	void insert(K Key, T *Data){
		this->Entries += 1;
		int CurrentIndex = this->Entries;
		*this->Entry->at(CurrentIndex) = {Key, *Data};
		while(CurrentIndex > 1){
			int ParentIndex = CurrentIndex / 2;
			priority_queue_entry<K, T> *Current = this->Entry->at(CurrentIndex);
			priority_queue_entry<K, T> *Parent = this->Entry->at(ParentIndex);
			if(Parent->Key <= Current->Key)
				break;
			std::swap(*Current, *Parent);
			CurrentIndex = ParentIndex;
		}
	}

	void deleteMin(void){
		if(this->Entries < 1){
			error("priority_queue::deleteMin: Warteschlange ist leer.\n");
			return;
		}

		if(this->Entries > 1){
			int CurrentIndex = 1;
			int LastIndex = this->Entries;
			std::swap(*this->Entry->at(CurrentIndex),
					*this->Entry->at(LastIndex));

			// TODO(fusion): This may be an oversight but the decompiled version
			// checks in the loop below would INCLUDE `LastIndex`, which I assume
			// is a bug? The first and last elements were just swapped and what
			// was the first element in the queue is now at the end and should be
			// considered "removed".
			while(1){
				int SmallestIndex = CurrentIndex * 2;
				if(SmallestIndex >= LastIndex){
					break;
				}

				priority_queue_entry<K, T> *Smallest = this->Entry->at(SmallestIndex);
				if((SmallestIndex + 1) < LastIndex){
					priority_queue_entry<K, T> *Other = this->Entry->at(SmallestIndex + 1);
					if(Other->Key < Smallest->Key){
						Smallest = Other;
						SmallestIndex += 1;
					}
				}

				priority_queue_entry<K, T> *Current = this->Entry->at(CurrentIndex);
				if(Current->Key <= Smallest->Key){
					break;
				}

				std::swap(*Current, *Smallest);
				CurrentIndex = SmallestIndex;
			}
		}

		this->Entries -= 1;
	}

	// DATA
	// =========================================================================
	vector<priority_queue_entry<K, T>> *Entry;
	int Entries;
};

// TODO(fusion): We only use this structure two global queues:
//	priority_queue<uint32, uint32>			ToDoQueue(5000, 1000);
//	priority_queue<uint32, TAttackWave*>	AttackWaveQueue(100, 100);

template<typename T>
struct matrix{
	// REGULAR FUNCTIONS
	// =========================================================================
	matrix(int xmin, int xmax, int ymin, int ymax){
		int dx = (xmax - xmin) + 1;
		int dy = (ymax - ymin) + 1;

		if(dx < 1 || dy < 1){
			error("matrix: Ungueltige Feldgroesse %d..%d, %d..%d.\n", xmin, xmax, ymin, ymax);

			if(dx < 1){
				dx = 1;
			}

			if(dy < 1){
				dy = 1;
			}
		}

		this->xmin = xmin;
		this->ymin = ymin;
		this->dx = dx;
		this->dy = dy;
		this->entry = new T[dx * dy];
	}

	matrix(int xmin, int xmax, int ymin, int ymax, T init) : matrix(xmin, xmax, ymin, ymax) {
		int count = this->dx * this->dy;
		for(int i = 0; i < count; i += 1){
			this->entry[i] = init;
		}
	}

	// TODO(fusion): Probably missing some inlined destructor?

	T *at(int x, int y){
		int xoffset = x - this->xmin;
		int yoffset = y - this->ymin;
		if(xoffset < 0 || xoffset >= this->dx || yoffset < 0 || yoffset >= this->dy){
			error("matrix::operator(): Ungueltiger Index %d/%d.\n", x, y);
			return &this->entry[0];
		}else{
			// TODO(fusion): Are we really storing this in row major order?
			return &this->entry[yoffset * this->dx + xoffset];
		}
	}

	// DATA
	// =========================================================================
	int xmin;
	int ymin;
	int dx;
	int dy;
	T *entry;
};

template<typename T>
struct matrix3d{
	// REGULAR FUNCTIONS
	// =========================================================================
	matrix3d(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax){
		int dx = (xmax - xmin) + 1;
		int dy = (ymax - ymin) + 1;
		int dz = (zmax - zmin) + 1;

		if(dx < 1 || dy < 1 || dz < 1){
			error("matrix3d: Ungueltige Feldgroesse %d..%d, %d..%d, %d..%d.\n",
					xmin, xmax, ymin, ymax, zmin, zmax);

			if(dx < 1){
				dx = 1;
			}

			if(dy < 1){
				dy = 1;
			}

			if(dz < 1){
				dz = 1;
			}
		}

		this->xmin = xmin;
		this->ymin = ymin;
		this->dx = dx;
		this->dy = dy;
		this->entry = new T[dx * dy];
	}

	matrix3d(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, T init)
			: matrix3d(xmin, xmax, ymin, ymax, zmin, zmax) {
		int count = this->dx * this->dy * this->dz;
		for(int i = 0; i < count; i += 1){
			this->entry[i] = init;
		}
	}

	// TODO(fusion): Probably missing some inlined destructor?

	T *at(int x, int y, int z){
		int xoffset = x - this->xmin;
		int yoffset = y - this->ymin;
		int zoffset = z - this->zmin;
		if(xoffset < 0 || xoffset >= this->dx
				|| yoffset < 0 || yoffset >= this->dy
				|| zoffset < 0 || zoffset >= this->dz){
			error("matrix3d::operator(): Ungueltiger Index %d/%d/%d.\n", x, y, z);
			return &this->entry[0];
		}else{
			// TODO(fusion): Same as `matrix::at` on the XY plane.
			return &this->entry[zoffset * this->dx * this->dy
								+ yoffset * this->dx
								+ xoffset];
		}
	}

	// DATA
	// =========================================================================
	int xmin;
	int ymin;
	int zmin;
	int dx;
	int dy;
	int dz;
	T *entry;
};

#endif //TIBIA_CONTAINERS_HH_