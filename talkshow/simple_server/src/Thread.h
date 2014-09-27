//===================================================================================
//
//
#ifndef __THREAD_H_
#define __THREAD_H_
//===================================================================================
//
//
#include "BaseThread.h"

namespace netcore
{
	template<class TCallObj>
	class CThread : public CBaseThread
	{
	private:
		typedef void(TCallObj::*ThreadFun)();
		TCallObj	*m_pCallObj;
		ThreadFun	m_pThreadFun;
	public:
		//
		CThread()
		{
			m_pCallObj		= NULL;
			m_pThreadFun	= NULL;
		}
		~CThread()
		{
		}
		//
		bool Create( TCallObj *pCallObj, ThreadFun pThreadFun )
		{
			m_pCallObj		=	pCallObj;
			m_pThreadFun	=	pThreadFun;
			return BeginThread();
		}

		inline void *DoAction()
		{
			if( m_pCallObj && m_pThreadFun )
				(m_pCallObj->*m_pThreadFun)();
			return NULL;
		}
	};
}

#endif
