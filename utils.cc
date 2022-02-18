#include <iostream>
#include <stdio.h>
#include <string.h>

#include "utils.h"

enum ParserState {
  Init,
  Parsing,
  Done,
};

int pow(int n, int e) {
  int res = 1;
  while (e > 0) {
    res *= n;
    e--;
  }
  return res;
}

int strToInt(char *buf, int len) {
  int curr = 0;
  int res = 0;
  int last = 0;

  while (buf[curr] != '\0') {
    curr++;
  }

  last = --curr;

  while (curr >= 0) {
    res = res + ((buf[curr] - 48) * pow(10, last - curr));
    curr--;
  }
  return res;
}

void printList(const Node *list) {
  std::cout << "[";

  while (list) {
    std::cout << list->val;
    if (list->next) {
      std::cout << ", ";
    }
    list = list->next;
  }
  std::cout << "]";
}

Node *parseList(const std::string str) {
  auto current = 0;
  auto state = ParserState::Init;
  char buffer[32] = {'\0'};
  char nextBufferChar = 0;
  Node *head = nullptr, *tail = nullptr;
  while (state != ParserState::Done) {
    auto c = str[current++];
    switch (state) {
    case ParserState::Init:
      if (c == '[') {
        state = ParserState::Parsing;
      }
      break;

    case ParserState::Parsing:
      if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
          c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
        buffer[nextBufferChar++] = c;
      } else if (c == ',' || c == ']') {
        auto val = strToInt(buffer, 32);
        auto node = new Node{val};
        memset(buffer, '\0', 32);
        nextBufferChar = 0;

        // add character to list
        if (head == nullptr) {
          head = tail = node;
        } else {
          tail->next = node;
          tail = node;
        }

        if (c == ']') {
          state = ParserState::Done;
        }
      }
      break;
    case ParserState::Done:
      break;
    }
  }
  return head;
}
