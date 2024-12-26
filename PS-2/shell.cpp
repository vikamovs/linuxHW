#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>

void execute_command(const std::vector<std::string>& args, bool silent) {
	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		if (silent) {
			std::string log_file = std::to_string(getpid()) + ".log";
			int fd_out = open(log_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out < 0) {
				perror("Failed to open log file");
				exit(EXIT_FAILURE);
			}
			dup2(fd_out, STDOUT_FILENO);
			dup2(fd_out, STDERR_FILENO);
			close(fd_out);
		}
		std::vector<char*> c_args;
		for (const auto& arg : args) {
			c_args.push_back(const_cast<char*>(arg.c_str()));
		}
		c_args.push_back(nullptr);
		std::string current_dir = getcwd(nullptr, 0);
		std::string new_path = "PATH=" + current_dir + ":" + std::getenv("PATH");
		putenv(const_cast<char*>(new_path.c_str()));
		execvp(c_args[0], c_args.data());
		perror("execvp failed");
		exit(EXIT_FAILURE);
	}
	else {
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			std::cout << "Process " << pid << " exited with code " << WEXITSTATUS(status) << "." << std::endl;
		}
		else {
			std::cerr << "Process " << pid << " terminated abnormally." << std::endl;
		}
	}
}

int main()
{
	std::string input;
	while (true) {
		std::cout << ">> ";
		std::getline(std::cin, input);
		if (input.empty()) {
			continue;
		}
		if (input == "exit") {
			break;
		}
		std::istringstream iss(input);
		std::vector<std::string> args;
		std::string arg;
		while (iss >> arg) {
			args.push_back(arg);
		}
		if (args.empty()) {
			continue;
		}
		bool silent = false;
		if (args[0] == "silent") {
			silent = true;
			args.erase(args.begin());
		}

       		execute_command(args, silent);
	}

	std::cout << "Shell exited successfully." << std::endl;

	return 0;
}
