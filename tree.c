// Simple implementation of a non-balanced binary tree.

#include <stdio.h>
#include <stdlib.h>

// the memory layout of structs in C are in the same order as they are defined.
// size without padding: 20 bytes, with padding: 24 bytes.
struct tnode {
  int value;  // 4 bytes + (padding of 4 bytes if height were declared at the
              // end)
  int height; // 4 bytes
  // no padding here because value + height = 8 bytes
  struct tnode *left;  // 8 bytes (64 bit CPU)
  struct tnode *right; // 8 bytes
  // if I declare height here, then `value` would get padding of + 4 bytes.
};

// get height of a node
int height(struct tnode *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

// get maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }

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
  node->height = 1;
  return node;
}

void insert(struct tnode *node, int value) {
  if (node == NULL) {
    node = new_node(value);
    return;
  }

  // continue on the LEFT side
  if (value < node->value) {
    if (node->left == NULL) {
      node->left = new_node(value);
    } else {
      insert(node->left, value);
    }
  }

  // continue on the RIGHT side
  if (value > node->value) {
    if (node->right == NULL) {
      node->right = new_node(value);
    } else {
      insert(node->right, value);
    }
  }
  node->height = 1 + max(height(node->left), height(node->right));
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

  printf("root value: %d height: %d \n", node->value, node->height);
  printf("root left: %d height: %d\n", node->left->value, node->left->height);
  printf("root left left: %d height: %d\n", node->left->left->value,
         node->left->left->height);
  printf("root right: %d height: %d\n", node->right->value,
         node->right->height);

  printf("found 17?: %p\n", get(node, 17));
  printf("found 20?: %p\n", get(node, 20));

  free_tree(node);
  return 0;
}
