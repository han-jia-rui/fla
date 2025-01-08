import subprocess
import pytest

from util import EXIT_SUCCESS, EXIT_FAILURE, HELP_INFO, EXEC_PATH


class TestInterface:
    @pytest.mark.parametrize(
        "args, returncode, stderr",
        [
            # single options
            (["--help"], EXIT_SUCCESS, HELP_INFO),
            (["-h"], EXIT_SUCCESS, HELP_INFO),
            (["--verbose"], EXIT_FAILURE, HELP_INFO),
            (["-v"], EXIT_FAILURE, HELP_INFO),
            # multiple options
            (["-h", "-v", "--help", "--verbose"], EXIT_SUCCESS, HELP_INFO),
            # Unknown options
            (["--unknown"], EXIT_FAILURE, "Unknown option: --unknown\n" + HELP_INFO),
            (
                ["--unknown", "--help"],
                EXIT_FAILURE,
                "Unknown option: --unknown\n" + HELP_INFO,
            ),
        ],
    )
    def test_options(self, args, returncode, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == ""
        assert result.stderr == stderr

    @pytest.mark.parametrize(
        "args, returncode, stderr",
        [
            (["arg1"], EXIT_FAILURE, HELP_INFO),
            (["-h", "arg1"], EXIT_SUCCESS, HELP_INFO),
            (["-v", "arg1"], EXIT_FAILURE, HELP_INFO),
            (["arg1", "arg2", "arg3"], EXIT_FAILURE, HELP_INFO),
            (["-h", "arg1", "arg2", "arg3"], EXIT_SUCCESS, HELP_INFO),
            (["-v", "arg1", "arg2", "arg3"], EXIT_FAILURE, HELP_INFO),
            (
                ["file.unknown", "input"],
                EXIT_FAILURE,
                "Unknown file type: file.unknown\nThe file format must be '*.pda' or '*.tm'\n",
            ),
        ],
    )
    def test_args(self, args, returncode, stderr):
        result = subprocess.run([EXEC_PATH] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stdout == ""
        assert result.stderr == stderr
