"Define linter aspects"
# Coped from https://github.com/aspect-build/rules_lint/blob/v1.0.8/docs/linting.md

load("@aspect_rules_lint//lint:buf.bzl", "lint_buf_aspect")
load("@aspect_rules_lint//lint:clang_tidy.bzl", "lint_clang_tidy_aspect")
load("@aspect_rules_lint//lint:flake8.bzl", "lint_flake8_aspect")
load("@aspect_rules_lint//lint:lint_test.bzl", "lint_test")
load("@aspect_rules_lint//lint:ruff.bzl", "lint_ruff_aspect")
load("@aspect_rules_lint//lint:shellcheck.bzl", "lint_shellcheck_aspect")

buf = lint_buf_aspect(
    config = "@@//:buf.yaml",
)
flake8 = lint_flake8_aspect(
    binary = "@@//tools/lint:flake8",
    config = "@@//:.flake8",
)

flake8_test = lint_test(aspect = flake8)

ruff = lint_ruff_aspect(
    binary = "@multitool//tools/ruff",
    configs = [
        "@@//:.ruff.toml",
        "@@//src/subdir:ruff.toml",
    ],
)

ruff_test = lint_test(aspect = ruff)

shellcheck = lint_shellcheck_aspect(
    binary = "@multitool//tools/shellcheck",
    config = "@@//:.shellcheckrc",
)

shellcheck_test = lint_test(aspect = shellcheck)

clang_tidy = lint_clang_tidy_aspect(
    binary = "@@//tools/lint:clang_tidy",
    configs = [
        "@@//:.clang-tidy",
        "@@//src/cpp/lib:get/.clang-tidy",
    ],
    lint_target_headers = True,
    angle_includes_are_system = False,
    verbose = False,
)

clang_tidy_test = lint_test(aspect = clang_tidy)

# an example of setting up a different clang-tidy aspect with different
# options. This one uses a single global clang-tidy file
clang_tidy_global_config = lint_clang_tidy_aspect(
    binary = "@@//tools/lint:clang_tidy",
    global_config = "@@//:.clang-tidy",
    lint_target_headers = True,
    angle_includes_are_system = False,
    verbose = False,
)
