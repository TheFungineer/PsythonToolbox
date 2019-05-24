/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#include "LinkedList.hpp"
#include <algorithm>
#include <cassert>

namespace psy {

LinkUnit::LinkUnit()
	: mOwner(nullptr)
	, mNext(nullptr)
	, mPrevious(nullptr)
{

}

void LinkUnit::recycle()
{
	if (mOwner) {
		mOwner->pushFront(unchain());
	}
}

LinkUnit* LinkUnit::unchain()
{
	if (mPrevious) {
		mPrevious->mNext = mNext;
	}
	if (mNext) {
		mNext->mPrevious = mPrevious;
	}
	mNext = nullptr;
	mPrevious = nullptr;
	return this;
}

bool LinkUnit::isChained() const
{
	// A chained element will always at least have a previous element
	// corresponding to the list head internal structure.
	return mPrevious != nullptr; 
}

LinkUnit* LinkUnit::next() const
{
	return mNext;
}

LinkUnit* LinkUnit::previous() const
{
	// do not return LinkedList Head
	if (mPrevious && mPrevious->mPrevious) { 
		return mPrevious;
	}
	return nullptr;
}

LinkedList* LinkUnit::getOwner() const
{
	return mOwner;
}

/*/////////////////////////////*/

LinkedList::LinkedList()
{
	// no inline
}

LinkedList::~LinkedList()
{
	clear();
}

void LinkedList::pushFront(LinkUnit* inObject)
{
	if (!inObject) {
		return;
	}
	inObject->unchain();
	inObject->mNext = mNext;
	inObject->mPrevious = this;
	if (mNext) {
		mNext->mPrevious = inObject;
	}
	mNext = inObject;
}

LinkUnit* LinkedList::popFront()
{
	if (isEmpty()) {
		return nullptr;
	}
	return mNext->unchain();
}

void LinkedList::clear()
{
	LinkUnit* lHead = mNext;
	while (lHead) {
		if (lHead->mOwner != this && lHead->mOwner != nullptr) {
			lHead->recycle();
		}
		else {
			lHead->unchain();
		}
		lHead = mNext;
	}
	mNext = nullptr;
}

bool LinkedList::isEmpty() const
{
	return !mNext;
}

LinkUnit* LinkedList::front() const
{
	return mNext;
}

void LinkedList::acquireOwnership(LinkUnit* inObject)
{
	if (!inObject) {
		return;
	}
	inObject->mOwner = this;
}

void LinkedList::revokeOwnership(LinkUnit* inObject)
{
	if (!inObject) {
		return;
	}
	assert(inObject->mOwner == this);
	inObject->mOwner = nullptr;
}

} // namespace psy