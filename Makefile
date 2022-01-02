SOURCES_DIR = src/


all:
	@cd $(SOURCES_DIR); make


.PHONY: clean
clean:
	@cd $(SOURCES_DIR); make clean


