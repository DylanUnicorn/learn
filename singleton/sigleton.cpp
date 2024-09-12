#include <iostream>

class Singleton
{
public:
	static Singleton& Get()
	{

	}
private:
	Singleton() {};
	static Singleton s_Instance;
};

Singleton Singleton::s_Instance;


int main()
{
	Singleton()
}