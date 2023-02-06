/*
 *  EagleModule.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once
#include <utils/TypeInfo.hpp>
#include <functional>
#include <utils/NonCopyable.h>
#include <utils/Utils.h>
#include <bitset>
namespace eg {
    
    ENUM_FLAGS(ModuleStage) {
        START = 1 << 1,
        ALWAYS = 1 << 2,
        PRE_UPDATE= 1 << 3,
        UPDATE= 1 << 4,
        POST_UPDATE= 1 << 5,
        PRE_RENDER= 1 << 6,
        RENDER= 1 << 7,
        POST_RENDER = 1 << 8
    };

    class EagleModule;
    class ModuleFilter {
    public:
        ModuleFilter() = delete;
        ~ModuleFilter() = delete;
        template<typename Module>
        static bool Check() {
            return Check(EagleModule::getTypeInfo().id<Module>());
        }
        static bool Check(TypeID id) {
            return includes.test(id);
        }
        template<typename Module>
        static void Include() {
            Include(EagleModule::getTypeInfo().id<Module>());
        }
        static void Include(TypeID id) {
            includes.set(id);
        }
        template<typename Module>
        static void Exclude() {
            Exclude(EagleModule::getTypeInfo().id<Module>());
        }
        static void Exclude(TypeID id) {
            includes.reset(id);
        }
    protected:
        inline static std::bitset<128> includes{};
    };

    template<typename Base>
    class ModuleCreator : public NonCopyable {
    public:
        ModuleCreator() = default;
        ~ModuleCreator() = default;
        virtual void execute(ModuleStage curr) = 0;
        struct ModuleVal {
            ModuleStage stage{ ModuleStage::ALWAYS };
            std::vector<TypeID> requires{};
            std::function<std::unique_ptr<Base>()> createFunc;
        };
        template<typename Module>
        class Register: public Base {
        public:
            template<typename ...Reqs>
            static bool Registar(ModuleStage stage) {
                auto typeId = type.id<Module>();
                ModuleFilter::Include(typeId);
                std::vector<TypeID> ids{};
                (ids.emplace_back(type.id<Base, Reqs>()), ...);
                moduleCreates[typeId] = { stage, ids, []() -> std::unique_ptr<Module> {
                    _instance = new Module();
                    return std::unique_ptr<Module>(_instance);
                }
                };
                return true;
            };
            static Module* Get() { return _instance; };
        protected:
            inline static Module* _instance{ nullptr };
        };
        
        static const auto& GetModuleCreates() { return moduleCreates; };
        static const TypeInfo& getTypeInfo() {
            return type;
        }
        inline static const bool IsRegister{ false };
    protected:
        // Module type and values ​​needed to create a module
        inline static std::unordered_map<TypeID, ModuleVal> moduleCreates{};
        inline static TypeInfo type{};
    };

    class EagleModule : public ModuleCreator<EagleModule> {
        friend class ModuleFilter;
    public:
        EagleModule() = default;
        ~EagleModule() = default;
    };
}
