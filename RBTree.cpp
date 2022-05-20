#include <iostream>
#include <queue>

template <class T>
class RBTree {
private:
    enum Color {RED, BLACK};
    struct Node {
        T data;
        bool color;
        Node *left, *right, *parent;
        Node(T data) {
           this->data = data;
           left = right = parent = NULL;
           this->color = RED;
        }

    };
    Node *root;
    void rotateLeft(Node *&root, Node *&ptr) {
        Node *ptr_right = ptr->right;
        ptr->right = ptr_right->left;
        if (ptr->right != NULL) ptr->right->parent = ptr;
        ptr_right->parent = ptr->parent;
        if (ptr->parent == NULL) root = ptr_right;
        else if (ptr == ptr->parent->left)  ptr->parent->left = ptr_right;
        else ptr->parent->right = ptr_right;
        ptr_right->left = ptr;
        ptr->parent = ptr_right;
    }
    void rotateRight(Node *&root, Node *&ptr) {
        Node *ptr_left = ptr->left;
        ptr->left = ptr_left->right;
        if (ptr->left != NULL)  ptr->left->parent = ptr;
        ptr_left->parent = ptr->parent;
        if (ptr->parent == NULL) root = ptr_left;
        else if (ptr == ptr->parent->left)  ptr->parent->left = ptr_left;
        else ptr->parent->right = ptr_left;
        ptr_left->right = ptr;
        ptr->parent = ptr_left;
    }
    Node* BSTInsert(Node* root, Node *ptr){
        if (root == NULL) return ptr;
        if (ptr->data < root->data) {
            root->left  = BSTInsert(root->left, ptr);
            root->left->parent = root;
        }
        else if (ptr->data > root->data) {
            root->right = BSTInsert(root->right, ptr);
            root->right->parent = root;
        }
        return root;
    }
    Node* BSTReplace(Node *ptr){
        if(ptr->left == NULL && ptr->right == NULL)
            return NULL;
        if(ptr-> left != NULL && ptr->right !=NULL){
            Node *temp = ptr->right;
            while(temp->left != NULL)
                temp = temp -> left;
            return temp;
        }

        if(ptr->left != NULL)
            return ptr-> left;

        return ptr-> right;
    }
    void BSTDelete(Node* root,Node* ptr){
        Node *replace = BSTReplace(ptr);
        bool bothBlack = ((replace == NULL || replace -> color == BLACK)&&(ptr->color == BLACK));
        if(replace == NULL){
            if(ptr == root){
                root = NULL;
            }
            else{
                if(bothBlack)
                    fixBlack(root,ptr);
                else{
                    Node *sibling;
                    if(ptr->parent == NULL) 
                        sibling = NULL;
                    else if(ptr->parent->left == ptr)
                        sibling = ptr->parent->right;
                    else 
                        sibling = ptr->parent->left;
                    if(sibling != NULL)
                        sibling->color = RED;
                }

                if(ptr->parent->left == ptr)
                    ptr->parent->left = NULL;
                else 
                    ptr->parent->right = NULL;
            }
            delete ptr;
            return;
        }
        if(ptr->left == NULL || ptr->right == NULL){
            if(ptr == root){
                ptr->data = replace->data;
                ptr->left = ptr->right = NULL;
                delete replace;
            }
            else{
                if(ptr->parent->left == ptr)
                    ptr->parent->left = replace;
                else 
                    ptr->parent->right = replace;
                replace->parent = ptr->parent;
                delete ptr;
                if(bothBlack)
                    fixBlack(root,replace);
                else
                    replace->color = BLACK;
                
            }
            return;
        }
        T temp = replace->data;
        replace->data = ptr->data;
        ptr->data = temp;
        std::cout<<replace->data <<" "<<ptr->data<<std::endl;
        BSTDelete(root,replace);
    }
    void inorderHelper(Node *root) {
        if (root == NULL)  return;
        inorderHelper(root->left);
        std::cout << root->data << "  ";
        inorderHelper(root->right);
    }
    void fixBlack(Node *&root, Node *&ptr){
        Node *parent = NULL;
        while(ptr!=root){
            parent = ptr->parent;
            Node *sibling;
            if(parent == NULL) 
                sibling = NULL;
            else if(parent->left == ptr)
                sibling = parent->right;
            else 
                sibling = parent->left;
            if(sibling == NULL)
                ptr = parent;
            else{
                if(sibling->color == RED){
                    parent->color = RED;
                    sibling->color = BLACK;
                    if(parent->left == sibling)
                        rotateRight(root,parent);
                    else 
                        rotateLeft(root,parent);
                }
                else{
                    if( sibling->left != NULL && sibling->left->color == RED) {
                        if(parent->left == sibling){
                            sibling->left->color = sibling->color;
                            sibling->color = parent-> color;
                            rotateRight(root,parent);
                        }
                        else{
                            sibling->left->color = parent->color;
                            rotateRight(root,sibling);
                            rotateLeft(root,parent);
                        }
                        parent->color=BLACK;
                        break;
                    }
                    else if (sibling->right != NULL && sibling->right->color == RED){
                        if(parent->left == sibling){
                            sibling->right->color = parent->color;
                            rotateLeft(root,sibling);
                            rotateRight(root,parent);
                            
                        }
                        else{
                            sibling->right->color = sibling->color;
                            sibling->color = parent-> color;
                            rotateLeft(root,parent);
                        }
                        parent->color=BLACK;
                        break;
                    }
                    else{
                        sibling->color = RED;
                        if(parent->color == BLACK)
                            ptr=parent;
                        else {
                            parent->color = BLACK;
                            break;
                        }
                    }
                }
            }
        }
    }
    void fixRed(Node *&root, Node *&ptr) {
        Node *parent_ptr = NULL;
        Node *grand_parent_ptr = NULL;
        while ((ptr != root) && (ptr->color != BLACK) &&  (ptr->parent->color == RED)) {
            parent_ptr= ptr->parent;
            grand_parent_ptr = ptr->parent->parent;
            if (parent_ptr == grand_parent_ptr->left) {
                Node *uncle_ptr = grand_parent_ptr->right;
                if (uncle_ptr != NULL && uncle_ptr->color == RED)
                {
                    grand_parent_ptr->color = RED;
                    parent_ptr->color = BLACK;
                    uncle_ptr->color = BLACK;
                    ptr = grand_parent_ptr;
                }
                else {
                    if (ptr == parent_ptr->right)  {
                        rotateLeft(root, parent_ptr);
                        ptr = parent_ptr;
                        parent_ptr = ptr->parent;
                    }
                    rotateRight(root, grand_parent_ptr);
                    std::swap(parent_ptr->color, grand_parent_ptr->color);
                    ptr = parent_ptr;
                }
            }
            else {
                Node *uncle_ptr = grand_parent_ptr->left;
                if ((uncle_ptr != NULL) && (uncle_ptr->color == RED))  {
                    grand_parent_ptr->color = RED;
                    parent_ptr->color = BLACK;
                    uncle_ptr->color = BLACK;
                    ptr = grand_parent_ptr;
                }
                else{
                    if (ptr == parent_ptr->left) {
                        rotateRight(root, parent_ptr);
                        ptr = parent_ptr;
                        parent_ptr = ptr->parent;
                    }
                    rotateLeft(root, grand_parent_ptr);
                    std::swap(parent_ptr->color, grand_parent_ptr->color);
                    ptr = parent_ptr;
                }
            }
        }
        root->color = BLACK;
    }
public:
    RBTree() { 
        root = NULL; 
    }
    
    void insert(const T& data) {
        Node *pt = new Node(data);
        root = BSTInsert(root, pt);
        fixRed(root, pt);
    }
    
    void deleteElement(const T& data) {
        if(root == NULL) return;
        Node *ptr = root;
        while(ptr != NULL){
            if(data < ptr->data){
                if(ptr->left == NULL){
                    std::cout<<"not in tree"<<std::endl;
                    return;
                }
                ptr = ptr->left;
            }
            else if(data > ptr->data){
                if(ptr->right == NULL){
                    std::cout<<"not in tree"<<std::endl;
                    return;
                }
                ptr = ptr->right;
            }
            else{
                break;
            }
        }
        BSTDelete(root,ptr);
    }
    void inorder(){inorderHelper(root);}
    void levelorder(){
        if (root == NULL) return;
        std::queue<Node *> q;
        q.push(root);
        while (!q.empty()) {
            Node *temp = q.front();
            std::cout << temp->data << "  ";
            q.pop();
            if (temp->left != NULL) q.push(temp->left);
            if (temp->right != NULL) q.push(temp->right);
        }
    }
};
