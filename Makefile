RM = rm -fv

clean:
	find -type f -name "*~" -exec $(RM) '{}' \;
	find -name "prog" -exec $(RM) '{}' \;
