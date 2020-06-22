/*=============================================================================
//	Copyright (C) 2001 YAMAHA CORPORATION.
//
//	Title		:	masmw.h
//
//	Description	:	MA-3 SMW emulator DLL Loader.
//
//	History		:
//					Oct 18, 2001	1st try.
//============================================================================*/
#include "stdafx.h"
#include "masmw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// ç\íz/è¡ñ≈
//////////////////////////////////////////////////////////////////////

CMASMW::CMASMW()
{
	ClearAddress();

	m_hInstance = LoadLibrary(MASMWLIBNAME);
	if(m_hInstance != NULL) {
		m_pfnInitialize			= (MASMWINIT)				GetProcAddress(m_hInstance, MASMW_INITIALIZE);
		m_pfnCreate				= (MASMWCREATE)				GetProcAddress(m_hInstance, MASMW_CREATE);
		m_pfnLoad				= (MASMWLOAD)				GetProcAddress(m_hInstance, MASMW_LOAD);
		m_pfnOpen				= (MASMWOPEN)				GetProcAddress(m_hInstance, MASMW_OPEN);
		m_pfnStandby			= (MASMWSTANDBY)			GetProcAddress(m_hInstance, MASMW_STANDBY);
		m_pfnSeek				= (MASMWSEEK)				GetProcAddress(m_hInstance, MASMW_SEEK);
		m_pfnStart				= (MASMWSTART)				GetProcAddress(m_hInstance, MASMW_START);
		m_pfnStop				= (MASMWSTOP)				GetProcAddress(m_hInstance, MASMW_STOP);
		m_pfnPause				= (MASMWPAUSE)				GetProcAddress(m_hInstance, MASMW_PAUSE);
		m_pfnRestart			= (MASMWRESTART)			GetProcAddress(m_hInstance, MASMW_RESTART);
		m_pfnClose				= (MASMWCLOSE)				GetProcAddress(m_hInstance, MASMW_CLOSE);
		m_pfnUnload				= (MASMWUNLOAD)				GetProcAddress(m_hInstance, MASMW_UNLOAD);
		m_pfnDelete				= (MASMWDELETE)				GetProcAddress(m_hInstance, MASMW_DELETE);
		m_pfnControl			= (MASMWCTRL)				GetProcAddress(m_hInstance, MASMW_CONTROL);
		m_pfnTerminate			= (MASMWEND)				GetProcAddress(m_hInstance, MASMW_END);

		m_pfnPhrInit			= (MASMWPHRINIT)			GetProcAddress(m_hInstance, MASMW_PHRASE_INITIALIZE);
		m_pfnPhrGetInfo			= (MASMWPHRGETINFO)			GetProcAddress(m_hInstance, MASMW_PHRASE_GETINFO);
		m_pfnPhrChkData			= (MASMWPHRCHKDATA)			GetProcAddress(m_hInstance, MASMW_PHRASE_CHECKDATA);
		m_pfnPhrSetData			= (MASMWPHRSETDATA)			GetProcAddress(m_hInstance, MASMW_PHRASE_SETDATA);
		m_pfnPhrSeek			= (MASMWPHRSEEK)			GetProcAddress(m_hInstance, MASMW_PHRASE_SEEK);
		m_pfnPhrPlay			= (MASMWPHRPLAY)			GetProcAddress(m_hInstance, MASMW_PHRASE_PLAY);
		m_pfnPhrPause			= (MASMWPHRPAUSE)			GetProcAddress(m_hInstance, MASMW_PHRASE_PAUSE);
		m_pfnPhrRestart			= (MASMWPHRRESTART)			GetProcAddress(m_hInstance, MASMW_PHRASE_RESTART);
		m_pfnPhrStop			= (MASMWPHRSTOP)			GetProcAddress(m_hInstance, MASMW_PHRASE_STOP);
		m_pfnPhrRemoveData		= (MASMWPHRREMOVEDATA)		GetProcAddress(m_hInstance, MASMW_PHRASE_REMOVEDATA);
		m_pfnPhrKill			= (MASMWPHRKILL)			GetProcAddress(m_hInstance, MASMW_PHRASE_KILL);
		m_pfnPhrGetStatus		= (MASMWPHRGETSTATUS)		GetProcAddress(m_hInstance, MASMW_PHRASE_GETSTATUS);
		m_pfnPhrGetPos			= (MASMWPHRGETPOS)			GetProcAddress(m_hInstance, MASMW_PHRASE_GETPOSITION);
		m_pfnPhrGetVol			= (MASMWPHRGETVOL)			GetProcAddress(m_hInstance, MASMW_PHRASE_GETVOLUME);
		m_pfnPhrGetPan			= (MASMWPHRGETPAN)			GetProcAddress(m_hInstance, MASMW_PHRASE_GETPANPOT);
		m_pfnPhrGetLink			= (MASMWPHRGETLINK)			GetProcAddress(m_hInstance, MASMW_PHRASE_GETLINK);
		m_pfnPhrSetVol			= (MASMWPHRSETVOL)			GetProcAddress(m_hInstance, MASMW_PHRASE_SETVOLUME);
		m_pfnPhrSetPan			= (MASMWPHRSETPAN)			GetProcAddress(m_hInstance, MASMW_PHRASE_SETPANPOT);
		m_pfnPhrSetLink			= (MASMWPHRSETLINK)			GetProcAddress(m_hInstance, MASMW_PHRASE_SETLINK);
		m_pfnPhrSetEvHandler	= (MASMWPHRSETEVHANDLER)	GetProcAddress(m_hInstance, MASMW_PHRASE_SETEVHANDLER);
		m_pfnPhrTerm			= (MASMWPHRTERM)			GetProcAddress(m_hInstance, MASMW_PHRASE_TERMINATE);

		Initialize();
	} else {
		CString	Err;
		Err.Format(_T("Error : Can not load DLL(%s)"), MASMWLIBNAME);
		AfxMessageBox(Err, MB_OK);
	}
}

