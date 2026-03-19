#include "../../include/io/Server.hpp"
#include <csignal>

volatile sig_atomic_t g_running = 1;

void signal_handler(int sig)
{
	(void)sig;
	g_running = 0;
}

void setupSignals()
{
	signal(SIGINT,  signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, SIG_IGN);
}