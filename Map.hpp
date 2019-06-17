//An Indexable map using a skiplist.

#ifndef MAP_H
#define MAP_H

#include <utility>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <experimental/optional>

namespace cs540
{
	
template <typename Key_T, typename Mapped_T> 
class Iterator;
template <typename Key_T, typename Mapped_T> 
class ConstIterator;
template <typename Key_T, typename Mapped_T> 
class ReverseIterator;
	
template <typename Key_T, typename Mapped_T> 
class Map_Node
{
	public:
		//We make it optional so that no default constructor
		//of key or mapped is called when head or tail nodes
		//are constructed.
		std::experimental::optional<std::pair<Key_T, Mapped_T>> p;
		Map_Node *next;
		Map_Node *prev;		
		//Holds the next node in the "levels" of list
		std::vector<Map_Node *> ptrs;
		//Holds the distance to the next node per "level"
		std::vector<int> indexer;
		
		Map_Node() : Map_Node(1) {}
		Map_Node(int h) 
		{
			ptrs = std::vector<Map_Node *>(h); 
			indexer = std::vector<int>(h); 
			for(int x = 0; x < h; x++) indexer[x] = 1;
			next = nullptr; 
			prev = nullptr;
		}
		Map_Node(const std::pair<Key_T, Mapped_T> &pair) : Map_Node(pair, 1) {} 
		Map_Node(const std::pair<Key_T, Mapped_T> &pair, int h) : p(pair) 
		{
			ptrs = std::vector<Map_Node *>(h); 
			indexer = std::vector<int>(h); 
			for(int x = 0; x < h; x++) indexer[x] = 1;
			next = nullptr; 
			prev = nullptr;
		}
		size_t getHeight() {return ptrs.size();}
};

template <typename Key_T, typename Mapped_T> 
class Map
{
	public:
	
	class Iterator
	{
		public:
			friend class Map;
			Iterator() = delete;
			Iterator &operator++()
			{
				ptr = ptr -> next;
				return *this;
			}
			Iterator operator++(int)
			{
				Iterator rv(*this);
				ptr = ptr -> next;
				return rv;
			}
			Iterator &operator--()
			{
				ptr = ptr -> prev;
				return *this;
			}
			Iterator operator--(int)
			{
				Iterator rv(*this);
				ptr = ptr -> prev;
				return rv;
			}
			std::pair<Key_T, Mapped_T> &operator*() const{return ptr -> p.value();}
			std::pair<Key_T, Mapped_T> *operator->() const{return &(ptr -> p.value());}
			
			//Equality
			//ConstIterator comparisons are declared as free functions
			bool operator==(const Iterator &it) const{return ptr == it.ptr;}
			bool operator!=(const Iterator &it) const{return ptr != it.ptr;}
		
		private:
			Iterator(Map_Node<Key_T, Mapped_T> *p) : ptr(p) {};
			Map_Node<Key_T, Mapped_T> *ptr;
	};

	class ConstIterator
	{
		public:
			friend class Map;
			ConstIterator() = delete;
			ConstIterator &operator++()
			{
				ptr = ptr -> next;
				return *this;
			}
			ConstIterator operator++(int)
			{
				ConstIterator rv(*this);
				ptr = ptr -> next;
				return rv;
			}
			ConstIterator &operator--()
			{
				ptr = ptr -> prev;
				return *this;
			}
			ConstIterator operator--(int)
			{
				ConstIterator rv(*this);
				ptr = ptr -> prev;
				return rv;
			}
			const std::pair<Key_T, Mapped_T> &operator*() const{return ptr -> p.value();}
			const std::pair<Key_T, Mapped_T> *operator->() const{return &(ptr -> p.value());}
			
			//Equality
			bool operator==(const ConstIterator &it) const{return ptr == it.ptr;}
			bool operator!=(const ConstIterator &it) const{return ptr != it.ptr;}
			bool operator==(const Iterator &it) const{return ptr == it.ptr;}
			bool operator!=(const Iterator &it) const{return ptr != it.ptr;}
		
