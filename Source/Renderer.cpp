#include <Renderer.h>

//=======================================================================================================
//Constructor
//=======================================================================================================
Renderer::Renderer(void): _maxVerticies(1000) {
	_verticies    	  = new F32[_maxVerticies];
	_colors       	  = new F32[_maxVerticies];
	_uvs          	  = new GLfloat[_maxVerticies];
	_batchSize    	  = 0;
	_renderingProgram = _CompileShaders();
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
}

//=======================================================================================================
//Instance
//=======================================================================================================
//=====Static Member Declaration=====
Renderer* Renderer::_instance = NULL;

Renderer* Renderer::Instance(void) {
	if(_instance == NULL) { _instance = new Renderer(); }

	return _instance;
}

//=======================================================================================================
//AddCell
//=======================================================================================================
void Renderer::AddCell(Cell &cell) {
	if(_batchSize + cell.TotalVertices() > _maxVerticies) { Render(); }

	for(U32 i = 0; i < cell.TotalPositions(); i++) {
		_verticies[_batchSize] = cell.VertexPositions()[i].x;
		_verticies[_batchSize+1] = cell.VertexPositions()[i].y;
		_verticies[_batchSize+2] = cell.VertexPositions()[i].z;
		_colors[_batchSize]    = cell.VertexColors()[i].Red;
		_colors[_batchSize+1]	   = cell.VertexColors()[i].Green;
		_colors[_batchSize+2]    = cell.VertexColors()[i].Blue;
	}
}

//=======================================================================================================
//Manual Add functions
//
//These are potentially temporary functions that are meant to allow the user to manually add vertex or
//Color data to the buffer objects.
//
//=======================================================================================================
/*void Renderer::ManualAddData(F32* vertices, U32 numVertices) {
	if(_batchSize + numVertices > _maxVerticies) { Render(); }

	for(U32 i = 0; i < numVertices; i++) {
		_verticies[_batchSize] = vertices[i];
		_colors[_batchSize] = 0.5f;
		_batchSize++;
	}
}

void Renderer::ManualAddData(F32* vertices, F32* colors, U32 numVertices) {
	if(_batchSize + numVertices > _maxVerticies) { Render(); }

	for(U32 i = 0; i < numVertices; i++) {
		_verticies[_batchSize] = vertices[i];
		_colors[_batchSize] = colors[i];
		_batchSize++;
	}
}
*/
//=======================================================================================================
//SetPointers
//=======================================================================================================
void Renderer::SetPointers(void) {
	 GLuint buffers[2];
	 glGenBuffers(2, buffers);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(_verticies), _verticies, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(0);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(_colors), _colors, GL_STATIC_DRAW);
	 glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(1);

}

//=======================================================================================================
//Render
//=======================================================================================================
void Renderer::Render(void) {
	if(_batchSize == 0) { return; } //End if there are no verticies to draw
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const GLfloat color[] = { 0.5, 0.5, 0.5, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

    glUseProgram(_renderingProgram);

    SetPointers();
    //view port set up
    //glTranslatef(0.0f, 0.0f, -20.0f);
	
	glDrawArrays(GL_TRIANGLES, 0, _batchSize);

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	_batchSize = 0;
}

//====================================================
//_CompileShaders
//====================================================

GLuint Renderer::_CompileShaders(void){
	GLuint vertexShaderProgram;
	GLuint tessControlProgram;
	GLuint tessEvalProgram;
	GLuint fragmentShaderProgram;
	GLuint finalProgram;

	//=====Vertex Shader=====
	static const GLchar* _vertexShaderSource[] = {
		"#version 430 core																\n"
		"																				\n"
		"layout (location = 0) in vec4 position;										\n"
		"layout (location = 1) in vec4 color; 											\n"
		"																				\n"
		"out vec4 vs_color;																\n"
		"																				\n"
		"void main(void){																\n"
		"	gl_Position = position;														\n"
		"	vs_color = color; 															\n"
		"}																				\n"
	};

	//=====Tesselsation Control Shader=====
	static const GLchar* _tessControlSource[] = {
		"#version 430 core 																\n"
		"																				\n"
		"layout (vertices = 3) out;			 											\n"
		"																				\n"
		"void main(void) {																\n"	
		"	if(gl_InvocationID == 0) {													\n"
		"		gl_TessLevelInner[0] = 5.0;		 										\n"
		"		gl_TessLevelOuter[0] = 5.0;												\n"
		"		gl_TessLevelOuter[1] = 5.0;												\n"
		"		gl_TessLevelOuter[2] = 5.0;												\n"
		"	} 																			\n"
		"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; 	\n"
		"} 																				\n"
		
	};

	//=====Tesselsation Evaluation Shader=====
	static const GLchar* _tessEvalShaderSource[] = {
		"#version 430 core 																\n"
		"																				\n"
		"layout (triangles, equal_spacing, cw) in; 										\n"
		"																				\n"
		"void main(void) {																\n"
		"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +						\n"
		"				   gl_TessCoord.y * gl_in[1].gl_Position +						\n"
		"				   gl_TessCoord.z * gl_in[2].gl_Position);						\n"
		"}																			 	\n"
	};

	//=====Fragment Shader=====
	static const GLchar* _fragmentShaderSource[] = {
		"#version 430 core								\n"
		"												\n"
		"in vec4 vs_color;								\n"
		"out vec4 color;								\n"
		"												\n"
		"void main(void) {								\n"
		"	color = vs_color;							\n"
		"}												\n"
	};

	//=====Create and compile vertext shader=====
	vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderProgram, 1, _vertexShaderSource, NULL);
	glCompileShader(vertexShaderProgram);

	//=====Create and compile tessellation control shader=====
	tessControlProgram = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tessControlProgram, 1, _tessControlSource, NULL);
	glCompileShader(tessControlProgram);

	//======Create and comple tessellation evaluation shader=====
	tessEvalProgram = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tessEvalProgram, 1, _tessEvalShaderSource, NULL);
	glCompileShader(tessEvalProgram);

	//=====Create and compile fragment shader=====
	fragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderProgram, 1, _fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderProgram);

	//=====Create program, attach shader to it and link it=====
	finalProgram = glCreateProgram();
	glAttachShader(finalProgram, vertexShaderProgram);
	//glAttachShader(finalProgram, tessControlProgram);
	//glAttachShader(finalProgram, tessEvalProgram);
	glAttachShader(finalProgram, fragmentShaderProgram);
	glLinkProgram(finalProgram);

	//=====Delete the sahders as the program now has them=====
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(tessControlProgram);
	glDeleteShader(tessEvalProgram);
	glDeleteShader(fragmentShaderProgram);

	return finalProgram;
}

//=======================================================================================================
//ShutDown
//=======================================================================================================
void Renderer::ShutDown(void) {
	delete _verticies;
	delete _colors;
	delete _uvs;
	delete _instance;
	glDeleteVertexArrays(1, &_vertexArrayObject);
	glDeleteProgram(_renderingProgram);
	glDeleteVertexArrays(1, &_vertexArrayObject);
}