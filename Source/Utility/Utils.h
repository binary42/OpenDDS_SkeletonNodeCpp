#pragma once

/**
 * Node utility namepspace. (nod to spiderkeys)
 *
 * https://github.com/spiderkeys/SensorPublisher
 */

// Logging header
#include "easylogging++.h"

// Standard library headers
#include <time.h>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <map>
#include <functional>
#include <memory>
#include <unistd.h>
#include <condition_variable>
#include <atomic>

// Defines
#define SafeDelete(a) if( a != nullptr ){ delete a; a = nullptr; }

// Node utilities namespace. All helper functions should be located here
namespace nodeutils
{
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
	static const std::string CurrentDateTime()
	{
		time_t     now = time(0);
		struct tm  tstruct;

		// Get current local time
		tstruct = *localtime( &now );

		std::stringstream datetime;

		// Format string
		datetime << tstruct.tm_mday << "/" <<  tstruct.tm_mon << "/" << tstruct.tm_year << " " << tstruct.tm_hour << ":" << tstruct.tm_min << ":" << tstruct.tm_sec;

		return datetime.str();
	}

	static uint64_t GetUnixTimestampMs()
	{
		return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() ).count();
	}

	static uint64_t UnixTimestampFromString( std::string timestampStringIn )
	{
		uint64_t output = 0;

		std::istringstream ss( timestampStringIn );

		// Attempt to convert string to uint64_t
		if( !( ss >> output ) )
		{
			LOG( ERROR ) << "Failure converting string to uint64_t";
		}

		return output;
	}

	static std::string UnixTimestampToString( uint64_t timestampIn )
	{
		// Convert uint64_t to string
		std::ostringstream o;
		o << timestampIn;
		return o.str();
	}

	static double GetTimeInSecs()
	{
		struct timeval tv;
		gettimeofday( &tv, NULL );
		return ( tv.tv_sec + ( tv.tv_usec / 1000000.0 ) );
	}

	// Wrapper class for non-recursive, unfair pthread mutex
	class CMutex
	{
		pthread_mutex_t  m_mutex;

	public:
		// just initialize to defaults
		CMutex() { pthread_mutex_init( &m_mutex, NULL ); }
		virtual ~CMutex() { pthread_mutex_destroy(&m_mutex); }

		int Lock() { return pthread_mutex_lock( &m_mutex ); }
		int TryLock() { return pthread_mutex_lock( &m_mutex ); }
		int Unlock() { return pthread_mutex_unlock( &m_mutex ); }
	};

	// Lock class for exception-safe non-recursive locking
	class CLock
	{
	public:
		CMutex *m_pMutex;

		CLock( CMutex &mutexIn )
		{
			// Lock mutex
			m_pMutex = &mutexIn;
			m_pMutex->Lock();
		}

		~CLock()
		{
			// Unlock mutex
			m_pMutex->Unlock();
		}

	};

	// A function that takes a void pointer, and is passed to the thread creation function.
	typedef void* (*ThreadFunction)(void *);

	class CThread
	{

	public:
		// --- Constructor ---
		CThread( ThreadFunction functionIn, void *functionParamIn )
		{
			m_thread 			= 0;
			m_pFunction 		= functionIn;
			m_pFunctionParam 	= functionParamIn;
		}

		// Run the thread
		void Run()
		{
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			pthread_create( &m_thread, &threadAttr, m_pFunction, (void *)m_pFunctionParam );
			pthread_attr_destroy(&threadAttr);
		}

	private:
	    pthread_t m_thread;

		// Function called by OS-specific thread
		ThreadFunction m_pFunction;

		// Parameter to the function
		void *m_pFunctionParam;
	};
}
