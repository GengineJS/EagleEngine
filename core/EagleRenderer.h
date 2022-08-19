#pragma once
#include <graphics/Context.h>
#include <pipeline/RenderPipeline.h>
#include <utils/EventSystem.h>
#include <utils/FileUtils.h>
#include <utils/Utils.h>
namespace eg {
    using namespace graphics;
    using namespace pipeline;
    using namespace utils;
    class EagleApplication;
    class EagleRenderer: public EventSystem, std::enable_shared_from_this<EagleRenderer>
    {
        friend class EagleApplication;
    public:
        static const std::string ON_START;
        static const std::string ON_UPDATE;
        static const std::string ON_RENDER_BEFORE;
        static const std::string ON_RENDER_AFTER;
        static const std::string ON_WINDOW_RESIZE;
        EagleRenderer(const ContextInfo& info);
        virtual ~EagleRenderer();
        inline const std::shared_ptr<Context> getContext() const { return _context; };
        inline const std::shared_ptr<RenderPipeline> getRenderPipeline() const { return _pipeline; }
    protected:
        void onApplicationInit(std::shared_ptr<EagleApplication> app);
        void onApplicationExit(std::shared_ptr<EagleApplication> app);
        void onApplicationPause();
        void onApplicationFocus();
        void onStart();
        void onUpdate();
        void onRender();
        void onResize(uint32_t w, uint32_t h);
        void _active();
        std::shared_ptr<Context> _context{ nullptr };
        std::shared_ptr<RenderPipeline> _pipeline{ std::make_shared<RenderPipeline>() };
    };
}
