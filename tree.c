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

  int th = node->height;
  int h = (th << 1) - 1;
  int w = (1 << th);
  if (w % 2 == 0)
    w++;

  int buf_size = w * h + h + 1;
  char *buf = malloc(buf_size);
  memset(buf, '.', buf_size);
  buf[buf_size - 1] = '\0';

  // place newlines
  for (int i = 0; i < h; i++) {
    buf[i * (w + 1) + w] = '\n';
  }

  // single array that can hold a complete tree
  struct tnode *nodes[(1 << 4) + 1];
  int front = 0, rear = 0;

  // BFS with position tracking
  // also it is needed to keep track of null nodes to properly position them in
  // the buffer.
  nodes[rear++] = node;
  int tree_level = 0;

  while (front < rear && tree_level < th) {
    int display_row = tree_level * 2;
    int per_row = 1 << tree_level;
    int level_span = 1 << (th - tree_level);

    // loop over numbers in the row "i"
    for (int j = 0; j < per_row && front < rear; j++) {
      struct tnode *current = nodes[front++];

      if (current != NULL) {
        int left_edge = j * level_span;
        int right_edge = (j + 1) * level_span - 1;
        int number_pos = (left_edge + right_edge) / 2;
        int abs_pos = display_row * (w + 1) + number_pos;

        if (abs_pos < buf_size) {
          char num_str[12];
          snprintf(num_str, sizeof(num_str), "%d", current->value);
          buf[abs_pos] = num_str[0];
        }

        // enqueue children
        if (rear < (1 << 4)) {
          nodes[rear++] = current->left;
          nodes[rear++] = current->right;
        }
      } else {
        // enqueue NULL placeholders
        if (rear < (1 << 4)) {
          nodes[rear++] = NULL;
          nodes[rear++] = NULL;
        }
      }
    }

    tree_level++;
  }

  printf("%s\n", buf);
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
