#version 450

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inEyeWorldPosition;
layout(location = 2) in vec2 inUvs;
layout(location = 3) in mat3 inTbn;

layout(location = 0) out vec4 outFragColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D occlusionRoughnessMetalnessMap;
uniform samplerCube reflectionMap;

void main() {
    float eta = 1.0 / 1.3;
    vec3 worldNormal = normalize(inTbn * (texture(normalMap, inUvs).xyz * 2.0 - 1.0));
    vec3 eyesToWorldPosition = normalize(inWorldPosition - inEyeWorldPosition);
    vec3 orm = texture(occlusionRoughnessMetalnessMap, inUvs).rgb;
    vec4 albedo = texture(albedoMap, inUvs);
    
    vec3 reflectionDirection = reflect(eyesToWorldPosition, worldNormal);
    vec3 reflectionColor = textureLod(reflectionMap, reflectionDirection, orm.g * 6).rgb;
    float fresnel = dot(worldNormal, -eyesToWorldPosition);

    outFragColor = vec4(mix(albedo.rgb, reflectionColor, orm.b), 1 - fresnel);//This will to reflection and transparency properly.
    //TODO: I could do screenspace reflection where I fallback to the skybox if the screen doesn't intersecet the reflected ray.
    //TODO: The gbuffer is populated so I could to screenspace refraction instead of sampling from the skybox. This would allow refraction and transparency.
}
