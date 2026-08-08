#!/usr/bin/env python3
"""format_keymaps.py

Reformats QMK `keymap.c` files in this repository to a consistent style:

  * **Globally aligned columns** -- column widths are computed across *every*
    layer in a keymap, so column N sits at the same character offset in every
    layer (instead of each layer being aligned independently).
  * **MIT thumb row** -- for MIT layouts (`LAYOUT_*_mit`) the bottom row has
    one 2u key (the space bar).  That key spans two columns, starting at the
    middle column, so the key after it lines up with the column two over.

The same MIT alignment is applied to `ledmap` arrays (`const uint8_t PROGMEM
ledmap[][RGB_MATRIX_LED_COUNT][3]`), so each layer's per-LED HSV colors line
up with the key columns, with the 2u space bar spanning two columns.

Keycodes, colors and comments (including the ASCII-art layer diagrams) are left
untouched; only the matrix rows inside the `keymaps` / `ledmap` arrays are
realigned.

Usage:
  python3 format_keymaps.py                    # format every keymap in the repo
  python3 format_keymaps.py [path ...]         # format specific files/dirs
  python3 format_keymaps.py --dry-run          # show what would change, write nothing
  python3 format_keymaps.py --check            # exit 1 if any keymap needs formatting
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SEARCH_DIRS = ("keyboards", "users", "layouts")

ROW_INDENT = "        "    # 8 spaces for matrix rows
HEADER_INDENT = "    "      # 4 spaces for the [NAME] = LAYOUT_x( line

# Matches the start of a layer definition, e.g.  [_BASE] = LAYOUT_planck_mit(
LAYER_START_RE = re.compile(r"\[(\w+)\]\s*=\s*(LAYOUT_\w+)\(")


def split_top_level(text: str) -> list[str]:
    """Split on commas that are not inside parentheses or braces."""
    parts, depth, cur = [], 0, []
    for ch in text:
        if ch in "({":
            depth += 1
        elif ch in ")}":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur).strip())
    return parts


def strip_comments(line: str) -> str:
    line = re.sub(r"/\*.*?\*/", "", line, flags=re.DOTALL)
    line = re.sub(r"//.*", "", line)
    return line


def find_region(text: str) -> tuple[int, int] | None:
    """Return [start, end) of the keymaps array, or None if not found."""
    off = text.find("// clang-format off")
    on = text.find("// clang-format on")
    if off != -1 and on != -1 and off < on:
        return off, on + len("// clang-format on")
    m = re.search(r"const\s+uint16_t\s+PROGMEM\s+keymaps", text)
    if not m:
        return None
    start = m.start()
    end = text.find("};", start)
    if end == -1:
        return None
    return start, end + len("};")


def parse_layers(region: str) -> list[tuple[int, int, str, str, str]]:
    """Return (start, end, name, macro, body) for every layer in the array."""
    layers = []
    i, n = 0, len(region)
    while i < n:
        m = LAYER_START_RE.search(region, i)
        if not m:
            break
        name, macro = m.group(1), m.group(2)
        # Start at the beginning of the header line so the leading indent is
        # not double-counted when the layer is re-rendered.
        start = region.rfind("\n", 0, m.start()) + 1
        open_idx = region.index("(", m.start())
        depth, j = 0, open_idx
        while j < n:
            c = region[j]
            if c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        body = region[open_idx + 1 : j]
        end = j + 1
        if end < n and region[end] == ",":
            end += 1  # swallow the trailing comma of this layer
        layers.append((start, end, name, macro, body))
        i = end
    return layers


def parse_rows(body: str) -> list[list[str]]:
    rows = []
    for line in body.split("\n"):
        toks = [t for t in split_top_level(strip_comments(line)) if t]
        if toks:
            rows.append(toks)
    return rows


def compute_widths(parsed: list[tuple[str, list[list[str]]]]) -> tuple[list[int], int]:
    """Return (column widths, number of columns) aligned across all layers."""
    cols = max(len(row) for _, rows in parsed for row in rows)
    widths = [0] * cols

    def col_of(i: int, row_len: int) -> int | None:
        if row_len == cols:
            return i
        if row_len == cols - 1:  # MIT thumb row: one 2u key in the middle
            mid = cols // 2 - 1
            if i < mid:
                return i
            if i == mid:
                return None  # the 2u key spans two columns
            return i + 1
        return i

    for _, rows in parsed:
        for row in rows:
            for i, tok in enumerate(row):
                c = col_of(i, len(row))
                if c is not None:
                    widths[c] = max(widths[c], len(tok))
    return widths, cols


def render_mit_row(toks: list[str], cols: int, widths: list[int]) -> str:
    mid = cols // 2 - 1
    if mid + 2 >= cols:  # too narrow to span two columns; fall back
        return ROW_INDENT + ", ".join(t.ljust(widths[i]) for i, t in enumerate(toks))

    starts = [0] * cols
    for c in range(1, cols):
        starts[c] = starts[c - 1] + widths[c - 1] + 2

    buf = [" "] * (starts[cols - 1] + widths[cols - 1] + 4)

    def put(pos: int, text: str) -> None:
        for k, ch in enumerate(text):
            if pos + k < len(buf):
                buf[pos + k] = ch

    # keys before the 2u key
    for i in range(mid):
        put(starts[i], toks[i].ljust(widths[i]) + ", ")

    # the 2u key spans columns mid and mid+1 (from starts[mid] to starts[mid+2]-2)
    span_start = starts[mid]
    span_end = starts[mid + 2] - 2
    put(span_start, toks[mid].ljust(span_end - span_start) + ", ")

    # keys after the 2u key
    for i in range(mid + 1, cols - 1):
        col = i + 1
        sep = "" if i == cols - 2 else ", "
        put(starts[col], toks[i].ljust(widths[col]) + sep)

    return ROW_INDENT + "".join(buf).rstrip()


def render_row(toks: list[str], cols: int, widths: list[int]) -> str:
    if len(toks) == cols - 1:  # MIT thumb row
        return render_mit_row(toks, cols, widths)
    return ROW_INDENT + ", ".join(t.ljust(widths[i]) for i, t in enumerate(toks))


def render_layer(name: str, macro: str, rows: list[list[str]], cols: int, widths: list[int]) -> str:
    lines = [f"{HEADER_INDENT}[{name}] = {macro}("]
    for r, row in enumerate(rows):
        suffix = "" if r == len(rows) - 1 else ","
        lines.append(render_row(row, cols, widths) + suffix)
    lines.append(f"{HEADER_INDENT}),")
    return "\n".join(lines)


def format_region(region: str) -> str | None:
    layers = parse_layers(region)
    if not layers:
        return None
    parsed = [(name, parse_rows(body)) for _, _, name, _, body in layers]
    widths, cols = compute_widths(parsed)

    out: list[str] = []
    prev = 0
    for idx, (start, end, name, macro, _body) in enumerate(layers):
        out.append(region[prev:start])  # keep comments / blank lines verbatim
        out.append(render_layer(name, macro, parsed[idx][1], cols, widths))
        prev = end
    out.append(region[prev:])
    return "".join(out)


LEDMAP_RE = re.compile(r"const\s+uint8_t\s+PROGMEM\s+ledmap")
LEDMAP_LAYER_RE = re.compile(r"\[(\w+)\]\s*=\s*\{")


def find_ledmap_region(text: str) -> tuple[int, int] | None:
    """Return [start, end) of the ledmap array, or None if not found."""
    m = LEDMAP_RE.search(text)
    if not m:
        return None
    start = m.start()
    end = text.find("};", start)
    if end == -1:
        return None
    return start, end + len("};")


def parse_ledmap_layers(region: str) -> list[tuple[int, int, str, str]]:
    """Return (start, end, name, body) for every layer in the ledmap array."""
    layers = []
    # Skip the declaration header (`...ledmap[][RGB_MATRIX_LED_COUNT][3] = {`)
    # so the dimension `[3]` isn't mistaken for a layer name.
    i = region.index("{") + 1
    n = len(region)
    while i < n:
        m = LEDMAP_LAYER_RE.search(region, i)
        if not m:
            break
        name = m.group(1)
        # Start at the beginning of the header line so the leading indent is
        # not double-counted when the layer is re-rendered.
        start = region.rfind("\n", 0, m.start()) + 1
        open_idx = region.index("{", m.start())
        depth, j = 0, open_idx
        while j < n:
            c = region[j]
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        body = region[open_idx + 1 : j]
        end = j + 1
        if end < n and region[end] == ",":
            end += 1  # swallow the trailing comma of this layer
        layers.append((start, end, name, body))
        i = end
    return layers


def parse_ledmap_body(body: str) -> list[list[str]]:
    """Split a flat ledmap layer into rows; MIT gets 12-12-12-11 rows."""
    toks = [t for t in split_top_level(body) if t]
    total = len(toks)
    if total == 0:
        return []
    cols = (total + 1) // 4  # 3 full rows + 1 thumb row of cols - 1
    if total != 4 * cols - 1:
        return [toks]  # not an MIT-style layout; keep as a single row
    return [toks[i * cols : (i + 1) * cols] for i in range(3)] + [toks[3 * cols :]]


def render_ledmap_layer(name: str, rows: list[list[str]], cols: int, widths: list[int]) -> str:
    lines = [f"{HEADER_INDENT}[{name}] = {{"]
    for r, row in enumerate(rows):
        suffix = "" if r == len(rows) - 1 else ","
        lines.append(render_row(row, cols, widths) + suffix)
    lines.append(f"{HEADER_INDENT}}},")
    return "\n".join(lines)


def format_ledmap_region(region: str) -> str | None:
    layers = parse_ledmap_layers(region)
    if not layers:
        return None
    parsed = [(name, parse_ledmap_body(body)) for _, _, name, body in layers]
    widths, cols = compute_widths(parsed)

    out: list[str] = []
    prev = 0
    for idx, (start, end, name, _body) in enumerate(layers):
        out.append(region[prev:start])  # keep the declaration line verbatim
        out.append(render_ledmap_layer(name, parsed[idx][1], cols, widths))
        prev = end
    out.append(region[prev:])
    return "".join(out)


def format_file(path: Path) -> tuple[str | None, str]:
    text = path.read_text()
    result = text
    found = False

    region = find_region(text)
    if region is not None:
        start, end = region
        new_region = format_region(text[start:end])
        if new_region is not None:
            found = True
            result = result[:start] + new_region + result[end:]

    lregion = find_ledmap_region(result)
    if lregion is not None:
        start, end = lregion
        new_region = format_ledmap_region(result[start:end])
        if new_region is not None:
            found = True
            result = result[:start] + new_region + result[end:]

    return (result, text) if found else (None, text)


def discover(paths: list[str]) -> list[Path]:
    if paths:
        files: list[Path] = []
        for p in paths:
            p = Path(p)
            if p.is_dir():
                files.extend(p.rglob("keymap.c"))
            elif p.is_file():
                files.append(p)
        return sorted(set(files))
    files = []
    for d in SEARCH_DIRS:
        base = ROOT / d
        if base.is_dir():
            files.extend(base.rglob("keymap.c"))
    return sorted(set(files))


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("paths", nargs="*", help="specific files or directories (default: whole repo)")
    ap.add_argument("--dry-run", action="store_true", help="report what would change, write nothing")
    ap.add_argument("--check", action="store_true", help="exit 1 if any keymap needs formatting")
    args = ap.parse_args(argv)

    files = discover(args.paths)
    changed: list[Path] = []
    for f in files:
        new_text, orig = format_file(f)
        if new_text is None:
            print(f"[skip] {f}  (no keymaps array found)")
        elif new_text == orig:
            print(f"[ ok ] {f}")
        else:
            changed.append(f)
            if args.dry_run or args.check:
                print(f"[diff] {f}")
            else:
                f.write_text(new_text)
                print(f"[fmt ] {f}")

    if args.check:
        if changed:
            print(f"\n{len(changed)} file(s) need formatting")
            return 1
        print("\nAll keymaps are formatted")
        return 0

    if args.dry_run:
        print(f"\n{len(changed)} file(s) would be changed")
    else:
        print(f"\nFormatted {len(changed)} file(s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
