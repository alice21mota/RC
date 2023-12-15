#include "auxFuncs.h"

//Gets file size in bytes
size_t getFileSize(string asset_fname) {

    ifstream file(asset_fname, ios::binary | ios::ate);
    size_t size = file.tellg();
    file.close();

    return size;
}

string fileSizeString(string asset_fname) {

    size_t size = getFileSize(asset_fname);
    ostringstream sizeStr;
    sizeStr << size;

    if (sizeStr.str().size() > 8) {
        return "Too many digits";
    }

    return sizeStr.str();
}

int setup_signal_handlers() {
  // set SIGINT/SIGTERM handler to close server gracefully
    struct sigaction sa;
    sa.sa_handler = exitUserApp;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        cerr << "Error setting up SIGINT signal handler. Exiting." << endl;
        return EXIT_FAILURE;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        cerr << "Error setting up SIGTERM signal handler. Exiting." << endl;
        return EXIT_FAILURE;
    }

    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    return EXIT_SUCCESS;
}

void exitUserApp(int sig) {
    (void)sig;

    if (logged_in) {
        cout << endl << "Please logout before exiting the application. Do you want to logout now? (y/n): ";

        char response;
        cin >> response;

        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');


        if (response == 'y' || response == 'Y') {
            // Perform any necessary logout logic here
            cout << "Logging out...";
            logged_in = false;
        }
        else {
            cout << "Continuing the program." << endl;
            signalReceived = false;
            return; // Continue the program without exiting
        }
    }

    cout << endl << "Exiting the program." << endl;
    shouldExit = true;  // Set the flag to exit the program
    signalReceived = true;
}


