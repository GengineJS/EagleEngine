#pragma once
#include <chrono>
#include <functional>
namespace eg {
	using namespace std::chrono;
	class Time
	{
	public:
		constexpr Time() = default;
		template<typename Rep, typename Period>
		constexpr Time(const duration<Rep, Period> &dur): _duration(duration_cast<microseconds>(dur)) {}
		~Time() = default;
		static Time Tick() {
			return steady_clock::now().time_since_epoch();
		}
		constexpr Time operator -(const Time& rt) {
			return duration_cast<microseconds>(_duration - rt._duration);
		}
		constexpr auto durAsSecounds() {
			return duration_cast<seconds>(_duration).count();
		}
		constexpr auto durAsMilliSeconds() {
			return static_cast<float>(_duration.count())/1000.f;
		}
		constexpr auto durAsMicroseconds() {
			return duration_cast<microseconds>(_duration).count();
		}
	private:
		microseconds _duration{};
	};
	class TimeElapse {
	public:
		TimeElapse() = default;
		explicit TimeElapse(const Time& start):_startTime(start) {}
		explicit TimeElapse(const Time& start, float elapse) :_startTime(start), _elapse(elapse) {}
		inline void setElapse(float elapse) { _elapse = elapse; }
		inline bool update(const std::function<void(float)> &callback) {
			_currTime = Time::Tick();
			const auto currElapse = (_currTime - _startTime).durAsMilliSeconds();
			if (currElapse > _elapse) {
				callback(currElapse);
				// It doesn't matter whether you put the callback before or after
				_startTime = _currTime;
				return true;
			}
			return false;
		}
	private:
		Time _startTime;
		Time _currTime;
		// Generally, it is the time interval for running one frame, in ms
		float _elapse{ 0.0f };
	};
}
