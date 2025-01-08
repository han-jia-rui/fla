import subprocess
import os
import pytest

exec_path = os.path.join(os.path.dirname(__file__), "../bin/fla")

help_info = (
    "Usage:\tfla [-h|--help]\n"
    + "      \tfla [-v|--verbose] <pda> <input>\n"
    + "      \tfla [-v|--verbose] <tm> <input>\n"
)

EXIT_SUCCESS = 0
EXIT_FAILURE = 1


class TestInterface:
    @pytest.mark.parametrize(
        "args, returncode, output",
        [
            # single options
            (["--help"], EXIT_SUCCESS, help_info),
            (["-h"], EXIT_SUCCESS, help_info),
            (["--verbose"], EXIT_FAILURE, help_info),
            (["-v"], EXIT_FAILURE, help_info),
            # multiple options
            (["-h", "-v", "--help", "--verbose"], EXIT_SUCCESS, help_info),
            # Unknown options
            (["--unknown"], EXIT_FAILURE, "Unknown option: --unknown\n" + help_info),
            (
                ["--unknown", "--help"],
                EXIT_FAILURE,
                "Unknown option: --unknown\n" + help_info,
            ),
        ],
    )
    def test_options(self, args, returncode, output):
        result = subprocess.run([exec_path] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stderr == output

    @pytest.mark.parametrize(
        "args, returncode, output",
        [
            (["arg1"], EXIT_FAILURE, help_info),
            (["-h", "arg1"], EXIT_SUCCESS, help_info),
            (["-v", "arg1"], EXIT_FAILURE, help_info),
            (["arg1", "arg2", "arg3"], EXIT_FAILURE, help_info),
            (["-h", "arg1", "arg2", "arg3"], EXIT_SUCCESS, help_info),
            (["-v", "arg1", "arg2", "arg3"], EXIT_FAILURE, help_info),
            (
                ["file.unknown", "input"],
                EXIT_FAILURE,
                "Unknown file type: file.unknown\nThe file format must be '*.pda' or '*.tm'\n",
            ),
        ],
    )
    def test_args(self, args, returncode, output):
        result = subprocess.run([exec_path] + args, capture_output=True, text=True)
        assert result.returncode == returncode
        assert result.stderr == output
