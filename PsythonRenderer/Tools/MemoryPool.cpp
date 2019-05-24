/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#include "MemoryPool.hpp"
#include "Alignment.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <new>

namespace psy {
	
namespace details {
	
MemoryChunk::MemoryChunk(void* inMemory)
	: mChunkStart(reinterpret_cast<intptr_t>(inMemory))
	, mTop(0)
{
	assert(mChunkStart != nullptr);
}

void* MemoryChunk::suballoc(size_t inSize, intptr_t inAlignment)
{
	if (inSize > getChunkSize()) {
		return nullptr;
	}
	intptr_t lStart = mChunkStart + mTop;
	ptrdiff_t lFrontPadding = paddingFor(lStart, inAlignment);
	ptrdiff_t lTailPadding = paddingFor(static_cast<intptr_t>(inSize), inAlignment);
	ptrdiff_t lTotalSize = lFrontPadding + static_cast<ptrdiff_t>(inSize) + lTailPadding;
	if (getChunkSize() - static_cast<size_t>(mTop) >= static_cast<size_t>(lTotalSize)) {
		lStart += lFrontPadding;
		mTop += lTotalSize;
		assert(isAligned(lStart, inAlignment));
		assert(isAligned(mChunkStart + mTop, inAlignment));
		return reinterpret_cast<void*>(lStart);
	}
	return nullptr;
}

void MemoryChunk::free()
{
	mTop = 0;
}

size_t MemoryChunk::getChunkSize() const
{
	return static_cast<MemoryChunkCollection*>(getOwner())->getChunkSize();
}

intptr_t MemoryChunk::getBaseAlignment() const
{
	return static_cast<MemoryChunkCollection*>(getOwner())->getBaseAlignment();
}
	
/*/////////////////////////////*/
	
GuardedAlloc::GuardedAlloc(size_t inAllocSize, intptr_t inBaseAlignment)
	: mMemory(std::malloc(inAllocSize))
{
	if (mMemory) {
		uintptr_t lChunkStart = reinterpret_cast<uintptr_t>(mMemory) + sizeof(MemoryChunk);
		lChunkStart += paddingFor(static_cast<intptr_t>(lChunkStart), inBaseAlignment);
		new (mMemory) MemoryChunk(reinterpret_cast<void*>(lChunkStart));
	}
}

GuardedAlloc::GuardedAlloc(GuardedAlloc&& inOther) noexcept
	: mMemory(nullptr)
{
	std::swap(mMemory, inOther.mMemory);
	assert(inOther.mMemory == nullptr);
}

GuardedAlloc::~GuardedAlloc()
{
	release();
	assert(mMemory == nullptr);
}

GuardedAlloc& GuardedAlloc::operator=(GuardedAlloc&& inOther) noexcept
{
	release();
	assert(mMemory == nullptr);
	std::swap(mMemory, inOther.mMemory);
	return *this;
}

MemoryChunk* GuardedAlloc::getMemoryChunk() const
{
	return std::launder(reinterpret_cast<MemoryChunk*>(mMemory));
}

void GuardedAlloc::release()
{
	if (mMemory) {
		getMemoryChunk()->~MemoryChunk();
		std::free(mMemory);
		mMemory = nullptr;
	}
}

/*/////////////////////////////////////*/

MemoryChunkCollection::MemoryChunkCollection(size_t inChunkSize, size_t inInitialChunkCount, intptr_t inBaseAlignment)
	: mChunkSize(mChunkSize)
	, mBaseAlignment(inBaseAlignment)
{
	commitAdditionalMemory(inInitialChunkCount);
}

MemoryChunkCollection::~MemoryChunkCollection()
{
	
}

size_t MemoryChunkCollection::getChunkSize() const
{
	return mChunkSize;
}

intptr_t MemoryChunkCollection::getBaseAlignment() const
{
	return mBaseAlignment;
}

bool MemoryChunkCollection::commitAdditionalMemory(size_t inChunkCount)
{
	if (mChunksPool.size() + inChunkCount > mChunksPool.capacity()) {
		mChunksPool.reserve(mChunksPool.size() + inChunkCount);
	}
	size_t lCount = 0;
	while (lCount != inChunkCount) {
		GuardedAlloc lNextAlloc(mChunkSize, mBaseAlignment);
		if (!lNextAlloc.getMemoryChunk()) {
			break;
		}
		MemoryChunk* lMemoryChunk = lNextAlloc.getMemoryChunk();
		acquireOwnership(lMemoryChunk);
		lMemoryChunk->recycle();
		mChunksPool.push_back(std::move(lNextAlloc));
		++lCount;
	}
	return lCount != 0;
}

size_t MemoryChunkCollection::getChunksCount() const
{
	return mChunksPool.size();
}

} // namespace details

/*/////////////////////////////////////*/

MemoryPool::MemoryPool(size_t inChunkSize, size_t inInitialChunkCount, intptr_t inBaseAlignment)
	: mFreeChunks(inChunkSize, inInitialChunkCount, inBaseAlignment)
{

}

MemoryPool::~MemoryPool()
{
	// no inline
}

MemoryChunk* MemoryPool::allocateChunk()
{
	if (mFreeChunks.isEmpty()) {
		if (!mFreeChunks.commitAdditionalMemoryChunks(mFreeChunks.getChunksCount())) {
			return nullptr;
		}
	}
	assert(!mFreeChunks.isEmpty());
	return static_cast<MemoryChunk*>(mFreeChunks.popFront());
}

} // namespace psy