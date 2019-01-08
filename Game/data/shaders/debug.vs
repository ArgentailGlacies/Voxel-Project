#include "data/shaders/common/matrices.vs"

layout(location = 0) in vec3 inPosition;

void main()
{
  gl_Position = matrices.projection * matrices.view * matrices.model * vec4(inPosition, 1.0);
}