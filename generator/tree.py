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

class TreeNode:
    def __init__(self, key: int, value: str):
        self.key = key
        self.value = value
        self.left = None
        self.right = None

def build_balanced_bst(pairs: list[tuple[int, str]]) -> TreeNode:
    if not pairs:
        return None
    mid = len(pairs) // 2
    key, value = pairs[mid]
    root = TreeNode(key, value)
    root.left = build_balanced_bst(pairs[:mid])
    root.right = build_balanced_bst(pairs[mid + 1:])
    return root

def bst_to_array_zero_indexed(root: TreeNode) -> list[tuple[int, str] | None]:
    """Convert a balanced BST to an array representation."""
    result = []

    def insert(node: TreeNode, index: int):
        if node is None:
            return
        while len(result) <= index:
            result.append(None)
        result[index] = (node.key, node.value)
        insert(node.left, 2 * index + 1)
        insert(node.right, 2 * index + 2)

    insert(root, 0)
    return result

def search_bst_array(tree: list[tuple[int, str] | None], key: int) -> str | None:
    index = 0
    while index < len(tree):
        node = tree[index]
        if node is None:
            return None  # trou dans l'arbre : clé non trouvée
        node_key, node_value = node
        if key == node_key:
            return node_value
        elif key < node_key:
            index = 2 * index + 1
        else:
            index = 2 * index + 2
    return None  # hors limites : clé non trouvée
