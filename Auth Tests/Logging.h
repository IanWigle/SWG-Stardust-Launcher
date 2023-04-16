#pragma once



namespace Log
{
	void LogVariantMap(const std::map<Variant, Variant>& variant_map, int indent);
	void LogVariantVector(const std::vector<Variant>& variants, int indent);
	void LogSignInResult(const SignInResult& result);
}