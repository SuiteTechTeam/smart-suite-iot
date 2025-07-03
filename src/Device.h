#ifndef DEVICE_H
#define DEVICE_H

#include "EventHandler.h"
#include "CommandHandler.h"

class Device : public EventHandler, public CommandHandler {
public:
    /**
     * @brief Handles an event received by the device.
     * @param event The event to process.
     */
    virtual void on(Event event) override = 0;

    /**
     * @brief Handles a command issued to the device.
     * @param command The command to execute.
     */
    virtual void handle(Command command) override = 0;

    virtual ~Device() = default; ///< Virtual destructor for safe inheritance.
};

#endif // DEVICE_H
