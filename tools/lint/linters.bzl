"Define linter aspects."
# Copied from https://github.com/aspect-build/rules_lint/blob/main/example/tools/lint/linters.bzl

load("@aspect_rules_lint//lint:flake8.bzl", "lint_flake8_aspect")
load("@aspect_rules_lint//lint:lint_test.bzl", "lint_test")
load("@aspect_rules_lint//lint:ruff.bzl", "lint_ruff_aspect")
load("@aspect_rules_lint//lint:shellcheck.bzl", "lint_shellcheck_aspect")

ruff = lint_ruff_aspect(
    binary = "@@//tools/lint:ruff",
    configs = [
        "@@//:.ruff.toml",
        "@@//src/subdir:ruff.toml",
    ],
)

ruff_test = lint_test(aspect = ruff)

flake8 = lint_flake8_aspect(
    binary = "@@//tools/lint:flake8",
    config = "@@//:.flake8",
)

flake8_test = lint_test(aspect = flake8)

shellcheck = lint_shellcheck_aspect(
    binary = "@@//tools/lint:shellcheck",
    config = "@@//:.shellcheckrc",
)

shellcheck_test = lint_test(aspect = shellcheck)
