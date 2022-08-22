#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;
void main() {
    outColor = vec4(texture(texSampler, fragTexCoord).rgb * smoothstep(0.5, 0.2, length(fragTexCoord - 0.5)), 1.0);
}