#include <Engine/Renderer.h>
#include <iostream>

namespace KillerEngine 
{

//==========================================================================================================================
//
//Private Renderer Functions
//
//==========================================================================================================================
//=======================================================================================================
//_CompileShaders
//=======================================================================================================
	void Renderer::_CompileShaders(void)
	{
		//=====Vertex Shaders=====
		//=====Color Shader=====
		//This is used when only colors, not textures are used to render
		//a pirmitive
		static const GLchar* _vertexShaderSourceColor[] = 
		{
			"#version 430 core																\n"
			"																				\n"
			"layout (location = 0) in vec4 position;										\n"
			"layout (location = 1) in vec4 color; 											\n"					
			"uniform mat4 transform_mat;													\n"
			"																				\n"
			"out vec4 vs_color;																\n"
			"																				\n"
			"void main(void) 																\n"
			"{																				\n"
			"	gl_Position = transform_mat * position;										\n"
			"	vs_color = color;															\n"
			"}																				\n"
		};

		//=====Texture Shader=====
		//This is used when the pirmitive to be drawn has a texture
		//to be drawn with it
		static const GLchar* _vertexShaderSourceTexture[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"layout (location = 0) in vec4 position;										\n"
			"layout (location = 1) in vec2 tex_coord; 										\n"					
			"uniform mat4 transform_mat;													\n"
			"																				\n"
			"out vec2 vs_tex_coord;															\n"
			"																				\n"
			"void main(void) 																\n"
			"{																				\n"
			"	gl_Position = transform_mat * position;										\n"
			"	vs_tex_coord = vec2(tex_coord.x, tex_coord.y);								\n"
			"}																				\n"
		};

		//=====Fragment Shaders=====
		//=====Color Shader=====
		//This is used when only colors, not textures are used to render
		//a pirmitive
		static const GLchar* _fragmentShaderSourceColor[] = 
		{
			"#version 430 core																\n"
			"																				\n"
			"in vec4 vs_color;																\n"
			"out vec4 color;																\n"
			"																				\n"
			"void main(void) 																\n"
			"{																				\n"
			"	color = vs_color;															\n"
			"}																				\n"
		};

		//=====Texture Shader=====
		//This is used when the pirmitive to be drawn has a texture
		//to be drawn with it
		static const GLchar* _fragmentShaderSourceTexture[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"uniform sampler2D ourTexture;													\n"
			"in vec2 vs_tex_coord;															\n"
			"out vec4 color;																\n"
			"																				\n"
			"void main(void) 																\n"
			"{																				\n"
			//=====test for textures, failed finish later=====
			//"	color = texelFetch(sampler, ivec2(gl_FragCoord.xy), 0);						\n"
			"	color = texture(ourTexture, vs_tex_coord);									\n"
			//"	GL_FragColor = texture(ourTexture, vs_tex_coord);									\n"
			"}																				\n"
		};

		//=====Create and compile vertext shader=====
		GLuint vertexShaderProgramColor;
		GLuint vertexShaderProgramTexture;
		GLuint fragmentShaderProgramColor;
		GLuint fragmentShaderProgramTexture;

		//=====Compiled vertex shaders=====
		//=====Color=====
		vertexShaderProgramColor = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderProgramColor, 1, _vertexShaderSourceColor, NULL);
		glCompileShader(vertexShaderProgramColor);

		//=====Texutre=====
		vertexShaderProgramTexture = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderProgramTexture, 1, _vertexShaderSourceTexture, NULL);
		glCompileShader(vertexShaderProgramTexture);

		//=====Compile fragment shaders=====
		//=====Color=====
		fragmentShaderProgramColor = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderProgramColor, 1, _fragmentShaderSourceColor, NULL);
		glCompileShader(fragmentShaderProgramColor);

		//=====Texture=====
		fragmentShaderProgramTexture = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderProgramTexture, 1, _fragmentShaderSourceTexture, NULL);
		glCompileShader(fragmentShaderProgramTexture);

		//=====Create Color Shader, attach shader to it and link it=====
		_renderingProgramColor = glCreateProgram();
		glAttachShader(_renderingProgramColor, vertexShaderProgramColor);
		glAttachShader(_renderingProgramColor, fragmentShaderProgramColor);
		glLinkProgram(_renderingProgramColor);

		//=====Create Texture Shader, attach shader to it and link it=====
		_renderingProgramTexture = glCreateProgram();
		glAttachShader(_renderingProgramTexture, vertexShaderProgramTexture);
		glAttachShader(_renderingProgramTexture, fragmentShaderProgramTexture);
		glLinkProgram(_renderingProgramTexture);

		//=====Delete the sahders as the program now has them=====
		glDeleteShader(vertexShaderProgramColor);
		glDeleteShader(fragmentShaderProgramColor);
		glDeleteShader(vertexShaderProgramTexture);
		glDeleteShader(fragmentShaderProgramTexture);
	}

