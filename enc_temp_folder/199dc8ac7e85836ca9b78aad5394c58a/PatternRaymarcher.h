#pragma once


#include "Pattern.h"
#include "../Vec3.h"
#include "../Vec2.h"


class PatternRaymarcher : public Pattern {
public:
	PatternRaymarcher() : Pattern("Raymarcher") {}

	uint8_t drawFrame() {
		renderRays();

		offset += 0.02;
		//if (offset > TWO_PI) {
		//	offset -= TWO_PI;
		//}
		return 0;
	}

	float intersectSDF(float distA, float distB) {
		return max(distA, distB);
	}

	float unionSDF(float distA, float distB) {
		return min(distA, distB);
	}

	float differenceSDF(float distA, float distB) {
		return max(distA, -distB);
	}

	float smoothMin(float a, float b, float k = 0.4)
	{
		float h = CLAMP(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
		return interpolate(b, a, h) - k * h*(1.0 - h);
	}

	// polynomial smooth min (k = 0.1);
	float smoothMinCubic(float a, float b, float k = 0.1)
	{
		float h = max(k - abs(a - b), 0.0f) / k;
		return min(a, b) - h*h*h * k * (1.0 / 6.0);
	}

	float smoothMinExp(float a, float b, float k = 32)
	{
		float res = exp2(-k * a) + exp2(-k * b);
		return -log2(res) / k;
	}

	// p: arbitrary point in 3D space
	// c: the center of our sphere
	// r: the radius of our sphere
	float sphereSDF(Vec3 p, Vec3 c, float r)
	{
		return (p - c).Len() - r;
	}

	/**
	 * Signed distance function for a sphere centered at the origin with radius r.
	 */
	float sphereSDF(Vec3 p, float r)
	{
		return (p).Len() - r;
	}
	/**
	 * Signed distance function for a cube centered at the origin
	 * with Vec3 b = (width,height,length)
	 */
	float cubeSDF(Vec3 p, Vec3 size) {
		Vec3 d = p.Abs() - (size / 2);

		// Assuming p is inside the cube, how far is it from the surface?
		// Result will be negative or zero.
		float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0f);

		// Assuming p is outside the cube, how far is it from the surface?
		// Result will be positive or zero.
		d.x = max(d.x, 0.0f);
		d.y = max(d.y, 0.0f);
		d.z = max(d.z, 0.0f);
		float outsideDistance = d.Len();

		return insideDistance + outsideDistance;
	}

	float map_the_world(Vec3 p) {
		//float displacement = sin(5.0 * p.x + offset) * sin(5.0 * p.y + offset) * sin(5.0 * p.z + offset) * 0.25;
		const float r = 1.7f;
		float x1 = r * cos(PI / 2);
		float z1 = r * sin(PI / 2);
		float x2 = r * cos(PI / 2) + sin(offset) + GuiVars2;
		float z2 = r * sin(PI / 2) ;
		float sphere_0 = sphereSDF(p, Vec3(x1, 0.0f, z1), 1.0f);
		float sphere_1 = sphereSDF(p, Vec3(x2, 0.0f, z2), 0.5f);
		float box_1 = cubeSDF(p - Vec3(x2, 0.0f, z2), Vec3(1.0, 1.0, 1.0));



		return smoothMin(sphere_0, box_1, GuiVars1);// +displacement;
	}

	// ro=ray origin, rd=ray direction
	Vec3 ray_march(Vec3 ro, Vec3 rd) {
		float totalDistanceTraveled = 0.0f;
		const int NUMBER_OF_STEPS = 32;
		const float MINIMUM_HIT_DISTANCE = 0.001;
		const float MAXIMUM_TRACE_DISTANCE = 200.0;

		//lighting
		Vec3 K_a = Vec3(0.2, 0.2, 0.2); //ambient
		Vec3 K_d = Vec3(0.7, 0.2, 0.2); //difuse
		Vec3 K_s = Vec3(1.0, 1.0, 1.0); //specular
		const float shininess = 10.0;

		for (int i = 0; i < NUMBER_OF_STEPS; ++i)
		{
			// Calculate our current position along the ray
			Vec3 current_position = ro + rd * totalDistanceTraveled;

			// We wrote this function earlier in the tutorial -
			// assume that the sphere is centered at the origin
			// and has unit radius
			float distance_to_closest = map_the_world(current_position);

			if (distance_to_closest < MINIMUM_HIT_DISTANCE) // hit
			{
				Vec3 normal = estimateNormal(current_position);

				// use surface normal as ambient colour
				K_a = normal + Vec3(1.0, 1.0, 1.0) / 2.0f;
				K_d = K_a;

				// phong shader
				Vec3 p = ro + rd * distance_to_closest;
				Vec3 color = phongIllumination(K_a, K_d, K_s, shininess, p, ro);
				return color;
				//// For now, hard-code the light's position in our scene
				//Vec3 light_position = { 0.0f, -GuiVars1 * 2 - 1, 0.0f };
				//// Calculate the unit direction vector that points from
				//// the point of intersection to the light source
				//Vec3 direction_to_light = (-current_position + light_position).GetNormalized();
				//float diffuse_intensity = max(0.0f, normal * direction_to_light);
				//return Vec3(1.0, 0.0, 0.0) * diffuse_intensity;

				// Remember, each component of the normal will be in 
				// the range -1..1, so for the purposes of visualizing
				// it as an RGB color, let's remap it to the range
				// 0..256
				//return (normal + 1.0f) * 128;
			}

			if (totalDistanceTraveled > MAXIMUM_TRACE_DISTANCE) // miss
			{
				break;
			}

			// accumulate the distance traveled thus far
			totalDistanceTraveled += distance_to_closest;
		}

		// If we get here, we didn't hit anything so just
		// return a background color (black)
		return Vec3(0.0, 0.0, 0.0);
	}



