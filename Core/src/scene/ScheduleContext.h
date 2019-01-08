#pragma once

#include "scene/RenderKey.h"
#include "scene/RenderQueue.h"
#include "scene/TransformationStack.h"

namespace core
{
	class Camera;
	class UBORegistry;

	using ConstCameraPtr = const Camera *;

	struct ScheduleContext
	{
		ScheduleContext() = delete;
		ScheduleContext(const UBORegistry & ubos, float pf) : ubos(ubos), partialFrame(pf) {}

		/**
			The UBO registry contains all current UBOs that are available in the system.
		*/
		const UBORegistry & ubos;

		/**
			Stores the information of where any renderer should be placed in a render queue. The
			system will update its values as neccessary.
		*/
		RenderKey key;
		/**
			The render queue holds all rendering tasks that should be performed this render frame.
		*/
		RenderQueue queue;

		/**
			The transformation stack which holds the current transformation of the scene graph. The
			transformation is from model to world space.
		*/
		TransformationStack stack;

		/**
			The camera which is currently being processed. All subsequent schedules will be for only
			that specific camera.
		*/
		ConstCameraPtr camera;

		/**
			If more frames are rendered than there are logic updates, this represents how much of
			the new versus the old logic frame should be used. By applying this factor, the effects
			of stuttering can be reduced on fast machines.
		*/
		const float partialFrame;
	};
}