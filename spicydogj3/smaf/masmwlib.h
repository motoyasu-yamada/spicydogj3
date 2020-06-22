/*=============================================================================
//	Copyright (C) 2001 YAMAHA CORPORATION.
//
//	Title		:	masmwlib.h
//
//	Description	:	header of MA-3 Sounde MiddleWare Emulate Library.
//
//	History		:
//					Oct 12, 2001	1st try.
//					Nov 08, 2001	Add Phrase Wrapper Interfac.
//============================================================================*/
#define	MASMWLIB					__declspec(dllexport)
#define	MASMWLIBNAME				_T("ma3smwemu.dll")

#define	MASMW_DEVICECONTROL			_T("MaSound_DeviceControl")
#define	MASMW_INITIALIZE			_T("MaSound_Initialize")
#define	MASMW_CREATE				_T("MaSound_Create")
#define	MASMW_LOAD					_T("MaSound_Load")
#define	MASMW_OPEN					_T("MaSound_Open")
#define	MASMW_CONTROL				_T("MaSound_Control")
#define	MASMW_STANDBY				_T("MaSound_Standby")
#define	MASMW_SEEK					_T("MaSound_Seek")
#define	MASMW_START					_T("MaSound_Start")
#define	MASMW_STOP					_T("MaSound_Stop")
#define	MASMW_PAUSE					_T("MaSound_Pause")
#define MASMW_RESTART				_T("MaSound_Restart")
#define	MASMW_CLOSE					_T("MaSound_Close")
#define	MASMW_UNLOAD				_T("MaSound_Unload")
#define	MASMW_DELETE				_T("MaSound_Delete")
#define	MASMW_END					_T("MaSound_End")

#define	MASMW_PHRASE_INITIALIZE		_T("Phrase_Initialize")
#define	MASMW_PHRASE_GETINFO		_T("Phrase_GetInfo")
#define	MASMW_PHRASE_CHECKDATA		_T("Phrase_CheckData")
#define	MASMW_PHRASE_SETDATA		_T("Phrase_SetData")
#define	MASMW_PHRASE_SEEK			_T("Phrase_Seek")
#define	MASMW_PHRASE_PLAY			_T("Phrase_Play")
#define	MASMW_PHRASE_RESTART		_T("Phrase_Restart")
#define	MASMW_PHRASE_PAUSE			_T("Phrase_Pause")
#define	MASMW_PHRASE_STOP			_T("Phrase_Stop")
#define	MASMW_PHRASE_REMOVEDATA		_T("Phrase_RemoveData")
#define	MASMW_PHRASE_KILL			_T("Phrase_Kill")
#define	MASMW_PHRASE_GETSTATUS		_T("Phrase_GetStatus")
#define	MASMW_PHRASE_GETPOSITION	_T("Phrase_GetPosition")
#define	MASMW_PHRASE_GETVOLUME		_T("Phrase_GetVolume")
#define	MASMW_PHRASE_GETPANPOT		_T("Phrase_GetPanpot")
#define	MASMW_PHRASE_GETLINK		_T("Phrase_GetLink")
#define	MASMW_PHRASE_SETVOLUME		_T("Phrase_SetVolume")
#define	MASMW_PHRASE_SETPANPOT		_T("Phrase_SetPanpot")
#define	MASMW_PHRASE_SETLINK		_T("Phrase_SetLink")
#define	MASMW_PHRASE_SETEVHANDLER	_T("Phrase_SetEvHandler")
#define	MASMW_PHRASE_TERMINATE		_T("Phrase_Terminate")


/*	MASMW Library Error Code	*/
#define MASMW_SUCCESS					(0)		/* 成功									*/
#define MASMW_ERROR						(-1)	/* エラー								*/
#define MASMW_ERROR_ARGUMENT			(-2)	/* 引数が不正							*/
#define MASMW_ERROR_RESOURCE_OVER		(-3)	/* リソース数を越えている				*/
#define MASMW_ERROR_ID					(-4)	/* ID が不正							*/

#define MASMW_ERROR_FILE				(-16)	/* ファイル内容が不正					*/
#define MASMW_ERROR_CONTENTS_CLASS		(-17)	/* SMAF Contents Class が再生不可の値	*/
#define MASMW_ERROR_CONTENTS_TYPE		(-18)	/* SMAF Contents Type が再生不可の値	*/
#define MASMW_ERROR_CHUNK_SIZE			(-19)	/* SMAF Chunk Size が不正な値			*/
#define MASMW_ERROR_CHUNK				(-20)	/* SMAF Trachk Chunk が不正値			*/
#define MASMW_ERROR_UNMATCHED_TAG		(-21)	/* 指定されたタグ情報がなかった			*/
#define MASMW_ERROR_SHORT_LENGTH		(-22)	/* シーケンスデータ長が短い				*/

