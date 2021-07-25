#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//максимальная длина строки
enum {
    MAX_NODE_STR_LEN = 128,
};

enum NODETYPE {
    TYPE_NUM, TYPE_VAR, TYPE_SIGN, TYPE_FUNCTION,
};

class Node {
    public: 
    Node (); //конструктор 
    Node (char* dt);
    Node (Node* lch, Node* rch, char* dt, int len, Node* pr, NODETYPE type);

    void recursivePrint();

    char* data_;
    Node* left_child_;
    Node* right_child_;
    Node* parent_;
    NODETYPE type_;
};
//
Node::Node():
    data_           ((char*) calloc (MAX_NODE_STR_LEN, sizeof(char))),
    left_child_     (NULL),
    right_child_    (NULL),
    parent_         (NULL)
    {
        
    }
Node::Node(char* dt):
    data_           (dt),
    left_child_     (NULL),
    right_child_    (NULL),
    parent_         (NULL)
    {
        
    }

Node::Node(Node* lch, Node* rch, char* dt, int len, Node* pr, NODETYPE type):
    data_           ((char*) calloc (MAX_NODE_STR_LEN, sizeof(char))),
    left_child_     (lch),
    right_child_    (rch),
    parent_         (pr),
    type_           (type)
    {
        strncpy(data_, dt, len);
    }

// функция для рекурсивной печати дерева
void Node::recursivePrint () {
    printf("%s\n", this->data_);
    if (this->left_child_) {
        this->left_child_->recursivePrint();
    }
    if (this->right_child_) {
        this->right_child_->recursivePrint();
    }
}



class Differentator {
    public:
    Differentator();

    Node* getSumSub();
    Node* getMultDiv();
    Node* getPow();
    Node* getFunctions();
    Node* getBraces();
    Node* getOperand();

    char* expression_;
    int ctr_; //хранит номер символа в выражении
};

Differentator::Differentator():
    expression_ ((char*) calloc (MAX_NODE_STR_LEN, sizeof(char))),
    ctr_         (0)
    {

    }    

Node* Differentator::getSumSub(){
    Node* left_child = getMultDiv();
    while(expression_[ctr_]=='+' || expression_[ctr_]=='-') {
        char* sign = &expression_[ctr_];
        ctr_++;

        Node* right_child = getMultDiv();

        left_child = new Node(left_child, right_child, sign, 1, NULL, TYPE_SIGN);
    }
    return left_child;
}
Node* Differentator::getMultDiv(){

    Node* left_child = getPow();
    

    while(expression_[ctr_]=='*' || expression_[ctr_]=='/') {
    
        char* sign = &expression_[ctr_];
        ctr_++;

        Node* right_child = getPow();
        
        left_child = new Node(left_child, right_child, sign, 1, NULL, TYPE_SIGN);

    }
    return left_child;
}
Node* Differentator::getPow(){
    Node* left_child = getFunctions();
    while(expression_[ctr_]=='^') {

        char* sign = &expression_[ctr_];
        ctr_++;

        Node* right_child = getFunctions();
        
        left_child = new Node(left_child, right_child, sign, 1, NULL, TYPE_SIGN);
    }
    return left_child;
}
Node* Differentator::getFunctions() {
    
    return getOperand();
}
// Node* Differentator::getBraces(){
//     getOperand();
//     while() {
//         getOperand();
//     }
// }
Node* Differentator::getOperand(){
    int len;
    char* operand = (char*) calloc (MAX_NODE_STR_LEN, sizeof(char));
    sscanf(&expression_[ctr_], "%[0-9a-zA-Z]%n", &operand, &len);
    ctr_ += len;
    NODETYPE type = TYPE_VAR;
    if (operand[0] >= '0' && operand[0] <= '9') {
        type = TYPE_NUM;
    }
    Node* node = new Node(NULL, NULL, (char*)(&operand), len, NULL, type);
    return node;
}

int main() {
    char expression[30]; //здесь хранится выражение
    int length; 
    scanf("%s%n", expression, &length); 

    Differentator* diff = new Differentator();
    diff -> expression_ = expression;

    Node* root = diff -> getSumSub(); //возвращает корень собранного дерева выражения

    root -> recursivePrint();

    //printf("%s %d", root->data_, length);
    //printf("%d", expression);
    return 0;
}