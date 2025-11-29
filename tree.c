// Simple implementation of a non-balanced binary tree.

#include <stdio.h>
#include <stdlib.h>

// the memory layout of structs in C are in the same order as they are defined.
// size without padding: 20 bytes, with padding: 24 bytes.
struct tnode {
  int value;           // 4 bytes + padding of 4 bytes
  struct tnode *left;  // 8 bytes (64 bit CPU)
  struct tnode *right; // 8 bytes
};

// Return a pointer to tnode on the heap.
struct tnode *new_node(int value) {
  struct tnode *node = malloc(sizeof(struct tnode));
  if (node == NULL) {
    printf("failed to alloc\n");
    exit(1);
  }
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void insert(struct tnode *root, int value) {
  if (root == NULL) {
    root = new_node(value);
    return;
  }

  // continue on the LEFT side
  if (value < root->value) {
    if (root->left == NULL) {
      root->left = new_node(value);
      return;
    }
    insert(root->left, value);
  }

  // continue on the RIGHT side
  if (value > root->value) {
    if (root->right == NULL) {
      root->right = new_node(value);
      return;
    }
    insert(root->right, value);
  }
  // don't insert duplicates
}

struct tnode *get(struct tnode *node, int value) {
  if (node == NULL) {
    return NULL;
  }
  if (value < node->value) {
    return get(node->left, value);
  }
  if (value > node->value) {
    return get(node->right, value);
  }
  return node;
}

void free_tree(struct tnode *root) {
  if (root == NULL)
    return;
  free_tree(root->left);
  free_tree(root->right);
  free(root);
}

int main() {
  struct tnode *node = new_node(19);
  insert(node, 18);
  insert(node, 17);
  insert(node, 20);
  insert(node, 21);

  printf("root value: %d\n", node->value);
  printf("root left: %d\n", node->left->value);
  printf("root left left: %d\n", node->left->left->value);
  printf("root right: %d\n", node->right->value);

  printf("found 17?: %p\n", get(node, 17));
  printf("found 20?: %p\n", get(node, 20));

  free_tree(node);
  return 0;
}
