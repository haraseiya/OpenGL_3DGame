#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float Time;
varying vec2 UV;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
  vec3 color = texture2D(screenTexture, UV).rgb;

  // Random number.
  vec2 pos = UV;
  pos *= sin(Time);
  float r = rand(pos);

  // Noise color using random number.
  vec3 noise = vec3(r);
  float noise_intensity = 0.5;

  // Combined colors.
  color = mix(color, noise, noise_intensity);

  gl_FragColor = vec4(color, 1.0).rgba;
}
