#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

/**
 * @brief Represents a command with a unique identifier.
 * 
 * Commands are lightweight structs used to instruct devices or actuators to perform actions.
 * Define custom commands by assigning unique IDs in your application.
 */
struct Command {
    int id; ///< Unique identifier for the command type.

    explicit Command(int commandId) : id(commandId) {}
    bool operator==(const Command& other) const { return id == other.id; }
};

/**
 * @brief Abstract interface for handling commands.
 * 
 * Implement this interface in classes that need to execute commands. The `handle` method is called
 * when a command is issued, allowing for custom execution logic.
 */
class CommandHandler {
public:
    virtual void handle(Command command) = 0; ///< Pure virtual method to process a command.
    virtual ~CommandHandler() = default; ///< Virtual destructor for safe inheritance.
};

#endif // COMMAND_HANDLER_H