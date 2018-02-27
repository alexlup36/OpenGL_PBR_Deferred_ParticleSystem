#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform float gammaHDR;
uniform float exposure;
uniform float exposureBias;

uniform int toneMapper;
const int NORMAL = 0x00;
const int REINHARD = 0x01;
const int EXPOSURE_TONE_MAP = 0x02;
const int GAMMA_CORRECTED = 0x03;
const int JIM_TONE_MAP = 0x04;
const int UNCHARTED = 0x05; 

const float A = 0.15f;
const float B = 0.5f;
const float C = 0.1f;
const float D = 0.2f;
const float E = 0.02f;
const float F = 0.3f;
const float W = 11.2f;

vec3 gammaCorrection(vec3 ldColor, float gamma)
{
	return pow(ldColor, vec3(1.0f / gamma));
}

vec3 unchartedToneMap(vec3 x)
{
	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main()
{
	// Sample the HDR buffer
	vec3 hdrFragmentColor = texture(renderedTexture, UV).rgb;

	// Do tone mapping
	switch (toneMapper)
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
		case JIM_TONE_MAP:
		{
			// Exposure adjustment
			hdrFragmentColor *= exposure;
			vec3 x = max(vec3(0.0f), hdrFragmentColor - vec3(0.004f));
			vec3 finalColor = (x * (6.2f * x + 0.5f)) / (x * (6.2f * x + 1.7f) + 0.06f);
			// Final color
			color = vec4(finalColor, 1.0f);

			break;
		}
		case UNCHARTED:
		{
			// Exposure adjustment
			hdrFragmentColor *= exposure;

			// Apply the uncharted tone mapper
			vec3 toneMapped = unchartedToneMap(hdrFragmentColor * exposureBias);

			vec3 whiteScale = 1.0f / unchartedToneMap(vec3(W));
			toneMapped *= whiteScale;

			// Gamma correction
			vec3 gammaCorrected = gammaCorrection(toneMapped, gammaHDR);

			// Final color
			color = vec4(gammaCorrected, 1.0f);

			break;
		}
		default:
			color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
}