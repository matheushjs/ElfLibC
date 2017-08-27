RM = rm -fv

clean:
	find -type f -name "*~" -exec $(RM) '{}' \;
	find -name "prog" -exec $(RM) '{}' \;

replace:
	for i in $( find -name "elf_encodings.h" ); do cp string/encodings/elf_encodings.h $1 done
