CC      = gcc
CFLAGS  = -Wall -Wextra -g
CUNIT   = -lcunit

HASH_OBJS = hash_table.o
LIST_OBJS = linked_list.o
DATABASE_OBJS = db.o utils.o

HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


test_db: db_tests.o $(DATABASE_OBJS) $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@ $(CUNIT)

db: $(DATABASE_OBJS) $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@


run_menu: menu.o $(DATABASE_OBJS) $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@

TESTS = test_db
MENU = run_menu

run: $(MENU)
	./run_menu

test: $(TESTS)
	./test_db

memtest: $(TESTS)
	valgrind --leak-check=full ./test_db

COV_FLAGS     = --coverage -O0
HASH_COV_OBJS = hash_table.cov.o
LIST_COV_OBJS = linked_list.cov.o

%.cov.o: %.c
	$(CC) $(CFLAGS) $(COV_FLAGS) -c $< -o $@

cov_db: db_tests.cov.o db.cov.o utils.cov.o $(HASH_COV_OBJS) $(LIST_COV_OBJS)
	$(CC) $(COV_FLAGS) $^ -o $@ $(CUNIT)

coverage: cov_db
	./cov_db
	gcov -b db.cov.gcda

clean:
	rm -f *.o test_db run_menu \
	      cov_db *.gcda *.gcno *.gcov

.PHONY: clean