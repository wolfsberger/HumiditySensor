#ifndef COMMANDHANDLER_H_INCLUDED
#define COMMANDHANDLER_H_INCLUDED

class CommandBase
{
public:
    // Type definitions
    typedef uint8_t IDType;
    typedef uint32_t ParameterType;
    typedef bool(*CallbackType)(ParameterType);

    CommandBase(IDType id)
        : id_(id), nextCommand_(NULL)
    {}

    CommandBase * next()
    {
        return nextCommand_;
    }
    void setNext(CommandBase * command)
    {
        nextCommand_ = command;
    }
    IDType id()
    {
        return id_;
    }
    void operator()(ParameterType param)
    {
        exec(param);
    }
    virtual bool exec(ParameterType param) = 0;
private:

    IDType id_;
    CommandBase * nextCommand_;
};

/** Class to create a command that points to a class member.
 *
 */
template <typename T>
class MemberCommand : public CommandBase
{
public:
     MemberCommand(CommandBase::IDType id, T* target, bool (T::*func)(CommandBase::ParameterType))
        : CommandBase(id), target_(target), function_(func)
     {}

     bool exec(ParameterType param)
     {
        return (target_->*function_)(param);
     }

private:
     T * target_;
     bool (T::*function_)(CommandBase::ParameterType);
};

/** Class to create a command that points to function.
 *
 */
class FunctionCommand : public CommandBase
{
public:
     FunctionCommand(CommandBase::IDType id, CommandBase::CallbackType func)
        :  CommandBase(id), function_(func)
     {}

     bool exec(ParameterType param)
     {
        return function_(param);
     }

private:
     CommandBase::CallbackType function_;
};


class CommandHandler
{
public:
    CommandHandler()
        : firstCommand_(NULL), lastCommand_(NULL)
    {}
    void addCommand(CommandBase * command)
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
    void handleCommand(CommandBase::IDType id, CommandBase::ParameterType param)
    {
        CommandBase * cmd = firstCommand_;
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
    CommandBase * nextCommand();
    CommandBase * firstCommand_;
    CommandBase * lastCommand_;
};

#endif /* COMMANDHANDLER_H_INCLUDED */
