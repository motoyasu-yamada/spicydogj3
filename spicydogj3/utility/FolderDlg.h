#pragma once

#include <shlobj.h>

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE	0x0040
#endif


class FolderDlg
{
private:	
	CString m_strTitle;		// キャプション
	CString m_strRoot;		// ルートフォルダ
	CString m_strSelected;	// 選択されたフォルダ
	
	BROWSEINFO	m_bi;
	HWND m_hwndMe;			// 自分自身
	HWND m_hwndParent;		// 親ウインドウ
	BOOL m_bDoneModal;		// DoModalが呼び出されたかどうか

//	インターフェース
public:
	FolderDlg(HWND hParent,LPCTSTR ptszTitle,LPCTSTR ptszRoot = NULL,UINT uiFlag = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE);

	// ユーザにフォルダを選択させる
	virtual int DoModal();
	//	ユーザが選択したフォルダーを取得する
	CString GetFolderPath() const;
	// 選択されたフォルダに関連図けられているシステムイメージリストへのインデックス
	int GetFolderImage() const;
	// ダイアログスタイルの設定/取得
	//          BIF_BROWSEFORCOMPUTER    コンピュータのみ選択可能
	//          BIF_BROWSEFORPRINTER     プリンターのみ選択可能
	//          BIF_DONTGOBELOWDOMAIN    ネットワークフォルダは選択不可能
	//          BIF_RETURNFSANCESTORS    ファイルシステムアンセスターのみ選択可能
	//          BIF_RETURNONLYFSDIRS     ファイルシステムディレクトリのみ選択可能
	//          BIF_STATUSTEXT           ダイアログボックスにステータスエリアを含む
	UINT GetStyle() const;
	void SetStyle(UINT ulFlags);


// 継承クラスからの操作
protected:
	void EnableOK		(BOOL bEnable);
	void SetSelection	(LPCITEMIDLIST pidl);
	void SetSelection	(LPCTSTR);
	void SetStatusText	(LPCTSTR);

//	インプリメント
public:
	virtual ~FolderDlg();
protected:
	// BROWSEFOLDERのメッセージ処理
	virtual void OnInit();
	virtual void OnSelChanged(LPCITEMIDLIST pidl);

	// コールバック処理
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};
