// Simple implementation of a non-balanced binary tree.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES (1 << 4) // = 16

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
  } else if (value > node->value) {
    insert(&node->right, value);
  } else {
    return;
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

// .......3.........
// .................
// ...1.......7.....
// .................
// .0...2...5...8...
// .................
// ........4.6...9..
void print_tree(struct tnode *node) {
  if (node == NULL) {
    printf("(empty tree)\n");
    return;
  }

  const int th = node->height;
  const int h = (th << 1) - 1;
  const int w = (1 << th) | 1; // force odd using OR
  const int buf_size = (w + 1) * h;
  char *const buf = malloc(buf_size);
  buf[buf_size - 1] = '\0';

  for (int i = 0; i < h; i++) {
    char *row_start = buf + i * (w + 1);
    memset(row_start, '.', w);
    row_start[w] = '\n';
  }

  struct tnode *nodes[MAX_NODES];
  int front = 0, rear = 0;

  // single-pass BFS with printing
  nodes[rear++] = node;
  int level_start = 0;
  int level_end = 1;

  for (int i = 0; i < th; i++) {
    const int display_row = i << 1;        // tree_level * 2
    const int per_row = 1 << i;            // 2^i
    const int level_span = 1 << (th - i);  // 2^(th-i)
    const int half_span = level_span >> 1; // level_span / 2

    int next_start = level_end;
    int next_end = next_start;

    // iterate on nodes of each row "i"
    for (int j = 0; j < per_row; j++) {
      const int node_idx = level_start + j;
      if (node_idx >= level_end)
        break;

      struct tnode *current = nodes[node_idx];

      if (current != NULL) {
        const int number_pos = (j * level_span) + half_span;
        const int abs_pos = display_row * (w + 1) + number_pos;

        if (abs_pos < buf_size) {
          buf[abs_pos] = '0' + (current->value % 10);
        }

        // enqueue children
        if (next_end < MAX_NODES - 1) {
          nodes[next_end++] = current->left;
          nodes[next_end++] = current->right;
        }
      } else {
        // NULL nodes
        if (next_end < MAX_NODES - 1) {
          nodes[next_end++] = NULL;
          nodes[next_end++] = NULL;
        }
      }
    }

    level_start = next_start;
    level_end = next_end;
  }

  printf("%s", buf);
  free(buf);
}

int main() {
  struct tnode *node = NULL;

  for (int i = 0; i < 10; i++) {
    insert(&node, i);
  }

  print_tree(node);

  free_tree(node);
  return 0;
}