/* MaSound_Create : function ID */
#define	MASMW_CNVID_HWF					(0)		/* Hardware Format */
#define MASMW_CNVID_MMF					(1)		/* SMAF/MA-1/MA-2/MA-3 */
#define MASMW_CNVID_PHR					(2)		/* SMAF/Phrase L1 */
#define	MASMW_CNVID_RMD					(3)		/* Realtime MIDI */
#define	MASMW_CNVID_AUD					(4)		/* Audio */
#define	MASMW_CNVID_MID					(5)		/* SMF format 0/GM Lite or Level 1 */
#define	MASMW_CNVID_MFI					(6)		/* MFi/1/2/3 */
#define	MASMW_CNVID_MLP					(7)		/* MFi/Phrase */

/* MaSound_Control : control number */
#define MASMW_SET_VOLUME				(0)		/* set volume */
#define MASMW_SET_SPEED					(1)		/* set speed */
#define MASMW_SET_KEYCONTROL			(2)		/* set key control */
#define MASMW_GET_TIMEERROR				(3)		/* get time error */
#define MASMW_GET_POSITION				(4)		/* get position */
#define MASMW_GET_LENGTH				(5)		/* get length */
#define MASMW_GET_STATE					(6)		/* get state */
#define MASMW_SEND_MIDIMSG				(7)		/* send midi message */
#define MASMW_SEND_SYSEXMIDIMSG			(8)		/* send sys.ex. midi message */
#define MASMW_SET_BIND					(9)		/* set bind */
#define MASMW_GET_CONTENTSDATA			(10)	/* get contents data */
#define MASMW_GET_PHRASELIST			(11)	/* get phrase list */
#define MASMW_SET_STARTPOINT			(12)	/* set start point */
#define MASMW_SET_ENDPOINT				(13)	/* set end point */
#define MASMW_SET_PANPOT				(14)	/* set panpot */
#define MASMW_GET_LEDSTATUS				(15)	/* get LED status */
#define MASMW_GET_VIBSTATUS				(16)	/* get VIB status */
#define MASMW_GET_EVENT					(17)	/* get event */
#define MASMW_GET_LOADINFO				(19)	/* get load information */
#define MASMW_SET_LOADINFO				(20)	/* set load information */


typedef struct _MASMW_MIDIMSG
{
	unsigned char*	msg;
	unsigned long	size;
} MASMW_MIDIMSG, *PMASMW_MIDIMSG;

typedef struct _MASMW_CONTENTSINFO
{
	unsigned short	code;
	unsigned char	tag[2];
	unsigned char*	buf;
	unsigned long	size;
} MASMW_CONTENTSINFO, *PMASMW_CONTENTSINFO;

typedef struct _MASMW_PHRASELIST
{
	unsigned char	tag[2];
	unsigned long	start;
	unsigned long	stop;
} MASMW_PHRASELIST, *PMASMW_PHRASELIST;


struct	event {
	int	ch;
	int	mode;
};

struct info {
	long	MakerID;
	int		DeviceID;
	int		VersionID;
	int		MaxVoice;
	int		MaxChannel;
	int		SupportSMAF;
	long	Latency;
};

