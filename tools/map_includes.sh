#!/usr/bin/env bash
# NOTE: no -e, no pipefail; grep is allowed to "fail" (no matches)
set -u

# 1) All project source-ish files (tracked by git)
git ls-files '*.[ch]' '*.[ch]pp' > all_sources.txt

# 2) Build source -> include edges
> include_edges.tsv

while IFS= read -r src; do
  # Collect all #include lines in this file (if none, grep just returns 1 and we skip)
  matches=$(grep '#include' "$src" 2>/dev/null || true)

  # If this file has no includes, move on
  [ -z "$matches" ] && continue

  # Extract just the header/path part from each include
  printf '%s\n' "$matches" | \
    sed -E 's/.*#include[[:space:]]*[<"]([^">]+)[">].*/\1/' | \
    while IFS= read -r inc; do
      # src<TAB>included_header_or_path
      printf '%s\t%s\n' "$src" "$inc" >> include_edges.tsv
    done
done < all_sources.txt

echo "Wrote include_edges.tsv"
