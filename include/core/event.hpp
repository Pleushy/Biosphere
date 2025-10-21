#pragma once

#include <any>
#include <optional>
#include <string>
#include <vector>

/** Event data structure */
struct Event {
    int event_type;                     // Type of event
    std::string name;                   // Unique name of event
    std::optional<std::any> data;       // Optional: Data of event
    bool called;                        // Whether event was called or not
};

static std::vector<Event> Events;

/** Event handler class */
class EventHandler {
    private:
        // Checks whether an event with the given name already exists
        bool event_exists(std::string name);
    public:
        // Removes an event from the event vector
        void free_event(std::string name);

        // Adds an event to the event vector
        void queue_event(Event event);

        // Calls an event by name with optional data
        void call_event(std::string name, std::optional<std::any> data);

        // Calls one or more events by their type with optional data
        void call_event_by_type(int event_type, std::optional<std::any> data);
};