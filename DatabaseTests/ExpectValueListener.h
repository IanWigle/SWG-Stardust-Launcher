#pragma once

// A ValueListener that expects a specific value to be set.
class ExpectValueListener : public firebase::database::ValueListener {
public:
    explicit ExpectValueListener(firebase::Variant wait_value)
        : wait_value_(wait_value.AsString()), got_value_(false) {}
    void OnValueChanged(
        const firebase::database::DataSnapshot& snapshot) override {
        if (snapshot.value().AsString() == wait_value_) {
            got_value_ = true;
        }
        else {
            LogMessage(
                "FAILURE: ExpectValueListener did not receive the expected result.");
        }
    }
    void OnCancelled(const firebase::database::Error& error_code,
        const char* error_message) override {
        LogMessage("ERROR: ExpectValueListener canceled: %d: %s", error_code,
            error_message);
    }

    bool got_value() { return got_value_; }

private:
    firebase::Variant wait_value_;
    bool got_value_;
};