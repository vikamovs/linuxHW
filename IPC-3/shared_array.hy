#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <iostream>
#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdexcept>

class shared_array {
private:
	const char* name_;
    	size_t size_;
    	int* arr_;
    	int shm_fd_;
    	sem_t* sem_;

public:
    	shared_array(const char* name, size_t size) : name_(name), size_(size) {
        	if (size_ < 1 || size_ > 1000000000) {
			return;
        }

        shm_fd_ = shm_open(name_, O_CREAT | O_RDWR, 0666);
        if (shm_fd_ == -1) {
        	return;
	}

        if (ftruncate(shm_fd_, size_ * sizeof(int)) == -1) {
            	close(shm_fd_);
		return;
        }

        arr_ = static_cast<int*>(mmap(nullptr, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
        if (arr_ == MAP_FAILED) {
            	close(shm_fd_);
        	return;
	}

        std::string sem_name = std::string("/") + name_ + "_sem";
        sem_ = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
        if (sem_ == SEM_FAILED) {
            	munmap(arr_, size_ * sizeof(int));
            	close(shm_fd_);
		return;
        }
    }

	~shared_array() {
        	munmap(arr_, size_ * sizeof(int));
        	close(shm_fd_);
    	}

	int& operator[](size_t index) {
        	if (index >= size_) {
        		return arr_[0];
		}
        	return arr_[index];
    	}	

    	void lock() { 
		sem_wait(sem_); 
	}
   	void unlock() { 
		sem_post(sem_); 
	}

};

#endif
