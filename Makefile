DOCS = html

all: wrapper $(DOCS)

.PHONY: clean
clean:
	$(RM) --recursive $(DOCS)

.PHONY: wrapper
wrapper:
	$(MAKE) --directory src/wrapper/

.PHONY: $(DOCS)
$(DOCS): Doxyfile
	doxygen

.PHONY: serve
serve: $(DOCS)
	python -m http.server --bind 127.0.0.1 --directory $(DOCS)