CMASMW::~CMASMW()
{
	Terminate();

	if(m_hInstance != NULL) {
		FreeLibrary(m_hInstance);
		m_hInstance = NULL;
		ClearAddress();
	}
}

void	CMASMW::ClearAddress(void)
{
	m_pfnInitialize			= NULL;
	m_pfnCreate				= NULL;
	m_pfnLoad				= NULL;
	m_pfnOpen				= NULL;
	m_pfnStandby			= NULL;
	m_pfnSeek				= NULL;
	m_pfnStart				= NULL;
	m_pfnStop				= NULL;
	m_pfnPause				= NULL;
	m_pfnRestart			= NULL;
	m_pfnClose				= NULL;
	m_pfnUnload				= NULL;
	m_pfnDelete				= NULL;
	m_pfnControl			= NULL;
	m_pfnTerminate			= NULL;

	m_pfnPhrInit			= NULL;
	m_pfnPhrGetInfo			= NULL;
	m_pfnPhrChkData			= NULL;
	m_pfnPhrSetData			= NULL;
	m_pfnPhrSeek			= NULL;
	m_pfnPhrPlay			= NULL;
	m_pfnPhrPause			= NULL;
	m_pfnPhrRestart			= NULL;
	m_pfnPhrStop			= NULL;
	m_pfnPhrRemoveData		= NULL;
	m_pfnPhrKill			= NULL;
	m_pfnPhrGetStatus		= NULL;
	m_pfnPhrGetPos			= NULL;
	m_pfnPhrGetVol			= NULL;
	m_pfnPhrGetPan			= NULL;
	m_pfnPhrGetLink			= NULL;
	m_pfnPhrSetVol			= NULL;
	m_pfnPhrSetPan			= NULL;
	m_pfnPhrSetLink			= NULL;
	m_pfnPhrSetEvHandler	= NULL;
	m_pfnPhrTerm			= NULL;
}

