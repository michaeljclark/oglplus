/**
 *  @example oglplus/026_torus_halo.cpp
 *  @brief Shows how to render a halo around a torus
 *
 *  @image html 026_torus_halo.png
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class HaloExample : public Example
{
private:
	// the torus vertex attribute builder
	shapes::Torus make_torus;
	// here will be stored the indices used by the drawing instructions
	shapes::Torus::IndexArray torus_indices;
	// the instructions for drawing the torus
	shapes::DrawingInstructions torus_instr;

	// wrapper around the current OpenGL context
	Context gl;

	// Shaders and programs for rendering of the object
	VertexShader vs_torus, vs_plane;
	FragmentShader fs_torus, fs_plane;
	Program torus_prog, plane_prog;

	// Shaders and program for rendering of the halo effect
	VertexShader vs_halo;
	GeometryShader gs_halo;
	FragmentShader fs_halo;
	Program halo_prog;

	// A vertex array object for the torus
	VertexArray torus;
	// VBOs for the torus' vertices and normals
	Buffer torus_verts, torus_normals;

	// A vertex array object for the plane
	VertexArray plane;
	// VBOs for the plane's vertices and normals
	Buffer plane_verts, plane_normals;
public:
	HaloExample(void)
	 : make_torus(1.0, 0.7, 72, 48)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , vs_torus("Torus VS")
	 , vs_plane("Plane VS")
	 , fs_torus("Torus FS")
	 , fs_plane("Plane FS")
	 , vs_halo("Halo VS")
	 , gs_halo("Halo GS")
	 , fs_halo("Halo FS")
	{
		vs_torus.Source(
			"#version 330\n"
			"in vec4 Position;"
			"in vec3 Normal;"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec3 LightPos;"
			"out vec3 vertNormal;"
			"out vec3 vertViewNormal;"
			"out vec3 vertLight;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"	vertNormal = ("
			"		ModelMatrix *"
			"		vec4(Normal, 0.0)"
			"	).xyz;"
			"	vertViewNormal = ("
			"		CameraMatrix *"
			"		vec4(vertNormal, 0.0)"
			"	).xyz;"
			"	vertLight = ("
			"		vec4(LightPos, 0.0)-"
			"		ModelMatrix * Position"
			"	).xyz;"
			"}"
		);
		vs_torus.Compile();

		fs_torus.Source(
			"#version 330\n"
			"in vec3 vertNormal;"
			"in vec3 vertViewNormal;"
			"in vec3 vertLight;"
			"uniform mat4 CameraMatrix;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float ltlen = sqrt(length(vertLight));"
			"	float ltexp = dot("
			"		normalize(vertNormal),"
			"		normalize(vertLight)"
			"	);"
			"	float lview = dot("
			"		normalize(vertLight),"
			"		normalize(vec3("
			"			CameraMatrix[0][2],"
			"			CameraMatrix[1][2],"
			"			CameraMatrix[2][2] "
			"		))"
			"	);"
			"	float depth = normalize(vertViewNormal).z;"
			"	vec3 ftrefl = vec3(0.9, 0.8, 0.7);"
			"	vec3 scatter = vec3(0.9, 0.6, 0.1);"
			"	vec3 bklt = vec3(0.8, 0.6, 0.4);"
			"	vec3 ambient = vec3(0.5, 0.4, 0.3);"
			"	fragColor = vec4("
			"		pow(max(ltexp, 0.0), 8.0)*ftrefl+"
			"		( ltexp+1.0)/ltlen*pow(depth,2.0)*scatter+"
			"		(-ltexp+1.0)/ltlen*(1.0-depth)*scatter+"
			"		(-lview+1.0)*0.6*(1.0-abs(depth))*bklt+"
			"		0.2*ambient,"
			"		1.0"
			"	);"
			"}"
		);
		fs_torus.Compile();

		torus_prog.AttachShader(vs_torus);
		torus_prog.AttachShader(fs_torus);
		torus_prog.Link();

		vs_plane.Source(
			"#version 330\n"
			"in vec4 Position;"
			"in vec3 Normal;"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform vec3 LightPos;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"	vertNormal = Normal;"
			"	vertLight = LightPos-Position.xyz;"
			"}"
		);
		vs_plane.Compile();

		fs_plane.Source(
			"#version 330\n"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = sqrt(length(vertLight));"
			"	float e = dot("
			"		vertNormal,"
			"		normalize(vertLight)"
			"	);"
			"	float d = l > 0.0 ? e / l : 0.0;"
			"	float i = 0.2 + 2.5 * d;"
			"	fragColor = vec4(0.8*i, 0.7*i, 0.4*i, 1.0);"
			"}"
		);
		fs_plane.Compile();

		plane_prog.AttachShader(vs_plane);
		plane_prog.AttachShader(fs_plane);
		plane_prog.Link();

		vs_halo.Source(
			"#version 330\n"
			"in vec4 Position;"
			"in vec3 Normal;"
			"uniform mat4 ModelMatrix, CameraMatrix;"
			"out vec3 vertNormal;"
			"out float vd;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"	vertNormal = ("
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		vec4(Normal, 0.0)"
			"	).xyz;"
			"	vd = vertNormal.z;"
			"}"
		);
		vs_halo.Compile();

		gs_halo.Source(
			"#version 330\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 12) out;"

			"in vec3 vertNormal[];"
			"in float vd[];"

			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 LightPos;"

			"out float geomAlpha;"

			"void main(void)"
			"{"
			"	for(int v=0; v!=3; ++v)"
			"	{"
			"		int a = v, b = (v+1)%3, c = (v+2)%3;"
			"		vec4 pa = gl_in[a].gl_Position;"
			"		vec4 pb = gl_in[b].gl_Position;"
			"		vec4 pc = gl_in[c].gl_Position;"
			"		vec4 px, py;"
			"		vec3 na = vertNormal[a];"
			"		vec3 nb = vertNormal[b];"
			"		vec3 nc = vertNormal[c];"
			"		vec3 nx, ny;"
			"		if(vd[a] == 0.0 && vd[b] == 0.0)"
			"		{"
			"			px = pa;"
			"			nx = na;"
			"			py = pb;"
			"			ny = nb;"
			"		}"
			"		else if(vd[a] > 0.0 && vd[b] < 0.0)"
			"		{"
			"			float x = vd[a]/(vd[a]-vd[b]);"
			"			float y;"
			"			px = mix(pa, pb, x);"
			"			nx = mix(na, nb, x);"
			"			if(vd[c] < 0.0)"
			"			{"
			"				y = vd[a]/(vd[a]-vd[c]);"
			"				py = mix(pa, pc, y);"
			"				ny = mix(na, nc, y);"
			"			}"
			"			else"
			"			{"
			"				y = vd[c]/(vd[c]-vd[b]);"
			"				py = mix(pc, pb, y);"
			"				ny = mix(nc, nb, y);"
			"			}"
			"		}"
			"		else continue;"
			"		vec4 gx1 = vec4(px.xyz, 1.0);"
			"		vec4 gy1 = vec4(py.xyz, 1.0);"
			"		vec4 gx2 = vec4(px.xyz + nx*0.3, 1.0);"
			"		vec4 gy2 = vec4(py.xyz + ny*0.3, 1.0);"
			"		gl_Position = ProjectionMatrix * gy1;"
			"		geomAlpha = 1.0;"
			"		EmitVertex();"
			"		gl_Position = ProjectionMatrix * gx1;"
			"		geomAlpha = 1.0;"
			"		EmitVertex();"
			"		gl_Position = ProjectionMatrix * gy2;"
			"		geomAlpha = 0.0;"
			"		EmitVertex();"
			"		gl_Position = ProjectionMatrix * gx2;"
			"		geomAlpha = 0.0;"
			"		EmitVertex();"
			"		EndPrimitive();"
			"		break;"
			"	}"
			"}"
		);
		gs_halo.Compile();

		fs_halo.Source(
			"#version 330\n"
			"in float geomAlpha;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4("
			"		0.5, 0.4, 0.3,"
			"		pow(geomAlpha, 2.0)"
			"	);"
			"}"
		);
		fs_halo.Compile();

		halo_prog.AttachShader(vs_halo);
		halo_prog.AttachShader(gs_halo);
		halo_prog.AttachShader(fs_halo);
		halo_prog.Link();

		// bind the VAO for the torus
		torus.Bind();

		// bind the VBO for the torus vertices
		torus_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);

			torus_prog.Use();
			VertexAttribArray attr_o(torus_prog, "Position");
			attr_o.Setup(n_per_vertex, DataType::Float);
			attr_o.Enable();

			halo_prog.Use();
			VertexAttribArray attr_s(halo_prog, "Position");
			attr_s.Setup(n_per_vertex, DataType::Float);
			attr_s.Enable();
		}

		// bind the VBO for the torus normals
		torus_normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);

			torus_prog.Use();
			VertexAttribArray attr_o(torus_prog, "Normal");
			attr_o.Setup(n_per_vertex, DataType::Float);
			attr_o.Enable();
		}

		// bind the VAO for the plane
		plane.Bind();

		// bind the VBO for the plane vertices
		plane_verts.Bind(Buffer::Target::Array);
		{
			GLfloat data[4*3] = {
				-9.0f, 0.0f,  9.0f,
				-9.0f, 0.0f, -9.0f,
				 9.0f, 0.0f,  9.0f,
				 9.0f, 0.0f, -9.0f
			};
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			plane_prog.Use();
			VertexAttribArray attr(plane_prog, "Position");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		// bind the VBO for the plane normals
		plane_normals.Bind(Buffer::Target::Array);
		{
			GLfloat data[4*3] = {
				-0.1f, 1.0f,  0.1f,
				-0.1f, 1.0f, -0.1f,
				 0.1f, 1.0f,  0.1f,
				 0.1f, 1.0f, -0.1f
			};
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			plane_prog.Use();
			VertexAttribArray attr(plane_prog, "Normal");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		Vec3f lightPos(2.0f, 2.5f, 9.0f);
		torus_prog.Use();
		Uniform(torus_prog, "LightPos").Set(lightPos);
		plane_prog.Use();
		Uniform(plane_prog, "LightPos").Set(lightPos);

		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.ClearStencil(0);

		gl.Enable(Capability::DepthTest);
		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::One);
	}

	void Reshape(size_t width, size_t height)
	{
		gl.Viewport(width, height);
		auto projection = CamMatrixf::Perspective(
			Degrees(54),
			double(width)/height,
			1, 100
		);
		torus_prog.Use();
		Uniform(torus_prog, "ProjectionMatrix").SetMatrix(projection);
		plane_prog.Use();
		Uniform(plane_prog, "ProjectionMatrix").SetMatrix(projection);
		halo_prog.Use();
		Uniform(halo_prog, "ProjectionMatrix").SetMatrix(projection);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			6.5,
			FullCircles(time / 5.0),
			Degrees(15 + (-SineWave(time/10.0)+1.0)* 0.5 * 75)
		);
		ModelMatrixf model =
			ModelMatrixf::Translation(0.0f, 2.5f, 0.0) *
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(time / 7.0)
			);

		plane_prog.Use();
		Uniform(plane_prog, "CameraMatrix").SetMatrix(camera);

		plane.Bind();
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);

		torus_prog.Use();
		Uniform(torus_prog, "CameraMatrix").SetMatrix(camera);

		Uniform(torus_prog, "ModelMatrix").SetMatrix(model);
		torus.Bind();
		torus_instr.Draw(torus_indices);

		halo_prog.Use();
		Uniform(halo_prog, "CameraMatrix").SetMatrix(camera);
		Uniform(halo_prog, "ModelMatrix").SetMatrix(model);

		gl.DepthMask(false);
		gl.Enable(Capability::Blend);
		torus_instr.Draw(torus_indices);
		gl.Disable(Capability::Blend);
		gl.DepthMask(true);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}
};

std::unique_ptr<Example> makeExample(void)
{
	return std::unique_ptr<Example>(new HaloExample);
}

} // namespace oglplus
