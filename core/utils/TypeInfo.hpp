/*
 *  TypeInfo.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <limits>
namespace eg {
	using TypeID = uint32_t;
	class TypeInfo
	{
	public:
		TypeInfo() = default;
		~TypeInfo() = default;
		template<typename T>
		inline TypeID get() const noexcept {
			std::type_index t_idx = typeid(T);
			if (_typeMap.find(t_idx) != _typeMap.end()) {
				return _typeMap.at(t_idx);
			}
			return 0;
		};
		template<typename T>
		inline TypeID id() noexcept {
			std::type_index t_idx{ typeid(T) };
			if (_typeMap.find(t_idx) != _typeMap.end()) {
				return _typeMap.at(t_idx);
			}
			_typeMap[t_idx] = _nextId();
			return _typeMap[t_idx];
		};
		template<typename Base, typename Ext, typename = std::enable_if_t<std::is_convertible_v<Ext*, Base*>>>
		inline TypeID id() noexcept {
			return get<Ext>();
		}
	private:
		TypeID _currId{ 0 };
		std::unordered_map<std::type_index, TypeID> _typeMap{};
		inline TypeID _nextId() noexcept { _currId++; return _currId; }
	};
}
