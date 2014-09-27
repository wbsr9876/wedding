//===================================================================================
//
//
#ifndef __BASE_THREAD_H_
#define __BASE_THREAD_H_
#include "server_def.h"
namespace netcore
{
	//===================================================================================
	//
	//
	enum ThreadState
	{
		THREAD_STATE_START,
		THREAD_STATE_RUNNING,
		THREAD_STATE_END,
	};
	//===================================================================================
	//
	//

	class CBaseThread
	{
	private:
		ThreadState		m_eStatus;
#if defined(_WIN32) || defined(WIN64)
		HANDLE			m_hThread;
#else
		pthread_t       thread_id;
		pthread_attr_t  th_attr;
#endif
	public:
		CBaseThread();
		CBaseThread( bool bDetached );
		~CBaseThread();
		//
		bool			BeginThread();
		void			EndThread( const bool bEnd = true );
		virtual void	*DoAction() = 0;
	};
}

//
#endif
