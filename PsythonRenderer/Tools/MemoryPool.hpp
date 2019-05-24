/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#pragma once
#include "LinkedList.hpp"
#include <vector>
#include <cstdint>
#include <limits>

namespace psy {

namespace details {
	
class MemoryChunk : public LinkUnit
{
public:
	MemoryChunk(void* inMemory);
	void* suballoc(size_t inSize, intptr_t inAlignment);
	void free();
	size_t getChunkSize() const;
	intptr_t getBaseAlignment() const;
	
private:
	ptrdiff_t mTop;
	intptr_t mChunkStart;
};

/*////////////////////////////*/

class GuardedAlloc
{
public:
	GuardedAlloc(size_t inAllocSize, intptr_t inBaseAlignment);
	GuardedAlloc(GuardedAlloc&& inOther) noexcept;
	GuardedAlloc(const GuardedAlloc&) = delete;
	~GuardedAlloc();
	GuardedAlloc& operator=(GuardedAlloc&& inOther) noexcept;	
	GuardedAlloc& operator=(const GuardedAlloc&) = delete;	
		
	MemoryChunk* getMemoryChunk() const;
		
private:
	void release();
	
private:	
	void* mMemory;
};

/*////////////////////////////*/

class MemoryChunkCollection : public LinkedList
{
public:
	MemoryChunkCollection(size_t inChunkSize, size_t inInitialChunkCount, intptr_t inBaseAlignment);
	~MemoryChunkCollection();
	MemoryChunkCollection(const MemoryChunkCollection&) = delete;
	MemoryChunkCollection& operator=(const MemoryChunkCollection&) = delete;
	
	size_t getChunkSize() const;
	intptr_t getBaseAlignment() const;
	bool commitAdditionalMemory(size_t inChunkCount);
	size_t getChunksCount() const;
	
private:
	size_t mChunkSize;
	intptr_t mBaseAlignment;
	std::vector<details::GuardedAlloc> mChunksPool;
}

} // namespace details

/*////////////////////////////////*/

class MemoryPool
{
public:
	enum : uint32_t { INVALID_CHUNK_ID = std::numeric_limits<uint32_t>::max() };
	
public:
	MemoryPool(size_t inChunkByteLength, size_t inInitialChunkCount, intptr_t inBaseAlignment);
	~MemoryPool();
	MemoryPool(const MemoryPool&) = delete;
	MemoryPool& operator=(const MemoryPool&) = delete;
	
	MemoryChunk* allocateChunk();
	
private:
	MemoryChunkCollection mFreeChunks;
};
	
} // namespace psy
