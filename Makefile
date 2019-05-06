##############################
#   Clean Temporary Files    #
##############################

.PHONY: clean

clean:
	find . \( -name "*.EXE" -or -name "*.OBJ" \) -and ! -name "MASM.EXE" -and ! -name "LINK.EXE" -exec rm -rf {} \;