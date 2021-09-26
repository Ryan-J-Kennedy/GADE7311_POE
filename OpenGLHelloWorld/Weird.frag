precision mediump float;

uniform float time;
//uniform vec2 mouse;
uniform float mouseX;
uniform float mouseY;
//uniform vec2 resolution;
uniform float resolutionX;
uniform float resolutionY;

#extension GL_OES_standard_derivatives : enable

#define violett vec4(0.2, 0.5, 1.0, 1.0)

#define edge 0.06
#define waves 3.0

// clockwise rotation: pos *= [ca, sa, -sa, ca]
vec2 rotate2d (vec2 pos, float angle)   
{
  float ca = cos(angle),  sa = sin(angle);
  return pos * mat2(ca, -sa, sa, ca);  
}

float inWaveCircle(vec2 pt, vec2 center, float radius, float line)
{
  float d = distance(pt, center);

  return smoothstep(radius + line/2., radius, d)
       - smoothstep(radius, radius - line, d);
}

void main(void) 
{
  vec2 p = (2. * gl_FragCoord.xy - resolutionX * resolutionY) / resolutionX;
  p = rotate2d (p, 0.1*time);
  float a = atan(p.y, p.x);
  float f = sin((waves + floor(mouseY*15.)) * a);
  vec2 center = vec2(0.0, 0.0);
  float radius = 0.35 + 0.05 * f * mouseX * 5.0;
  gl_FragColor = violett * inWaveCircle(p, center, radius, edge);
}