#version 330

// very simple pixel shader that just samples a texture

in VERTEX
{
 vec2 UV;
 vec4 Color;
// vec4 ColorToAlpha;
}vertex;

out vec4 outColour;

uniform sampler2D diffuseTexture;

void main()
{
	vec4 texColor = texture2D(diffuseTexture, vertex.UV);
	//if( distance(vertex.ColorToAlpha.rgb, texColor.rgb) < 0.15 )
	//{
	//	discard;
	//}
	outColour =  texColor * vertex.Color;
}
