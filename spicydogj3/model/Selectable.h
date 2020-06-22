#pragma once

class Selectable {
private:
	BOOL m_selected;

protected:
	Selectable();
	virtual void CopyFrom(const Selectable&);
public:
	virtual void Unselect();
	virtual void Select();
	void SelectToggle();
	BOOL IsSelected() const;

protected:
	virtual ~Selectable();
};

