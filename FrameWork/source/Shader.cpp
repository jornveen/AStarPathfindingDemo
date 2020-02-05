#include <Shader.h>
#include <stdio.h>
#include <stdlib.h>
#include <Texture.h>
#include <Utils.h>
#include <Color.h>
#include <OpenGL.h>
#include <glad/glad.h>

using namespace std;
using namespace igad;

////////////////////////////////////////////////////////////////////////////////
// Compile shader and report success or failure
////////////////////////////////////////////////////////////////////////////////
bool CompileShader(GLuint* shader, GLenum type, const GLchar* source)
{
	GLint status;

	if (!source)
	{
		LOG("Failed to load shader %s \n", source);
		return false;
	}

	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &source, nullptr);
	GL_GET_ERROR();
	glCompileShader(*shader);

#if defined(DEBUG)
	GLint logLength = 0;
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar *log = static_cast<GLchar *>(malloc(logLength));
		glGetShaderInfoLog(*shader, logLength, &logLength, log);
		LOG("Shader compile log:\n%s", log);
		free(log);
	}
#endif

	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		glDeleteShader(*shader);
		return false;
	}

	GL_GET_ERROR();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Link Program
////////////////////////////////////////////////////////////////////////////////
bool LinkProgram(GLuint prog)
{
	GLint status;

	glLinkProgram(prog);

#if defined(DEBUG)
	GLint logLength = 0;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar *log = static_cast<GLchar *>(malloc(logLength));
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		LOG("Program link log:\n%s", log);
		free(log);
	}
#endif

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Validate program
////////////////////////////////////////////////////////////////////////////////
bool ValidateProgram(GLuint prog)
{
	GLint logLength = 0;
	GLint status = 0;

	glValidateProgram(prog);
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = static_cast<GLchar *>(malloc(logLength));
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		free(log);
	}

	logLength = 0;
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	if (status == 0)
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
//                              ShaderParameter
//
////////////////////////////////////////////////////////////////////////////////

void ShaderParameter::SetValue(float val)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_FLOAT);
	glUniform1f(_location, val);
	GL_GET_ERROR();
}


void ShaderParameter::SetValue(int val)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_INT);
	glUniform1i(_location, val);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(bool val)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_BOOL);
	glUniform1i(_location, val);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(const Vector2& vec)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_FLOAT_VEC2);
	glUniform2fv(_location, 1, &vec.x);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(const Vector3& vec)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_FLOAT_VEC3);
	glUniform3fv(_location, 1, vec.f);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(const Vector4& vec)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_FLOAT_VEC4);
	glUniform4fv(_location, 1, &vec.x);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(const Color& color)
{
	Vector4 c = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
	if (_type == GL_FLOAT_VEC4)
		glUniform4fv(_location, 1, &c.x);
	else if (_type == GL_FLOAT_VEC3)
		glUniform3fv(_location, 1, &c.x);
	GL_GET_ERROR();
}

void ShaderParameter::SetValue(const Matrix44& mtx, bool transpose)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_FLOAT_MAT4);
	glUniformMatrix4fv(_location, 1, transpose, mtx.f);
	GL_GET_ERROR();
}


void ShaderParameter::SetValue(const Texture &texture)
{
	if (!IsValid())
		return;

	ASSERT(_type == GL_SAMPLER_2D);
	// Use texture with index sampler. GL_TEXTURE1 = GL_TEXTURE10+1 is always true
	glActiveTexture(GL_TEXTURE0 + _sampler);
	GL_GET_ERROR();
	// Work with this texture
	glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
	GL_GET_ERROR();
	// Set the sampler
	glUniform1i(_location, _sampler);
	GL_GET_ERROR();
}

////////////////////////////////////////////////////////////////////////////////
//
//                          ShaderAttribute
//
////////////////////////////////////////////////////////////////////////////////

void ShaderAttribute::SetAttributePointer(GLint size,
	GLenum type,
	GLboolean normalized,
	GLsizei stride,
	const GLvoid *pointer)
{
	if (!IsValid())
		return;

	glVertexAttribPointer(
		_location,           // attribute
		size,               // number of elements per vertex element
		type,               // the type of each element
		normalized,         // take our values as-is or normalize
		stride,             // no extra data between each position
		pointer             // offset of first element
		);
	GL_GET_ERROR();

	glEnableVertexAttribArray(_location);
	GL_GET_ERROR();

}

void ShaderAttribute::DisableAttributePointer()
{
	if (!IsValid())
		return;

	glDisableVertexAttribArray(_location);
	GL_GET_ERROR();
}


////////////////////////////////////////////////////////////////////////////////
//
//                                  Shader
//
////////////////////////////////////////////////////////////////////////////////

Shader::Shader(const std::string& vertexFilename, const std::string& fragmentFilename)
	: Shader(vertexFilename, "", fragmentFilename)
{}

Shader::Shader(	const std::string& vertexFilename,
				const std::string& geometryFilename,
				const std::string& fragmentFilename)
	: _program(0)
{
	bool success = Load(vertexFilename, fragmentFilename, geometryFilename);
	if (!success)
	{
		LOG("Unable to load shader.");
	}
	else
	{
		LoadParameters();
	}
}

Shader::~Shader()
{
	if (_program > 0)
	{
		glDeleteProgram(_program);
		_program = 0;
		//GL_GET_ERROR();
	}
}

GLuint Shader::GetProgram() const
{
	return _program;
}

void igad::Shader::Deactivate()
{
	for(auto& a : _attributes)
		a.second->DisableAttributePointer();
}

