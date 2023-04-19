#pragma once
class SampleValueListener : public firebase::database::ValueListener {
public:
    void OnValueChanged(
        const firebase::database::DataSnapshot& snapshot) override {
        LogMessage("  ValueListener.OnValueChanged(%s)",
            snapshot.value().AsString().string_value());
        last_seen_value_ = snapshot.value();
        seen_values_.push_back(snapshot.value());
    }
    void OnCancelled(const firebase::database::Error& error_code,
        const char* error_message) override {
        LogMessage("ERROR: SampleValueListener canceled: %d: %s", error_code,
            error_message);
    }
    const firebase::Variant& last_seen_value() { return last_seen_value_; }
    bool seen_value(const firebase::Variant& value) {
        return std::find(seen_values_.begin(), seen_values_.end(), value) !=
            seen_values_.end();
    }
    size_t num_seen_values() { return seen_values_.size(); }

private:
    firebase::Variant last_seen_value_;
    std::vector<firebase::Variant> seen_values_;
};

