/*=============================================================================
//	Copyright (C) 2001 YAMAHA CORPORATION.
//
//	Title		:	masmw.h
//
//	Description	:	header of MA-3 Sound API
//
//	History		:
//					Oct 18, 2001	1st try.
//============================================================================*/
#if !defined(AFX_MASMW_H__5CFA9CF1_1AF3_4F24_9884_E5D9E906F221__INCLUDED_)
#define AFX_MASMW_H__5CFA9CF1_1AF3_4F24_9884_E5D9E906F221__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "masmwlib.h"

class CMASMW
{
public:
	CMASMW();
	virtual ~CMASMW();

private:
	HINSTANCE				m_hInstance;			/*	DLLインスタンス		*/
	MASMWINIT				m_pfnInitialize;		/*	MaSound_Initialize	*/
	MASMWCREATE				m_pfnCreate;			/*	MaSound_Create		*/
	MASMWLOAD				m_pfnLoad;				/*	MaSound_Load		*/
	MASMWOPEN				m_pfnOpen;				/*	MaSound_Open		*/
	MASMWSTANDBY			m_pfnStandby;			/*	MaSound_Standby		*/
	MASMWSEEK				m_pfnSeek;				/*	MaSound_Seek		*/
	MASMWSTART				m_pfnStart;				/*	MaSound_Start		*/
	MASMWSTOP				m_pfnStop;				/*	MaSound_Stop		*/
	MASMWPAUSE				m_pfnPause;				/*	MaSound_Pause		*/
	MASMWRESTART			m_pfnRestart;			/*	MaSound_Restart		*/
	MASMWCLOSE				m_pfnClose;				/*	MaSound_Close		*/
	MASMWUNLOAD				m_pfnUnload;			/*	MaSound_Unload		*/
	MASMWDELETE				m_pfnDelete;			/*	MaSound_Delete		*/
	MASMWCTRL				m_pfnControl;			/*	MaSound_Control		*/
	MASMWEND				m_pfnTerminate;			/*	MaSound_End			*/

	MASMWPHRINIT			m_pfnPhrInit;			/*	Phrase_Initialize	*/
	MASMWPHRGETINFO			m_pfnPhrGetInfo;		/*	Phrase_GetInfo		*/
	MASMWPHRCHKDATA			m_pfnPhrChkData;		/*	Phrase_CheckData	*/
	MASMWPHRSETDATA			m_pfnPhrSetData;		/*	Phrase_SetData		*/
	MASMWPHRSEEK			m_pfnPhrSeek;			/*	Phrase_Seek			*/
	MASMWPHRPLAY			m_pfnPhrPlay;			/*	Phrase_Play			*/
	MASMWPHRPAUSE			m_pfnPhrPause;			/*	Phrase_Restart		*/
	MASMWPHRRESTART			m_pfnPhrRestart;		/*	Phrase_Pause		*/
	MASMWPHRSTOP			m_pfnPhrStop;			/*	Phrase_Stop			*/
	MASMWPHRREMOVEDATA		m_pfnPhrRemoveData;		/*	Phrase_RemoveData	*/
	MASMWPHRKILL			m_pfnPhrKill;			/*	Phasee_Kill			*/
	MASMWPHRGETSTATUS		m_pfnPhrGetStatus;		/*	Phrase_GetStatus	*/
	MASMWPHRGETPOS			m_pfnPhrGetPos;			/*	Phrase_GetPosition	*/
	MASMWPHRGETVOL			m_pfnPhrGetVol;			/*	Phrase_GetVolume	*/
	MASMWPHRGETPAN			m_pfnPhrGetPan;			/*	Phrase_GetPanpot	*/
	MASMWPHRGETLINK			m_pfnPhrGetLink;		/*	Phrase_GetLink		*/
	MASMWPHRSETVOL			m_pfnPhrSetVol;			/*	Phrase_SetVolume	*/
	MASMWPHRSETPAN			m_pfnPhrSetPan;			/*	Phrase_SetPanpot	*/
	MASMWPHRSETLINK			m_pfnPhrSetLink;		/*	Phrase_SetLink		*/
	MASMWPHRSETEVHANDLER	m_pfnPhrSetEvHandler;	/*	Phrase_SetEvHandler	*/
	MASMWPHRTERM			m_pfnPhrTerm;			/*	Phrase_Terminate	*/

private:
	void	ClearAddress();

public:
	LONG	Initialize			(void);
	LONG	Terminate			(void);
	LONG	Create				(BYTE srm_id);
	LONG	Load				(LONG func_id, PBYTE file_ptr, DWORD file_size, BYTE mode, LONG (*func)(BYTE id), void *ext_args);
	LONG	Open				(LONG func_id, LONG file_id, WORD open_mode, void *ext_args);
	LONG	Control				(LONG func_id, LONG file_id, BYTE ctrl_num, void *prm, void *ext_args);
	LONG	Standby				(LONG func_id, LONG file_id, void *ext_args);
	LONG	Seek				(LONG func_id, LONG file_id, DWORD pos, BYTE flag, void *ext_args);
	LONG	Start				(LONG func_id, LONG file_id, WORD play_mode, void *ext_args);
	LONG	Stop				(LONG func_id, LONG file_id, void *ext_args);
	LONG	Pause				(LONG func_id, LONG file_id, void *ext_args);
	LONG	Restart				(LONG func_id, LONG file_id, void *ext_args);
	LONG	Close				(LONG func_id, LONG file_id, void *ext_args);
	LONG	UnLoad				(LONG func_id, LONG file_id, void *ext_args);
	LONG	Delete				(LONG func_id);
	
	int		Phrase_Initialize	(void);
	int		Phrase_Terminate	(void);
	int		Phrase_GetInfo		(struct info* dat);
	int		Phrase_CheckData	(PBYTE data, DWORD len);
	int		Phrase_SetData		(int ch, PBYTE data, DWORD len, int check);
	int		Phrase_Seek			(int ch, LONG pos);
	int		Phrase_Play			(int ch, int loop);
	int		Phrase_Stop			(int ch);
	int		Phrase_Pause		(int ch);
	int		Phrase_Restart		(int ch);
	int		Phrase_Kill			(void);
	int		Phrase_SetVolume	(int ch, int vol);
	int		Phrase_GetVolume	(int ch);
	int		Phrase_SetPanpot	(int ch, int vol);
	int		Phrase_GetPanpot	(int ch);
	int		Phrase_GetStatus	(int ch);
	LONG	Phrase_GetPosition	(int ch);
	int		Phrase_RemoveData	(int ch);
	int		Phrase_SetEvHandler	(void (* func)(struct event*));
	int		Phrase_SetLink		(int ch, unsigned long slave);
	DWORD	Phrase_GetLink		(int ch);
};

extern CMASMW	gMaSmwCtrl;

#endif // !defined(AFX_MASMW_H__5CFA9CF1_1AF3_4F24_9884_E5D9E906F221__INCLUDED_)