//=======================================================================================================
//_SetOrthoProjection
//=======================================================================================================
	void Renderer::_SetOrthoProjection(void) 
	{
		Mat projection{};
		projection.MakeOrthographic((F32)WinProgram::Instance()->GetWidth(), (F32)WinProgram::Instance()->GetHeight(), 200);

		Mat model(1.0f);

		Mat final = projection * model;

		const F32* data = final.GetElems();

		//=====Color Shader setup=====
//		glUseProgram(_currentShader);

		GLint transform1 = glGetUniformLocation(_currentShader, "transform_mat");

		glUniformMatrix4fv(transform1, 1, GL_FALSE, data);

/*
		glUseProgram(0);

		//=====Texture Shader setup=====
		glUseProgram(_renderingProgramTexture);

		GLint transform2 = glGetUniformLocation(_renderingProgramTexture, "transform_mat");

		glUniformMatrix4fv(transform2, 1, GL_FALSE, data);

		glUseProgram(0); 
*/
	}

//==========================================================================================================================
//
//Singleton Functions
//
//==========================================================================================================================
//=======================================================================================================
//Instance
//=======================================================================================================
	Renderer* Renderer::_instance = NULL;

	Renderer* Renderer::Instance(void) 
	{
		if(_instance == NULL) { _instance = new Renderer(); }
		return _instance;
	}

//=======================================================================================================
//ShutDown
//=======================================================================================================
	/*void Renderer::ShutDown(void) {
		delete _instance;
		glDeleteVertexArrays(1, &_vertexArrayObject);
		glDeleteProgram(_renderingProgram);
		glDeleteVertexArrays(1, &_vertexArrayObject);
	}*/


//=======================================================================================================
//AddToBatch
//=======================================================================================================
	void Renderer::AddToBatch(GLuint shader, Vec2& pos, U32 w, U32 h, Col& c)
	{
		if(_currentShader != shader)
		{
			Draw();
			_currentShader = shader;

			glUseProgram(_currentShader);

			_SetOrthoProjection();
		}

		if(_currentBatchSize + 1 >= _maxBatchSize) { Draw(); }

		_vertices.push_back(pos.GetX());
		_vertices.push_back(pos.GetY());
		_vertices.push_back(pos.GetZ());
		_vertices.push_back(pos.GetW());
		
		_dimensions.push_back((F32)w);
		_dimensions.push_back((F32)h);

		_colors.push_back(c.GetRed());
		_colors.push_back(c.GetGreen());
		_colors.push_back(c.GetBlue());
		_colors.push_back(c.GetAlpha());
		
		++_currentBatchSize;
	}

	void Renderer::AddToBatch(std::vector<F32> ver, std::vector<F32> col)
	{
		if(_currentBatchSize + ver.size() >= _maxBatchSize) { Draw(); }
		
		_vertices.reserve(_vertices.size() + ver.size());
		_vertices.insert(_vertices.end(), ver.begin(), ver.end());
		_currentBatchSize += ver.size();

		_colors.reserve(_colors.size() + col.size());
		_colors.insert(_colors.end(), col.begin(), col.end());

	}

//=======================================================================================================
//AddTextureToBatch
//=======================================================================================================


	void Renderer::AddTextureToBatch(std::vector<F32> ver, std::vector<F32> uv)
	{
		if(_currentBatchSize + ver.size() >= _maxBatchSize) { Draw(); }
		
		_vertices.reserve(_vertices.size() + ver.size());
		_vertices.insert(_vertices.end(), ver.begin(), ver.end());
		_currentBatchSize += ver.size();

		_uvs.reserve(_uvs.size() + uv.size());
		_uvs.insert(_uvs.end(), uv.begin(), uv.end());

	}


//=======================================================================================================
//Draw
//=======================================================================================================

	void Renderer::Draw(void)
	{
		if(_currentBatchSize == 0) return;

		GLuint buffers[4];
		glGenBuffers(4, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(F32) * _vertices.size()), &_vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(F32) * _colors.size()), &_colors[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		

		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(F32) * _dimensions.size()), &_dimensions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		
		glDrawArrays(GL_POINTS, 0, _currentBatchSize);

		//=====Reset All Containers and Counters=====
		_vertices.clear();
		_dimensions.clear();
		_colors.clear();
		_uvs.clear();
		_currentBatchSize = 0;
	}
//=======================================================================================================
//
//Constructor
//
//=======================================================================================================
	Renderer::Renderer(void): _maxBatchSize(1000), 
							  _currentBatchSize(0)
	{ 
	//	_CompileShaders();
		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);
		//_SetOrthoProjection();
	}

}//End namespace		