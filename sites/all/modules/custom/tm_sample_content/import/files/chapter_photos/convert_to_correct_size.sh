#/bin/sh
for i in *.jpg; do convert -resize 805x453 "$i" "$i"; done