		private:
			ConstIterator(Map_Node<Key_T, Mapped_T> *p) : ptr(p) {};
			Map_Node<Key_T, Mapped_T> *ptr;
	};

	class ReverseIterator
	{
		public:
			friend class Map;
			ReverseIterator() = delete;
			ReverseIterator &operator++()
			{
				ptr = ptr -> prev;
				return *this;
			}
			ReverseIterator operator++(int)
			{
				ReverseIterator rv(*this);
				ptr = ptr -> prev;
				return rv;
			}
			ReverseIterator &operator--()
			{
				ptr = ptr -> next;
				return *this;
			}
			ReverseIterator operator--(int)
			{
				ReverseIterator rv(*this);
				ptr = ptr -> next;
				return rv;
			}
			std::pair<Key_T, Mapped_T> &operator*() const{return ptr -> p.value();}
			std::pair<Key_T, Mapped_T> *operator->() const{return &(ptr -> p.value());}
			
			//Equality
			bool operator==(const ReverseIterator &it) const{return ptr == it.ptr;}
			bool operator!=(const ReverseIterator &it) const{return ptr != it.ptr;}
		
		private:
			ReverseIterator(Map_Node<Key_T, Mapped_T> *p) : ptr(p) {};
			Map_Node<Key_T, Mapped_T> *ptr;
	};

		Map();
		Map(const Map &);
		Map &operator=(const Map &);
		Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
		~Map();
		size_t size() const;
		bool empty() const;
		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;
		ReverseIterator rbegin();
		ReverseIterator rend();
		Iterator find(const Key_T &);
		ConstIterator find(const Key_T &) const;
		Mapped_T &at(const Key_T &key);
		const Mapped_T &at(const Key_T &) const;
		Mapped_T &operator[](const Key_T &);
		Mapped_T &get(int);
		std::pair<Iterator, bool> insert(const std::pair<Key_T, Mapped_T> &);
		template<typename IT_T>
		void insert(IT_T range_beg, IT_T range_end);
		void erase(const Key_T &);
		void erase(Iterator pos);
		void clear();
		
		//Extra Functions
		inline int getLength() const{return length;}
		
	private:
		Map_Node<Key_T, Mapped_T> *head;
		//Tail is used for iterators only - ptrs[x] never points to tail
		Map_Node<Key_T, Mapped_T> *tail;		
		size_t length;
		friend class Iterator;
		friend class ConstIterator;
		friend class ReverseIterator;
};





//Constructors
template <typename Key_T, typename Mapped_T> 
Map<Key_T, Mapped_T>::Map()
{
	head = new Map_Node<Key_T, Mapped_T>();
	tail = new Map_Node<Key_T, Mapped_T>();
	head -> next = tail;
	tail -> prev = head;
	length = 0;
}

template <typename Key_T, typename Mapped_T> 
Map<Key_T, Mapped_T>::Map(const Map &m)
{
	head = new Map_Node<Key_T, Mapped_T>(m.head -> getHeight());
	tail = new Map_Node<Key_T, Mapped_T>();
	head -> next = tail;
	tail -> prev = head;
	
	Map_Node<Key_T, Mapped_T> *mNode = m.head -> next;
	while(mNode != m.tail)
	{
		insert(mNode -> p.value());
		mNode = mNode -> next;
	}
	
	length = m.length;
}

template <typename Key_T, typename Mapped_T> 
Map<Key_T, Mapped_T> &Map<Key_T, Mapped_T>::operator=(const Map &m)
{
	clear();
	delete head;
	delete tail;
	head = new Map_Node<Key_T, Mapped_T>(m.head -> getHeight());
	tail = new Map_Node<Key_T, Mapped_T>();
	head -> next = tail;
	tail -> prev = head;
	
	Map_Node<Key_T, Mapped_T> *mNode = m.head -> next;
	while(mNode != m.tail)
	{
		insert(mNode -> p.value());
		mNode = mNode -> next;
	}
	
	length = m.length;	
	return *this;
}

template <typename Key_T, typename Mapped_T>
Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list)
{
	head = new Map_Node<Key_T, Mapped_T>();
	tail = new Map_Node<Key_T, Mapped_T>();
	head -> next = tail;
	tail -> prev = head;
	length = 0;
	for(auto it = list.begin(); it != list.end(); ++it)
		insert(*it);
}

