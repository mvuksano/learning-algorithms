struct Node {
  int val;
  Node *next;
};

Node *parseList(const std::string str);
void printList(const Node *list);
