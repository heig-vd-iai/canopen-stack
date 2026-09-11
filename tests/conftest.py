import pytest


def pytest_addoption(parser):
    parser.addoption(
        "--update-golden",
        action="store_true",
        default=False,
        help="Overwrite the golden files with the current generator output",
    )


@pytest.fixture
def update_golden(request):
    return request.config.getoption("--update-golden")
