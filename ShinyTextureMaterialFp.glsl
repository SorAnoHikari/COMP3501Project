#version 400

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 colour_interp;
in vec2 uv_interp;
in vec3 light_pos;
in vec3 light_pos2;

// Attributes passed with the material file
uniform vec4 ambient_colour;
uniform vec4 diffuse_colour;
uniform vec4 specular_colour;
uniform float ambient_amount;
uniform float phong_exponent;
uniform sampler2D diffuse_map;


void main() 
{
    // Phong Shading

    vec3 N, // Interpolated normal for fragment
	     L, // Light-source direction
		 L2, // Second light
		 R, // reflection
		 R2, // second reflection
		 V; // View vector

	// Compute Lambertian lighting
    N = normalize(normal_interp);

	L = (light_pos - position_interp);
	L = normalize(L);

	L2 = (light_pos2 - position_interp);
	L2 = normalize(L);

	float lambertian_amount = max(dot(N, L), 0.0);
	float lambertian_amount2 = max(dot(N, L2), 0.0);
	
	// Compute specular term for Phong shading
	V = - position_interp; // Eye position is (0, 0, 0)
    V = normalize(V);

	R = 2 * dot(L, N) * N - L;
	R2 = 2 * dot(L2, N) * N - L2;

    float spec_angle_cos = max(dot(V, R), 0.0);
	float specular_amount = pow(spec_angle_cos, phong_exponent);

	float spec_angle_cos2 = max(dot(V, R2), 0.0);
	float specular_amount2 = pow(spec_angle_cos2, phong_exponent);
	    
	// Retrieve texture value
	vec4 pixel = texture(diffuse_map, uv_interp);

	// Use texture in determining fragment colour
	//gl_FragColor = pixel;
	//gl_FragColor = (ambient_amount + lambertian_amount)*pixel + specular_amount*specular_colour;
	//gl_FragColor = (ambient_amount + lambertian_amount + specular_amount)*pixel;
	gl_FragColor = (ambient_amount + lambertian_amount + lambertian_amount2)*pixel + specular_amount*specular_colour + specular_amount2*specular_colour;
}