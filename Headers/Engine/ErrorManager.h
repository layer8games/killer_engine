/*========================================================================
A singleton class that will display any errors that can happen during
the initialization of any of the components of the engine. It will be
flushed out later to include more details. 

This is not free to use, and cannot be used without the express permission
of KillerWave.

Written by Maxwell Miller
========================================================================*/
#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

//=====Killer1 Includes=====
#include <Engine/Atom.h>

//=====STL includes=====
#include <map>
using std::map;

namespace KillerEngine 
{
	
//==========================================================================================================================
//
//Enum Error codes
//
//==========================================================================================================================
	enum ErrorCode 
	{
		EC_NoError = 0,
		EC_Unknown,
		EC_Game,
		EC_KillerEngine,
		EC_Windows,
		EC_OpenGL,
		EC_DirectInput,
		EC_TextureManager
	};

	class ErrorManager 
	{
	private:
		U32       			 _numErrors;
		map<U32, ErrorCode>  _errorCodes;
		map<U32, text>       _errorMessages;
		static ErrorManager* _instance;

	public:
//==========================================================================================================================
//
//Destructor
//
//==========================================================================================================================
		~ErrorManager(void) {  }

//==========================================================================================================================
//
//Singleton Functions
//
//==========================================================================================================================		
		static ErrorManager* Instance(void);

//==========================================================================================================================
//
//ErrorManager Functions
//
//==========================================================================================================================
		void SetError(ErrorCode code, text message);
		
		void DisplayErrors(void);

	protected:
//==========================================================================================================================
//
//Constructor
//
//==========================================================================================================================
		ErrorManager(void): _numErrors(0) {  }
	};

}//End namespace

#endif