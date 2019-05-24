/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/

#pragma once
#include <cstdint>
#include <vector>
#include <limits>

namespace psy {

struct PsythonEvent
{
	int32_t mType;
};

using PsythonEventCallback = void(*)(void* inContext, const PsythonEvent& inEvent);

class PsythonEventNotifier
{
public:
	enum : int32_t { INVALID_LISTENER_ID = std::numeric_limits<int32_t>::max() };
public:
	int32_t addListener(int32_t inEventType, PsythonEventCallback inCallback, void* inContext);
	void* getCallbackContext(int32_t inListenerId) const;
	void removeListener(int32_t inListenerId);
	size_t getListenerCount() const;
	int32_t getListenerIdByIndex(size_t inIndex) const;
	
protected:
	void notify(const PsythonEvent& inEvent);
	
private:
	static int32_t getNextListenerId();
	
private:
	struct PsythonEventListener
	{
		int32_t mEventType;	
		int32_t mListenerId;
		PsythonEventCallback mCallback;
		void* mContext;
	};
	std::vector<PsythonEventListener> mListeners;
	
};
	
} // namespace psy
