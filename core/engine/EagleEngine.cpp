#include "EagleEngine.h"
#include <modules/EagleWindow.hpp>
#include <assert.h>
#include <vector>
#include <math.h>
/*
 *  EagleApplication.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
namespace eg {
	EagleEngine* EagleEngine::_engine{ nullptr };
	EagleEngine::EagleEngine(const EngineInfo& info):
		_info(info)
	{
		_engine = this;
		auto &modules = EagleModule::GetModuleCreates();
		for (auto& currModule: modules) {
			_createModule(currModule.first);
		}
		auto tick = Time::Tick();
		const auto frameTime = info.fps > 0 ? 1000.0f / static_cast<float>(info.fps) : 0;
		const auto fixedUpdate = info.fixedUpdate <= 0.0f ? 0.0f : info.fixedUpdate;
		_updateTime = TimeElapse(tick, fixedUpdate);
		_renderTime = TimeElapse(tick, frameTime);
	}
	void EagleEngine::_createModule(TypeID currId)
	{
		assert(ModuleFilter::Check(currId));
		auto& modules = EagleModule::GetModuleCreates();
		auto& moduleVal = modules.at(currId);
		auto &reqs = moduleVal.requires;
		for (auto &reqId: reqs) {
			assert(ModuleFilter::Check(reqId) && reqId != 0);
			if (_modules.find(reqId) == _modules.end()) {
				_createModule(reqId);
			}
		}
		_modules.emplace(currId, moduleVal.createFunc());
	}
	EagleEngine* EagleEngine::Get()
	{
		return _engine;
	}
	void EagleEngine::execute()
	{
		_executeModule(ModuleStage::START);
		while (!_isPause) {
			if (!getModule<EagleWindow>()->peekMessage()) {
				_isPause = true;
			}
			_executeModule(ModuleStage::ALWAYS);
			_updateTime.update([&](float elapse) {
				_executeModule(ModuleStage::PRE_UPDATE);
				_executeModule(ModuleStage::UPDATE);
				_executeModule(ModuleStage::POST_UPDATE);
			});
			_renderTime.update([&](float elapse) {
				_fps =  static_cast<uint32_t>(roundf(1000.f / elapse));
				_deltaTime = elapse / 1000.f;
				_executeModule(ModuleStage::PRE_RENDER);
				_executeModule(ModuleStage::RENDER);
				_executeModule(ModuleStage::POST_RENDER);
			});
		}
	}
	
	void EagleEngine::_executeModule(ModuleStage stageVal)
	{
		_beforeExecute(stageVal);
		for (auto& currStage: EagleModule::GetModuleCreates()) {
			auto val = currStage.second.stage & stageVal;
			auto b = static_cast<uint32_t>(val);
			if (!!b) {
				_modules.at(currStage.first)->execute(stageVal);
			}
		}
		_afterExecute(stageVal);
	}
}
