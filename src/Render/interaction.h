#pragma once 

class Command
{
public:
	virtual ~Command(){}
	virtual void execute() = 0;
};

class Rotate :public Command
{

};

class RENDER_API Interatcion
{

};