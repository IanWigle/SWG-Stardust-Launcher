#include "pch.h"
#include "PhoneListener.h"

PhoneListener::PhoneListener() 
    : num_calls_on_verification_complete_(0),
    num_calls_on_verification_failed_(0),
    num_calls_on_code_sent_(0),
    num_calls_on_code_auto_retrieval_time_out_(0) {}

void PhoneListener::OnVerificationCompleted(Credential credential)
{
    LogMessage("PhoneListener: successful automatic verification.");
    num_calls_on_verification_complete_++;
}

void PhoneListener::OnVerificationFailed(const std::string& error)
{
    LogMessage("ERROR: PhoneListener verification failed with error, %s",
        error.c_str());
    num_calls_on_verification_failed_++;
}

void PhoneListener::OnCodeSent(const std::string& verification_id, const PhoneAuthProvider::ForceResendingToken& force_resending_token)
{
    LogMessage("PhoneListener: code sent. verification_id=%s",
        verification_id.c_str());
    verification_id_ = verification_id;
    force_resending_token_ = force_resending_token;
    num_calls_on_code_sent_++;
}

void PhoneListener::OnCodeAutoRetrievalTimeOut(const std::string& verification_id)
{
    verification_id_ = verification_id;
    num_calls_on_code_auto_retrieval_time_out_++;
}

const std::string& PhoneListener::GetVerificationId() const
{
    return verification_id_;
}

const PhoneAuthProvider::ForceResendingToken& PhoneListener::GetForceResendingToken() const
{
    return force_resending_token_;
}

int PhoneListener::NumCallsOnVerificationComplete() const
{
    return num_calls_on_verification_complete_;
}

int PhoneListener::NumCallsOnVerificationFailed() const
{
    return num_calls_on_verification_failed_;
}

int PhoneListener::NumCallsOnCodeSent() const
{
    return num_calls_on_code_sent_;
}

int PhoneListener::NumCallsOnCodeAutoRetrievalTimeOut() const
{
    return num_calls_on_code_auto_retrieval_time_out_;
}
