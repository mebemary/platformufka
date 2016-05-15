#pragma once

class State;

class IStateStackable
{
    public:
	    virtual void pushState(std::shared_ptr<State> newState) = 0;
	    virtual void popState() = 0;
        virtual void exitGame() = 0;
};

class IGameExitable
{
    public:
        virtual void exitGame() = 0;
};