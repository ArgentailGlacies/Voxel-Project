#pragma once

#include <glm/mat4x4.hpp>
#include <stack>

namespace core
{
	/**
		The transformation stack is used by the user during rendering to specify the exact
		transformation a renderer should operate with. Whenever a transformation changes, the data
		is automatically updated in all shaders.

		The transformation stack will only update the model transformation. The camera projection
		and view matrix must be handled seperately.
	*/
	class TransformationStack
	{
	public:
		TransformationStack();

		/**
			Pushes a translation transformation onto the model transformation stack. The final
			transformation is the cumulative multiplication of all transformations from the bottom
			up to the top.

			@param translation The translation to apply to the top of the transformation stack.
		*/
		void pushTranslation(const glm::vec3 & translation);
		/**
			Pushes a rotation transformation onto the model transformation stack. The final
			transformation is the cumulative multiplication of all transformations from the bottom
			up to the top. All rotations must be specified as radians.

			@param rotation The rotation (yaw, pitch, roll) to apply to the top of the stack.
		*/
		void pushRotation(const glm::vec3 & rotation);
		/**
			Pushes a scaling transformation onto the model transformation stack. The final
			transformation is the cumulative multiplication of all transformations from the bottom
			up to the top.

			@param scale The scale to apply to the top of the transformation stack.
		*/
		void pushScale(const glm::vec3 & scale);
		/**
			Pushes another model transformation onto the model transformation stack. The final
			transformation is the cumulative multiplication of all transformations from the bottom
			up to the top.

			@param transformation The transformation to multiply with the current transformation.
		*/
		void push(const glm::mat4 & transformation);
		/**
			Restores the previous transformation in the stack. The last transformation will always
			be the identity transformation, and cannot be popped.
		*/
		void pop();

		/**
			Retrieves the current transformation which will be applied to all subsequent rendering.

			@return The cumulative transformation of the entire stack.
		*/
		inline auto top() const { return m_transformations.top(); }

	private:
		std::stack<glm::mat4> m_transformations;
	};
}