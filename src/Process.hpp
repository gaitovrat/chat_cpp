/*
 * Process.hpp
 *
 *  Created on: Nov 15, 2021
 *      Author: gai0010
 */

#ifndef SRC_PROCESS_HPP_
#define SRC_PROCESS_HPP_

#include <sys/types.h>

namespace Chat
{
    struct Process
    {
        int socket;
        pid_t pid;

        ~Process();
    };
}


#endif /* SRC_PROCESS_HPP_ */
