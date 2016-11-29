#version 400

// Definition of the geometry shader
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// Attributes passed from the vertex shader
in vec4 particle_colour[];
in float particle_id[];

// Attributes passed with the material file
uniform mat4 projection_mat;
uniform float particle_size;

// Attributes passed to the fragment shader
out vec4 frag_colour;
out vec2 tex_coord;


void main(void){

	// Get the position of the particle
	vec4 position = gl_in[0].gl_Position;

    // Define the positions of the four vertices that will form a quad 
	// The positions are based on the position of the particle and its size
    // We simply add offsets to the position (we can think of it as the center of the particle),
	// since we are already in camera space
	vec4 v[4];
	v[0] = vec4(position.x - 0.5*particle_size, position.y - 0.5*particle_size, position.z, 1.0);
    v[1] = vec4(position.x + 0.5*particle_size, position.y - 0.5*particle_size, position.z, 1.0);
    v[2] = vec4(position.x - 0.5*particle_size, position.y + 0.5*particle_size, position.z, 1.0);
    v[3] = vec4(position.x + 0.5*particle_size, position.y + 0.5*particle_size, position.z, 1.0);

	// Create the new geometry: a quad with four vertices from the vector v
    int fid = int(floor(particle_id[0] * 4.0)); // 0-3 used to pick sector from flame 2x2 drawing
    for (int i = 0; i < 4; i++){
		gl_Position = projection_mat * v[i];
		tex_coord = vec2(floor(i / 2), (i % 2)); //texture coordinate
        frag_colour = vec4(vec3(0.0), particle_colour[0].a); // Specify only blending value
        EmitVertex();
     }

	 EndPrimitive();
}