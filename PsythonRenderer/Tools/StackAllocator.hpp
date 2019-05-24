/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#pragma once
#include "MemoryPool.hpp"
#include "LinkedList.hpp"

namespace psy {
	


/*//////////////////////////////*/

class StackAllocator
{
public:
	StackAllocator(MemoryPool& inMemoryPool);
	~StackAllocator();
	StackAllocator(const StackAllocator&) = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;
	
	void* alloc(size_t inSize, intptr_t inAlignment);
	void free();
	
private:
	MemoryPool& mMemoryPool;
	LinkedList mCommitedMemoryChunks;
};
	
} // namespace psy
