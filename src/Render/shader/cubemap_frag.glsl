#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform sampler2D envmap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), acos(v.y));
    uv *= invAtan;
    uv.x += 0.5;
    return uv;
}

void main()
{       
    vec2 uv = SampleSphericalMap(normalize(localPos));
	uv=vec2(uv.x,uv.y);
    vec3 color = texture(envmap, uv).rgb;

    FragColor = vec4(color, 1.0);
}

