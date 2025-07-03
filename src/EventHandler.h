#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

/**
 * @brief Represents an event with a unique identifier.
 * 
 * Events are lightweight structs used to signal occurrences (e.g., sensor triggers) within
 * the framework. Define custom events by assigning unique IDs in your application.
 */
struct Event {
    int id; ///< Unique identifier for the event type.

    explicit Event(int eventId) : id(eventId) {}
    bool operator==(const Event& other) const { return id == other.id; }
};

/**
 * @brief Abstract interface for handling events.
 * 
 * Implement this interface in classes that need to react to events. The `on` method is called
 * when an event occurs, allowing for custom handling logic.
 */
class EventHandler {
public:
    virtual void on(Event event) = 0; ///< Pure virtual method to handle an event.
    virtual ~EventHandler() = default; ///< Virtual destructor for safe inheritance.
};

#endif // EVENT_HANDLER_H