#version 450

layout(location = 0) in vec2 inUvs;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D worldPositionMap;
layout(binding = 2) uniform sampler2D worldNormalMap;
layout(binding = 3) uniform sampler2D albedoMap;
layout(binding = 4) uniform sampler2D emissionMap;
layout(binding = 5) uniform sampler2D occlusionRoughnessMetalnessMap;
layout(binding = 6) uniform sampler2D reflectionMap;

uniform vec3 lightColor;

void main() {
    vec4 worldPosition = texture(worldPositionMap, inUvs);
    vec3 worldNormal = texture(worldNormalMap, inUvs).xyz;
    vec3 albedo = texture(albedoMap, inUvs).rgb;
    vec3 occlusionRoughnessMetalness = texture(occlusionRoughnessMetalnessMap, inUvs).xyz;
    vec3 reflection = texture(reflectionMap, inUvs).xyz;
    float occlusion = occlusionRoughnessMetalness.r;
    float roughness = occlusionRoughnessMetalness.g;
    float metalness = occlusionRoughnessMetalness.b;
    float glossiness = 1.0 - roughness;
    vec3 emission = texture(emissionMap, inUvs).rgb;
    
    vec3 diffuse = lightColor * mix(albedo, reflection, metalness);

    fragColor.rgb = emission + diffuse * occlusion;
    fragColor.a = 1.0;
}
