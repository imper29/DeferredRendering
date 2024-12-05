#version 450

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inEyeWorldPosition;
layout(location = 2) in vec2 inUvs;
layout(location = 3) in mat3 inTbn;

layout(location = 0) out vec4 outWorldPosition;
layout(location = 1) out vec4 outWorldNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outEmission;
layout(location = 4) out vec4 outOcclusionRoughnessMetalness;
layout(location = 5) out vec4 outReflection;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D emissionMap;
uniform sampler2D occlusionRoughnessMetalnessMap;
uniform sampler2D displacementMap;
uniform samplerCube reflectionMap;

void main() {
    float eta = 1.0 / 1.3;
    vec3 worldNormal = normalize(inTbn * (texture(normalMap, inUvs).xyz * 2.0 - 1.0));
    vec3 eyesToWorldPosition = normalize(inWorldPosition - inEyeWorldPosition);
    vec3 orm = texture(occlusionRoughnessMetalnessMap, inUvs).rgb;

    outWorldPosition = vec4(inWorldPosition, 1.0);
    outWorldNormal = vec4(worldNormal, 1.0);
    outAlbedo = vec4(texture(albedoMap, inUvs).rgb, 1.0);
    outEmission = vec4(texture(emissionMap, inUvs).rgb, 1.0);
    outOcclusionRoughnessMetalness = vec4(orm, 1.0);
    
    vec3 reflectionDirection = reflect(eyesToWorldPosition, worldNormal);
    outReflection.rgb = textureLod(reflectionMap, reflectionDirection, orm.g * 6).rgb;
    outReflection.a = 1.0;
}
