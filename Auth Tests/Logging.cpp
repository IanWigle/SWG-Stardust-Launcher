#include "AuthTestsPCH.h"

namespace Log
{
    // Log a map of variants.
    void LogVariantMap(const std::map<Variant, Variant>& variant_map,
        int indent) {
        std::string indent_string(indent * 2, ' ');
        for (auto it = variant_map.begin(); it != variant_map.end(); ++it) {
            const Variant& key_string = it->first.AsString();
            const Variant& value = it->second;
            if (value.is_fundamental_type()) {
                const Variant& string_value = value.AsString();
                LogMessage("%s%s: %s,", indent_string.c_str(), key_string.string_value(),
                    string_value.string_value());
            }
            else {
                LogMessage("%s%s:", indent_string.c_str(), key_string.string_value());
                if (value.is_vector()) {
                    LogVariantVector(value.vector(), indent + 1);
                }
                else if (value.is_map()) {
                    LogVariantMap(value.map(), indent + 1);
                }
                else {
                    LogMessage("%s  ERROR: unknown type %d", indent_string.c_str(),
                        static_cast<int>(value.type()));
                }
            }
        }
    }

    void LogVariantVector(const std::vector<Variant>& variants, int indent) {
        std::string indent_string(indent * 2, ' ');
        LogMessage("%s[", indent_string.c_str());
        for (auto it = variants.begin(); it != variants.end(); ++it) {
            const Variant& item = *it;
            if (item.is_fundamental_type()) {
                const Variant& string_value = item.AsString();
                LogMessage("%s  %s,", indent_string.c_str(), string_value.string_value());
            }
            else if (item.is_vector()) {
                LogVariantVector(item.vector(), indent + 2);
            }
            else if (item.is_map()) {
                LogVariantMap(item.map(), indent + 2);
            }
            else {
                LogMessage("%s  ERROR: unknown type %d", indent_string.c_str(),
                    static_cast<int>(item.type()));
            }
        }
        LogMessage("%s]", indent_string.c_str());
    }

    // Display the sign-in result.
    void LogSignInResult(const SignInResult& result) {
        if (!result.user) {
            LogMessage("ERROR: User not signed in");
            return;
        }
        LogMessage("* User ID %s", result.user->uid().c_str());
        const AdditionalUserInfo& info = result.info;
        LogMessage("* Provider ID %s", info.provider_id.c_str());
        LogMessage("* User Name %s", info.user_name.c_str());
        LogVariantMap(info.profile, 0);
        const UserMetadata& metadata = result.meta;
        LogMessage("* Sign in timestamp %d",
            static_cast<int>(metadata.last_sign_in_timestamp));
        LogMessage("* Creation timestamp %d",
            static_cast<int>(metadata.creation_timestamp));
    }
}