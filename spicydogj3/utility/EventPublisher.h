#pragma once

#include <list>

/**
	�C�x���g���X�i�Ǘ�����у��X�i�ւ̃C�x���g���s�N���X
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
	//	�w�肵�����X�i��o�^����
	void AppendListener(_LISTENER* p)
	{
		if (FindListener(p) == m_Listeners.end())
		{
			m_Listeners.push_back(p);
		}
		//else
		//	THROW_INTERNAL_EXCEPTION;
	}
	//	�w�肵�����X�i�̓o�^������
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
	//	�o�^����Ă��郊�X�i��S�ĉ���
	void RemoveAllListeners()
	{
		m_Listeners.clear();
	}
	//	�o�^����Ă��郊�X�i�����擾
	void GetListenerCount() const
	{
		return m_Listeners.size();
	}
	//	�S�Ẵ��X�i��񋓂� _FIREENUMPROC �R�[���o�b�N�֐����Ăяo��
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

