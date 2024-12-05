#version 450

layout(location = 0) in vec2 inUvs;
layout(location = 1) in vec3 inEyeWorldPosition;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D worldPositionMap;
layout(binding = 2) uniform sampler2D worldNormalMap;
layout(binding = 3) uniform sampler2D albedoMap;
layout(binding = 5) uniform sampler2D occlusionRoughnessMetalnessMap;
layout(binding = 6) uniform sampler2D reflectionMap;

uniform vec3 lightColor;
uniform vec3 lightDirection;

void main() {
    vec4 worldPosition = texture(worldPositionMap, inUvs);
    vec3 worldNormal = texture(worldNormalMap, inUvs).xyz;
    vec3 albedo = texture(albedoMap, inUvs).xyz;
    vec3 occlusionRoughnessMetalness = texture(occlusionRoughnessMetalnessMap, inUvs).xyz;
    vec3 reflection = texture(reflectionMap, inUvs).xyz;
    float occlusion = occlusionRoughnessMetalness.r;
    float roughness = occlusionRoughnessMetalness.g;
    float metalness = occlusionRoughnessMetalness.b;
    float glossiness = 1.0 - roughness;

    vec3 viewDir = normalize(inEyeWorldPosition - worldPosition.xyz);
    vec3 lightRef = reflect(lightDirection, worldNormal);
    float exposure = max(dot(lightDirection, worldNormal), 0.0);

    float diffuseIntensity = exposure;
    vec3 diffuse = lightColor * mix(albedo, reflection, metalness) * diffuseIntensity;

    float specularIntensity = pow(max(-dot(viewDir, lightRef), 0.0), 1 + glossiness * 16.0) * glossiness;
    vec3 specular = lightColor * specularIntensity;

    fragColor.rgb = (diffuse + specular) * occlusion;
    fragColor.a = 1.0;
}
