OBJ = rcli.o

CFLAGS = -O3 -fPIC --std=c99 -Wall -Wextra -Wno-unused -I . -Wcomment -pthread
# CFLAGS = -g -O0 -fPIC --std=c99 -Wall -Wextra -Wno-unused -I . -Wcomment

lib: $(OBJ)
	ar csr librcli.a $(OBJ)

clean:
	rm -rf $(OBJ) librcli.a


