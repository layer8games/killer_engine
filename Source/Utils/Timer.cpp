#include <Timer.h>
#include <iostream>

//----------------------------------------------------------Static Member Declartions
F32 Timer::_frequency = 0;
Timer* Timer::_instance = NULL;

//----------------------------------------------------------Protected Constructor Timer()
Timer::Timer(F64 startTimerSeconds = 0.0) {
	_frequency  = _QueryFrequency();
	_deltaTime  = 0.0f;
	_timeScale  = 1.0f;
	_totalTime  = startTimerSeconds;
	_pastCycles = _QueryHiResTimer();
	_curCycles  = _QueryHiResTimer();
	_paused     = false;
}

//-----------------------------------------------------------Instance()
//-----------------------------------------------------------Singleton, if NULL, make a new one
Timer* Timer::Instance() {
	if(_instance == NULL) { _instance = new Timer(); }

	return _instance;
}

void Timer::Update() {
	if(!_paused) {
		_pastCycles = _curCycles;
		_curCycles  = _QueryHiResTimer();
		_deltaTime  = _CyclesToSeconds(_curCycles - _pastCycles) * _timeScale;
		_totalTime += _SecondsToCycles(_deltaTime);
	}
}

void Timer::SingleStep() {
	if(!_paused) {
		U64 oneStep = _SecondsToCycles((1.0f/30.0f) * _timeScale);

		_deltaTime = _CyclesToSeconds(oneStep);

		_totalTime += _SecondsToCycles(_deltaTime);
	}
}



//-------------------------------------------------------------------Private _QueryHiResTimer()
//-------------------------------------------------------------------Windows only implemenation
U64 Timer::_QueryHiResTimer() { 
	static LARGE_INTEGER _cycles;
	QueryPerformanceCounter(&_cycles);
	return _cycles.QuadPart;
}

//-------------------------------------------------------------------Private _QueryFrequence()
//-------------------------------------------------------------------Windows only implemenation
F32 Timer::_QueryFrequency() { 
	static LARGE_INTEGER _freq;
	QueryPerformanceFrequency(&_freq); 
	return (F32)_freq.QuadPart;
}