import os

EXEC_PATH = os.path.join(os.path.dirname(__file__), "../bin/fla")

HELP_INFO = (
    "Usage:\tfla [-h|--help]\n"
    + "      \tfla [-v|--verbose] <pda> <input>\n"
    + "      \tfla [-v|--verbose] <tm> <input>\n"
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1
