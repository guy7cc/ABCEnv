#!/usr/bin/env python3
"""
my-library/expander.py -- Custom library inliner for competitive programming.

Reads a C++ source file, finds #include "..." and #include <...> directives
that resolve to files within the specified lib directory, and recursively
expands them inline. All other includes (standard library, atcoder, etc.)
are preserved as-is.

Usage:
    python3 expander.py main.cpp --lib my-library -o submit.cpp
"""

import re
import argparse
from logging import basicConfig, getLogger
from os import getenv
from pathlib import Path
from typing import List, Set, Optional

logger = getLogger(__name__)


class LibExpander:
    # Matches both #include "..." and #include <...>
    any_include = re.compile(r'^\s*#\s*include\s+[<"]([^<>"]+)[>"]')

    def __init__(self, lib_dirs: List[Path]):
        self.lib_dirs = [p.resolve() for p in lib_dirs]
        self.included: Set[Path] = set()

    def resolve_lib_file(self, include_str: str, current_dir: Path) -> Optional[Path]:
        """Return the resolved Path if the include points into a lib dir, else None.

        Search order:
        1. Relative to the current file's directory (only if result is under a lib dir)
        2. Relative to each lib dir root  ← mimics -I lib_dir compiler flag
        """
        # Try 1: relative to current file's directory
        candidate = (current_dir / include_str).resolve()
        for lib_dir in self.lib_dirs:
            try:
                candidate.relative_to(lib_dir)
                if candidate.exists():
                    return candidate
            except ValueError:
                continue

        # Try 2: relative to each lib_dir (handles #include <guy7cc/...> style)
        for lib_dir in self.lib_dirs:
            candidate = (lib_dir / include_str).resolve()
            if candidate.exists():
                return candidate

        return None

    def expand_file(self, file_path: Path) -> List[str]:
        """Recursively expand a single file, skipping already-included files."""
        if file_path in self.included:
            logger.debug('skip (already included): %s', file_path.name)
            return []
        self.included.add(file_path)
        logger.info('expanding: %s', file_path)

        lines: List[str] = []
        for line in file_path.read_text(encoding='utf-8').splitlines():
            if line.strip() == '#pragma once':
                continue

            m = self.any_include.match(line)
            if m:
                lib_file = self.resolve_lib_file(m.group(1), file_path.parent)
                if lib_file:
                    lines.extend(self.expand_file(lib_file))
                    continue

            lines.append(line)
        return lines

    def expand(self, source_path: Path) -> str:
        """Expand the top-level source file and return the result as a string."""
        self.included = set()
        lines: List[str] = []

        for line in source_path.read_text(encoding='utf-8').splitlines():
            m = self.any_include.match(line)
            if m:
                lib_file = self.resolve_lib_file(m.group(1), source_path.parent)
                if lib_file:
                    lines.extend(self.expand_file(lib_file))
                    continue

            lines.append(line)

        return '\n'.join(lines)


if __name__ == '__main__':
    basicConfig(
        format='%(asctime)s [%(levelname)s] %(message)s',
        datefmt='%H:%M:%S',
        level=getenv('LOG_LEVEL', 'INFO'),
    )
    parser = argparse.ArgumentParser(
        description='Inline custom lib headers into a single C++ source file.'
    )
    parser.add_argument('source', help='Source .cpp file to process')
    parser.add_argument(
        '--lib', metavar='DIR', action='append', dest='libs',
        help='Library directory to inline (default: lib). Can be repeated.'
    )
    parser.add_argument(
        '-o', '--output', metavar='FILE',
        help='Output file path (default: stdout)'
    )
    opts = parser.parse_args()

    lib_dirs = [Path(d) for d in (opts.libs or ['lib'])]
    expander = LibExpander(lib_dirs)
    result = expander.expand(Path(opts.source))

    if opts.output:
        Path(opts.output).write_text(result + '\n', encoding='utf-8')
        logger.info('Written to %s', opts.output)
    else:
        print(result)
