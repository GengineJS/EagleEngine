#version 450
#extension GL_ARB_separate_shader_objects : enable

// 该binding指令与location属性指令相似。我们将在描述符布局中引用此绑定
layout(binding = 0) uniform UniformBufferObject {
    mat4 proj;
    mat4 model;
    mat4 view;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
	//output the position of each vertex
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0f);
    fragColor = inNormal;
    fragTexCoord = inTexCoord;
}