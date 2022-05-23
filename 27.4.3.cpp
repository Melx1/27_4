#include <iostream>
#include <vector>
#include <string>
#include <ctime>

template<typename Type>
Type inputSomething (const std::string & previewText) {
    Type result;
    while (true) {
        std::cin.rdbuf()->pubsync();
        std::cin.clear();
        std::cout << previewText;
        std::cin >> result;
        if (!std::cin.fail()) {
            break;
        }
    }
    return result;
}

enum TaskType {type_A = 1, type_B, type_C};

class Task {
    TaskType type;
public:
    char get_type () {
        switch (type) {
            case type_A: return 'A';
            case type_B: return 'B';
            case type_C: return 'C';
            default: std::cout << "Type error." << std::endl;
        }
    }
    
    Task (unsigned command) {
        srand(command);
        rand(); //overclocking
        rand(); //overclocking
        type = TaskType(rand() % 3 + 1);
    }
};

class Worker {
    std::string name = "None";
    int id = 0;
    Task* currentTask;
public:
    void set_id (int number) {
        id = number;
    }

    void set_name () {
        name = "worker " + std::to_string(id);
    }

    void set_currentTask (Task* newTask) {
        currentTask = newTask;
    }

    std::string& get_name () {
        return name;
    }

    Task* get_currentTask () {
        return currentTask;
    }


};

class Manager {
    std::string name = "None";
    int id = 0;
    std::vector<Worker> workgroup;
    std::vector<Task> taskPool;
public:
    void set_id (int number) {
        id = number;
    }

    void set_name () {
        name = "manager " + std::to_string(id);
    }

    bool createSubTasks (unsigned bosCommand) {
        std::cout << name << " receive command " << bosCommand << std::endl;
        std::srand(bosCommand + id);
        int tuskNumber = rand() % workgroup.size() + 1;
        for (int i = 0; i < tuskNumber; ++i) {
            taskPool.emplace_back(bosCommand + id);
            bool accepted = false;
            for (auto& it : workgroup) {
                if (it.get_currentTask() == nullptr) {
                    it.set_currentTask(&taskPool.back());
                    accepted = true;
                    std::cout << "\t" << it.get_name() << " accept task " << it.get_currentTask()->get_type() << std::endl;
                    break;
                }
            }
            if (!accepted) {
                return false;
            }
        }
        return true;
    }

    void createWorkgroup (int number) {
        for (int i = 0; i < number; ++i) {
            workgroup.emplace_back();
            workgroup.back().set_id(i + 1);
            workgroup.back().set_name();
        }
    }
};

class Boss {
    std::vector<Manager> managerPool;
public:

    bool command () {
        auto command = inputSomething<unsigned> ("Input direction: ");
        bool anyoneAccepted = false;
        for (auto& it : managerPool) {
            anyoneAccepted |= it.createSubTasks(command);
        }
        return anyoneAccepted;
    }

    bool createCompany () {
        int managersNumber = inputSomething<int> ("Input the number of managers: ");
        int workerNumber = inputSomething<int> ("Input the number of workers in a workgroup: ");
        if (managersNumber <= 0 or workerNumber <= 0) {
            std::cout << "Your company is not working." << std::endl;
            return false;
        }
        for (int i = 0; i < managersNumber; ++i) {
            managerPool.emplace_back();
            managerPool.back().set_id(i + 1);
            managerPool.back().set_name();
            managerPool.back().createWorkgroup(workerNumber);
        }
        return true;
    }
};


int main () {
    Boss boss;
    boss.createCompany();
    while (boss.command()) {
        std::cout << std::endl;
    }
    return 0;
}
