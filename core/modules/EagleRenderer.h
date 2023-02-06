/*
 *  EagleRenderer.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/Context.h>
#include <pipeline/RenderPipeline.h>
#include <utils/FileUtils.h>
#include <utils/Utils.h>
#include <modules/EagleWindow.hpp>

namespace eg {
    using namespace graphics;
    using namespace pipeline;
    using namespace utils;
    class EagleApplication;
    class EagleRenderer: public EagleModule::Register<EagleRenderer>
    {
        inline static const bool IsRegister = Registar<EagleWindow>(ModuleStage::START | ModuleStage::RENDER);
    public:
        /*static const std::string ON_START;
        static const std::string ON_UPDATE;
        static const std::string ON_RENDER_BEFORE;
        static const std::string ON_RENDER_AFTER;
        static const std::string ON_WINDOW_RESIZE;*/
        EagleRenderer() = default;
        // EagleRenderer(const ContextInfo& info);
        ~EagleRenderer() = default;
        inline const auto& getContext() const { return _context; };
        inline const auto& getRenderPipeline() const { return _pipeline; }
        void execute(ModuleStage) override;
    protected:
        /*void onApplicationInit(const EagleApplication* app);
        void onApplicationExit(const EagleApplication* app);*/
        /*void onApplicationPause();
        void onApplicationFocus();*/
        // void onUpdate();
        void _render();
        void _resize(uint32_t w, uint32_t h);
        void _active();
        std::unique_ptr<Context> _context{ nullptr };
        std::unique_ptr<RenderPipeline> _pipeline{ std::make_unique<RenderPipeline>() };
    };
}
