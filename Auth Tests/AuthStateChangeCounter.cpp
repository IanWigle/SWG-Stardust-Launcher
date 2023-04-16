#include "AuthTestsPCH.h"

void AuthStateChangeCounter::OnAuthStateChanged(Auth* auth)
{
    num_state_changes_++;
    LogMessage("OnAuthStateChanged User %p (state changes %d)",
        auth->current_user(), num_state_changes_);
}

void AuthStateChangeCounter::CompleteTest(const char* test_name, int expected_state_changes)
{
    CompleteTest(test_name, expected_state_changes, expected_state_changes);
}

void AuthStateChangeCounter::CompleteTest(const char* test_name, int min_state_changes, int max_state_changes)
{
    const bool success = min_state_changes <= num_state_changes_ && num_state_changes_ <= max_state_changes;
    LogMessage("%sAuthStateListener called %d time%s on %s.", 
               success ? "" : "ERROR: ", num_state_changes_,
               num_state_changes_ == 1 ? "" : "s", test_name);
    num_state_changes_ = 0;
}
