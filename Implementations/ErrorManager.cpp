#include <Engine/ErrorManager.h>
	
namespace KillerEngine 
{

//==========================================================================================================================
//
//Singleton Functions
//
//==========================================================================================================================
	ErrorManager* ErrorManager::_instance = NULL;

	ErrorManager* ErrorManager::Instance(void) 
	{
		if(_instance == NULL) { _instance = new ErrorManager(); }
		return _instance;
	}

//==========================================================================================================================
//
//ErrorManager Functions
//
//==========================================================================================================================
//=======================================================================================================
//SetError
//=======================================================================================================	
	void ErrorManager::SetError(ErrorCode code, string message) 
	{
		_errorCodes[_numErrors]	   = code;
		_errorMessages[_numErrors] = message;
		_numErrors++;
	}

//=======================================================================================================
//DisplayErrors
//=======================================================================================================
	void ErrorManager::DisplayErrors(void) 
	{
		if (_numErrors > 0) 
		{
			for (U32 i = 0; i < _numErrors; i++) 
			{
				switch (_errorCodes[i]) 
				{
				case EC_NoError: {
									 //later, it will print to a log file, maybe
				}
				case EC_Unknown: {
					MessageBox(NULL, _errorMessages[i].c_str(), "UNKNOWN", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_Game: {
					MessageBox(NULL, _errorMessages[i].c_str(), "GAME", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_KillerEngine: {
					MessageBox(NULL, _errorMessages[i].c_str(), "KILLER_ENGINE", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_Windows: {
					MessageBox(NULL, _errorMessages[i].c_str(), "WINDOWS", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_OpenGL: {
					MessageBox(NULL, _errorMessages[i].c_str(), "OPENGL", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_OpenGL_Shader: {
					MessageBox(NULL, _errorMessages[i].c_str(), "OPENGL SHADER", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_DirectInput: {
					MessageBox(NULL, _errorMessages[i].c_str(), "DIRECT_INPUT", MB_ICONERROR | MB_OK);
					break;
				}
				case EC_TextureManager: {
					MessageBox(NULL, _errorMessages[i].c_str(), "TEXTURE_MANAGER", MB_ICONERROR | MB_OK);						
				}
				default: break;
				}
			}
		}
	}

}//End namespace