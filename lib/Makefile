SUBDIRS = pugixml SOIL

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	-for d in $(SUBDIRS); do (cd $$d; $(MAKE) $@); done

dist-clean:
	-for d in $(SUBDIRS); do (cd $$d; $(MAKE) $@); done
