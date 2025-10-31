#pragma once

struct Node {
    int data;
    Node* next;

    Node() : data(0), next(nullptr) {}
    Node(int value) : data(value), next(nullptr) {}
    Node(int value, Node* nextNode) : data(value), next(nextNode) {}
};

