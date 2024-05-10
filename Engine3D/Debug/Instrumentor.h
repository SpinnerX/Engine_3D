#pragma once
#include <mutex>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>


/*

Usage: include this header and use like:
Instrumentor::get().BeginSession("Session Name"); // beginning the session
{
	InstrumentorTimer timer("Profiled Scope Name"); // placing code like this in scopes, you'd like to include in profiling
	// code...
}
Instrumentor::get().EndSession(); // Ending the profiling session

NOTE: Probably want to shorten how we call this to switch on/off easily and also consider using things like __FUNCSIG__ for the profile name.

*/

/*namespace Engine3D{ */

/*	struct ProfileResult{ */
/*		std::string name; */
/*		long long start, end; */
/*		std::thread::id threadID; */
/*	}; */

/*	struct InstrumentationSession{ */
/*		std::string name; */
/*	}; */


/*	class Instrumentor{ */
/*	public: */
/*		Instrumentor() : _currentSession(nullptr), _profileCount(0) {} */

/*		void BeginSession(const std::string& name, const std::string& filepath = "results.json"){ */
/*			std::lock_guard<std::mutex> guard(_mutex); */
			
/*			/1* */
/*			 * */
/*			 * @BeginSession */
/*			 * @note before starting a session if one session already exists, then close that session before starting a new one. */
/*			 * @note Profiling output means for original session will be the newly opened session instead (better then having it be badly formatted) */
/*			 * @note profiling output */
/*			 * */
/*			*/
/*			if(_currentSession){ */
/*				if(EngineLogger::GetCoreLogger()){ // We are checking just in case anothher session has already been initiated before EngineLogger has been initialized. */
/*					coreLogError("Instrumentor::BeginSession('{}') when session '{}' already open.", name, _currentSession->name); */
/*					internalEndSession(); */
/*				} */
				
/*			} */
/*			 _outputStream.open(filepath); */
/*			writeHeader(); */
/*			_currentSession = new InstrumentationSession{ name }; */
/*		} */
		
/*		void EndSession(){ */
/*			std::lock_guard<std::mutex> guard(_mutex); */
/*			internalEndSession(); */
/*		} */

/*		void writeProfile(const ProfileResult& result){ */
/*			std::stringstream json; */

/*			if (_profileCount++ > 0){ */
/*				_outputStream << ","; */
/*			} */

/*			std::string name = result.name; */
/*			std::replace(name.begin(), name.end(), '"', '\''); */

/*			json << "{"; */
/*			json << "\"cat\":\"function\","; */
/*			json << "\"dur\":" << (result.end - result.start) << ','; */
/*			json << "\"name\":\"" << name << "\","; */
/*			json << "\"ph\":\"X\","; */
			/* json << "\"pid\":0,"; */
			/* json << "\"tid\":" << result.threadID << ","; */
			/* json << "\"ts\":" << result.start; */
			/* json << "}"; */
			
			/* std::lock_guard<std::mutex> guard(_mutex); */
			/* if(_currentSession){ */
			/* 	_outputStream << json.str(); */
			/* 	_outputStream.flush(); */
			/* } */
		/* } */

		/* static Instrumentor& get(){ */
			/* static Instrumentor instance; */
			/* return instance; */
		/* } */

	/* private: */
		/* void writeHeader(){ */
			/* _outputStream << "{\"otherData\": {},\"traceEvents\":["; */
			/* _outputStream.flush(); */
		/* } */

		/* void writeFooter(){ */
			/* _outputStream << "]}"; */
			/* _outputStream.flush(); */
		/* } */
		
		/* // NOTE: Keep in mind that you must already own that mutex before */
		/* // calling on internalEndSession! */
		/* void internalEndSession(){ */
		/* 	if(_currentSession){ */
		/* 		writeFooter(); */
		/* 		_outputStream.close(); */
		/* 		delete _currentSession; */
		/* 		_currentSession = nullptr; */
		/* 	} */
		/* } */
	/* private: */
		/* InstrumentationSession* _currentSession; */
		/* std::ofstream _outputStream; */
		/* int _profileCount; */
		/* std::mutex _mutex; */
	/* }; */

	/* class InstrumentorTimer{ */
	/* public: */
		/* InstrumentorTimer(const char* n) : _name(n), _stopped(false) { */
		/* 	_startTimePoint = std::chrono::high_resolution_clock::now(); */
		/* } */

		/* ~InstrumentorTimer(){ */
		/* 	if(!_stopped){ */
		/* 		stop(); */
		/* 	} */
		/* } */

		/* void stop(){ */
		/* 	auto endTimepoint = std::chrono::high_resolution_clock::now(); */

			/* long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count(); */
			/* long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count(); */

			/* uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id()); */
			/* Instrumentor::get().writeProfile({ _name, start, end, std::this_thread::get_id() }); */

			/* _stopped = true; */
		/* } */

	/* private: */
		/* const char* _name; */
		/* std::chrono::time_point<std::chrono::high_resolution_clock> _startTimePoint; */
		/* bool _stopped; */
	/* }; */
