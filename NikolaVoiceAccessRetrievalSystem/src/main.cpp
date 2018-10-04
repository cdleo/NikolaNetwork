/* 
 * File:   main.cpp
 * Author: cleo
 *
 * Created on June 20, 2014, 12:13 AM
 */

#include <NVARS.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

#ifdef _DAEMON_

/* -------------------------------------------------
 * MAIN como daemon 
 * ------------------------------------------------- */

int main(void) {

    /* Our process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Open any logs here */    
    LOG->initialize("nvars", LOG_DAEMON);
    
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log the failure */
        LOG->WARNING("Unable to set new SID. Exitting!");
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log the failure */
        LOG->WARNING("Unable to change the current directory. Exitting!");
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization goes here */
    LOG->INFO("Daemon initialized OK!");     
    
    /* The Big Loop (en este caso dentro de la clase NVARS) */ 
    NVARS* nvars = new NVARS();
    nvars->run();
    delete nvars;
    
    LOG->INFO("Daemon exitting OK!");
    exit(EXIT_SUCCESS);
}

#else

/* -------------------------------------------------
 * MAIN como proceso de usuario 
 * ------------------------------------------------- */

int main (int argc, const char * argv[]) {
    
    std::string logName("nvars");
    LOG->initialize( logName );  
    LOG->INFO("*********************************************************");
    LOG->INFO("** Inicio NVARS (Nikola Voice Access/Retrieval System) **");
    LOG->INFO("*********************************************************");    
    
    NVARS* nvars;
    if (argc == 2) {
        nvars = new NVARS(argv[1]);
    } else if (argc == 1) {
        nvars = new NVARS();
    } else {
        std::cout << "Invalid parameters! Usage: ./nvars <routeToConfigFile>" << std::endl;
        exit(0);
    }
    nvars->run();
    delete nvars;
    
    LOG->INFO("NVARS exitting OK!");
    
    return EXIT_SUCCESS;
}

#endif