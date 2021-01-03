#/bin/bash
# drops toc from doc 
# generates new toc from toc-less doc
# creates 2-liner spacer file
# glues together toc, spacer and doc into new doc
#
sed -n '/^# HowTos/,$ p' <how-to-git.md > doc.txt && ./gh-md-toc.exe doc.txt > toc.txt && echo "x" | sed 's/x/\n\n/' >two.txt && cat toc.txt two.txt doc.txt > how-to-git.md
