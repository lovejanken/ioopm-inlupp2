CC      = gcc
CFLAGS  = -Wall -Wextra -g
CUNIT   = -lcunit

HASH_OBJS = hash_table.o
LIST_OBJS = linked_list.o

HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


test_db: db_tests.o db.o utils.o $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@ $(CUNIT)

db: db.o utils.o $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@ $(CUNIT)

TESTS = test_db

test: $(TESTS)
	./test_db

memtest: $(TESTS)
	valgrind --leak-check=full ./test_db

COV_FLAGS     = --coverage -O0
HASH_COV_OBJS = hash_table.cov.o
LIST_COV_OBJS = linked_list.cov.o

%.cov.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(COV_FLAGS) -c $< -o $@

cov_db: db_tests.cov.o db.cov.o utils.cov.o $(HASH_COV_OBJS) $(LIST_COV_OBJS)
	$(CC) $(COV_FLAGS) $^ -o $@ $(CUNIT)

coverage: cov_db
	rm -f *.gcda
	./cov_db
	gcov -b hash_table.cov.gcda linked_list.cov.gcda db.cov.gcda utils.cov.gcda

clean:
	rm -f *.o test_db \
	      cov_db *.gcda *.gcno *.gcov

.PHONY: all test memtest coverage clean