//===================================================================================
//
//
#include "BaseThread.h"
namespace netcore
{
	//===================================================================================
	//
	//
#ifdef WIN32
	static DWORD WINAPI ThreadProc( void *lpParameter)
	{
		CBaseThread *pThread = (CBaseThread*)lpParameter;
		pThread->DoAction();
		return 0;
	}
#else
	static void *ThreadProc( void *lpParameter)
	{
		CBaseThread *pThread = (CBaseThread*)lpParameter;
		pThread->DoAction();
	}
#endif
	//===================================================================================
	//
	//
	CBaseThread::CBaseThread()
	{
		m_eStatus = THREAD_STATE_START;
#ifdef WIN32
#else
		pthread_attr_init( &th_attr );
		pthread_attr_setdetachstate( &th_attr, PTHREAD_CREATE_DETACHED );
#endif
	}
	//===================================================================================
	//
	//
	CBaseThread::CBaseThread( bool bDetached )
	{
		m_eStatus = THREAD_STATE_START;
#ifdef WIN32
#else
		pthread_attr_init( &th_attr );
		if( bDetached )
		{
			pthread_attr_setdetachstate( &th_attr, PTHREAD_CREATE_DETACHED );
		}
#endif
	}
	//===================================================================================
	//
	//
	CBaseThread::~CBaseThread()
	{
#ifdef WIN32
		if( THREAD_STATE_RUNNING == m_eStatus )
		{
			TerminateThread( m_hThread, 0 );
		}
#else
#endif
	}
	//===================================================================================
	//
	//
	bool CBaseThread::BeginThread()
	{
#ifdef WIN32
		if( THREAD_STATE_START == m_eStatus )
		{
			m_hThread = CreateThread( NULL, 0, ThreadProc, this, 0, NULL );
			//
			if( m_hThread )
			{
				m_eStatus = THREAD_STATE_RUNNING;
				return true;
			}
		}
#else
		if( THREAD_STATE_START == m_eStatus && !pthread_create( &thread_id, &th_attr, ThreadProc, this ) )
		{
			m_eStatus = THREAD_STATE_RUNNING;
			return true;
		}
#endif
		return false;
	}
	//===================================================================================
	//
	//
	void CBaseThread::EndThread( const bool bEnd )
	{
		if( THREAD_STATE_RUNNING == m_eStatus )
		{
			if( bEnd )
			{
#ifdef WIN32
				TerminateThread( m_hThread, 0 );
#else
				pthread_cancel(thread_id);
				usleep(100);	// let thread process left work
#endif
			}
			m_eStatus = THREAD_STATE_END;
		}
	}
}

