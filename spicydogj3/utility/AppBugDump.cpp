#include "stdafx.h"

#include "AppBugDump.h"

#include "utility/StringUtility.h"
#include "utility/FileUtility.h"

#pragma comment(lib,"imagehlp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MODULE_NAME_LEN = 64;
const int SYMBOL_NAME_LEN = 128;

struct SYMBOL_INFO
{
	DWORD dwAddress;
	DWORD dwOffset;
	TCHAR szModule[MODULE_NAME_LEN];
	TCHAR szSymbol[SYMBOL_NAME_LEN];
}; // struct SYMBOL_INFO

static LPVOID __stdcall FunctionTableAccess(HANDLE hProcess, DWORD dwPCAddress);
static DWORD __stdcall GetModuleBase(HANDLE hProcess, DWORD dwReturnAddress);
void StackWalkThrough(DWORD dwMachType,HANDLE hProcess,STACKFRAME& stackFrame,CONTEXT* pContext,CDWordArray&);
DWORD InitStackFrameStruct(STACKFRAME& stackFrame,const CONTEXT*);
static BOOL ResolveSymbol(HANDLE hProcess, DWORD dwAddress,SYMBOL_INFO &siSymbol);

struct EXCEPTION_TAG
{
	EXCEPTION_TAG(DWORD dw,LPCTSTR ptsz)
	:	m_dwCode(dw),
		m_ptszName(ptsz)
	{}
	const DWORD m_dwCode;
	const LPCTSTR m_ptszName;
}; // struct EXCEPTION_TAG

