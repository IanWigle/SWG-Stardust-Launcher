#pragma once

// An example ChildListener class.
class SampleChildListener : public firebase::database::ChildListener {
public:
    void OnChildAdded(const firebase::database::DataSnapshot& snapshot,
        const char* previous_sibling) override {
        LogMessage("  ChildListener.OnChildAdded(%s)", snapshot.key());
        events_.push_back(std::string("added ") + snapshot.key());
    }
    void OnChildChanged(const firebase::database::DataSnapshot& snapshot,
        const char* previous_sibling) override {
        LogMessage("  ChildListener.OnChildChanged(%s)", snapshot.key());
        events_.push_back(std::string("changed ") + snapshot.key());
    }
    void OnChildMoved(const firebase::database::DataSnapshot& snapshot,
        const char* previous_sibling) override {
        LogMessage("  ChildListener.OnChildMoved(%s)", snapshot.key());
        events_.push_back(std::string("moved ") + snapshot.key());
    }
    void OnChildRemoved(
        const firebase::database::DataSnapshot& snapshot) override {
        LogMessage("  ChildListener.OnChildRemoved(%s)", snapshot.key());
        events_.push_back(std::string("removed ") + snapshot.key());
    }
    void OnCancelled(const firebase::database::Error& error_code,
        const char* error_message) override {
        LogMessage("ERROR: SampleChildListener canceled: %d: %s", error_code,
            error_message);
    }

    // Get the total number of Child events this listener saw.
    size_t total_events() { return events_.size(); }

    // Get the number of times this event was seen.
    int num_events(const std::string& event) {
        int count = 0;
        for (int i = 0; i < events_.size(); i++) {
            if (events_[i] == event) {
                count++;
            }
        }
        return count;
    }

public:
    // Vector of strings defining the events we saw, in order.
    std::vector<std::string> events_;
};