	Vec3 estimateNormal(Vec3 p) {
		const float epsilon = 0.0001f;
		//const Vec3 small_step = Vec3(0.001f, 0.0f, 0.0f);

		float gradient_x = map_the_world({ p.x + epsilon, p.y, p.z }) - map_the_world({ p.x - epsilon, p.y, p.z });
		float gradient_y = map_the_world({ p.x, p.y + epsilon, p.z }) - map_the_world({ p.x, p.y - epsilon, p.z });
		float gradient_z = map_the_world({ p.x, p.y, p.z + epsilon }) - map_the_world({ p.x, p.y, p.z - epsilon });

		Vec3 normal = Vec3(gradient_x, gradient_y, gradient_z);

		return (normal).GetNormalized();
	}

	/**
	 * Lighting contribution of a single point light source via Phong illumination.
	 *
	 * The Vec3 returned is the RGB color of the light's contribution.
	 *
	 * k_a: Ambient color
	 * k_d: Diffuse color
	 * k_s: Specular color
	 * alpha: Shininess coefficient
	 * p: position of point being lit
	 * eye: the position of the camera
	 * lightPos: the position of the light
	 * lightIntensity: color/intensity of the light
	 *
	 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
	 */
	Vec3 phongContribForLight(Vec3 k_d, Vec3 k_s, float alpha, Vec3 p, Vec3 eye,
		Vec3 lightPos, Vec3 lightIntensity) {
		Vec3 N = estimateNormal(p);
		Vec3 L = (lightPos - p).GetNormalized();
		Vec3 V = (eye - p).GetNormalized();
		Vec3 R = ((-L) - (N * (N * (-L)) * 2.0)).GetNormalized(); //I - 2.0 * dot(N, I) * N. or reflect(-L, N)

		float dotLN = (L * N);
		float dotRV = (R * V);

		if (dotLN < 0.0) {
			// Light not visible from this point on the surface
			return Vec3(0.0, 0.0, 0.0);
		}

		if (dotRV < 0.0) {
			// Light reflection in opposite direction as viewer, apply only diffuse
			// component
			return lightIntensity.GetHadamard(k_d * dotLN);
		}
		return lightIntensity.GetHadamard((k_d * dotLN + k_s * pow(dotRV, alpha)));
	}
	/**
	 * Lighting via Phong illumination.
	 *
	 * The Vec3 returned is the RGB color of that point after lighting is applied.
	 * k_a: Ambient color
	 * k_d: Diffuse color
	 * k_s: Specular color
	 * alpha: Shininess coefficient
	 * p: position of point being lit
	 * eye: the position of the camera
	 *
	 * See https://en.wikipedia.org/wiki/Phong_reflection_model#Description
	 */
	Vec3 phongIllumination(Vec3 k_a, Vec3 k_d, Vec3 k_s, float alpha, Vec3 p, Vec3 eye) {
		const Vec3 ambientLight = Vec3(1.0, 1.0, 1.0) * 0.5f;
		Vec3 color = ambientLight.GetHadamard(k_a);

		Vec3 light1Pos = Vec3(0.0, 0.0, 0.0);
		Vec3 light1Intensity = Vec3(0.3, 0.3, 0.3);

		color += phongContribForLight(k_d, Vec3(0.4, 0.4, 0.4), alpha, p, eye,
			light1Pos,
			light1Intensity);

		Vec3 light2Pos = Vec3(1.0 * sin(offset),
			-1.5,
			1.0 * cos(offset));
		Vec3 light2Intensity = Vec3(0.4, 0.4, 0.4);

		color += phongContribForLight(k_d, k_s, alpha, p, eye,
			light2Pos,
			light2Intensity);
		return color;
	}


	void renderRays() {

		Vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
		Vec3 ro = cameraPosition;

		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {

				float uvX = i / (SCREEN_HEIGHT / 2.0f) - 1.0;
				float uvY = j / (SCREEN_HEIGHT / 2.0f) - 1.0;
				float z = 1.0f;

				//spherical mapping
				const float step = TWO_PI / SCREEN_WIDTH;
				uvX = cos(i * step);
				z = sin(i * step);

				Vec3 rd = { uvX, uvY, z };

				Vec3 c = ray_march(ro, rd.GetNormalized());
				c.Clamp();

				//gamma
				//c.x = pow(c.x, 0.45f);
				//c.y = pow(c.y, 0.45f);
				//c.z = pow(c.z, 0.45f);

				//0..1 - 0..255
				c *= 255; 

				gfx.putPixel(i, j, CRGB(c.x, c.y, c.z));
			}
		}
	}

	float offset = 0;
};


