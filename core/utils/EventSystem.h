/*
 *  EventSystem.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
namespace eg {
	namespace utils {
		class EventSystem {
		public:
			EventSystem();
			virtual ~EventSystem();
			template<typename Callback>
			void on(const std::string& name, Callback cb) {
				auto it = _events.find(name);
				if (it == _events.end()) {
					_events[name] = {};
				}
				auto f = toFunction(cb);
				auto fn = new decltype(f)(toFunction(cb));
				_events[name].emplace_back(reinterpret_cast<void*>(fn));
			}
			template <typename Callback>
			void once(const std::string& name, Callback cb) {
			{
				auto it = _eventsOnce.find(name);
				if (it == _eventsOnce.end()) {
					_eventsOnce[name] = {};
				}
				auto f = toFunction(cb);
				auto fn = new decltype(f)(toFunction(cb));
				_eventsOnce[name].emplace_back(reinterpret_cast<void*>(fn));
			}
			}
			template <typename ...Args>
			void emit(const std::string& name, Args... args)
			{
				auto it = _events.find(name);
				if (it != _events.end()) {
					auto cbs = _events.at(name);
					for (auto cb : cbs) {
						auto fp = reinterpret_cast<std::function<void(Args...)>*>(cb);
						(*fp)(args...);
					}
				}

				auto once = _eventsOnce.find(name);
				if (once != _eventsOnce.end()) {
					auto cbs = _eventsOnce.at(name);
					for (auto cb : cbs) {
						auto fp = reinterpret_cast<std::function<void(Args...)>*>(cb);
						(*fp)(args...);
					}
					off(name);
				}
			}
			void off();
			void off(const std::string& name);
		protected:
			template <typename Callback>
			struct traits : public traits<decltype(&Callback::operator())> {
			};

			template <typename ClassType, typename R, typename... Args>
			struct traits<R(ClassType::*)(Args...) const> {

				using fn = std::function<R(Args...)>;
			};

			template <typename Callback>
			typename traits<Callback>::fn
			toFunction(Callback& cb) {
				return static_cast<typename traits<Callback>::fn>(cb);
			}
			std::unordered_map<std::string, std::vector<void*>> _events;
			std::unordered_map<std::string, std::vector<void*>> _eventsOnce;
		};
	}
}
