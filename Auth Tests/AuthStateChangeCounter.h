#pragma once
#include "firebase/auth.h"

using ::firebase::auth::Auth;

class AuthStateChangeCounter : public firebase::auth::AuthStateListener {
public:
    AuthStateChangeCounter() : num_state_changes_(0) {}

    virtual void OnAuthStateChanged(Auth* auth);

    void CompleteTest(const char* test_name, int expected_state_changes);

    void CompleteTest(const char* test_name, int min_state_changes, int max_state_changes);

private:
    int num_state_changes_;
};