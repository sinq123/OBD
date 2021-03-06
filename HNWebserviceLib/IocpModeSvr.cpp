// IocpModeSvr.cpp: implementation of the CIocpModeSvr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IocpModeSvr.h"
#include <fstream>
using namespace std;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma  warning(disable:4800)

DWORD WINAPI ServerWorkerProc(LPVOID lParam)
{
	CIocpModeSvr* pSvr=(CIocpModeSvr*)lParam;
	HANDLE CompletionPort=pSvr->CompletionPort;
	DWORD ByteTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	PPER_IO_OPERATION_DATA PerIoData;
	DWORD RecvByte;
	while(true)
	{
		bool bSuccess=GetQueuedCompletionStatus(CompletionPort,
												&ByteTransferred,
												(LPDWORD)&PerHandleData,
												(LPOVERLAPPED* )&PerIoData,
												INFINITE);
		//退出信号到达，退出线程
		if(ByteTransferred==-1 && PerIoData==NULL)
		{
			return 1L;
		}
		//客户机已经断开连接或者连接出现错误
		if(ByteTransferred==0 && 
		   (PerIoData->OperType==RECV_POSTED || PerIoData->OperType==SEND_POSTED))
		{
			//将该客户端数据删除
			int arrSize=0;
			bool bFind=false;
			::EnterCriticalSection(&pSvr->cInfoSection);
			while(arrSize<pSvr->ClientInfo.GetSize())
			{
				PER_HANDLE_DATA pPerHandleData=pSvr->ClientInfo.GetAt(arrSize);
				if((pPerHandleData.IpAddr==PerHandleData->IpAddr) &&
				   (pPerHandleData.sClient==PerHandleData->sClient))
				{
					bFind=true;
					pSvr->ClientInfo.RemoveAt(arrSize);
					break;
				}
				arrSize++;
			}
			::LeaveCriticalSection(&pSvr->cInfoSection);
			if(bFind)
			{
				//记录退出日志
				CString LogStr;
				in_addr in_A;
				in_A.S_un.S_addr=PerHandleData->IpAddr;
				LogStr.Format(L"Ip: %s,Socket : %d Disconneted",inet_ntoa(in_A),PerHandleData->sClient);
				pSvr->WriteLogString(LogStr);
				TRACE(L"\nSocket : %d Disconneted",PerHandleData->sClient);
				//调用回调函数，通知上层该客户端已经断开
				pSvr->m_pProcessRecvData(PerHandleData->IpAddr,
					PerHandleData->sClient,
					NULL,
					0);
				//关闭套接口
				closesocket(PerHandleData->sClient);
				GlobalFree(PerHandleData);
				GlobalFree(PerIoData);
			}
			continue;
		}
		//为读操作完成，处理数据
		if(PerIoData->OperType==RECV_POSTED)
		{
			//调用回调函数，处理数据
			pSvr->m_pProcessRecvData(PerHandleData->IpAddr,
				                     PerHandleData->sClient,
									 PerIoData->RecvBuf,
									 ByteTransferred);
			//将源数据置空
			memset(PerIoData->RecvBuf,0,BUFFER_SIZE);
			ByteTransferred=0;
			//重置IO操作数据
			unsigned long Flag=0;
			ZeroMemory(&(PerIoData->OverLapped),sizeof(OVERLAPPED));
			
			PerIoData->RecvDataBuf.buf=PerIoData->RecvBuf;
			PerIoData->RecvDataBuf.len=BUFFER_SIZE;
			PerIoData->OperType=RECV_POSTED;
			//提交另一个Recv请求
			WSARecv(PerHandleData->sClient,
				&(PerIoData->RecvDataBuf),
				1,
				&RecvByte,
				&Flag,
				&(PerIoData->OverLapped),
				NULL);
		}
		//发送完成，置空缓冲区，释放缓冲区
		if(PerIoData->OperType==SEND_POSTED)
		{
			memset(PerIoData,0,sizeof(PER_IO_OPERATION_DATA));
			GlobalFree(PerIoData);
			ByteTransferred=0;
		}
	}
	return 0L;
}
DWORD WINAPI ListenProc(LPVOID lParam)
{
	CIocpModeSvr* pSvr=(CIocpModeSvr*)lParam;
	SOCKET Accept;
	while(true)
	{
		//接收客户的请求
		Accept=WSAAccept(pSvr->ListenSocket,NULL,NULL,NULL,0);
		//申请新的句柄操作数据

		LPPER_HANDLE_DATA PerHandleData=(LPPER_HANDLE_DATA) \
			                            GlobalAlloc(GPTR,
										sizeof(PER_HANDLE_DATA)
										);
		//取得客户端信息
		sockaddr soad;
		sockaddr_in in;
		int len=sizeof(soad);
		if(getpeername(Accept,&soad,&len)==SOCKET_ERROR)
		{
			CString LogStr;
			LogStr.Format(L"getpeername() faild : %d",GetLastError());
			pSvr->WriteLogString(LogStr);
		}
		else{
			memcpy(&in,&soad,sizeof(sockaddr));
		}
		//句柄数据
		PerHandleData->sClient=Accept;
		PerHandleData->IpAddr=in.sin_addr.S_un.S_addr;
		//存储客户信息
		::EnterCriticalSection(&pSvr->cInfoSection);
		pSvr->ClientInfo.Add(*PerHandleData);
		::LeaveCriticalSection(&pSvr->cInfoSection);
		//转储信息
		CString LogStr;
		LogStr.Format(L"UserIP: %s ,Socket : %d Connected!",inet_ntoa(in.sin_addr),Accept);
		pSvr->WriteLogString(LogStr);

		TRACE(L"\nUserIP: %s ,Socket : %d Connected!",inet_ntoa(in.sin_addr),Accept);
		//关联客户端口到完成端口，句柄数据在此时被绑定到完成端口
		CreateIoCompletionPort((HANDLE)Accept,
			                   pSvr->CompletionPort,
							   (DWORD)PerHandleData,
							   0);
		//Io操作数据标志

		PPER_IO_OPERATION_DATA PerIoData=(PPER_IO_OPERATION_DATA) \
			                              GlobalAlloc(GPTR,
										  sizeof(PER_IO_OPERATION_DATA));
		unsigned long  Flag=0;
		DWORD RecvByte;
		ZeroMemory(&(PerIoData->OverLapped),sizeof(OVERLAPPED));

		PerIoData->RecvDataBuf.buf=PerIoData->RecvBuf;
		PerIoData->RecvDataBuf.len=BUFFER_SIZE;
		PerIoData->OperType=RECV_POSTED;
		//提交首个接收数据请求
		//这时
		//如果客户端断开连接
		//则也可以以接收数据时得到通知	
		WSARecv(PerHandleData->sClient,
			&(PerIoData->RecvDataBuf),
			1,
			&RecvByte,
			&Flag,
			&(PerIoData->OverLapped),
			NULL);
	}
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIocpModeSvr::CIocpModeSvr()
{
	IsStart=false;
}
CIocpModeSvr::~CIocpModeSvr()
{
}
bool CIocpModeSvr::SendMsg(LPCTSTR TargetIp,char * pData,unsigned long Length)
{
	if(TargetIp==L"" || pData==NULL || Length==0 || !IsStart)
		return false;

	bool bFind=false;
	PER_HANDLE_DATA pPerHandleData;
	memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));

	char szTargetIp[256];
	memset(szTargetIp, '\0', sizeof(szTargetIp));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,TargetIp,-1,szTargetIp,sizeof(szTargetIp),NULL,NULL);
	unsigned long tIp=inet_addr(szTargetIp);
	//在表中查找该客户端数据并发送数据
	
	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	while(arrSize<ClientInfo.GetSize())
	{
		pPerHandleData=ClientInfo.GetAt(arrSize);
		if((pPerHandleData.IpAddr==tIp))
		{
			bFind=true;
			break;
		}
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
	//找到该客户端，可以提交发送数据请求了
	if(bFind)
	{
		return SendMsg(pPerHandleData.sClient,pData,Length);
	}
	else
	{
		CString LogStr;
		pData[Length]='\0';
		LogStr.Format(L"试图发送数据到没有连接的客户端:%s数据:%s",TargetIp,pData);
		WriteLogString(LogStr);
		return false;
	}
	return false;
}
//提交发送消息请求，
//如果提交发送消息失败，
//则将导致在工作线程里将目标客户端的连接切断
bool CIocpModeSvr::SendMsg(SOCKET sClient,char * pData,unsigned long Length)
{
	if(sClient==INVALID_SOCKET || pData==NULL || Length==0 || !IsStart)return false;

	//申请操作键
	PPER_IO_OPERATION_DATA PerIoData=(PPER_IO_OPERATION_DATA) \
									  GlobalAlloc(GPTR,
		                              sizeof(PER_IO_OPERATION_DATA));

	//准备缓冲
	unsigned long  Flag=0;
	DWORD SendByte;
	ZeroMemory(&(PerIoData->OverLapped),sizeof(OVERLAPPED));
	memcpy(PerIoData->SendBuf,pData,Length);
	PerIoData->SendDataBuf.buf=PerIoData->SendBuf;
	PerIoData->SendDataBuf.len=Length;
	PerIoData->OperType=SEND_POSTED;
	int bRet=WSASend(sClient,
		             &(PerIoData->SendDataBuf),
		             1,
		             &SendByte,
		             Flag,
		             &(PerIoData->OverLapped),
		             NULL);
	if(bRet==SOCKET_ERROR && GetLastError()!=WSA_IO_PENDING)
	{
		CString LogStr;
		LogStr.Format(L"WSASend With Error : %d",GetLastError());
		WriteLogString(LogStr);
		return false;
	}
	else return true;
	
	return false;
}
bool CIocpModeSvr::SendMsgToAll(char * pData,unsigned long Length)
{
	if(pData==NULL || Length==0 || !IsStart)return false;

	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		SendMsg(pPerHandleData.sClient,pData,Length);
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
	return true;
}
bool CIocpModeSvr::SendMsgToOther(LPCTSTR ExceptIp,char *pData,unsigned long Length)
{
	if(ExceptIp==L"" || pData==NULL || Length==0 || !IsStart)return false;

	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		char szExceptIp[256];
		memset(szExceptIp, '\0', sizeof(szExceptIp));
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,ExceptIp,-1,szExceptIp,sizeof(szExceptIp),NULL,NULL);
		if(pPerHandleData.IpAddr!=inet_addr(szExceptIp))
			SendMsg(pPerHandleData.sClient,pData,Length);
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
    return true;
}
bool CIocpModeSvr::SendMsgToOther(SOCKET ExceptSocket,char * pData,unsigned long Length)
{
	if(ExceptSocket==INVALID_SOCKET || pData==NULL || Length==0 || !IsStart)return false;

	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		if(pPerHandleData.sClient!=ExceptSocket)
			SendMsg(pPerHandleData.sClient,pData,Length);
			arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
	return true;
}
bool CIocpModeSvr::Init(ProcessRecvData* pProcessRecvData,
						unsigned long iSvrPort)
{
	if(IsStart || pProcessRecvData==NULL)return false;

	m_SvrPort=iSvrPort;

	::InitializeCriticalSection(&cInfoSection);

	m_pProcessRecvData=pProcessRecvData;
	int  bRet=InitNetWork(iSvrPort,&HostIpAddr);
	if(bRet==0)
	{
		IsStart=true;
		return true;
	}
	else
		return false;
}
void CIocpModeSvr::DisConnectAll()
{
	if(!IsStart)return ;
	
	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		shutdown(pPerHandleData.sClient,1);
		closesocket(pPerHandleData.sClient);
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
}
bool CIocpModeSvr::DisConnectClient(LPCTSTR ClientIp)
{
	if(!IsStart || ClientIp==L"")return false;
	
	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		char szClientIp[256];
		memset(szClientIp, '\0', sizeof(szClientIp));
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,ClientIp,-1,szClientIp,sizeof(szClientIp),NULL,NULL);
		if(pPerHandleData.IpAddr==inet_addr(szClientIp))
		{
			shutdown(pPerHandleData.sClient,1);
			closesocket(pPerHandleData.sClient);
			::LeaveCriticalSection(&cInfoSection);
			return true;
		}
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
	return false;
}
bool CIocpModeSvr::DisConnectClient(SOCKET sClient)
{
	if(!IsStart || sClient==INVALID_SOCKET)return false;
	
	::EnterCriticalSection(&cInfoSection);
	int arrSize=0;
	PER_HANDLE_DATA pPerHandleData;
	while(arrSize<ClientInfo.GetSize())
	{
		memset(&pPerHandleData,0,sizeof(PER_HANDLE_DATA));
		pPerHandleData=ClientInfo.GetAt(arrSize);
		if(pPerHandleData.sClient==sClient)
		{
			shutdown(pPerHandleData.sClient,1);
			closesocket(pPerHandleData.sClient);
			::LeaveCriticalSection(&cInfoSection);
			return true;
		}
		arrSize++;
	}
	::LeaveCriticalSection(&cInfoSection);
	return false;
}
void CIocpModeSvr::UnInit()
{
	if(!IsStart)return;
	//退出工作线程
	SYSTEM_INFO sys_Info;
	GetSystemInfo(&sys_Info);
	for(int i=0;i<sys_Info.dwNumberOfProcessors*2+2;i++)
	{
		//寄出退出消息
		PostQueuedCompletionStatus(CompletionPort,
								   -1,
							       -1,
							       NULL);
	}
	//退出侦听线程
	::TerminateThread(ListenThreadHandle,1L);
	::WaitForSingleObject(ListenThreadHandle,10);
	CloseHandle(ListenThreadHandle);
	//关闭网络的侦听
	shutdown(ListenSocket,0);
	closesocket(ListenSocket);
	//切断当前所有连接
	DisConnectAll();	
	::DeleteCriticalSection(&cInfoSection);
	m_pProcessRecvData=NULL;
	IsStart=false;
}
int CIocpModeSvr::InitNetWork(unsigned int SvrPort,
							  std::string *pHostIpAddress)
{
	//启动网络
	CString LogStr;
	int Error=0;
	WSADATA wsaData;
	char Name[100];
	hostent *pHostEntry;
	in_addr rAddr;
	//Net Start Up
	Error=WSAStartup(MAKEWORD(0x02,0x02),&wsaData);
	if(Error!=0)
	{
		Error = WSAGetLastError();
		pHostIpAddress->assign( "" );
		
		LogStr.Format(L"WSAStartUp Faild With Error: %d",Error);
		WriteLogString(LogStr);
		
		return Error;
	}
	//Make Version
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup( );
		
		WriteLogString(L"The Local Net Version Is not 2");
		
		return -1;
	}
	//Get Host Ip
	Error = gethostname ( Name, sizeof(Name) );
	if( 0 == Error )
	{
		pHostEntry = gethostbyname( Name );
		if( pHostEntry != NULL )
		{
			memcpy( &rAddr, pHostEntry->h_addr_list[0], sizeof(struct in_addr) );
			pHostIpAddress->assign( inet_ntoa( rAddr ) );
		}
		else
		{
			Error = WSAGetLastError();
			LogStr.Format(L"GetHostIp faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;
			
		}
	}
	else
	{
		Error = WSAGetLastError();
		LogStr.Format(L"gethostname faild with Error: %d",Error);
		WriteLogString(LogStr);
		return Error;
	}
	if(0==Error)
	{
		//创建侦听端口
		ListenSocket=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
		if(ListenSocket==INVALID_SOCKET)
		{
			Error = WSAGetLastError();
			LogStr.Format(L"CreateSocket faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;
		}
	}
	//绑定到目标地址
	if(0==Error)
	{
		sockaddr_in InternetAddr;
		InternetAddr.sin_family=AF_INET;
		InternetAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
		InternetAddr.sin_port=htons(SvrPort);
		if(bind(ListenSocket,
			    (PSOCKADDR )&InternetAddr,
				sizeof(InternetAddr))==SOCKET_ERROR)
		{
			Error=GetLastError();
			LogStr.Format(L"bind Socket faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;
		}
	}
	//侦听端口上的连接请求
	if(0==Error)
	{
		if(	listen(ListenSocket,5)==SOCKET_ERROR)
		{
			Error=GetLastError();
			LogStr.Format(L"listen Socket faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;
		}
	}
	//创建完成端口句柄
	if(0==Error)
	{
		CompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
		if(CompletionPort==INVALID_HANDLE_VALUE)
		{
			Error=GetLastError();
			LogStr.Format(L"CreateIoCompletionPort faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;
		}
	}
	//启动工作线程，线程数为CPU处理器数量*2+2
	if(0==Error)
	{	
		SYSTEM_INFO sys_Info;
		GetSystemInfo(&sys_Info);
		for(int i=0;i<sys_Info.dwNumberOfProcessors*2+2;i++)
		{
			HANDLE ThreadHandle;
			DWORD ThreadID;
			
			ThreadHandle=CreateThread(NULL,
				0,
				ServerWorkerProc,
				this,
				0,
				&ThreadID);
			if(ThreadHandle==NULL)
			{
				Error = WSAGetLastError();
				LogStr.Format(L"Create Server Work Thread faild with Error: %d",Error);
				WriteLogString(LogStr);
				return Error;
			}	
			CloseHandle(ThreadHandle);
		}
	}
	//启动侦听线程
	if(0==Error)
	{
		DWORD thID;
		ListenThreadHandle=CreateThread(NULL,
			                            0,
										ListenProc,
										this,
										0,
										&thID);
		if(ListenThreadHandle==NULL)
		{
			Error = WSAGetLastError();
			LogStr.Format(L"Create Listen Thread faild with Error: %d",Error);
			WriteLogString(LogStr);
			return Error;		
		}
	}
	return Error;
}
void CIocpModeSvr::WriteLogString(LPCTSTR strLog)
{
	SYSTEMTIME sysTm;
	::GetLocalTime(&sysTm);
	ofstream       OutputFile;
	OutputFile.open( L"NetSvrLog.txt" ,ios::app);
	OutputFile<<sysTm.wYear<<L"年" \
		      <<sysTm.wMonth<<L"月" \
			  <<sysTm.wDay<<L"日" \
			  <<sysTm.wHour<<L"时" \
			  <<sysTm.wMinute<<L"分" \
			  <<sysTm.wSecond<<L"秒" \
			  <<L":" \
			  <<L"    " \
			  <<strLog \
			  <<L"\n" \
			  <<endl;
	OutputFile.close();
}