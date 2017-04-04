#ifndef OBJLOADER_H
#define OBJLOADER_H

//these codes are copied & modified from http://www.opengl-tutorial.org/download/

struct vec2
{
	float x;
	float y;
};

struct vec3
{
	float x;
	float y;
	float z;
};

bool loadOBJ(
	const char * path, 
	std::vector<vec3> & out_vertices, 
	std::vector<vec2> & out_uvs, 
	std::vector<vec3> & out_normals
);

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<vec3> & vertices,
	std::vector<vec2> & uvs,
	std::vector<vec3> & normals
);

#endif