LONG	CMASMW::Initialize(void)
{
	LONG	lRet = -1;

	TRACE("Initialize ");

	if(m_pfnInitialize != NULL) {
		lRet = m_pfnInitialize();
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Terminate(void)
{
	LONG	lRet = -1;

	TRACE("Terminate ");

	if(m_pfnTerminate != NULL) {
		lRet = m_pfnTerminate();
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Create(BYTE srm_id)
{
	LONG	lRet = -1;

	TRACE("Create[%02X] ", srm_id);

	if(m_pfnCreate != NULL) {
		lRet = m_pfnCreate(srm_id);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Load(LONG func_id, PBYTE file_ptr, DWORD file_size, BYTE mode, LONG (*func)(BYTE id), void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Load[%08lX %08lX %08lX %02X] ", func_id, file_ptr, file_size, mode);

	if(m_pfnLoad != NULL) {
		lRet = m_pfnLoad(func_id, file_ptr, file_size, mode, func, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Open(LONG func_id, LONG file_id, WORD open_mode, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Open[%08lX %08lX %04X] ", func_id, file_id, open_mode);

	if(m_pfnOpen != NULL) {
		lRet = m_pfnOpen(func_id, file_id, open_mode, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Control(LONG func_id, LONG file_id, BYTE ctrl_num, void *prm, void *ext_args)
{
	LONG	lRet = -1;

//	TRACE("Control[%08lX %08lX %02X] ", func_id, file_id, ctrl_num);

	if(m_pfnControl != NULL) {
		lRet = m_pfnControl(func_id, file_id, ctrl_num, prm, ext_args);
	}

//	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Standby(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Standby[%08lX %08lX] ", func_id, file_id);

	if(m_pfnStandby != NULL) {
		lRet = m_pfnStandby(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Seek(LONG func_id, LONG file_id, DWORD pos, BYTE flag, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Seek[%08lX %08lX %08lX %02X] ", func_id, file_id, pos, flag);

	if(m_pfnSeek != NULL) {
		lRet = m_pfnSeek(func_id, file_id, pos, flag, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Start(LONG func_id, LONG file_id, WORD play_mode, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Start[%08lX %08lX %04X] ", func_id, file_id, play_mode);

	if(m_pfnStart != NULL) {
		lRet = m_pfnStart(func_id, file_id, play_mode, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Stop(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Stop[%08lX %08lX] ", func_id, file_id);

	if(m_pfnStop != NULL) {
		lRet = m_pfnStop(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);


	return (lRet);
}

LONG	CMASMW::Pause(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Pause[%08lX %08lX] ", func_id, file_id);

	if(m_pfnPause != NULL) {
		lRet = m_pfnPause(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);


	return (lRet);
}

LONG	CMASMW::Restart(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Restart[%08lX %08lX] ", func_id, file_id);

	if(m_pfnRestart != NULL) {
		lRet = m_pfnRestart(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Close(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("Close[%08lX %08lX] ", func_id, file_id);

	if(m_pfnClose != NULL) {
		lRet = m_pfnClose(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::UnLoad(LONG func_id, LONG file_id, void *ext_args)
{
	LONG	lRet = -1;

	TRACE("UnLoad[%08lX %08lX] ", func_id, file_id);

	if(m_pfnUnload != NULL) {
		lRet = m_pfnUnload(func_id, file_id, ext_args);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

LONG	CMASMW::Delete(LONG func_id)
{
	LONG	lRet = -1;

	TRACE("Delete[%08lX] ", func_id);

	if(m_pfnDelete != NULL) {
		lRet = m_pfnDelete(func_id);
	}

	TRACE("/End[%08lX] \n", lRet);

	return (lRet);
}

int	CMASMW::Phrase_Initialize(void)
{
	int iRet = -1;

	if(m_pfnPhrInit != NULL) {
		iRet = m_pfnPhrInit();
	}

	return (iRet);
}

int	CMASMW::Phrase_Terminate(void)
{
	int iRet = -1;

	if(m_pfnPhrTerm != NULL) {
		iRet = m_pfnPhrTerm();
	}

	return (iRet);
}

int	CMASMW::Phrase_GetInfo(struct info* dat)
{
	int iRet = -1;

	if(m_pfnPhrGetInfo != NULL) {
		iRet = m_pfnPhrGetInfo(dat);
	}

	return (iRet);
}

int	CMASMW::Phrase_CheckData(PBYTE data, DWORD len)
{
	int iRet = -1;

	if(m_pfnPhrChkData != NULL) {
		iRet = m_pfnPhrChkData(data, len);
	}

	return (iRet);
}

int	CMASMW::Phrase_SetData(int ch, PBYTE data, DWORD len, int check)
{
	int		iRet = -1;

	if(m_pfnPhrSetData != NULL) {
		iRet = m_pfnPhrSetData(ch, data, len, check);
	}

	return (iRet);
}

int	CMASMW::Phrase_Seek(int ch, LONG pos)
{
	int iRet = -1;

	TRACE("Phrase_Seek[ch:%d pos:%ld]", ch, pos);

	if(m_pfnPhrSeek != NULL) {
		iRet = m_pfnPhrSeek(ch, pos);
	}

	TRACE("/End[%d] \n", iRet);

	return (iRet);
}

int	CMASMW::Phrase_Play(int ch, int loop)
{
	int iRet = -1;

	if(m_pfnPhrPlay != NULL) {
		iRet = m_pfnPhrPlay(ch, loop);
	}

	return (iRet);
}

int	CMASMW::Phrase_Stop(int ch)
{
	int iRet = -1;

	if(m_pfnPhrStop != NULL) {
		iRet = m_pfnPhrStop(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_Pause(int ch)
{
	int iRet = -1;

	if(m_pfnPhrPause != NULL) {
		iRet = m_pfnPhrPause(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_Restart(int ch)
{
	int iRet = -1;

	if(m_pfnPhrRestart != NULL) {
		iRet = m_pfnPhrRestart(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_Kill(void)
{
	int iRet = -1;

	if(m_pfnPhrKill != NULL) {
		iRet = m_pfnPhrKill();
	}

	return (iRet);
}

int	CMASMW::Phrase_SetVolume(int ch, int vol)
{
	int iRet = -1;

	if(m_pfnPhrSetVol != NULL) {
		iRet = m_pfnPhrSetVol(ch, vol);
	}

	return (iRet);
}

int	CMASMW::Phrase_GetVolume(int ch)
{
	int iRet = -1;

	if(m_pfnPhrGetVol != NULL) {
		iRet = m_pfnPhrGetVol(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_SetPanpot(int ch, int vol)
{
	int iRet = -1;

	if(m_pfnPhrSetPan != NULL) {
		iRet = m_pfnPhrSetPan(ch, vol);
	}

	return (iRet);
}

int	CMASMW::Phrase_GetPanpot(int ch)
{
	int iRet = -1;

	if(m_pfnPhrGetPan != NULL) {
		iRet = m_pfnPhrGetPan(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_GetStatus(int ch)
{
	int iRet = 0;

	if(m_pfnPhrGetStatus != NULL) {
		iRet = m_pfnPhrGetStatus(ch);
	}

	return (iRet);
}

LONG	CMASMW::Phrase_GetPosition(int ch)
{
	LONG lRet = -1;

	if(m_pfnPhrGetPos != NULL) {
		lRet = m_pfnPhrGetPos(ch);
	}

	return (lRet);
}

int	CMASMW::Phrase_RemoveData(int ch)
{
	int iRet = -1;

	if(m_pfnPhrRemoveData != NULL) {
		iRet = m_pfnPhrRemoveData(ch);
	}

	return (iRet);
}

int	CMASMW::Phrase_SetEvHandler(void (* func)(struct event*))
{
	int iRet = -1;

	if(m_pfnPhrSetEvHandler != NULL) {
		iRet = m_pfnPhrSetEvHandler(func);
	}

	return (iRet);
}

int	CMASMW::Phrase_SetLink(int ch, DWORD slave)
{
	int iRet = -1;

	if(m_pfnPhrSetLink != NULL) {
		iRet = m_pfnPhrSetLink(ch, slave);
	}

	return (iRet);
}

DWORD	CMASMW::Phrase_GetLink(int ch)
{
	DWORD lRet = 0;

	if(m_pfnPhrGetLink != NULL) {
		lRet = m_pfnPhrGetLink(ch);
	}

	return (lRet);
}

CMASMW	gMaSmwCtrl;
