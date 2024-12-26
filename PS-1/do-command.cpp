#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <chrono>

void do_command(char** argv) {
	auto start_time = std::chrono::high_resolution_clock::now();
	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		execvp(argv[0], argv);
		perror("execvp failed");
		exit(EXIT_FAILURE);
	}
	else {
		int status;
		waitpid(pid, &status, 0);
		auto end_time = std::chrono::high_resolution_clock::now();
        	std::chrono::duration<double> duration = end_time - start_time;
		if (WIFEXITED(status)) {
			std::cout << "Command completed with " << WEXITSTATUS(status) << " exit code and took " << duration.count() << " seconds." << std::endl;
		}
 		else {
			std::cerr << "Command failed to execute properly." << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: ./do-command <command> [arguments...]" << std::endl;
		return 1;
	}
	char** command_args = new char*[argc];
	for (int i = 1; i < argc; ++i) {
		command_args[i - 1] = argv[i];
	}
	command_args[argc - 1] = nullptr;
	do_command(command_args);
	delete[] command_args;
	return 0;
}
