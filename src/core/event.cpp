#include <any>
#include <iostream>
#include <iterator>
#include <optional>
#include <queue>
#include <string>
#include "core/event.hpp"

bool EventHandler::event_exists(std::string name) {
    for (Event event : Events) {
        if (event.name == name) return true;
    }
    return false;
}

void EventHandler::queue_event(Event event) {
    if (event_exists(event.name)) {
        std::cerr << "Event '" << event.name << "' already exists!" << std::endl;
        return;
    };
    Events.push_back(event);
}

void EventHandler::free_event(std::string name) {
    if (!event_exists(name)) {
        std::cerr << "Event '" << name << "' does not exist!" << std::endl;
        return;
    };
    for (int i = 0; i < Events.size(); i++) {
        if (Events[i].name == name) {
            Events.erase(Events.begin()+i);
            break;
        }
    }
}

void EventHandler::call_event(std::string name, std::optional<std::any> data) {
    if (!event_exists(name)) {
        std::cerr << "Event '" << name << "' does not exist!" << std::endl;
        return;
    };
    for (Event event : Events) {
        if (event.name == name) {
            event.data = data;
            event.called = true;
            break;
        }
    }
}

void EventHandler::call_event_by_type(int event_type, std::optional<std::any> data) {
    for (Event event : Events) {
        if (event.event_type == event_type) call_event(event.name, data);
    }
}