
cfiles="$(find -name "*.c" -type f) $(find -name "*.h" -type f)"

# Create license file with C comments
echo "/*" > LICENSE.new
while IFS='' read -r line || [[ -n "$line" ]]; do
    echo " * $line"
done < LICENSE >> LICENSE.new
echo " */" >> LICENSE.new

# Add to files
for i in $cfiles; do
	cat LICENSE.new $i > $i.new;
	mv $i.new $i;
done



makefiles="$(find -name "Makefile" -type f)";

# Create license file with Makefile comments
rm LICENSE.new
while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "# $line"
done < LICENSE >> LICENSE.new

for i in $makefiles; do
	cat LICENSE.new $i > $i.new;
	mv $i.new $i;
done


rm LICENSE.new
