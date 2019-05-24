/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#include "PsythonScene.hpp"

namespace psy {

StackAllocator::StackAllocator(MemoryPool& inMemoryPool)
	: mMemoryPool(inMemoryPool)
{
	
}

StackAllocator::~StackAllocator()
{
	free();
}

void* StackAllocator::alloc(size_t inSize, intptr_t inAlignment)
{
	auto* lHead = static_cast<MemoryChunk*>(mCommitedMemoryChunks.front());
	if (lHead && inSize + inAlignment > lHead->getChunkSize()) {
		return nullptr;
	}
	while (lHead) {
		void* lAlloc = lHead->suballoc(insize, inAlignment);
		if (lAlloc) {
			return lAlloc;
		}
		lHead = lHead->next();
	}
	lHead = mMemoryPool.allocateChunk();
	if (!lHead) {
		return nullptr;
	}
	void* lAlloc = lHead->suballoc(insize, inAlignment);
	if (!lAlloc) {
		return nullptr;
	}
	mCommitedMemoryChunks.pushFront(lHead);
	return lAlloc;
}

void StackAllocator::free()
{
	auto* lHead = static_cast<MemoryChunk*>(mCommitedMemoryChunks.front());
	while (lHead) {
		lHead->free();
		lHead->recycle();
		lHead = static_cast<MemoryChunk*>(mCommitedMemoryChunks.front());
	}
}
	
} // namespace psy