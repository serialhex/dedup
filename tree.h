
#include <stdint.h>
#include <stdlib.h>

#include "fibb-hash.h"

#pragma once

typedef struct Binary_Tree {
  struct Binary_Tree* left;
  struct Binary_Tree* right;
  struct Binary_Tree* parent;
  File_Hash data;
} Binary_Tree;

// a single node is a binary tree, just a lonly one. so this is simple.
Binary_Tree* bt_make() {
  Binary_Tree* tree = (Binary_Tree*)malloc(sizeof(Binary_Tree));
  tree->data = null_file;
  tree->left = NULL;
  tree->right = NULL;
  tree->parent = NULL;
  return tree;
}

// The beginning of the print functions
void __print_binary_tree_helper(const Binary_Tree* tree, uint8_t depth);

void print_binary_tree(const Binary_Tree* tree) {
  printf("root:\n");
  __print_binary_tree_helper(tree, 0);
  printf("\n");
}

Binary_Tree* bt_init(File_Hash data) {
  Binary_Tree* tree = bt_make();
  tree->data = data;
  return tree;
}

// recursive implementation, because recursion is sexy!
void bt_destroy(Binary_Tree* tree) {
  if (tree->left) {
    bt_destroy(tree->left);
  }
  if (tree->right) {
    bt_destroy(tree->right);
  }
  free(tree);
}

// finds the File_Hash struct in a binary tree
File_Hash* bt_find(Binary_Tree* tree, hash_value value) {
  while (tree != NULL) {
    hash_value cur_val = tree->data.hash;
    if (value < cur_val) {
      tree = tree->left;
    } else if (value > cur_val) {
      tree = tree->right;
    } else {
      return &tree->data;
    }
  }
  return NULL;
}

Binary_Tree* bt_find_last(Binary_Tree* tree, File_Hash* data) {
  Binary_Tree *cur = tree, *prev = NULL;
  const hash_value val = data->hash;
  while (cur != NULL) {
    prev = cur;
    if (val < cur->data.hash) {
      cur = cur->left;
    } else if (val > cur->data.hash) {
      cur = cur->right;
    } else {
      return cur;
    }
  }
  return prev;
}

// adding stuff to a binary tree
bool __bt_add_child(Binary_Tree* parent, Binary_Tree* child);

// see comment at __bt_add_child
bool bt_add(Binary_Tree* tree, File_Hash* data) {
  // check for fresh tree
  if (files_eq(tree->data, null_file)) {
    tree->data = *data;
    return true;
  }

  Binary_Tree* prev = bt_find_last(tree, data);
  Binary_Tree* mew = bt_init(*data);
  return __bt_add_child(prev, mew);
}

// misc things down here

void __ins_tabs(uint8_t tabs) {
  for (uint8_t i = 0; i < tabs; i++) {
    printf("       ");
  }
}

void __print_binary_tree_helper(const Binary_Tree* tree, uint8_t depth) {
  // __ins_tabs(depth);
  printf("data: ");
  print_file_hash(tree->data);
  printf("\n");

  __ins_tabs(depth);
  printf("left:  ");
  if (tree->left) {
    __print_binary_tree_helper(tree->left, depth + 1);
  } else {
    printf("(empty)");
  }
  printf("\n");

  __ins_tabs(depth);
  printf("right: ");
  if (tree->right) {
    __print_binary_tree_helper(tree->right, depth + 1);
  } else {
    printf("(empty)");
  }
}

void print_dup(File_Hash parent, File_Hash child) {
  FILE* file = fopen("dup-files.txt", "a");
  if (file) {
    fprintf(
      file,
      "%s%s\n",
      str_fhash(parent),
      str_fhash(child));
    fclose(file);
  } else {
    fprintf(stderr, "Couldn't open \"dup-files.txt\" for some reason...\n");
  }
}

// Maybe I should add a check for the size, as the hash values may be the same...
bool __bt_add_child(Binary_Tree* parent, Binary_Tree* child) {
  const hash_value phash = parent->data.hash;
  const hash_value chash = child->data.hash;
  if (chash < phash) {
    parent->left = child;
  } else if (chash > phash) {
    parent->right = child;
  } else {
    print_dup(parent->data, child->data);
    fprintf(stderr,
      "There's a child with the same hash already in the tree!!!\n%s\n%s\n",
      str_file_hash(parent->data),
      str_file_hash(child->data));
    return false;
  }
  child->parent = parent;
  return true;
}