/*==============================================================================
//	signed long	MaSound_DeviceControl
//	(
//		unsigned char	cmd,		: command number
//		unsigned char	param1,		: parameter 1
//		unsigned char	param2,		: parameter 2
//		unsigned char	param3		: parameter 3
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWDEVCTRL)(unsigned char, unsigned char, unsigned char, unsigned char);

/*==============================================================================
//	signed long	MaSound_Initialize
//	(
//		void
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWINIT)(void);

/*==============================================================================
//	signed long	MaSound_Create
//	(
//		unsinged char	cnv_id	: stream converter id
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWCREATE)(unsigned char);

/*==============================================================================
//	signed long	MaSound_Load
//	(
//		signed long		func_id,	: function id
//		unsigned char	*file_ptr,	: pointer to file
//		unsigned long	file_size,	: size of file
//		unsigned char	mode,		: error check mode
//		signed long (*func)(unsigned char id),
//									: callback function
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWLOAD)(signed long, unsigned char*, unsigned long, unsigned char, signed long (*func)(unsigned char), void*);

/*==============================================================================
//	signed long	MaSound_Open
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		unsigned int	open_mode,	: open mode
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWOPEN)(signed long, signed long, unsigned int, void*);

/*==============================================================================
//	signed long	MaSound_Control
//	(
//		signed long		func_id,	: function id
//		signed long		file_id		: file id
//		unsigned char	ctrl_num,	: control number
//		void			*prm,		: parameter
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWCTRL)(signed long, signed long, unsigned char, void*, void*);

/*==============================================================================
//	signed long	MaSound_Standby
//	(
//		signed long		func_id,	: function id
//		signed long		file_id		: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWSTANDBY)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Seek
//	(
//		signed long		func_id,	: function id
//		signed long		file_id		: file id
//		unsigned long	pos,		: start position (msec)
//		unsigned char	flag,		: flag
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWSEEK)(signed long, signed long, unsigned long, unsigned char, void*);

/*==============================================================================
//	signed long	MaSound_Start
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		unsigned int	play_mode,	: playing mode
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWSTART)(signed long, signed long, unsigned int, void*);

/*==============================================================================
//	signed long	MaSound_Pause
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWPAUSE)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Restart
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWRESTART)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Stop
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWSTOP)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Close
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWCLOSE)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Unload
//	(
//		signed long		func_id,	: function id
//		signed long		file_id,	: file id
//		void			*ext_args	: for future extension
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWUNLOAD)(signed long, signed long, void*);

/*==============================================================================
//	signed long	MaSound_Delete
//	(
//		singed long	func_id			:	function id
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWDELETE)(signed long);

/*==============================================================================
//	signed long	MaSound_End
//	(
//		void
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef signed long (*MASMWEND)(void);

/*==============================================================================
//	int	Phrase_Initialize
//	(
//		void
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRINIT)(void);

/*==============================================================================
//	int	Phrase_GetInfo
//	(
//		struct info*	dat		: pointer to struct info
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRGETINFO)(struct info* dat);

/*==============================================================================
//	int	Phrase_CheckData
//	(
//		unsigned char*	data,	: pointer to file
//		unsigned long	len		: size of file
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRCHKDATA)(unsigned char* data, unsigned long len);

/*==============================================================================
//	int	Phrase_SetData
//	(
//		int				ch,		: channel (0..3)
//		unsigned char*	data,	: pointer to file
//		unsigned long	len,	: size of file
//		int				check	: error check flag
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRSETDATA)(int ch, unsigned char* data, unsigned long len, int check);

/*==============================================================================
//	int	Phrase_Seek
//	(
//		int				ch,		: channel (0..3)
//		signed long		pos		: start position (msec)
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRSEEK)(int ch, signed long pos);

/*==============================================================================
//	int	Phrase_Play
//	(
//		int				ch		: channel (0..3)
//		int				loop	: playing mode
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRPLAY)(int ch, int loop);

/*==============================================================================
//	int	Phrase_Pause
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRPAUSE)(int ch);

/*==============================================================================
//	int	Phrase_Restart
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRRESTART)(int ch);

/*==============================================================================
//	int	Phrase_Stop
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRSTOP)(int ch);

/*==============================================================================
//	int	Phrase_RemoveData
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRREMOVEDATA)(int ch);

/*==============================================================================
//	int	Phrase_Kill
//	(
//		void
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRKILL)(void);

/*==============================================================================
//	int	Phrase_GetStatus
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	status
//
==============================================================================*/
typedef int (*MASMWPHRGETSTATUS)(int ch);

/*==============================================================================
//	signed long	Phrase_GetPosition
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	position in milli second
//
==============================================================================*/
typedef signed long (*MASMWPHRGETPOS)(int ch);

/*==============================================================================
//	int	Phrase_GetVolume
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	volume
//
==============================================================================*/
typedef int (*MASMWPHRGETVOL)(int ch);

/*==============================================================================
//	int	Phrase_GetPanpot
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	panpot
//
==============================================================================*/
typedef int (*MASMWPHRGETPAN)(int ch);

/*==============================================================================
//	unsigned long	Phrase_GetLink
//	(
//		int				ch		: channel (0..3)
//	)
//
//	return	map of link
//
==============================================================================*/
typedef unsigned long (*MASMWPHRGETLINK)(int ch);

/*==============================================================================
//	int	Phrase_SetVolume
//	(
//		int				ch		: channel (0..3)
//		int				vol		: volume  (-1..127)
//											-1 means set default volume.
//	)
//
//	return	volume
//
==============================================================================*/
typedef int (*MASMWPHRSETVOL)(int ch, int vol);

/*==============================================================================
//	int	Phrase_SetPanpot
//	(
//		int				ch		: channel (0..3)
//		int				pan		: panpot  (-1..127)
//											-1 means set default panpot.
//	)
//
//	return	panpot
//
==============================================================================*/
typedef int (*MASMWPHRSETPAN)(int ch, int pan);

/*==============================================================================
//	int	Phrase_SetLink
//	(
//		int				ch		: channel (0..3)
//		unsigned long	slave	: map of link
//											bit0  ... ch#1
//											bit31 ... ch#32
//	)
//
//	return	0 means no error!
//
==============================================================================*/
typedef int (*MASMWPHRSETLINK)(int ch, unsigned long slave);

/*==============================================================================
//	int	Phrase_SetEvHandler
//	(
//		void (*func)(sturct event* eve) : pointer to handler.
//	)
//
//	return	0 means no error!
//
==============================================================================*/
typedef int (*MASMWPHRSETEVHANDLER)(void (*func)(struct event* eve));

/*==============================================================================
//	int	Phrase_Terminate
//	(
//		void
//	)
//
//	return	0 means no error!!
//
==============================================================================*/
typedef int (*MASMWPHRTERM)(void);
