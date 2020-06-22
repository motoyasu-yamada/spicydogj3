#pragma once

/** Undo / Redo サポートの為の抽象基底クラス */
class Cmd {
private:
	CString m_name;
	int     m_iconIndex;
protected:
	Cmd(UINT resourceID);
public:
	virtual void Do()   = 0;
	virtual void Undo() = 0;
	virtual ~Cmd();
	/** コマンド名 */
	CString GetName() const;
	/** アイコン画像のリソースID */
	int GetIconIndex() const;
	/** @param resid - -1:     現在の nameを書式フォーマットとして使用
	                   -1以外: 指定したIDの文字列リソースを書式フォーマットとして使用 */
	void SetCmdName(UINT resid,...);
};
