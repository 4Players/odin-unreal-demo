#include "Console.h"
#include "GameFramework/Actor.h"

Console::Console(void)
	: mpImp(NULL)
{
}

Console::~Console(void)
{
}

void Console::update(void)
{
}

void Console::write(const ExitGames::Common::JString& str)
{
	log(str, FColor::Blue);
	std::wcout << str;
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
	write(str);
	std::wcout << std::endl;
}

Console& Console::get(void)
{
	static Console console;
	return console;
}