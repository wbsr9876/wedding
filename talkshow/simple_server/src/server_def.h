#ifndef __SERVER_DEF_H_
#define __SERVER_DEF_H_

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#include <windows.h>

#else
#include <unistd.h>
#include <pthread.h>
#endif // WIN32

#endif