#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <cstdint>

using namespace std;

void sigusr1_handler(int sig, siginfo_t *info, void *ucontext) {
	pid_t sender_pid = info->si_pid;
	uid_t sender_uid = info->si_uid;
	struct passwd *pwd = getpwuid(sender_uid);
	const char *username = (pwd != nullptr) ? pwd->pw_name : "Unknown";
	ucontext_t *context = (ucontext_t *)ucontext;
	uintptr_t rip = (uintptr_t)(context->uc_mcontext.gregs[REG_RIP]);
	uintptr_t rax = (uintptr_t)(context->uc_mcontext.gregs[REG_RAX]);
	uintptr_t rbx = (uintptr_t)(context->uc_mcontext.gregs[REG_RBX]);
	cout << "Received a SIGUSR1 signal from process " << sender_pid
	<< " executed by " << sender_uid << " (" << username << ")." << endl;
	cout << "State of the context: RIP = " << hex << rip
	<< ", RAX = " << rax << ", RBX = " << rbx << dec << endl;
}

int main() {
	pid_t pid = getpid();
	cout << "Process PID: " << pid << endl;
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigusr1_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		return 1;
	}

	while (true) {
		sleep(10);
	}

	return 0;
}
