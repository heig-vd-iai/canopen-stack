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
