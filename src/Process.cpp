/*
 * Process.cpp
 *
 *  Created on: Nov 15, 2021
 *      Author: gai0010
 */
#include "Process.hpp"

#include <unistd.h>
#include <signal.h>

Chat::Process::~Process() {
    close(socket);
}