template <typename Key_T, typename Mapped_T> 
Map<Key_T, Mapped_T>::~Map()
{
	Map_Node<Key_T, Mapped_T> *del, *node = head;
	while(node != nullptr)
	{
		del = node;
		node = node -> next;
		delete del;
		del = 0;
	}
}

//Size
template <typename Key_T, typename Mapped_T>
size_t Map<Key_T, Mapped_T>::size() const {return length;}

template <typename Key_T, typename Mapped_T>
bool Map<Key_T, Mapped_T>::empty () const
{
	if(length <= 0) return true;
	return false;
}

//Iterators
template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin()
{
	return Map<Key_T, Mapped_T>::Iterator(head -> next);
}

template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end()
{
	return Map<Key_T, Mapped_T>::Iterator(tail);
}

template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const
{
	return Map<Key_T, Mapped_T>::ConstIterator(head -> next);
}

template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const
{
	return Map<Key_T, Mapped_T>::ConstIterator(tail);
}

template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin()
{
	return Map<Key_T, Mapped_T>::ReverseIterator(tail -> prev);
}

template <typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend()
{
	return Map<Key_T, Mapped_T>::ReverseIterator(head);
}


//Element Access
template <typename Key_T, typename Mapped_T> 
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T &key)
{
	Map_Node<Key_T, Mapped_T> *node = head;
	for(int x = node -> getHeight() - 1; x >= 0; x--)
		while(node -> ptrs[x] != nullptr)
		{
			if(node -> ptrs[x] -> p.value().first == key)
				return Map<Key_T, Mapped_T>::Iterator(node -> ptrs[x]);
			else if(node -> ptrs[x] -> p.value().first < key)
				node = node -> ptrs[x];
			else	break;
		}
	return end();	
}

template <typename Key_T, typename Mapped_T> 
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T &key) const
{
	Map_Node<Key_T, Mapped_T> *node = head;
	for(int x = node -> getHeight() - 1; x >= 0; x--)
		while(node -> ptrs[x] != nullptr)
		{
			if(node -> ptrs[x] -> p.value().first == key)
				return Map<Key_T, Mapped_T>::ConstIterator(node -> ptrs[x]);
			else if(node -> ptrs[x] -> p.value().first < key)
				node = node -> ptrs[x];
			else	break;
		}
	return end();	
}

template <typename Key_T, typename Mapped_T> 
Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key)
{
	Map_Node<Key_T, Mapped_T> *node = head;
	for(int x = node -> getHeight() - 1; x >= 0; x--)
		while(node -> ptrs[x] != nullptr)
		{
			if(node -> ptrs[x] -> p.value().first == key)
				return node -> ptrs[x] -> p.value().second;
			else if(node -> ptrs[x] -> p.value().first < key)
				node = node -> ptrs[x];
			else	break;
		}
	throw std::out_of_range ("");
}

template <typename Key_T, typename Mapped_T> 
const Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key) const
{
	Map_Node<Key_T, Mapped_T> *node = head;
	for(int x = node -> getHeight() - 1; x >= 0; x--)
		while(node -> ptrs[x] != nullptr)
		{
			if(node -> ptrs[x] -> p.value().first == key)
				return node -> ptrs[x] -> p.value().second;
			else if(node -> ptrs[x] -> p.value().first < key)
				node = node -> ptrs[x];
			else	break;
		}
	throw std::out_of_range ("");
}

template <typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::operator[] (const Key_T &key)
{
	try
	{
		return at(key);
	}
	catch(std::out_of_range e)
	{
		insert(std::make_pair(key, Mapped_T{}));
		return at(key);
	}
}