void Shader::LoadParameters()
{
	/// The shader should invalidate when reloading a new shader file
	/// a some information can be old
	for (auto& itr : _parameters)
		itr.second->Invalidate();

	// Get the number of uniforms and resize the parameters collection accordingly
	GLint numActiveUniforms = 0;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	//parameters.resize(numActiveUniforms);

	// Get the maximum name length
	GLint maxUniformNameLength = 0;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
	vector<GLchar> uniformNameData(maxUniformNameLength);

	// Go over all the uniforms
	int samplerCount = 0;
	for (int uniform = 0; uniform < numActiveUniforms; uniform++)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(
			_program,
			uniform,
			maxUniformNameLength,
			&actualLength,
			&arraySize,
			&type,
			&uniformNameData[0]);
		string name(&uniformNameData[0], actualLength);
		GLint location = glGetUniformLocation(_program, name.c_str());

		auto itr = _parameters.find(name);
		if (itr != _parameters.end())
		{
			if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
				itr->second->Reset(this, name, type, location, samplerCount++);		
			else
				itr->second->Reset(this, name, type, location);
		}
		else
		{
			ShaderParameter* param = nullptr;
			if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
				param = new ShaderParameter(this, name, type, location, samplerCount++);
			else
				param = new ShaderParameter(this, name, type, location);
			_parameters[name] = unique_ptr<ShaderParameter>(param);
		}
	}


	GLint numActiveAttribs = 0;
	glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);


	GLint maxAttribNameLength = 0;
	glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
	vector<GLchar> attribNameData(maxAttribNameLength);


	for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveAttrib(_program,
			attrib,
			attribNameData.size(),
			&actualLength,
			&arraySize,
			&type,
			&attribNameData[0]);
		std::string name( static_cast<char*>(&attribNameData[0]));
		GLint location = glGetAttribLocation(_program, name.c_str());

		auto itr = _attributes.find(name);
		if (itr != _attributes.end())
		{
			itr->second->Reset(this, name, type, location);
		}
		else
		{
			ShaderAttribute* attribute = new ShaderAttribute(this, name, type, location);
			_attributes[name] = unique_ptr<ShaderAttribute>(attribute);
		}
	}
}

ShaderParameter* Shader::GetParameter(const string& name)
{
	// Try to find param
	auto itr = _parameters.find(name);
	if (itr != _parameters.end())
		return itr->second.get();

	// Create and return a non-valid param that is stored in collection
	// in case it becomes valid after a reload
	ShaderParameter* param = new ShaderParameter();
	_parameters[name] = unique_ptr<ShaderParameter>(param);
	return param;
}


ShaderAttribute* Shader::GetAttribute(const string& name)
{
	// Try to find param
	auto itr = _attributes.find(name);
	if (itr != _attributes.end())
		return itr->second.get();

	// Create and return a non-valid param that is stored in collection
	// in case it becomes valid after a reload
	ShaderAttribute* attrib = new ShaderAttribute();
	_attributes[name] = unique_ptr<ShaderAttribute>(attrib);
	return attrib;
}

void Shader::Activate()
{
	glUseProgram(GetProgram());
	GL_GET_ERROR();
}

bool Shader::Validate()
{
#if defined(DEBUG)
	if (!ValidateProgram(_program))
	{
		LOG("Failed to validate program: %d", _program);
		GL_GET_ERROR();
		return false;
	}
#endif
	return true;
}

bool Shader::Load(	const string& vertexFilename,
					const string& fragmentFilename,
					const string& geometryFilename)
{
	
	string vertShaderSource = ReadFile(vertexFilename);
	string geomShaderSource = "";
	if (geometryFilename.length() > 0)
		geomShaderSource = ReadFile(geometryFilename);
	string fragShaderSource = ReadFile(fragmentFilename);

	return LoadSource(vertShaderSource, geomShaderSource, fragShaderSource);
}

bool Shader::LoadSource(const string& vertexShader,
						const string& geometryShader,
						const string& fragmentShader)
{
	GLuint vertShader = 0;
	GLuint fragShader = 0;
	GLuint geomShader = 0;

	_program = glCreateProgram();

	// Temp result
	GLboolean res;

	res = CompileShader(&vertShader, GL_VERTEX_SHADER, vertexShader.c_str());
	if (!res)
	{
		LOG("Shader::Load() Failed to compile vertex shader");
		return false;
	}

	// Geometry shader is optional
	if (geometryShader.length() > 0)
	{
		res = CompileShader(&geomShader, GL_GEOMETRY_SHADER, geometryShader.c_str());
		if (!res)
		{
			LOG("Shader::Load() Failed to compile geometry shader");
			return false;
		}
	}

	// Create and compile fragment shader
	res = CompileShader(&fragShader, GL_FRAGMENT_SHADER, fragmentShader.c_str());
	if (!res)
	{
		LOG("Shader::Load() Failed to compile fragment shader");
		return false;
	}

	// Attach vertex shader to program
	glAttachShader(_program, vertShader);

	// Attach geometry shader to program
	if (geomShader)
		glAttachShader(_program, geomShader);

	// Attach fragment shader to program
	glAttachShader(_program, fragShader);

	// Link program
	if (!LinkProgram(_program))
	{
		if (vertShader)
		{
			glDeleteShader(vertShader);
			vertShader = 0;
		}
		if (fragShader)
		{
			glDeleteShader(fragShader);
			fragShader = 0;
		}
		if (geomShader)
		{
			glDeleteShader(geomShader);
			geomShader = 0;
		}
		if (_program)
		{
			glDeleteProgram(_program);
			_program = 0;
		}

		GL_GET_ERROR();

		// We crash here, else the logs will be flooded with repeated
		// error messages.
		ASSERT(false);

		return false;
	}

	glDeleteShader(vertShader);
	GL_GET_ERROR();
	glDeleteShader(geomShader);
	GL_GET_ERROR();
	glDeleteShader(fragShader);
	GL_GET_ERROR();

	LoadParameters();

	return true;
}
