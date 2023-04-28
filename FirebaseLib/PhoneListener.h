#pragma once
class PhoneListener : public PhoneAuthProvider::Listener
{
public:
	PhoneListener();

	void OnVerificationCompleted(Credential credential) override;
	void OnVerificationFailed(const std::string& error) override;
	
	void OnCodeSent(const std::string& verification_id, const PhoneAuthProvider::ForceResendingToken& force_resending_token) override;

	void OnCodeAutoRetrievalTimeOut(const std::string& verification_id) override;

	const std::string& GetVerificationId() const;
	const PhoneAuthProvider::ForceResendingToken& GetForceResendingToken() const;

	int NumCallsOnVerificationComplete() const;
	int NumCallsOnVerificationFailed() const;
	int NumCallsOnCodeSent() const;
	int NumCallsOnCodeAutoRetrievalTimeOut() const;

private:
	std::string verification_id_;
	PhoneAuthProvider::ForceResendingToken force_resending_token_;
	int num_calls_on_verification_complete_;
	int num_calls_on_verification_failed_;
	int num_calls_on_code_sent_;
	int num_calls_on_code_auto_retrieval_time_out_;
};

