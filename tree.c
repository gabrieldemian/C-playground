// Simple implementation of a non-balanced binary tree.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the memory layout of structs in C are in the same order as they are defined.
// size without padding: 20 bytes, with padding: 24 bytes.
struct tnode {
  int value; // 4 bytes + (padding of 4 bytes if height were declared at the
             // end)

  // a height is determined by it's biggest sub-tree either left or right
  int height; // 4 bytes

  // no padding here because value + height = 8 bytes
  struct tnode *left;  // 8 bytes (64 bit CPU)
  struct tnode *right; // 8 bytes
  // if I declare height here, then `value` would get padding of + 4 bytes
};

// get height of a node
int height(struct tnode *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

// get balance difference from the left side perspective
int get_balance_diff(struct tnode *node) {
  if (node == NULL)
    return 0;
  return height(node->left) - height(node->right);
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

struct tnode *right_rotate(struct tnode *y) {
  struct tnode *x = y->left;
  struct tnode *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x; // new root
}

struct tnode *left_rotate(struct tnode *x) {
  struct tnode *y = x->right;
  struct tnode *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y; // new root
}

// Insert a new value into the node
void insert(struct tnode **node_ptr, int value) {
  struct tnode *node = *node_ptr;

  if (node == NULL) {
    *node_ptr = new_node(value);
    return;
  }

  if (value < node->value) {
    insert(&node->left, value);
  }

  if (value > node->value) {
    insert(&node->right, value);
  }

  node->height = 1 + max(height(node->left), height(node->right));

  int balance = get_balance_diff(node);

  // if unbalanced, there are 4 cases

  if (balance > 1) {
    if (value < node->left->value) {
      *node_ptr = right_rotate(node);
    } else {
      node->left = left_rotate(node->left);
      *node_ptr = right_rotate(node);
    }
  } else if (balance < -1) {
    if (value > node->right->value) {
      *node_ptr = left_rotate(node);
    } else {
      node->right = right_rotate(node->right);
      *node_ptr = left_rotate(node);
    }
  } else {
    // no rotation needed, just update the current root
    *node_ptr = node;
  }
}

// get a node in the tree where it's value equals `value`
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

void free_tree(struct tnode *node) {
  if (node == NULL)
    return;
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

//     3
//   /   \
//  2     5
// / \   / \
// 2   2 2   5
void print_tree(struct tnode *node) {
  if (node == NULL) {
    printf("(empty tree)\n");
    return;
  }

  // height of the entire tree
  int th = max(height(node->left), height(node->right));

  // height of the tree in the output, including lines of edges
  int h = ((th + 1) << 1) - 1;
  int total_nodes = th << 1;
  printf("total %d\n", total_nodes);
  int w = (1 << th) + th;
  if (w < 11)
    w = 11;

  // alloc a buffer of chars w * h + (\n * h)
  int buf_size = w * h + h;
  char *buf = (char *)malloc(buf_size);
  memset(buf, '.', buf_size);

  // place \n in the right places
  for (int i = 0; i < h; i++) {
    int new_line_i = i * (w + 1) + w;
    buf[new_line_i] = '\n';

    // numbers are always on even rows
    if (i % 2 != 0) {
      continue;
    }

    int tree_level = i >> 1;

    // how many numbers in the current  line
    int per_row = 1 << tree_level; // 2^tree_level
    int spacing = w / (per_row + 1);
    int middle = w / 2;
    printf("height %d per_line %d\n", tree_level, per_row);

    // in each row there can be many numbers
    for (int j = 0; j < per_row; j++) {
      int number_pos = (j + 1) * spacing;
      int abs_pos = i * (w + 1) + number_pos;
      if (abs_pos < buf_size) {
        buf[abs_pos] = '1';
      }
    }
  }

  printf("%s", buf);
  free(buf);
}

int main() {
  struct tnode *node = new_node(1);

  for (int i = 0; i < 4; i++) {
    insert(&node, i + 1);
  }
  // height 2 = 7 width

  print_tree(node);
  // printf("\n");
  // printf("root v: %d\n", node->value);
  // printf("found 3?: %p\n", get(node, 3));
  // printf("found 2?: %p\n", get(node, 2));

  free_tree(node);
  return 0;
}
