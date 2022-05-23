#include <iostream>
#include <random>
#include <chrono>

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

class Node;
class Forest;


class Node {
protected:
    Node* parentNode = nullptr;
    Node* leftNode   = nullptr;
    Node* rightNode  = nullptr;
    Node* childNode  = nullptr; //the leftmost child

    bool isLiving = false;
    std::string name;
public:

    //setters
    void set_parentNode(Node* ptr) {
        parentNode = ptr;
    }
    void set_leftNode(Node* ptr) {
        leftNode = ptr;
    }
    void set_rightNode(Node* ptr) {
        rightNode = ptr;
    }
    void set_childNode(Node* ptr) {
        childNode = ptr;
    }

    void set_name (){
        auto newName = inputSomething<std::string> ("Input the elf name: ");
        if (newName == "None") {
            isLiving = false;
            name = "";
        }
        else {
            isLiving = true;
            name = newName;
        }
    }

    //getters
    Node* get_parentNode () {
        return parentNode;
    }
    Node* get_leftNode () {
        return leftNode;
    }
    Node* get_rightNode () {
        return rightNode;
    }
    Node* get_childNode () {
        return childNode;
    }

    bool get_isLiving () {
        return isLiving;
    }

    std::string get_name () {
        return name;
    }

    //default constructor
    Node () = default;

    //constructor create the child node (front insert)
    Node (Node* parentPtr) {
        if (parentPtr != nullptr) {
            parentNode = parentPtr;
            childNode = nullptr;
            leftNode = nullptr;
            if (parentPtr->get_childNode() == nullptr) {
                parentPtr->set_childNode(this);
                rightNode = nullptr;
            }
            else {
                rightNode = parentPtr->get_childNode();
                parentPtr->set_childNode(this);
                rightNode->set_leftNode(this);
            }
        }
    }

    //the constructor copies the node and pastes it to the right of the original
    Node (Node& original) {
        parentNode = original.get_parentNode();
        leftNode = &original;
        rightNode = original.get_rightNode();
        childNode = nullptr;

        original.set_rightNode(this);
        if (rightNode != nullptr) {
            rightNode->set_leftNode(this);
        }
    }
};


class Forest {
private:
    //default count node (trees, branches) in forest;
    static constexpr int defaultCountTrees = 5;
    static constexpr int defaultMinBigBranch = 3;
    static constexpr int defaultMaxBigBranch = 5;
    static constexpr int defaultMinMediumBranch = 2;
    static constexpr int defaultMaxMediumBranch = 3;

    Node* beginPtr = nullptr;
    Node* endPtr = nullptr;

    void pop() {
        if (endPtr == nullptr) return;
        Node* deletedNode = endPtr;
        if (endPtr->get_leftNode() != nullptr) {
            endPtr = endPtr->get_leftNode();
            endPtr->set_rightNode(nullptr);
            while (endPtr->get_rightNode() != nullptr or endPtr->get_childNode() != nullptr) {
                if (endPtr->get_rightNode() != nullptr) {
                    endPtr = endPtr->get_rightNode();
                    continue;
                }
                endPtr = endPtr->get_childNode();
            }
        }
        else if (endPtr->get_parentNode() != nullptr){
            endPtr = endPtr->get_parentNode();
            endPtr->set_childNode(nullptr);
        }
        else {
            beginPtr = nullptr;
            endPtr = nullptr;
        }
        delete deletedNode;
    }

    void clean () {
        while (endPtr != nullptr) {
            this->pop();
        }
    }

public:
    ~Forest () {
        this->clean();
    };

    void createRandomForest (int countTrees = defaultCountTrees,
                       int minBigBranch = defaultMinBigBranch,
                       int maxBigBranch = defaultMaxBigBranch,
                       int minMediumBranch = defaultMinMediumBranch,
                       int maxMediumBranch = defaultMaxMediumBranch){
        if (maxBigBranch < minBigBranch) {
            maxBigBranch = minBigBranch;
        }
        if (maxMediumBranch < minMediumBranch) {
            maxMediumBranch = minBigBranch;
        }

        std::mt19937 randGenerator (std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> randCountBigBranch(minBigBranch, maxBigBranch);
        std::uniform_int_distribution<int> randCountMediumBranch(minMediumBranch, maxMediumBranch);

        this->clean();
        for (int i = 0; i < countTrees; ++i) {
            //create tree
            Node* treePtr;
            if (i == 0) {
                treePtr = new Node;
                beginPtr = treePtr;
            }
            else {
                treePtr = new Node(endPtr);
            }
            endPtr = treePtr;

            //create branches of the tree
            int countBigBranch = randCountBigBranch(randGenerator);
            for (int bigBranchCounter = 0; bigBranchCounter < countBigBranch; ++bigBranchCounter) {
                Node* bigBranchPtr = new Node(treePtr);
                endPtr = bigBranchPtr;
                int countMediumBranch = randCountMediumBranch(randGenerator);
                for (int mediumBranchCounter = 0; mediumBranchCounter < countMediumBranch; ++mediumBranchCounter){
                    endPtr = new Node(bigBranchPtr);
                }
            }
        }
    }

    Node* nextNode (Node* currentNodePtr = nullptr) {
         if (currentNodePtr == nullptr) {
             return beginPtr;
         }

         if (currentNodePtr->get_childNode() != nullptr) {
             return currentNodePtr->get_childNode();
         }
         if (currentNodePtr->get_rightNode() != nullptr) {
             return currentNodePtr->get_rightNode();
         }

         Node* bufferPtr = currentNodePtr;
         while (bufferPtr->get_parentNode() != nullptr) {
             bufferPtr = bufferPtr->get_parentNode();
             if (bufferPtr->get_rightNode() != nullptr) {
                 return bufferPtr->get_rightNode();
             }
         }

        return nullptr;
    }

    void filling () {
        Node* ptr = nullptr;
        while (true) {
            ptr = this->nextNode(ptr);
            if (ptr == nullptr) break;
            if (ptr->get_parentNode() != nullptr) {
                ptr->set_name();
            }
        }
    }

    Node* searchElf(std::string& elfName) {
        Node* elfPtr = nullptr;
        while (true) {
            elfPtr = this->nextNode(elfPtr);
            if (elfPtr == nullptr) break;
            if (!(elfPtr->get_isLiving())) continue;
            if (elfPtr->get_name() == elfName) break;
        }
        return elfPtr;
    }
};

int main() {
    Forest forest;
    forest.createRandomForest();
    forest.filling();

    //search elf
    auto elfName = inputSomething<std::string> ("Input the elf name for search: ");
    Node * elfPtr = forest.searchElf(elfName);
    if (elfPtr == nullptr) {
        std::cout << elfName << " is not found." << std::endl;
        return 0;
    }

    //go top branch
    Node* topBranchPtr = elfPtr;
    while (topBranchPtr->get_parentNode()->get_parentNode() != nullptr) {
        topBranchPtr = topBranchPtr->get_parentNode();
    }

    //counter
    int elfCounter = 0;
    Node* ptr = topBranchPtr;
    while (true) {
        if (ptr == nullptr or ptr->get_leftNode() == topBranchPtr) break;
        if (ptr->get_isLiving()) elfCounter++;
        ptr = forest.nextNode(ptr);
    }

    std::cout << "The total number of elves living with " << elfName << " on one large branch: " <<  --elfCounter << std::endl;

    return 0;
}
