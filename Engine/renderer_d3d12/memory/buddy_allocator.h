#pragma once
#include <vector>
#include <bitset>
namespace ue
{

static const int multiply_de_bruijn_bit_positions[] = 
{
	0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
	31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

inline unsigned count_trailing_bits(unsigned v) {
	return multiply_de_bruijn_bit_positions[((unsigned)((v & -v) * 0x077CB531U)) >> 27];
}
 
//#define BUDDY_DEBUG

// This version uses external book keeping, suitable for gpu allocations
class BuddyAllocatorExt
{
public:

	struct Block
	{
		Block *next;
		Block *prev;
		bool allocated;
#if defined(BUDDY_DEBUG)
		unsigned index;
#endif
	};

	BuddyAllocatorExt(void *buffer, size_t buffer_size, size_t leaf_size)
		: _buffer(buffer)
		, _buffer_size(buffer_size)
		, _leaf_size(leaf_size)
	{
		_num_levels = count_trailing_bits(buffer_size / leaf_size) + 1;
		unsigned block_count = (1 << _num_levels) - 1;
		_all_blocks.resize(block_count);
		memset(_free_list, 0, sizeof(void*)*MAX_LEVELS);
		Block *first_block = &_all_blocks[0];
		first_block->prev = nullptr;
		first_block->next = nullptr;
		_free_list[0] = &_all_blocks[0];

		size_t block_size = sizeof(Block);
		size_t book_keeping_size = block_size * block_count;

		int apa = 1;

	}

	~BuddyAllocatorExt()
	{}
	
	inline unsigned round_to_power_of_two(unsigned size) {
		size--;
		size |= size >> 1;
		size |= size >> 2;
		size |= size >> 4;
		size |= size >> 8;
		size |= size >> 16;
		size++;
		return size;
	}

	inline unsigned block_count_at_level(unsigned level) {
		return (_buffer_size / size_of_level(level));
	}

	inline unsigned level_from_size(size_t size) {
		return count_trailing_bits((unsigned)(_buffer_size / size));
	}

	inline unsigned size_of_level(unsigned level) {
		return _buffer_size / (1 << level);
	}

	inline unsigned level_index_from_block_index(unsigned block_index, unsigned level) {
		return -((1 << level) - block_index - 1);
	}

	inline unsigned block_index_from_level_index(unsigned index_in_level, unsigned level) {
		return (1 << level) + index_in_level - 1;
	}

	inline unsigned block_index_from_block(Block *block) {
		return (unsigned)((reinterpret_cast<size_t>(block) - reinterpret_cast<size_t>(&_all_blocks[0])) / (sizeof(Block)));
	}

	inline void *allocate_block(Block *block, unsigned level, size_t level_size) {
		unsigned block_index = block_index_from_block(block);
		unsigned level_index = level_index_from_block_index(block_index, level);
		block->allocated = true;
		return reinterpret_cast<void*>(reinterpret_cast<size_t>(_buffer) + level_index * level_size);
	}

	inline Block *alloc_at_level(unsigned level) {
		if (_free_list[level] == nullptr) {
			if (level == 0)
				return nullptr;
			Block *block = alloc_at_level(level - 1);
			if (block == nullptr)
				return nullptr;
			unsigned block_index = block_index_from_block(block);
			unsigned parent_level_index = level_index_from_block_index(block_index, level - 1);
			unsigned this_level_index = parent_level_index * 2;
			unsigned this_index = block_index_from_level_index(this_level_index, level);
			Block *block_a = &_all_blocks[this_index];
			Block *block_b = &_all_blocks[this_index + 1];
#if defined(BUDDY_DEBUG)
			block_a->index = this_index;
			block_b->index = this_index + 1;
#endif
			block_b->next = _free_list[level];
			block_b->prev = block_a;
			block_a->next = block_b;
			block_a->prev = nullptr;
			_free_list[level] = block_a;
		}
		Block *block = (Block*)_free_list[level];
		block->allocated = true;
		_free_list[level] = block->next;
		if(_free_list[level])
			_free_list[level]->prev = nullptr;
		return block;
	}

	inline void *allocate(size_t size)
	{
		unsigned leaf_aligned_size = (size + (_leaf_size - 1) & ~(_leaf_size - 1));
		unsigned power_of_two_size = round_to_power_of_two(leaf_aligned_size);
		unsigned level = level_from_size(power_of_two_size);
		Block *block = alloc_at_level(level);
		if (block == nullptr)
			return nullptr;
		return allocate_block(block, level, power_of_two_size);
	}

	void deallocate(void *ptr, size_t size)
	{
		unsigned level = level_from_size(size);
		unsigned level_index = (reinterpret_cast<size_t>(ptr) - reinterpret_cast<size_t>(_buffer)) / size;
		unsigned index = block_index_from_level_index(level_index, level);
		Block *block = &_all_blocks[index];
		merge_block_at_level(block, level_index, level);
	}

	void deallocate(void *ptr)
	{
		unsigned leaf_index = (reinterpret_cast<size_t>(ptr) - reinterpret_cast<size_t>(_buffer)) / _leaf_size;
		unsigned index = block_index_from_level_index(leaf_index, _num_levels - 1);
		unsigned level = _num_levels - 1;
		Block *found_block = nullptr;
		while (level > 0) {
			Block *block = &_all_blocks[index];
			if (block->allocated) {
				found_block = block;
				break;
			}
			index /= 2;
			level--;
		}
		merge_block_at_level(found_block, level_index_from_block_index(index, level), level);
	}

	inline void merge_block_at_level(Block *block, unsigned level_index, unsigned level) {
		

		Block *buddy = (level_index & 1) ? block - 1 : block + 1;
		if (!buddy->allocated) {
			unsigned parent_level_index = level_index / 2;
			unsigned parent_index = block_index_from_level_index(parent_level_index, level - 1);
			Block *parent_block = &_all_blocks[parent_index];
			merge_block_at_level(parent_block, parent_level_index, level - 1);
			
			// remove buddy from linked list
			if(buddy->prev)
				buddy->prev->next = buddy->next;
			if (_free_list[level] == buddy) {
				_free_list[level] = _free_list[level]->next;
			}
		} else {
			block->allocated = false;
			block->next = _free_list[level];
			if (_free_list[level])
				_free_list[level]->prev = block;
			block->prev = nullptr;
			_free_list[level] = block;
		}
	}


	enum {MAX_LEVELS = 32};
	Block *_free_list[MAX_LEVELS];
	unsigned _num_levels;
	size_t _buffer_size;
	size_t _leaf_size;

	void *_buffer;
	std::vector<Block> _all_blocks;

};

}