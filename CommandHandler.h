#ifndef COMMANDHANDLER_H_INCLUDED
#define COMMANDHANDLER_H_INCLUDED

class Command
{
public:
    // Type definitions
    typedef uint8_t IDType;
    typedef uint32_t ParameterType;
    typedef bool(*CallbackType)(ParameterType);

    Command(IDType id, CallbackType callback_)
        : id_(id), callback_(callback_), nextCommand_(NULL)
    {}

    Command * next()
    {
        return nextCommand_;
    }
    void setNext(Command * command)
    {
        nextCommand_ = command;
    }
    IDType id()
    {
        return id_;
    }
    bool exec(ParameterType param)
    {
        if (callback_)
        {
            return callback_(param);
        }
        return false;
    }
private:
    IDType id_;
    CallbackType callback_;
    Command * nextCommand_;
};

class CommandHandler
{
public:
    CommandHandler()
        : firstCommand_(NULL), lastCommand_(NULL)
    {}
    void addCommand(Command * command)
    {
        if (firstCommand_ == NULL)
        {
            firstCommand_ = command;
            lastCommand_ = command;
        }
        else
        {
            lastCommand_->setNext(command);
            lastCommand_ = command;
        }
    }
    void handleCommand(Command::IDType id, Command::ParameterType param)
    {
        Command * cmd = firstCommand_;
        while (cmd != 0)
        {
            if (cmd->id() == id && cmd->exec(param))
            {
                return;
            }
            cmd = cmd->next();
        }
    }
private:
    Command * nextCommand();
    Command * firstCommand_;
    Command * lastCommand_;
};

#endif /* COMMANDHANDLER_H_INCLUDED */
