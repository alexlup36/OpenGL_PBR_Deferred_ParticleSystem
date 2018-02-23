#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform float gammaHDR;
uniform float exposure;

uniform int displayMode;
const int NORMAL = 0x00;
const int REINHARD = 0x01;
const int EXPOSURE_TONE_MAP = 0x02;
const int GAMMA_CORRECTED = 0x03;

vec3 gammaCorrection(vec3 ldColor, float gamma)
{
	return pow(ldColor, vec3(1.0f / gamma));
}

void main()
{
	// Sample the HDR buffer
	vec3 hdrFragmentColor = texture(renderedTexture, UV).rgb;

	// Do tone mapping
	switch (displayMode)
	{
		case NORMAL:
		{
			color = vec4(hdrFragmentColor, 1.0f);
			break;
		}
		case GAMMA_CORRECTED:
		{
			// Gamma correction
			vec3 gammaCorrected = gammaCorrection(hdrFragmentColor, gammaHDR);
			// Final color
			color = vec4(gammaCorrected, 1.0f);

			break;
		}
		case REINHARD:
		{
			// Reinhard tone mapping
			vec3 mapped = hdrFragmentColor / (hdrFragmentColor + vec3(1.0f));
			// Gamma correction
			vec3 gammaCorrected = gammaCorrection(mapped, gammaHDR);
			// Final color
			color = vec4(gammaCorrected, 1.0f);

			break;
		}
		case EXPOSURE_TONE_MAP:
		{
			// Exposure tone mapping
			vec3 mapped = vec3(1.0f) - exp(-hdrFragmentColor * exposure);
			// Gamma correction
			vec3 gammaCorrected = gammaCorrection(mapped, gammaHDR);
			// Final color
			color = vec4(gammaCorrected, 1.0f);

			break;
		}
		default:
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
}