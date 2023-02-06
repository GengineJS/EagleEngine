/*
 *  EagleApplication.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once
#include <iostream>
#include <modules/EagleModule.hpp>
#include <utils/Time.hpp>
#include <assert.h>
#include <utils/Delegate.hpp>
namespace eg {
    struct EngineInfo
    {
        std::string applicationName;
        float applicationVersion;
        uint32_t width{ 1280 };
        uint32_t height{ 720 };
        uint32_t fps{ 60 };
        float fixedUpdate{ 15.78f };
        bool fullscreen{ false };
        bool guiOverlay{ false };
        bool useSwapchain{ true };
        bool vsync{ false };
        std::vector<std::string> enabledExtensions;
    };

    class EagleEngine: public NonCopyable {
    public:
        EagleEngine() = delete;
        ~EagleEngine() = default;
        EagleEngine(const EngineInfo&);

        template<typename ModuleType, typename = std::enable_if_t<std::is_convertible_v<ModuleType*, EagleModule*>>>
        inline ModuleType* getModule() {
            auto typeId = EagleModule::getTypeInfo().get<ModuleType>();
            assert(typeId && "No related module registration information");
            return reinterpret_cast<ModuleType*>(_modules.at(typeId).get());
        }
        static EagleEngine* Get();
        inline auto& getEngineInfo() { return _info; }
        void execute();
        inline bool getPause() { return _isPause; }
        inline float getDeltaTime() { return _deltaTime; }
        inline uint32_t getFPS() { return _fps; }
        inline auto& onBeforeExecute() { return _beforeExecute; }
        inline auto& onAfterExecute() { return _afterExecute; }
    private:
        std::unordered_map<TypeID, std::unique_ptr<EagleModule>> _modules{};
        static EagleEngine* _engine;
        EngineInfo _info{};
        bool _isPause{ false };
        uint32_t _fps{ 0 };
        // In seconds
        float _deltaTime{ 0.f };
        TimeElapse _updateTime, _renderTime;
        Delegate<void(ModuleStage)> _beforeExecute{};
        Delegate<void(ModuleStage)> _afterExecute{};
        void _createModule(TypeID currId);
        void _executeModule(ModuleStage stage);
    };
}
