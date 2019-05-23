#include "PsythonEvent.hpp"

namespace psy {
	
int32_t PsythonEventNotifier::addListener(int32_t inEventType, PsythonEventCallback inCallback, void* inContext)
{
	if (!inCallback) {
		return INVALID_LISTENER_ID;
	}
	int32_t lListenerId = getNextListenerId();
	mListeners.push_back({inEventType, lListenerId, inCallback, inContext});
	return lListenerId;
}

void* PsythonEventNotifier::getCallbackContext(int32_t inListenerId) const
{
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
		if (it->mListenerId == inListenerId) {
			return it->mContext;
		}
	}
	return nullptr;
}

void PsythonEventNotifier::removeListener(int32_t inListenerId)
{
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
		if (it->mListenerId == inListenerId) {
			mListeners.erase(it); 
			break;
		}
	}
}

size_t PsythonEventNotifier::getListenerCount() const
{
	return mListeners.size();
}

int32_t PsythonEventNotifier::getListenerIdByIndex(size_t inIndex) const
{
	if (!mListeners.empty() && inIndex < mListeners.size()) {
		return mListeners[inIndex].mListenerId;
	}
	return INVALID_LISTENER_ID;
}

void PsythonEventNotifier::notify(const PsythonEvent& inEvent)
{
	for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
		if (it->mEventType == inEvent.mType) {
			(it->mCallback)(it->mContext, inEvent);
		}
	}
}
	
int32_t PsythonEventNotifier::getNextListenerId()
{
	static int32_t sNextId = 0;
	return sNextId++;
}
	
} // namespace psy