import subprocess
import os
import pytest

from util import EXIT_SUCCESS, EXIT_FAILURE, EXEC_PATH

TM_DIR = os.path.join(os.path.dirname(__file__), "../tm/")

TM_INPUT_ERROR = "illegal input\n"


class TestPDA:
    @pytest.mark.parametrize(
        "args, returncode, stdout, stderr",
        [
            # accept examples
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "1001001"],
                EXIT_SUCCESS,
                "true\n",
                "",
            ),
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "11111"],
                EXIT_SUCCESS,
                "true\n",
                "",
            ),
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "1010101"],
                EXIT_SUCCESS,
                "true\n",
                "",
            ),
            # reject examples
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "110"],
                EXIT_SUCCESS,
                "false\n",
                "",
            ),
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "11101"],
                EXIT_SUCCESS,
                "false\n",
                "",
            ),
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "0000011111"],
                EXIT_SUCCESS,
                "false\n",
                "",
            ),
            # error examples
            (
                [TM_DIR + "palindrome_detector_2tapes.tm", "22222"],
                EXIT_FAILURE,
                "",
                TM_INPUT_ERROR,
            ),
        ],
    )
    def test_palindrome(self, args, returncode, stdout, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == stdout
        assert result.stderr == stderr

    @pytest.mark.parametrize(
        "args, returncode, stdout, stderr",
        [
            # accept examples
            (
                [TM_DIR + "case1.tm", "ab"],
                EXIT_SUCCESS,
                "c\n",
                "",
            ),
            (
                [TM_DIR + "case1.tm", "aaabbb"],
                EXIT_SUCCESS,
                "ccccccccc\n",
                "",
            ),
            (
                [TM_DIR + "case1.tm", "aabbbb"],
                EXIT_SUCCESS,
                "cccccccc\n",
                "",
            ),
            # reject examples
            (
                [TM_DIR + "case1.tm", "aaaa"],
                EXIT_SUCCESS,
                "illegal_input\n",
                "",
            ),
            (
                [TM_DIR + "case1.tm", "bbb"],
                EXIT_SUCCESS,
                "illegal_input\n",
                "",
            ),
            (
                [TM_DIR + "case1.tm", "aaabbbaaabbb"],
                EXIT_SUCCESS,
                "illegal_input\n",
                "",
            ),
            # error examples
            (
                [TM_DIR + "case1.tm", "aaaccc"],
                EXIT_FAILURE,
                "",
                TM_INPUT_ERROR,
            ),
        ],
    )
    def test_case1(self, args, returncode, stdout, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == stdout
        assert result.stderr == stderr
