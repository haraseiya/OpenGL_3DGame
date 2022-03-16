// ライト球体描画用シェーダー
#version 330 core
layout (location = 0) out vec4 FragBuffer;     // HDRバッファに出力
layout (location = 1) out vec4 BrightBuffer;   // 高輝度バッファ

uniform vec3  color;     // ライトの色
uniform float luminance; // 輝度

void main()
{
    vec3 result;
    result = color * luminance;

    // 輝度
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.8)
    {
        BrightBuffer = vec4(result, 0.0f);
    }
    else
    {
        BrightBuffer = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    FragBuffer  = vec4(result, 1.0);
}