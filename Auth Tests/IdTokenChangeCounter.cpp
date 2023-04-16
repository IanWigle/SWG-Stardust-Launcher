#include "AuthTestsPCH.h"

void IdTokenChangeCounter::OnIdTokenChanged(Auth* auth)
{
    num_token_changes_++;
    LogMessage("OnIdTokenChanged User %p (token changes %d)",
               auth->current_user(), num_token_changes_);
}

void IdTokenChangeCounter::CompleteTest(const char* test_name, int token_changes)
{
    CompleteTest(test_name, token_changes, token_changes);
}

void IdTokenChangeCounter::CompleteTest(const char* test_name, int min_token_changes, int max_token_changes)
{
    const bool success = min_token_changes <= num_token_changes_ && num_token_changes_ <= max_token_changes;
    LogMessage("%sIdTokenListener called %d time%s on %s.",
               success ? "" : "ERROR: ", num_token_changes_,
               num_token_changes_ == 1 ? "" : "s", test_name);
    num_token_changes_ = 0;
}