template <typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::get (int index)
{
	//Index starts from 1
	index++;
	if(index <= 0) throw std::out_of_range ("");
	
	Map_Node<Key_T, Mapped_T> *node = head;
	int z = 0;	
	for(int x = node -> getHeight() - 1; x >= 0; x--)
		while(node -> ptrs[x] != nullptr)
		{
			z += node -> indexer[x];
			if(z  == index)
				return node -> ptrs[x] -> p.value().second;
			else if(z < index)
				node = node -> ptrs[x];
			else	
			{
				z -= node -> indexer[x];
				break;
			}
		}
	throw std::out_of_range ("");
}

//Modifiers
template <typename Key_T, typename Mapped_T> 
std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(const std::pair<Key_T, Mapped_T> &in)
{
	Map_Node<Key_T, Mapped_T> *node = head;
	unsigned int h = node -> getHeight();
	//assert(h > 0); //Very bad

	//Determine which pointers need to be updated
	std::vector<Map_Node<Key_T, Mapped_T> *> updater(h);
	for(int x = h - 1; x >= 0; x--)
	{
		while(node -> ptrs[x] != nullptr && node -> ptrs[x] -> p.value().first < in.first)
			node = node -> ptrs[x];
		updater[x] = node;
	}

	//Return false if duplicate
	if(node -> next != tail && node -> next -> p.value().first == in.first)	
		return std::make_pair<Map<Key_T, Mapped_T>::Iterator, bool>(Map<Key_T, Mapped_T>::Iterator(node -> next), false);
	
	//Determine random height
	unsigned int newHeight = 1;
	while(true)
	{
		if(newHeight > h) break; //newHeight = h + 1
		if(rand() % 2 == 1) newHeight++;
		else	break;
	}
	
	//Create new node
	Map_Node<Key_T, Mapped_T> *ins = new Map_Node<Key_T, Mapped_T>(in, newHeight);
	length++;

	//Update pointers
	int z;
	for(unsigned int x = 0; x <= newHeight - 1; x++)
	{
		if(x >= updater.size()) break;
		//Pointers
		ins -> ptrs[x] = updater[x] -> ptrs[x];
		updater[x] -> ptrs[x] = ins;
		//Indexes
		z = 0;
		if(x > 0)
		{
			node = updater[x];
			//Use previous level to determine distance.
			//Should be lg(n) as previous level should have
			//	about lg(n) nodes between updater[x] and ins.
			while(node != ins)
			{
				z += node -> indexer[x - 1];
				node = node -> ptrs[x - 1];
			}
		}
		else z = 1;
		ins -> indexer[x] = updater[x] -> indexer[x] + 1 - z;
		updater[x] -> indexer[x] = z;
	}
	for(unsigned int x = newHeight; x < h; x++)
		updater[x] -> indexer[x]++;
	
	//Iterator pointers updated
	ins -> next = updater[0] -> next;
	if(updater[0] -> next != tail)
		updater[0] -> next -> prev = ins;
	updater[0] -> next = ins;
	ins -> prev = updater[0];
	if(ins -> next == tail) tail -> prev = ins;
	
	//Readjust height of head if necessary
	if(newHeight > h)
	{
		head -> ptrs.push_back(ins);
		ins -> ptrs[h] = nullptr;
		
		//Write new indexes
		node = head;
		z = 0;
		while(node != nullptr)
		{
			z += node -> indexer[h - 1];
			node = node -> ptrs[h - 1];
			if(node == ins)
			{
				head -> indexer.push_back(z);
				z = 0;
			}
			else if(node == nullptr)
				ins -> indexer[h] = z;
		}	
	}

	return std::make_pair<Map<Key_T, Mapped_T>::Iterator, bool>(ins, true);
}

template <typename Key_T, typename Mapped_T>
template <typename IT_T>
void Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end)
{
	for(range_beg; range_beg != range_end; range_beg++)
		insert(*range_beg);
}

