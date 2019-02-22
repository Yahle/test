 ///
 /// @file    observer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-02-21 17:31:00
 ///
 
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <memory>
using std::cout;
using std::endl;
using std::list;
using std::find;
using std::string;
using std::unique_ptr;

class Observer;//前向
class Subject
{
public:
	virtual void attach(Observer * ob) = 0;
	virtual void detach(Observer * ob) = 0;
	virtual void notify() = 0;

	virtual ~Subject(){ cout << "~Subject()" << endl;}
};

class Observer
{
public:
	virtual void update() = 0;
	virtual ~Observer()
	{
		cout << "~Observer()" << endl;
	}
};

class Ring
:public Subject
{
public:
	void attach(Observer *ob)
	{
		auto iter = std::find(_oblist.begin(), _oblist.end(), ob);
		if(iter == _oblist.end())
		{
			_oblist.push_back(ob);
		}
	}

	void detach(Observer * ob)
	{
		auto iter = std:: find(_oblist.begin(), _oblist.end(), ob);
		if(iter != _oblist.end())
		{
			_oblist.erase(iter);
		}
	}

	void notify()
	{
		for(auto & ob: _oblist)
		{
			ob->update();
		}
	}

	void setStatus(bool on)
	{
		_isKnocking = on;
		notify();
	}

private:
	list<Observer *> _oblist;
	list<Observer *> _vipList;//观察者的分类
	bool _isKnocking;
};

class Baby
:public Observer
{
public:
	Baby(const string & name)
	:_name(name)
	,_random(-1)
	{}

	void update()
	{
		::srand(::clock());//时钟频率
		_random = ::rand() % 100;
		if(_random < 30)
		{
			cout <<">> Baby " << _name << "start crying !" << endl;
		}
		else
			cout << ">> Baby " << _name << " is sleeping sweetly!"<< endl;
	}

	~Baby(){	cout << "~Baby()" << _name << endl;}

private:
	string _name;
	int _random;
};

class Nurse
:public Observer
{
public:
	Nurse(const string & name)
	:_name(name)
	,_random(-1)
	{}

	void action()
	{
		update();
	}

	void update()
	{
		::srand(::clock());//时钟频率
		_random = ::rand() % 100;
		if(_random < 70)
		{
			cout << ">> Nurse " << _name << "is awake,start to open the door!" << endl;
		}
		else
			cout << ">> Nurse " << _name << " is sleeping sweetly!"<< endl;
	}

	~Nurse(){	cout << "~Nurse()" << _name << endl;}

private:
	string _name;
	int _random;
};

class Guest
{
public:
	Guest(const string & name)
	:_name(name)
	{}

	void knock(Ring & ring)
	{
		cout << _name << "is pressing the ring of the door !" << endl;
		ring.setStatus(true);
	}

private:
	string _name;
};

int main(void)
{
	Ring ring;
	unique_ptr<Observer> baby1(new Baby("大宝"));
	unique_ptr<Observer> baby2(new Baby("二宝"));
	unique_ptr<Observer> nurse1(new Nurse("小美")); 
	unique_ptr<Observer> nurse2(new Nurse("小丽")); 

	ring.attach(baby1.get());
	ring.attach(baby2.get());
	ring.attach(nurse1.get());
	ring.attach(nurse2.get());

	Guest guest("大鹏");
	guest.knock(ring);

	cout << endl << ">>>one hour later ...! " << endl;
	ring.detach(baby1.get());
	guest.knock(ring);
 return 0;
}
