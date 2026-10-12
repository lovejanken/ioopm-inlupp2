# Makefile för inlupp 1 (hash table, länkad lista, iteratorer, frekvensräknare)
# OBS: kommandoraderna under varje mål måste börja med ett TAB-tecken, inte mellanslag.

CC      = gcc
CFLAGS  = -Wall -Wextra -g
CUNIT   = -lcunit

# Källfiler för respektive del (justera efter dina riktiga filnamn)
HASH_OBJS = hash_table.o hash_table_iterator.o
LIST_OBJS = linked_list.o list_iterator.o

# Alla headers i mappen. Ändras en av dem kompileras alla objektfiler om.
# (wildcard undviker att make klagar på en header som inte finns.)
HEADERS = $(wildcard *.h)


# Mönsterregel: varje .o byggs från sin .c (och beror på alla headers)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


# Testprogram (CUnit). Varje testfil har sin egen main(), alltså ett program var.
test_db: db_tests.o $(HASH_OBJS) $(LIST_OBJS)
	$(CC) $^ -o $@ $(CUNIT)

TESTS = test_db

test: $(TESTS)
	./test_db

# Minnestest med Valgrind
memtest: $(TESTS)
	valgrind --leak-check=full ./test_db

# Kodtäckning med gcov (separata .cov.o-filer, så vanliga bygget inte påverkas)
COV_FLAGS     = --coverage -O0
HASH_COV_OBJS = hash_table.cov.o hash_table_iterator.cov.o
LIST_COV_OBJS = linked_list.cov.o list_iterator.cov.o

%.cov.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(COV_FLAGS) -c $< -o $@

cov_db: db_tests.cov.o $(HASH_COV_OBJS)
	$(CC) $(COV_FLAGS) $^ -o $@ $(CUNIT)

# Testprogram som delar bibliotek-objekt slår ihop sin täckning i samma .gcda-filer.
# Siffrorna gäller alltså alla testprogram för respektive del tillsammans.
coverage: cov_db
	rm -f *.gcda
	./cov_db
	gcov -b hash_table.cov.gcda hash_table_iterator.cov.gcda \
	        linked_list.cov.gcda list_iterator.cov.gcda

clean:
	rm -f *.o test_db \
	      cov_db *.gcda *.gcno *.gcov

.PHONY: all test memtest coverage clean