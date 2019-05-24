/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/
#pragma once
#include <cstdint>

namespace psy {
class LinkedList;

class LinkUnit
{
public:
	virtual ~LinkUnit() = default;
	void recycle();
	LinkUnit* unchain();
	bool isChained() const;
	LinkedList* getOwner() const;
	LinkUnit* next() const;
	LinkUnit* previous() const;

protected:
	LinkUnit();

private:
	LinkedList* mOwner;
	LinkUnit* mNext;
	LinkUnit* mPrevious;
	friend class LinkedList;
};

/*////////////////////////*/

class LinkedList : private LinkUnit
{
public:
	LinkedList();
	~LinkedList();
	void pushFront(LinkUnit* inObject);
	LinkUnit* popFront();
	void clear();
	bool isEmpty() const;
	LinkUnit* front() const;

protected:
	void acquireOwnership(LinkUnit* inObject);
	void revokeOwnership(LinkUnit* inObject);
};

} // namespace psy
