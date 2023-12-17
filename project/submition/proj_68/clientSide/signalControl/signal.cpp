#include "signal.h"

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

        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        if (response == 'y' || response == 'Y') {
            // Perform any necessary logout logic here

            string comm, res, stat;

            comm = logout();
            res = sendUDP(comm);
            stat = logoutStatus(res);

            if (stat == "Successful !logout") {
                cout << "Logging out..." << endl;
                logged_in = false;

                shouldExit = true;  // Set the flag to exit the program
                signalReceived = true;
            }

            else {
                cout << "Problems logging out in server. Exiting User Application anyway" << endl;
                logged_in = false;

                shouldExit = true;  // Set the flag to exit the program
                signalReceived = true;
            }

        }
        else {
            cout << "Continuing the program." << endl;
            signalReceived = false;
            return; // Continue the program without exiting
        }
    }

    cout << "Exiting the program." << endl;
    shouldExit = true;  // Set the flag to exit the program
    signalReceived = true;
}


