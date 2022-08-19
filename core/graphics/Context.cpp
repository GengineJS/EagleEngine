#include <graphics/Context.h>
#if defined(USE_GFX_VULKAN)
#include <graphics/vulkan/VKContext.h>
#elif defined(USE_GFX_GLES3)
#elif defined(USE_GFX_GLES3)
#elif defined(USE_GFX_WEBGL)
#elif defined(USE_GFX_WEBGPU)
#elif defined(USE_GFX_METAL)
#elif defined(USE_GFX_DX12)
#endif // USE_GFX_VULKAN
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <graphics/vulkan/VKTools.h>
#include <graphics/VertexInput.h>
namespace eg {
	namespace graphics {
		CommandLineParser::CommandLineParser()
		{
			add("help", { "--help" }, 0, "Show help");
			add("validation", { "-v", "--validation" }, 0, "Enable validation layers");
			add("vsync", { "-vs", "--vsync" }, 0, "Enable V-Sync");
			add("fullscreen", { "-f", "--fullscreen" }, 0, "Start in fullscreen mode");
			add("width", { "-w", "--width" }, 1, "Set window width");
			add("height", { "-h", "--height" }, 1, "Set window height");
			add("shaders", { "-s", "--shaders" }, 1, "Select shader type to use (glsl or hlsl)");
			add("gpuselection", { "-g", "--gpu" }, 1, "Select GPU to run on");
			add("gpulist", { "-gl", "--listgpus" }, 0, "Display a list of available Vulkan devices");
			add("benchmark", { "-b", "--benchmark" }, 0, "Run example in benchmark mode");
			add("benchmarkwarmup", { "-bw", "--benchwarmup" }, 1, "Set warmup time for benchmark mode in seconds");
			add("benchmarkruntime", { "-br", "--benchruntime" }, 1, "Set duration time for benchmark mode in seconds");
			add("benchmarkresultfile", { "-bf", "--benchfilename" }, 1, "Set file name for benchmark results");
			add("benchmarkresultframes", { "-bt", "--benchframetimes" }, 0, "Save frame times to benchmark results file");
			add("benchmarkframes", { "-bfs", "--benchmarkframes" }, 1, "Only render the given number of frames");
		}

		void CommandLineParser::add(std::string name, std::vector<std::string> commands, bool hasValue, std::string help)
		{
			options[name].commands = commands;
			options[name].help = help;
			options[name].set = false;
			options[name].hasValue = hasValue;
			options[name].value = "";
		}

		void CommandLineParser::printHelp()
		{
			std::cout << "Available command line options:\n";
			for (auto option : options) {
				std::cout << " ";
				for (size_t i = 0; i < option.second.commands.size(); i++) {
					std::cout << option.second.commands[i];
					if (i < option.second.commands.size() - 1) {
						std::cout << ", ";
					}
				}
				std::cout << ": " << option.second.help << "\n";
			}
			std::cout << "Press any key to close...";
		}

		void CommandLineParser::parse(std::vector<const char*> arguments)
		{
			bool printHelp = false;
			// Known arguments
			for (auto& option : options) {
				for (auto& command : option.second.commands) {
					for (size_t i = 0; i < arguments.size(); i++) {
						if (strcmp(arguments[i], command.c_str()) == 0) {
							option.second.set = true;
							// Get value
							if (option.second.hasValue) {
								if (arguments.size() > i + 1) {
									option.second.value = arguments[i + 1];
								}
								if (option.second.value == "") {
									printHelp = true;
									break;
								}
							}
						}
					}
				}
			}
			// Print help for unknown arguments or missing argument values
			if (printHelp) {
				options["help"].set = true;
			}
		}

		bool CommandLineParser::isSet(std::string name)
		{
			return ((options.find(name) != options.end()) && options[name].set);
		}

		std::string CommandLineParser::getValueAsString(std::string name, std::string defaultValue)
		{
			assert(options.find(name) != options.end());
			std::string value = options[name].value;
			return (value != "") ? value : defaultValue;
		}

		int32_t CommandLineParser::getValueAsInt(std::string name, int32_t defaultValue)
		{
			assert(options.find(name) != options.end());
			std::string value = options[name].value;
			if (value != "") {
				char* numConvPtr;
				int32_t intVal = strtol(value.c_str(), &numConvPtr, 10);
				return (intVal > 0) ? intVal : defaultValue;
			}
			else {
				return defaultValue;
			}
			return int32_t();
		}

		std::shared_ptr<Context> Context::context{ nullptr };

		std::shared_ptr<Context> Context::CreateContext(const ContextInfo& info)
		{
#if defined(USE_GFX_VULKAN)
			context = std::make_shared<VKContext>(info);
#elif defined(USE_GFX_GLES3)
#elif defined(USE_GFX_GLES2)
#elif defined(USE_GFX_WEBGL)
#elif defined(USE_GFX_WEBGPU)
#else
			throw std::exception("A graphics backend must be selected");
#endif
			context->initialize();
			std::shared_ptr<Device> device = context->createDevice();
			context->_swapchain = std::shared_ptr<Swapchain>(device->createSwapchain());
			
			return context;
		}

		Context::Context(const ContextInfo& info) : _info(info), _isValidation(VALIDATION)
		{
#if defined(USE_GFX_VULKAN)
			_graphics = GraphicsAPI::VULKAN;
#elif defined(USE_GFX_GLES3)
			_graphics = GraphicsAPI::GLES3;
#elif defined(USE_GFX_GLES2)
			_graphics = GraphicsAPI::GLES2;
#elif defined(USE_GFX_WEBGL)
			_graphics = GraphicsAPI::WEBGL;
#elif defined(USE_GFX_WEBGPU)
			_graphics = GraphicsAPI::WEBGPU;
#endif
		}

		void Context::_createGpuSwapchain(const AppHandler& handler)
		{
			_swapchain->create(handler);
		}

		void Context::_createCommandBuffers(uint32_t size)
		{
			for (uint32_t i = 0; i < size; i++) {
				CommandBufferInfo cmdInfo{};
				cmdInfo.queueIdx = _device->getQueueFamilyIndices().graphics;
				_commandBuffers.emplace_back(_device->createCommandBuffer(cmdInfo));
			}
		}

		std::shared_ptr<Context> Context::GetContext()
		{
			return context;
		}

		void Context::onApplicationInit(const AppHandler& handler)
		{
			_handler = handler;
			if(_isInit) {
				_swapchain->destroy();
				_commandBuffers.clear();
			}
			_isInit = true;
			_createGpuSwapchain(handler);
			_createCommandBuffers(static_cast<uint32_t>(_swapchain->getTextures().size()));
		}

		void Context::onResize(uint32_t w, uint32_t h)
		{
			_info.width = w;
			_info.height = h;
			_swapchain->destroy();
			_commandBuffers.clear();
			_createGpuSwapchain(_handler);
			_createCommandBuffers(static_cast<uint32_t>(_swapchain->getTextures().size()));
		}

		Context::~Context()
		{
		}
	}
}