template <typename Key_T, typename Mapped_T> 
void Map<Key_T, Mapped_T>::erase(const Key_T &key)
{	
	Map_Node<Key_T, Mapped_T> *node = head;
	unsigned int h = node -> getHeight();
	//assert(h > 0); //Very bad

	//Determine which pointers need to be updated
	std::vector<Map_Node<Key_T, Mapped_T> *> updater(h);
	for(int x = h - 1; x >= 0; x--)
	{
		while(node -> ptrs[x] != nullptr && node -> ptrs[x] -> p.value().first < key)
			node = node -> ptrs[x];
		updater[x] = node;
	}

	//printf("%d	%d\n", node -> next -> p.value().first, key);
	if(node -> next != tail && node -> next -> p.value().first == key)
	{
		length--;
		node = node -> next;
		//Update pointers
		for(int x = 0; x < h; x++)
		{
			updater[x] -> indexer[x]--;
			if(updater[x] -> ptrs[x] != node)
				continue;
			updater[x] -> indexer[x] += node -> indexer[x];
			updater[x] -> ptrs[x] = node -> ptrs[x];
		}
		//Update iterator pointers
		updater[0] -> next = node -> next;
		if(node -> next != tail)
			node -> next -> prev = node -> prev;
		
		//Reduce height if needed
		if(h - 1 > 1 && head -> ptrs[h - 1] == nullptr)
		{
			head -> ptrs.pop_back();
			head -> indexer.pop_back();
		}
	
		if(node -> next == tail) tail -> prev = node -> prev;
		
		delete node;
		node = 0;
	}
	else	throw std::out_of_range ("");
}

template <typename Key_T, typename Mapped_T> 
void Map<Key_T, Mapped_T>::erase(Iterator pos)
{
	erase((*pos).first);
}

template <typename Key_T, typename Mapped_T> 
void Map<Key_T, Mapped_T>::clear()
{
	Map_Node<Key_T, Mapped_T> *del, *node = head -> next;
	if(node != tail)
	{
		do
		{
			del = node;
			if(node -> next != tail)
				node = node -> next;
			delete del;
		} while(del != node && node != tail);
	}

	for(unsigned int x = 0; x < head -> getHeight(); x++)
	{
		head -> ptrs[x] = nullptr;
		head -> indexer[x] = 1;
	}
	head -> next = tail;
	tail -> prev = head;
	length = 0;
}

//Comparison
template <typename Key_T, typename Mapped_T> 
bool operator==(const Map<Key_T, Mapped_T> &mOne, const Map<Key_T, Mapped_T> &mTwo)
{
	if(mOne.getLength() != mTwo.getLength()) return false;
	auto itTwo = mTwo.begin();
	for(auto itOne = mOne.begin(); itOne != mOne.end(); ++itOne)
	{
		if(*itOne != *itTwo) return false;
		++itTwo;
	}
	return true;
}

template <typename Key_T, typename Mapped_T> 
bool operator!=(const Map<Key_T, Mapped_T> &mOne, const Map<Key_T, Mapped_T> &mTwo) {return !(mOne == mTwo);}

template <typename Key_T, typename Mapped_T> 
bool operator<(const Map<Key_T, Mapped_T> &mOne, const Map<Key_T, Mapped_T> &mTwo)
{
	auto itTwo = mTwo.begin();
	for(auto itOne = mOne.begin(); itOne != mOne.end() && itTwo != mTwo.end(); ++itOne)
	{
		if(*itOne < *itTwo) return true;
		else if(*itOne > *itTwo) return false;
		++itTwo;
	}
	if(mOne.getLength() < mTwo.getLength()) return true;
	if(mTwo.getLength() < mOne.getLength()) return false;
}

template <typename Key_T, typename Mapped_T> 
bool operator==(const Iterator<Key_T, Mapped_T> &itOne, const ConstIterator<Key_T, Mapped_T> &itTwo) {return itOne.ptr == itTwo.ptr;}

template <typename Key_T, typename Mapped_T> 
bool operator!=(const Iterator<Key_T, Mapped_T> &itOne, const ConstIterator<Key_T, Mapped_T> &itTwo) {return itOne.ptr != itTwo.ptr;}

}

#endif
