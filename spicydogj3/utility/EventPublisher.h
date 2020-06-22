#pragma once

#include <list>

/**
	イベントリスナ管理およびリスナへのイベント発行クラス
*/
template <typename _PARENT,typename _LISTENER>
class CEventPublisher
{
private:
	typedef void (_PARENT::*_FIREENUMPROC)(_PARENT*,_LISTENER*);
	typedef std::list<_LISTENER*> LISTENERS;

private:
	LISTENERS m_Listeners;

public:
	//	指定したリスナを登録する
	void AppendListener(_LISTENER* p)
	{
		if (FindListener(p) == m_Listeners.end())
		{
			m_Listeners.push_back(p);
		}
		//else
		//	THROW_INTERNAL_EXCEPTION;
	}
	//	指定したリスナの登録を解除
	void RemoveListener(_LISTENER* p)
	{
		LISTENERS::iterator i = FindListener(p);
		if (i != m_Listeners.end())
		{
			m_Listeners.erase(i);
		}
		//else
		//	THROW_INTERNAL_EXCEPTION;
	}
	//	登録されているリスナを全て解除
	void RemoveAllListeners()
	{
		m_Listeners.clear();
	}
	//	登録されているリスナ数を取得
	void GetListenerCount() const
	{
		return m_Listeners.size();
	}
	//	全てのリスナを列挙し _FIREENUMPROC コールバック関数を呼び出す
	void EnumListeners(_PARENT* pParent,_FIREENUMPROC func)
	{
		LISTENERS listenersToFire(m_Listeners);
		ASSERT(listenersToFire.size() == m_Listeners.size());

		for (LISTENERS::iterator i = listenersToFire.begin(); i != listenersToFire.end();)
		{
			LISTENERS::iterator iFire = i;
			i++;
			(pParent->*func)((_PARENT*)this,*iFire);
		}
	}
private:
	LISTENERS::iterator FindListener(_LISTENER* p)
	{
		return std::find(m_Listeners.begin(),m_Listeners.end(),p);
	}
};