const EXCEPTION_TAG EXCEPTION_TAGS[] = 
{
#if 1
	EXCEPTION_TAG(-1,								_T("不明なエラー")),
	EXCEPTION_TAG(EXCEPTION_ACCESS_VIOLATION,		_T("一般保護エラー")),
	EXCEPTION_TAG(EXCEPTION_ARRAY_BOUNDS_EXCEEDED,	_T("配列範囲超過")),
	EXCEPTION_TAG(EXCEPTION_BREAKPOINT,				_T("ブレークポイント")),
	EXCEPTION_TAG(EXCEPTION_DATATYPE_MISALIGNMENT,	_T("データタイプミスアライメント")),
	EXCEPTION_TAG(EXCEPTION_FLT_DENORMAL_OPERAND,	_T("不正なオペランドエラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_DIVIDE_BY_ZERO,		_T("0除算エラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_INEXACT_RESULT,		_T("不正確なエラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_INVALID_OPERATION,	_T("不正なオペレーションエラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_OVERFLOW,			_T("オーバーフローエラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_STACK_CHECK,		_T("スタックチェックエラー")),
	EXCEPTION_TAG(EXCEPTION_FLT_UNDERFLOW,			_T("アンダーフローエラー")),
	EXCEPTION_TAG(EXCEPTION_ILLEGAL_INSTRUCTION,	_T("不正な命令")),
	EXCEPTION_TAG(EXCEPTION_IN_PAGE_ERROR,			_T("ページ内エラー")),
	EXCEPTION_TAG(EXCEPTION_INT_DIVIDE_BY_ZERO,		_T("INT ゼロ除算")),
	EXCEPTION_TAG(EXCEPTION_INT_OVERFLOW,			_T("INT オーバーフロー")),
	EXCEPTION_TAG(EXCEPTION_INVALID_DISPOSITION,	_T("不正なディスポジション")),
	EXCEPTION_TAG(EXCEPTION_NONCONTINUABLE_EXCEPTION,_T("継続不可能な例外が発生")),
	EXCEPTION_TAG(EXCEPTION_PRIV_INSTRUCTION,		_T("特権命令エラー")),
	EXCEPTION_TAG(EXCEPTION_SINGLE_STEP,			_T("シングルステップ")),
	EXCEPTION_TAG(EXCEPTION_STACK_OVERFLOW,			_T("スタックオーバーフロー"))
#else
	EXCEPTION_TAG(-1,								_T(UnknownError)),
	EXCEPTION_TAG(EXCEPTION_ACCESS_VIOLATION,		_T("AccessViolation")),
	EXCEPTION_TAG(EXCEPTION_ARRAY_BOUNDS_EXCEEDED,	_T("ArrayBoundExceeded")),
	EXCEPTION_TAG(EXCEPTION_BREAKPOINT,				_T("BreakPoint")),
	EXCEPTION_TAG(EXCEPTION_DATATYPE_MISALIGNMENT,	_T("DataTypeMisAlignment")),
	EXCEPTION_TAG(EXCEPTION_FLT_DENORMAL_OPERAND,	_T("Fault Denormal Operand")),
	EXCEPTION_TAG(EXCEPTION_FLT_DIVIDE_BY_ZERO,		_T("Fault Divied By Zero")),
	EXCEPTION_TAG(EXCEPTION_FLT_INEXACT_RESULT,		_T("Fault InexactResult")),
	EXCEPTION_TAG(EXCEPTION_FLT_INVALID_OPERATION,	_T("Fault Invalid Operation")),
	EXCEPTION_TAG(EXCEPTION_FLT_OVERFLOW,			_T("Fault Overflow")),
	EXCEPTION_TAG(EXCEPTION_FLT_STACK_CHECK,		_T("Fault Stack Check")),
	EXCEPTION_TAG(EXCEPTION_FLT_UNDERFLOW,			_T("Fault Underflow")),
	EXCEPTION_TAG(EXCEPTION_ILLEGAL_INSTRUCTION,	_T("Illegal Instruction")),
	EXCEPTION_TAG(EXCEPTION_IN_PAGE_ERROR,			_T("InPageError"),),
	EXCEPTION_TAG(EXCEPTION_INT_DIVIDE_BY_ZERO,		_T("IntDividedByZero"),),
	EXCEPTION_TAG(EXCEPTION_INT_OVERFLOW,			_T("IntOverflow"),),
	EXCEPTION_TAG(EXCEPTION_INVALID_DISPOSITION,	_T("InvalidDisposition"),	),
	EXCEPTION_TAG(EXCEPTION_NONCONTINUABLE_EXCEPTION,_T("NoncontinuableException")),
	EXCEPTION_TAG(EXCEPTION_PRIV_INSTRUCTION,		_T("Priv Instruction")),
	EXCEPTION_TAG(EXCEPTION_SINGLE_STEP,			_T("SingleStep")),
	EXCEPTION_TAG(EXCEPTION_STACK_OVERFLOW,			_T("StackOverflow"))
#endif
}; // const EXCEPTION_TAG EXCEPTION_TAGS[]

/**	例外コードから名前を取得する */
LPCTSTR GetExceptionCodeName(DWORD dw) {
	int nc = sizeof(EXCEPTION_TAGS) / sizeof(EXCEPTION_TAGS[0]);
	for (int n = 0;n < nc;n++) {
		const EXCEPTION_TAG& e = EXCEPTION_TAGS[n];
		if (e.m_dwCode == dw) {
			return e.m_ptszName;
		}
	}
	return NULL;
}

/**
	アプリケーションによって発生した構造化例外のダンプ報告
*/
const LPCTSTR CRLF = _T("\r\n");
const int STACK_DUMP_UINT = 8;
const int STACK_MAX_DUMP = 8;
const int FRAMEPTR_DUMP_UINT = 12;
const int FRAMEPTR_MAX_DUMP = 10;

LPCTSTR AppBugDump::GetSubject() const
{
	return m_strSubject;
}

LPCTSTR AppBugDump::GetBody() const
{
	return m_strBody;
}

AppBugDump::AppBugDump(PEXCEPTION_POINTERS p) 
{
	CWaitCursor wc;

	if (p != NULL)
	{
		DumpExceptionRecord(p->ExceptionRecord);
	}
	else
	{
		m_strSubject = _T("--------:NullExceptionPointers:[--------]");
		Dump(m_strSubject);
	}

	DumpAppVer();
	DumpDate();

	if (p != NULL)
	{
		DumpContext(p->ContextRecord);
	}

	DumpAppFiles();
}

void AppBugDump::DumpExceptionRecord(PEXCEPTION_RECORD per)
{
	DumpHeader(_T("ExceptionRecord"));
	if (per == NULL)
		DumpLn(_T("---"));
	else
	{
		CString str;
		str.Format(_T("%08X:%s:[%08X]"),
					per->ExceptionCode,
					GetExceptionCodeName(per->ExceptionCode),
					per->ExceptionAddress);

		m_strSubject += str;
		DumpLn(str);
	}
}

void AppBugDump::DumpAppVer()
{
	CString strAppPath = GetAppExePath();

	DumpHeader(_T("AppExeFile"));
	DumpLn(strAppPath);

	DumpHeader(_T("AppVersion"));
	DumpLn(GetFileVersion(strAppPath));
}

void AppBugDump::DumpDate()
{
    SYSTEMTIME  st;
    GetLocalTime(&st);
	DumpHeader(_T("RaiseDate"));
	DumpLn(FormatString(_T("%04u/%02u/%02u %02u:%02u:%02u.%03u"),
							st.wYear,
							st.wMonth,
							st.wDay,
							st.wHour,
							st.wMinute,
							st.wSecond,
							st.wMilliseconds));
}

void AppBugDump::DumpContext(CONTEXT* pcont) 
{
	if (pcont != NULL)
	{
		DumpRegister(pcont);
		DumpStackWalk(pcont);
		DumpStackDump(pcont);
		DumpFramePtr(pcont);
	}
	else
	{
		DumpHeader(_T("CONTEXT"));
		DumpLn(_T("---"));
	}
}

void AppBugDump::DumpRegister(CONTEXT* pcont)
{
	DumpHeader(_T("Register"));
	DumpLn(FormatString("eax=%08X ebx=%08X ecx=%08X",pcont->Eax, pcont->Ebx, pcont->Ecx));
	DumpLn(FormatString("edx=%08X esi=%08X edi=%08X",pcont->Edx, pcont->Esi, pcont->Edi));
	DumpLn(FormatString("ebp=%08X esp=%08X eip=%08X",pcont->Ebp, pcont->Esp, pcont->Eip));
}

void AppBugDump::DumpStackDump(CONTEXT* pcont)
{
	DumpHeader(_T("StackDump"));

	DWORD* esp = (DWORD*) pcont->Esp;

	for(int i = 0; i < STACK_MAX_DUMP; i++ ) 
	{
		if(IsBadReadPtr(esp,STACK_DUMP_UINT * sizeof(DWORD)))
			break;
		ASSERT(STACK_DUMP_UINT == 8);

		DumpLn(FormatString(_T("%08X %08X %08X %08X-%08X %08X %08X %08X"),
								  esp[0], esp[1], esp[2], esp[3], 
								  esp[4], esp[5], esp[6], esp[7]));
		esp += STACK_DUMP_UINT;
	}
}

void AppBugDump::DumpStackWalk(CONTEXT* pContext)
{
	DumpHeader(_T("StackWalk"));

	HANDLE hProcess = ::GetCurrentProcess();
	if (!SymInitialize(hProcess, NULL, FALSE))
		{ASSERT(FALSE);return;}
		
	{
		DWORD dw = SymGetOptions();
		dw &= ~SYMOPT_UNDNAME;
		SymSetOptions(dw);
	}
	
	STACKFRAME stackFrame;
	DWORD dwMachType = InitStackFrameStruct(stackFrame,pContext);
	
	CDWordArray adwAddress;
	adwAddress.SetSize(0, 16);

	StackWalkThrough(dwMachType,hProcess,stackFrame,pContext,adwAddress);
	DumpStackAddressed(hProcess,adwAddress);
}

void AppBugDump::DumpStackAddressed(HANDLE hProcess,CDWordArray& adwAddress)
{
	int cAddresses = adwAddress.GetSize();
	for (int nAddress = 0; nAddress < cAddresses; nAddress++)
	{
		SYMBOL_INFO info;
		DWORD dwAddress = adwAddress[nAddress];

		TCHAR sz[20];
		wsprintf(sz,_T("%8.8X: "), dwAddress);
		Dump(sz);

		if (ResolveSymbol(hProcess, dwAddress, info))
		{
			Dump(info.szModule);
			Dump(info.szSymbol);
		}
		else
			Dump(_T("<nosymbol>"));
		DumpEol();
	}
}

void AppBugDump::DumpFramePtr(CONTEXT* pcont)
{
	DumpHeader(_T("FramePtr"));

    DumpLn(_T("Param#1  Param#2  Param#3  Param#4  Param#5  Param#6  Param#7  Param#8  Param#9  Param#10"));
    int i = 0;
    DWORD*  ebp = (DWORD*)pcont->Ebp;
    while( i < FRAMEPTR_MAX_DUMP && 
		   !IsBadReadPtr( ebp, sizeof(DWORD) * (FRAMEPTR_DUMP_UINT) ) ) 
	{
		ASSERT(FRAMEPTR_MAX_DUMP == 10);
        DumpLn(FormatString("%08X %08X %08X %08X %08X "
								  "%08X %08X %08X %08X %08X ",
								   ebp[0], ebp[1], ebp[2], ebp[3], ebp[4], 
								   ebp[5], ebp[6], ebp[7], ebp[8], ebp[9]));
        i ++;
        ebp = (DWORD*)*ebp;
    }
}

void AppBugDump::Dump(LPCTSTR ptsz) 
{
	m_strBody += ptsz;
}

void AppBugDump::DumpLn(LPCTSTR ptsz) 
{
	Dump(ptsz);
	DumpEol();
}

void AppBugDump::DumpEol()
{
	Dump(CRLF);
}

void AppBugDump::DumpHeader(LPCTSTR ptsz) 
{
	if (!m_strBody.IsEmpty())
		DumpEol();
	Dump(_T("[["));
	Dump(ptsz);
	Dump(_T("]]"));
	DumpEol();
}

void AppBugDump::DumpAppFiles()
{
	DumpHeader(_T("AppRootFiles"));
	DumpAppFilesHelper(0,GetAppRootPath());
}

void AppBugDump::DumpAppFilesHelper(int n,LPCTSTR ptszPath)
{
	if (8 < n)
	{
		DumpLn(_T("---"));
		return;
	}

	CFileFind find;
	BOOL b = find.FindFile(CatPath(ptszPath,_T("*.*")));
	while (b)
	{
		b = find.FindNextFile();
		if (find.IsDots())
			continue;
		if (find.IsDirectory())
		{
			DumpLn(_T("+") + find.GetFileName());
			DumpAppFilesHelper(n+1,find.GetFilePath());
		}
		else
		{
			CTime time;
			VERIFY(find.GetLastWriteTime(time));

			CString str;
			str.Format("%s\t%d\t%d-%d-%d %d:%d:%d",
					   find.GetFileName(),
					   find.GetLength(),
					   time.GetYear(),
					   time.GetMonth(),
					   time.GetDay(),
					   time.GetHour(),
					   time.GetMinute(),
					   time.GetSecond());
			DumpLn(CString(_T(' '),n) + str);
		}
	}
}

static LPVOID __stdcall FunctionTableAccess(HANDLE hProcess, DWORD dwPCAddress)
{
	return SymFunctionTableAccess(hProcess, dwPCAddress);
}

static DWORD __stdcall GetModuleBase(HANDLE hProcess, DWORD dwReturnAddress)
{
	IMAGEHLP_MODULE moduleInfo;

	if (SymGetModuleInfo(hProcess, dwReturnAddress, &moduleInfo))
		return moduleInfo.BaseOfImage;
	else
	{
		MEMORY_BASIC_INFORMATION memoryBasicInfo;

		if (::VirtualQueryEx(hProcess, (LPVOID) dwReturnAddress,
			&memoryBasicInfo, sizeof(memoryBasicInfo)))
		{
			DWORD cch = 0;
			char szFile[MAX_PATH] = { 0 };

		 cch = GetModuleFileNameA((HINSTANCE)memoryBasicInfo.AllocationBase,
										 szFile, MAX_PATH);

		 // Ignore the return code since we can't do anything with it.
		 if (!SymLoadModule(hProcess,
			   NULL, ((cch) ? szFile : NULL),
			   NULL, (DWORD) memoryBasicInfo.AllocationBase, 0))
			{
				DWORD dwError = GetLastError();
				TRACE1("Error: %d\n", dwError);
			}
		 return (DWORD) memoryBasicInfo.AllocationBase;
	  }
		else
			TRACE1("Error is %d\n", GetLastError());
	}

	return 0;
}

static BOOL ResolveSymbol(HANDLE hProcess, DWORD dwAddress,
	SYMBOL_INFO &siSymbol)
{
	BOOL fRetval = TRUE;

	siSymbol.dwAddress = dwAddress;

	union {
		CHAR rgchSymbol[sizeof(IMAGEHLP_SYMBOL) + 255];
		IMAGEHLP_SYMBOL  sym;
	};

	CHAR szUndec[256];
	CHAR szWithOffset[256];
	LPSTR pszSymbol = NULL;
	IMAGEHLP_MODULE mi;

	memset(&siSymbol, 0, sizeof(SYMBOL_INFO));
	mi.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

	if (!SymGetModuleInfo(hProcess, dwAddress, &mi)) {
		lstrcpyA(siSymbol.szModule, "<no module>");
	} else {
		lstrcpyn(siSymbol.szModule,
			     mi.ModuleName,
				 sizeof siSymbol.szModule / sizeof siSymbol.szModule[0] );
		lstrcat(siSymbol.szModule, "! ");
	}

	__try
	{
		sym.SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		sym.Address = dwAddress;
		sym.MaxNameLength = 255;

		if (SymGetSymFromAddr(hProcess, dwAddress, &(siSymbol.dwOffset), &sym))
		{
			pszSymbol = sym.Name;

			if (UnDecorateSymbolName(sym.Name, szUndec,sizeof szUndec / sizeof szUndec[0],
				UNDNAME_NO_MS_KEYWORDS | UNDNAME_NO_ACCESS_SPECIFIERS))
			{
				pszSymbol = szUndec;
			}
			else if (SymUnDName(&sym, szUndec,sizeof szUndec / sizeof szUndec[0]))
			{
				pszSymbol = szUndec;
			}

			if (siSymbol.dwOffset != 0)
			{
				wsprintfA(szWithOffset, "%s + %d bytes", pszSymbol, siSymbol.dwOffset);
				pszSymbol = szWithOffset;
			}
	  }
	  else
		  pszSymbol = "<no symbol>";
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		pszSymbol = "<EX: no symbol>";
		siSymbol.dwOffset = dwAddress - mi.BaseOfImage;
	}

	lstrcpynA(siSymbol.szSymbol, pszSymbol,sizeof(siSymbol.szSymbol) / sizeof (siSymbol.szSymbol[0]));
	return fRetval;
}

	
DWORD InitStackFrameStruct(STACKFRAME& stackFrame,const CONTEXT* pContext)
{
	ZeroMemory(&stackFrame,sizeof(stackFrame));
	stackFrame.AddrPC.Mode = AddrModeFlat;

	DWORD dwMachType;
#if defined(_M_IX86)
	dwMachType = IMAGE_FILE_MACHINE_I386;
	// program counter, stack pointer, and frame pointer
	stackFrame.AddrPC.Offset    = pContext->Eip;
	stackFrame.AddrStack.Offset = pContext->Esp;
	stackFrame.AddrStack.Mode   = AddrModeFlat;
	stackFrame.AddrFrame.Offset = pContext->Ebp;
	stackFrame.AddrFrame.Mode   = AddrModeFlat;
#elif defined(_M_MRX000)
	// only program counter
	dwMachType                  = IMAGE_FILE_MACHINE_R4000;
	stackFrame.AddrPC. Offset    = treadContext.Fir;
#elif defined(_M_ALPHA)
	// only program counter
	dwMachType                  = IMAGE_FILE_MACHINE_ALPHA;
	stackFrame.AddrPC.Offset    = (unsigned long) pContext->Fir;
#elif defined(_M_PPC)
	// only program counter
	dwMachType                  = IMAGE_FILE_MACHINE_POWERPC;
	stackFrame.AddrPC.Offset    = pContext->Iar;
#elif
#error("Unknown Target Machine");
#endif
	return dwMachType;
}

void StackWalkThrough
(
	DWORD dwMachType,
	HANDLE hProcess,
	STACKFRAME& stackFrame,
	CONTEXT* pContext,
	CDWordArray& adwAddress
)
{
	for (int nFrame = 0; nFrame < 1024; nFrame++)
	{
		if (!StackWalk(dwMachType, hProcess, hProcess,
			&stackFrame,pContext, NULL,
			FunctionTableAccess, GetModuleBase, NULL))
		{
			break;
		}

		adwAddress.SetAtGrow(nFrame, stackFrame.AddrPC.Offset);
	}
}
