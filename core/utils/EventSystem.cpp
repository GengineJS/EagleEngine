/*
 *  EventSystem.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <utils/EventSystem.h>

namespace eg {
	namespace utils {
		EventSystem::EventSystem()
		{
		}

		EventSystem::~EventSystem()
		{
			off();
		}
		void EventSystem::off()
		{
			for (auto eve: _events) {
				eve.second.clear();
			}
			_events.clear();
			for (auto eve : _eventsOnce) {
				eve.second.clear();
			}
			_eventsOnce.clear();
		}
		void EventSystem::off(const std::string& name)
		{
			auto it = _events.find(name);
			if (it != _events.end()) {
				_events.at(name).clear();
				_events.erase(it);
			}
			auto once = _eventsOnce.find(name);
			if (once != _eventsOnce.end()) {
				_eventsOnce.at(name).clear();
				_eventsOnce.erase(once);
			}
		}
		
	}
}
