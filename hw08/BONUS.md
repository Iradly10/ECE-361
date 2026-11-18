# Optional bonus — bst_remove

Start with the original seven-key tree:

```text
        27
       /  \
     14    35
    /  \  /  \
   10  19 31  42
```

## Remove 10 — leaf case
10 has no children, so free its node and set 14's left link to NULL.

```text
        27
       /  \
     14    35
       \  /  \
       19 31  42
```

## Remove 14 — one-child case
14 has only right child 19, so replace the link to 14 with a link to 19, then free 14.

```text
        27
       /  \
     19    35
          /  \
         31  42
```

## Remove 27 — two-children case
27 has two children. Its in-order successor is 31, the smallest key in its right subtree. Copy 31 into the root and remove the original node containing 31.

```text
        31
       /  \
     19    35
             \
             42
```

`bst_remove` removes one copy of the first matching key a normal search reaches. Missing keys and NULL trees return -1 without changing the tree. The implementation frees exactly one node per successful removal.

Run `make bonus` to compile and execute the removal tests with sanitizers.
