#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
cpm_bios_replace.py
Usage:
  python cpm_bios_replace.py CPM.SYS CUSTOM_BIOS.BIN [OUTPUT.SYS]

- CPM.SYS の 0x1600 以降を CUSTOM_BIOS.BIN で置き換えて新しい SYS を作成します
- 出力は既定で CPM_patched.SYS（第3引数で変更可）
- 末尾は128バイト境界に0x00でパディングします
"""

import sys
from pathlib import Path

BIOS_OFFSET = 0x1600       # 5632


def pad_to_128(b: bytes) -> bytes:
    rem = len(b) % 128
    if rem == 0:
        return b
    return b + bytes(128 - rem)


def main():
    if not (2 <= len(sys.argv) <= 3 or len(sys.argv) == 4):
        print(__doc__)
        sys.exit(1)

    cpm_path = Path(sys.argv[1])
    bios_path = Path(sys.argv[2])
    out_path = Path(sys.argv[3]) if len(
        sys.argv) >= 4 else Path("CPM_patched.SYS")

    if not cpm_path.exists():
        print(f"Error: CPM file not found: {cpm_path}")
        sys.exit(2)
    if not bios_path.exists():
        print(f"Error: BIOS file not found: {bios_path}")
        sys.exit(2)

    cpm_data = cpm_path.read_bytes()
    bios_data = bios_path.read_bytes()

    if len(cpm_data) < BIOS_OFFSET:
        print(
            f"Error: CPM file is too small (< 0x{BIOS_OFFSET:04X} bytes). Size={len(cpm_data)}")
        sys.exit(3)

    # 先頭(0x0000〜0x15FF)はそのまま、0x1600以降をカスタムBIOSに置き換え
    head = cpm_data[:BIOS_OFFSET]
    new_data = head + bios_data

    # SYSは128バイト境界で切れている方が扱いやすいのでパディングしておく
    new_data = pad_to_128(new_data)

    out_path.write_bytes(new_data)

    print("=== Completed ===")
    print(f" Input  CPM.SYS : {cpm_path} ({len(cpm_data)} bytes)")
    print(f" Input  BIOS    : {bios_path} ({len(bios_data)} bytes)")
    print(f" Output SYS     : {out_path} ({len(new_data)} bytes)")
    print(f" Replaced region: 0x{BIOS_OFFSET:04X} - end with custom BIOS")
    print(" Note: Original tail after 0x1600 is discarded (fully replaced).")


if __name__ == "__main__":
    main()
