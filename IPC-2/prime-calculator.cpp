#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>

using namespace std;

bool is_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

int calculate_prime(int m) {
    int count = 0;
    int num = 2;
    while (count < m) {
        if (is_prime(num)) {
            count++;
        }
        num++;
    }
    return num - 1;
}

int main() {
    int parent_to_child[2], child_to_parent[2];
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        while (true) {
            int m;
            cout << "[Parent] Please enter the number: ";
            cin >> m;

            if (cin.fail()) {
                string input;
                cin.clear();
                cin >> input;
                if (input == "exit") break;
                cout << "Invalid input. Please enter an integer or 'exit' to quit." << endl;
                continue;
            }

            cout << "[Parent] Sending " << m << " to the child process..." << endl;
            ssize_t bytes_written = write(parent_to_child[1], &m, sizeof(m));
            if (bytes_written != sizeof(m)) {
                cerr << "Error writing to pipe" << endl;
                return 1;
            }

            cout << "[Parent] Waiting for the response from the child process..." << endl;
            int result;
            ssize_t bytes_read = read(child_to_parent[0], &result, sizeof(result));
            if (bytes_read != sizeof(result)) {
                cerr << "Error reading from pipe" << endl;
                return 1;
            }

            cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << endl;
        }
    } else {
        while (true) {
            int m;
            ssize_t bytes_read = read(parent_to_child[0], &m, sizeof(m));
            if (bytes_read != sizeof(m)) {
                cerr << "Error reading from pipe" << endl;
                return 1;
            }

            cout << "[Child] Calculating " << m << "-th prime number..." << endl;
            int result = calculate_prime(m);

            cout << "[Child] Sending calculation result of prime(" << m << ")..." << endl;
            ssize_t bytes_written = write(child_to_parent[1], &result, sizeof(result));
            if (bytes_written != sizeof(result)) {
                cerr << "Error writing to pipe" << endl;
                return 1;
            }
        }
    }

    return 0;
}
