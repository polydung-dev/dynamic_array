warnings=-Wall -Wextra
sanitize=-fsanitize=address,undefined,leak
CFLAGS=$(warnings) $(sanitize) -g3 -O3 -MMD
LDFLAGS=$(sanitize)

# zig cc requires manual linking, but this seems to break gcc???
ifeq ($(CC), zig cc)
LDLIBS+=-lasan -lubsan -llsan
endif

all: build/ out/ out/da

%/:
	@mkdir -p $@

out/da: build/main.o build/da.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

-include build/main.d
build/main.o: src/main.c
	$(CC) $(CFLAGS) -std=c99 -pedantic $(CPPFLAGS) -c -o $@ $<

-include build/da.d
build/da.o: src/da.c
	$(CC) $(CFLAGS) -std=c89 -pedantic $(CPPFLAGS) -c -o $@ $<

clean:
	-rm -r build/
	-rm -r out/
