find . -name "*.c" -o -name "*.h" | xargs clang-format -i -style=file
python3 format_keymaps.py
