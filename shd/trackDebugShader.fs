#version 120

varying vec2 texCoord0;
varying vec3 normal0;

// uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform float colorBool;
const float PI = 3.1415926535;

void main()
{
    gl_FragColor = vec4((1-colorBool),colorBool,0,0) *
//    gl_FragColor = vec4(1,0,0,0) *
    	clamp(dot(lightDirection, normal0), 0.0, 1.0);
}

//void main()
//{
//  float aperture = 178.0;
//  float apertureHalf = 0.5 * aperture * (PI / 180.0);
//  float maxFactor = sin(apertureHalf);
//
//  vec2 uv;
//  vec2 xy = 2.0 * texCoord0.xy - 1.0;
//  float d = length(xy);
//  if (d < (2.0-maxFactor))
//  {
//    d = length(xy * maxFactor);
//    float z = sqrt(1.0 - d * d);
//    float r = atan(d, z) / PI;
//    float phi = atan(xy.y, xy.x);
//
//    uv.x = r * cos(phi) + 0.5;
//    uv.y = r * sin(phi) + 0.5;
//  }
//  else
//  {
//    uv = texCoord0.xy;
//  }
//  vec4 c = texture2D(sampler, uv);
//  gl_FragColor = c;
//}
