/*
 *  Context.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <string>
#include <graphics/GraphicsDef.h>
#include <graphics/Device.h>
#include <unordered_map>
namespace eg {
    namespace graphics {
        struct ContextInfo
        {
            std::string applicationName;
            float applicationVersion;
            uint32_t width{ 1280 };
            uint32_t height{ 720 };
            uint32_t fps{ 60 };
            bool fullscreen{ false };
            bool vsync{ false };
            bool guiOverlay{ true };
            bool useSwapchain{ true };
            QueueFlag queue = QueueFlag::GRAPHICS | QueueFlag::COMPUTE;
            std::vector<std::string> enabledExtensions;
        };

        class CommandLineParser
        {
        public:
            struct CommandLineOption {
                std::vector<std::string> commands;
                std::string value;
                bool hasValue = false;
                std::string help;
                bool set = false;
            };
            std::unordered_map<std::string, CommandLineOption> options;
            CommandLineParser();
            void add(std::string name, std::vector<std::string> commands, bool hasValue, std::string help);
            void printHelp();
            void parse(std::vector<const char*> arguments);
            bool isSet(std::string name);
            std::string getValueAsString(std::string name, std::string defaultValue);
            int32_t getValueAsInt(std::string name, int32_t defaultValue);
        };

        class Context
        {
        public:
            Context(Context const&) = delete;
            Context(Context&&) = delete;
            Context& operator=(Context const&) = delete;
            Context& operator=(Context&&) = delete;
            virtual ~Context();

            static std::shared_ptr<Context> CreateContext(const ContextInfo& info);
            static std::shared_ptr<Context> GetContext();
            void onApplicationInit(const AppHandler& handler);
            virtual void onResize(uint32_t w, uint32_t h);
            inline virtual void initialize() = 0;
            inline virtual std::shared_ptr<Device> createDevice() = 0;

            inline const std::shared_ptr<Device> getDevice() const { return _device; }
            inline const std::shared_ptr<Swapchain> getSwapchain() const { return _swapchain; }
            inline const std::vector<std::shared_ptr<CommandBuffer>>& getCommandBuffers() const { return _commandBuffers; }
            inline GraphicsAPI getGraphics() const { return _graphics; }
            inline ContextInfo& getContextInfo() { return _info; }
            inline uint32_t getApiVersion() const { return apiVersion; }
            inline const std::vector<std::string>& getEnableExtensions() const { return _enableExtensions; }
            inline const std::vector<std::string>& getSupportExtensions() const { return _supportExtensions; }
            inline bool getValidation() { return _isValidation; }
            inline const AppHandler& getAppHandler() const { return _handler; }
        protected:
            Context() = default;
            Context(const ContextInfo& info);
            static std::shared_ptr<Context> context;
            GraphicsAPI _graphics{ GraphicsAPI::VULKAN };
            ContextInfo _info{};
            bool _isInit{false};
            AppHandler _handler{};
            std::shared_ptr<Device> _device{ nullptr };
            std::shared_ptr<Swapchain> _swapchain{ nullptr };
            std::vector<std::shared_ptr<CommandBuffer>> _commandBuffers{};
            uint32_t apiVersion{ 0 };
            // context extensions
            std::vector<std::string> _enableExtensions;
            std::vector<std::string> _supportExtensions;
            bool _isValidation{ false };
            void _createGpuSwapchain(const AppHandler& handler);
            void _createCommandBuffers(uint32_t size);
        };
    }
}
