import subprocess
import os
import sys

# Get max commit size in MB (default: 10 MB)
MAX_MB = float(sys.argv[1]) if len(sys.argv) > 1 else 10.0
MAX_BYTES = MAX_MB * 1024 * 1024

def get_git_status_lines():
    result = subprocess.run(["git", "status", "--porcelain"], capture_output=True, text=True)
    return result.stdout.strip().splitlines()

def get_file_size(path):
    try:
        return os.path.getsize(path)
    except (OSError, FileNotFoundError):
        return 0

def add_file(path):
    subprocess.run(["git", "add", "--", path])

def commit_batch(total_bytes):
    subprocess.run(["git", "commit", "-m", f"Auto commit"])

def main():
    lines = get_git_status_lines()
    total = 0
    added_any = False

    for line in lines:
        # Git status line format: XY filename (possibly with rename info)
        parts = line.strip().split(maxsplit=1)
        if len(parts) < 2:
            continue
        filepath = parts[1]

        # Handle renamed files (e.g., "R  oldname -> newname")
        if "->" in filepath:
            filepath = filepath.split("->")[1].strip()

        size = get_file_size(filepath)
        add_file(filepath)
        total += size
        added_any = True

        if total >= MAX_BYTES:
            break

    if added_any:
        commit_batch(total)
        print(f"Committed {total / (1024 * 1024):.2f} MB of changes.")
    else:
        print("No files added. Nothing to commit.")

if __name__ == "__main__":
    main()
