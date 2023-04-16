#pragma once
#include "firebase/auth.h"

using ::firebase::auth::Auth;

class IdTokenChangeCounter : public firebase::auth::IdTokenListener
{
public:
	IdTokenChangeCounter() : num_token_changes_(0) {}

	virtual void OnIdTokenChanged(Auth* auth);

	void CompleteTest(const char* test_name, int token_changes);
	void CompleteTest(const char* test_name, int min_token_changes, int max_token_changes);

private:
	int num_token_changes_;
};

