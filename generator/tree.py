"""
Create a balanced binary search tree (BST) from a list of key-value pairs,
and convert it to an array representation.

>>> bst = build_balanced_bst(list({4: 'four', 8: 'eight', 15: 'fifteen', 16: 'sixteen', 23: 'twenty-three', 42: 'fourty-two'}.items()))
>>> bst_to_array_zero_indexed(bst)
[(16, 'sixteen'),
    (8, 'eight'),
    (42, 'fourty-two'),
    (4, 'four'),
    (15, 'fifteen'),
    (23, 'twenty-three')]
>>> search_bst_array(bst_to_array_zero_indexed(bst), 15)
'fifteen'
"""

from dataclasses import dataclass
from typing import List, Optional, Tuple


@dataclass
class TreeNode:
    key: int
    value: str
    left: Optional["TreeNode"] = None
    right: Optional["TreeNode"] = None


def build_balanced_bst(data: List[Tuple[int, str]]) -> Optional[TreeNode]:
    """Create a balanced BST from a list of key-value pairs."""
    return _build_balanced_bst(sorted(data))


def _build_balanced_bst(pairs: List[Tuple[int, str]]) -> Optional[TreeNode]:
    if not pairs:
        return None
    mid = len(pairs) // 2
    key, value = pairs[mid]
    root = TreeNode(key, value)
    root.left = _build_balanced_bst(pairs[:mid])
    root.right = _build_balanced_bst(pairs[mid + 1 :])
    return root


def bst_to_array_zero_indexed(
    root: Optional[TreeNode],
) -> List[Optional[Tuple[int, str]]]:
    """Convert a balanced BST to an array representation."""
    result: List[Optional[Tuple[int, str]]] = []

    def insert(node: Optional[TreeNode], index: int):
        if node is None:
            return
        while len(result) <= index:
            result.append(None)
        result[index] = (node.key, node.value)
        insert(node.left, 2 * index + 1)
        insert(node.right, 2 * index + 2)

    insert(root, 0)
    return result


def search_bst_array(tree: List[Optional[Tuple[int, str]]], key: int) -> Optional[str]:
    """Search for a key in a BST represented as an array."""
    index = 0
    while index < len(tree):
        node = tree[index]
        if node is None:
            return None
        node_key, node_value = node
        if key == node_key:
            return node_value
        elif key < node_key:
            index = 2 * index + 1
        else:
            index = 2 * index + 2
    return None
