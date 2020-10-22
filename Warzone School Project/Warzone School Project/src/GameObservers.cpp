#include <iostream>
#include <vector>

class Observer
{
public:
	virtual void Update() = 0; 
};

class Subject
{
public:
	virtual ~Subject() { }

	void AddObserver(Observer* o) { m_observers.push_back(o); }
	
	void RemoveObserver(Observer* o) 
	{
		for (int i = 0; i < m_observers.size(); i++)
		{
			if (o == m_observers[i])
			{
				m_observers.erase(m_observers.begin() + i);
				break;
			}
		}
	}

	void Notify() 
	{
		for (Observer* o : m_observers)
		{
			o->Update();
		}
	}

private:
	std::vector<Observer*> m_observers;
};