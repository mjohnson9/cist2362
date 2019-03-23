#!/usr/bin/env bash

SAVEIFS=$IFS
IFS=$'\n'
CHANGED_FILES=($(git diff --cached --name-only --diff-filter=ACMRT | grep -E '\.(cpp|h)$'))
IFS=$SAVEIFS
unset SAVEIFS

EXIT_STATUS=0

for CHANGED_FILE in "${CHANGED_FILES[@]}"; do
	git show ":${CHANGED_FILE}" | clang-format -style=file -output-replacements-xml -assume-filename="${CHANGED_FILE}" - | grep -v '#include' | grep '<replacement '>/dev/null
	if [ $? -ne 1 ]; then
		echo "${CHANGED_FILE}: Not clang-formatted"
		EXIT_STATUS=1
	fi

	git show ":${CHANGED_FILE}" | cpplint --filter=-readability/nolint --assume_filename="${CHANGED_FILE}" - >/dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "${CHANGED_FILE}: Failed cpplint"
		EXIT_STATUS=1
	fi
done

exit $EXIT_STATUS
