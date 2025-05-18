#ifndef TIBIA_PRIORITY_QUEUE_HH_
#define TIBIA_PRIORITY_QUEUE_HH_ 1

#include "main.hh"
#include "vector.hh"

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

#endif //TIBIA_PRIORITY_QUEUE_HH_
