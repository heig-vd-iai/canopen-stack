"""Tests for BST tree operations."""
# pylint: disable=missing-function-docstring
import pytest

from generator.tree import (
    bst_to_array_zero_indexed,
    build_balanced_bst,
    search_bst_array,
)


@pytest.fixture
def sample_data():
    return [
        (4, "four"),
        (8, "eight"),
        (15, "fifteen"),
        (16, "sixteen"),
        (23, "twenty-three"),
        (42, "fourty-two"),
    ]


def test_build_balanced_bst_structure(sample_data):
    bst = build_balanced_bst(sample_data)

    assert bst is not None
    assert bst.key == 16

    assert bst.left is not None
    assert bst.left.key == 8

    assert bst.right is not None
    assert bst.right.key == 42

    assert bst.left.left is not None
    assert bst.left.left.key == 4

    assert bst.left.right is not None
    assert bst.left.right.key == 15

    assert bst.right.left is not None
    assert bst.right.left.key == 23


def test_bst_to_array_representation(sample_data):
    bst = build_balanced_bst(sample_data)
    array = bst_to_array_zero_indexed(bst)
    expected = [
        (16, "sixteen"),
        (8, "eight"),
        (42, "fourty-two"),
        (4, "four"),
        (15, "fifteen"),
        (23, "twenty-three"),
    ]
    assert array == expected

@pytest.mark.parametrize(
    "key, expected",
    [
        (16, "sixteen"),
        (8, "eight"),
        (15, "fifteen"),
        (42, "fourty-two"),
        (23, "twenty-three"),
        (4, "four"),
    ],
)
def test_search_existing_keys(sample_data, key, expected):
    bst = build_balanced_bst(sample_data)
    array = bst_to_array_zero_indexed(bst)
    assert search_bst_array(array, key) == expected


@pytest.mark.parametrize("missing_key", [1, 10, 50])
def test_search_missing_keys(sample_data, missing_key):
    bst = build_balanced_bst(sample_data)
    array = bst_to_array_zero_indexed(bst)
    assert search_bst_array(array, missing_key) is None


def test_empty_tree():
    bst = build_balanced_bst([])
    assert bst is None
    array = bst_to_array_zero_indexed(bst)
    assert array == []
    assert search_bst_array(array, 10) is None
