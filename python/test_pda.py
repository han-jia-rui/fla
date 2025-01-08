import subprocess
import os
import pytest

from util import EXIT_SUCCESS, EXIT_FAILURE, EXEC_PATH

PDA_DIR = os.path.join(os.path.dirname(__file__), "../pda/")

PDA_ACCEPT_OUTPUT = "true\n"
PDA_REJECT_OUTPUT = "false\n"

PDA_INPUT_ERROR = "illegal input\n"


class TestPDA:
    @pytest.mark.parametrize(
        "args, returncode, stdout, stderr",
        [
            # accept examples
            ([PDA_DIR + "anbn.pda", "ab"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            ([PDA_DIR + "anbn.pda", "aaabbb"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            ([PDA_DIR + "anbn.pda", "aaaaabbbbb"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            # reject examples
            ([PDA_DIR + "anbn.pda", "aabbb"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            ([PDA_DIR + "anbn.pda", "aab"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            ([PDA_DIR + "anbn.pda", "aaa"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            # error examples
            ([PDA_DIR + "anbn.pda", "c"], EXIT_FAILURE, "", PDA_INPUT_ERROR),
        ],
    )
    def test_anbn(self, args, returncode, stdout, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == stdout
        assert result.stderr == stderr

    @pytest.mark.parametrize(
        "args, returncode, stdout, stderr",
        [
            # accept examples
            ([PDA_DIR + "case.pda", "()"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            ([PDA_DIR + "case.pda", "()()()"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            ([PDA_DIR + "case.pda", "(()(())())"], EXIT_SUCCESS, PDA_ACCEPT_OUTPUT, ""),
            # reject examples
            ([PDA_DIR + "case.pda", "((()"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            ([PDA_DIR + "case.pda", "(()))"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            ([PDA_DIR + "case.pda", "()()(()()"], EXIT_SUCCESS, PDA_REJECT_OUTPUT, ""),
            # error examples
            ([PDA_DIR + "case.pda", "{}[]"], EXIT_FAILURE, "", PDA_INPUT_ERROR),
        ],
    )
    def test_case(self, args, returncode, stdout, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == stdout
        assert result.stderr == stderr
