#include "CloudStorageTestsPCH.h"
// CloudStorage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main(int argc, const char* argv[])
{
    std::cout << "Hello World!\n";

    ChangeToFileDirectory(FIREBASE_CONFIG_STRING[0] != '\0'
        ? FIREBASE_CONFIG_STRING
        : argv[0]);

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, TRUE);

    ::firebase::App* app = ::firebase::App::Create();

    LogMessage("Initialized Firebase App.");

    LogMessage("Initialize Firebase Auth and Cloud Storage.");

    // Use ModuleInitializer to initialize both Auth and Storage, ensuring no
// dependencies are missing.
    ::firebase::storage::Storage* storage = nullptr;
    ::firebase::auth::Auth* auth = nullptr;
    void* initialize_targets[] = { &auth, &storage };

    const firebase::ModuleInitializer::InitializerFn initializers[] = {
        [](::firebase::App* app, void* data) {
          LogMessage("Attempt to initialize Firebase Auth.");
          void** targets = reinterpret_cast<void**>(data);
          ::firebase::InitResult result;
          *reinterpret_cast<::firebase::auth::Auth**>(targets[0]) =
              ::firebase::auth::Auth::GetAuth(app, &result);
          return result;
        },
        [](::firebase::App* app, void* data) {
          LogMessage("Attempt to initialize Cloud Storage.");
          void** targets = reinterpret_cast<void**>(data);
          ::firebase::InitResult result;
          firebase::storage::Storage* storage =
              firebase::storage::Storage::GetInstance(app, kStorageUrl, &result);
          *reinterpret_cast<::firebase::storage::Storage**>(targets[1]) = storage;
          LogMessage("Initialized storage with URL %s, %s",
                     kStorageUrl ? kStorageUrl : "(null)",
                     storage->url().c_str());
          return result;
        } };

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(app, initialize_targets, initializers,
        sizeof(initializers) / sizeof(initializers[0]));

    WaitForCompletion(initializer.InitializeLastResult(), "Initialize");

    if (initializer.InitializeLastResult().error() != 0) {
        LogMessage("Failed to initialize Firebase libraries: %s",
            initializer.InitializeLastResult().error_message());
        ProcessEvents(2000);
        return 1;
    }
    LogMessage("Successfully initialized Firebase Auth and Cloud Storage.");

    // Sign in using Auth before accessing Storage.
    // The default Storage permissions allow anonymous users access. This will
    // work as long as your project's Authentication permissions allow anonymous
    // signin.
    {
        firebase::Future<firebase::auth::User*> sign_in_future =
            auth->SignInAnonymously();
        WaitForCompletion(sign_in_future, "SignInAnonymously");
        if (sign_in_future.error() == firebase::auth::kAuthErrorNone) {
            LogMessage("Auth: Signed in anonymously.");
        }
        else {
            LogMessage("ERROR: Could not sign in anonymously. Error %d: %s",
                sign_in_future.error(), sign_in_future.error_message());
            LogMessage(
                "  Ensure your application has the Anonymous sign-in provider "
                "enabled in Firebase Console.");
            LogMessage(
                "  Attempting to connect to Cloud Storage anyway. This may fail "
                "depending on the security settings.");
        }
    }

    // Generate a folder for the test data based on the time in milliseconds.
    int64_t time_in_microseconds = GetCurrentTimeInMicroseconds();

    char buffer[21] = { 0 };
    snprintf(buffer, sizeof(buffer), "%lld",
        static_cast<long long>(time_in_microseconds));  // NOLINT
    std::string saved_url = "Root";

    // Create a unique child in the storage that we can run in.
    firebase::storage::StorageReference ref;
    ref = storage->GetReference(saved_url);

    LogMessage("Storage URL: gs://%s%s", ref.bucket().c_str(),
        ref.full_path().c_str());

    // Read and write from memory. This will save a small file and then read it
  // back from the storage to confirm that it was uploaded. Then it will remove
  // the file.
    {
        const std::string kSimpleTestFile =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "
            "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
            "aliquip ex ea commodo consequat. Duis aute irure dolor in "
            "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
            "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
            "culpa qui officia deserunt mollit anim id est laborum.";
        {
            LogMessage("Write a sample file.");
            std::string custom_metadata_key = "specialkey";
            std::string custom_metadata_value = "secret value";
            firebase::storage::Metadata metadata;
            metadata.set_content_type("test/plain");
            (*metadata.custom_metadata())[custom_metadata_key] =
                custom_metadata_value;
            //firebase::Future<firebase::storage::Metadata> future =
            //    ref.Child("TestFile")
            //    .Child("SampleFile.txt")
            //    .PutBytes(&kSimpleTestFile[0], kSimpleTestFile.size(), metadata);
            StorageReference fileRef = ref.Child("testfile.txt");
            Future future = fileRef.PutBytes(&kSimpleTestFile[0], kSimpleTestFile.size(), metadata);
            WaitForCompletion(future, "Write");
            if (future.error() == 0) {
                if (future.result()->size_bytes() != kSimpleTestFile.size()) {
                    LogMessage("ERROR: Incorrect number of bytes uploaded.");
                }
            }
        }
        {
            LogMessage("Read back the sample file.");

            const size_t kBufferSize = 14000;
            char buffer[kBufferSize];

            StorageReference fileRef = ref.Child("testfile.txt");
            

            firebase::Future<size_t> future = fileRef.GetBytes(buffer, kBufferSize);
            //firebase::Future<size_t> future = fileRef.GetFile(".\\SWGLauncher.zip");
            WaitForCompletion(future, "Read");
            if (future.error() == 0) {
                if (*future.result() != kSimpleTestFile.size()) {
                    LogMessage("ERROR: Incorrect number of bytes uploaded.");
                }
                if (memcmp(&kSimpleTestFile[0], buffer, kSimpleTestFile.size()) != 0) {
                    LogMessage("ERROR: file contents did not match.");
                }
            }
            
        }
        {
            LogMessage("Delete the sample file.");

            StorageReference fileRef = ref.Child("testfile.txt");

            firebase::Future<void> future =
                fileRef.Delete();
            WaitForCompletion(future, "Delete");
        }
    }

    LogMessage("Shutdown the Storage library.");
    delete storage;
    storage = nullptr;

    LogMessage("Signing out from anonymous account.");
    auth->SignOut();

    LogMessage("Shutdown the Auth library.");
    delete auth;
    auth = nullptr;

    LogMessage("Shutdown Firebase App.");
    delete app;
    app = nullptr;

    // Wait until the user wants to quit the app.
    while (!ProcessEvents(1000)) {
    }

    return 0;
}