/* } */


namespace Engine3D{

	struct ProfileResult{
		std::string name;
		long long start, end;
		uint32_t threadID;
	};

	struct InstrumentationSession{
		std::string name;
	};


	class Instrumentor{
	public:
		Instrumentor() : _currentSession(nullptr), _profileCount(0) {}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json"){
			 _outputStream.open(filepath);
			writeHeader();
			_currentSession = new InstrumentationSession{ name };
		}
		
		void EndSession(){
			writeFooter();
			_outputStream.close();
			delete _currentSession;
			_currentSession = nullptr;
			_profileCount = 0;
		}

		void writeProfile(const ProfileResult& result){
			if (_profileCount++ > 0){
				_outputStream << ",";
			}

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			_outputStream << "{";
			_outputStream << "\"cat\":\"function\",";
			_outputStream << "\"dur\":" << (result.end - result.start) << ',';
			_outputStream << "\"name\":\"" << name << "\",";
			_outputStream << "\"ph\":\"X\",";
			_outputStream << "\"pid\":0,";
			_outputStream << "\"tid\":" << result.threadID << ",";
			_outputStream << "\"ts\":" << result.start;
			_outputStream << "}";

			_outputStream.flush();
		}

		void writeHeader(){
			_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			_outputStream.flush();
		}

		void writeFooter(){
			_outputStream << "]}";
			_outputStream.flush();
		}

		static Instrumentor& get(){
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* _currentSession;
		std::ofstream _outputStream;
		int _profileCount;
	};

	class InstrumentorTimer{
	public:
		InstrumentorTimer(const char* n) : _name(n), _stopped(false) {
			_startTimePoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentorTimer(){
			if(!_stopped){
				stop();
			}
		}

		void stop(){
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::get().writeProfile({ _name, start, end, threadID });

			_stopped = true;
		}

	private:
		const char* _name;
		std::chrono::time_point<std::chrono::high_resolution_clock> _startTimePoint;
		bool _stopped;
	};
}


// NOTE: May change this later on.. since this is temporary
// NOTE: Was thinking probable have a profiler counter that is static, that'll be used for each thread ID of some sort...
#define RENDER_PROFILE 1

#if RENDER_PROFILE
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC)  && (__ICC >= 600))
		#define RENDER_FUNCTION_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define RENDER_FUNCTION_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define RENDER_FUNCTION_SIG __PRETTY_FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define RENDER_FUNCTION_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define RENDER_FUNCTION_SIG __func__ 
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define RENDER_FUNCTION_SIG __func__
	#else
		#define RENDER_FUNCTION_SIG "RENDER_FUNCTION_SIG not supported!"
#endif

	#define RENDER_PROFILE_BEGIN_SESSION(name, filepath) ::Engine3D::Instrumentor::get().BeginSession(name, filepath);
	#define RENDER_PROFILE_END_SESSION() ::Engine3D::Instrumentor::get().EndSession();
	
	#define RENDER_PROFILE_SCOPE(name) ::Engine3D::InstrumentorTimer timer##__LINE__(name);

	// __FUNCSIG__ means function signature giving us the complete name of the function for uniqueness
	#define RENDER_PROFILE_FUNCTION() RENDER_PROFILE_SCOPE(RENDER_FUNCTION_SIG)
#else
	#define RENDER_PROFILE_BEGIN_SESSION(name, filepath)
	#define RENDER_PROFILE_END_SESSION()
	
	#define RENDER_PROFILE_FUNCTION()
	#define RENDER_PROFILE_SCOPE(name)
	
#endif