#!python3
"""Python Hash Table tester

A "test driver" for the python hash table. 
I've learned since P3. Hopefully some bugs will be found.

Change the value of ALWAYS_INSERT to match if necessary.
"""
from tester import HashTable
# Only works with int test files ...
test_filename = './tmp_test.int.in.txt'
ALWAYS_INSERT = False
table = None
with open(test_filename, 'r') as test_file:
    for i, line in enumerate(test_file):
        i += 1
        line = line.strip().split(" ")
        error_msg_start = "On line %d:" % i

        # Modifications
        if line[0] == "new":
            table = HashTable(5, always_insert=ALWAYS_INSERT)

        elif line[0] == "new:":
            table = HashTable(int(line[1]), always_insert=ALWAYS_INSERT)

        elif line[0] == "insert":
            try:
                table.insert(int(line[1]))
            except OverflowError:
                assert(False), error_msg_start + "Expected insert to be OK, but got overflow error"

        elif line[0] == "insert!":
            try:
                table.insert(int(line[1]))
                assert(False), error_msg_start + "Expected insert to throw an overflow but got OK"
            except OverflowError:
                pass

        elif line[0] == "erase":
            expected_erase_result = bool(int(line[2]))
            actual_erase_result = table.erase(int(line[1]))

            nice_error_msg = "Expected erasing of %d to be %r but it was actually not." % (
                int(line[1]),
                "successful" if expected_erase_result else "unsuccessful"
            )
            assert(expected_erase_result == actual_erase_result), error_msg_start + nice_error_msg

        elif line[0] == "clear":
            table.clear()

        # Assertions
        elif line[0] == "size":
            nice_error_msg = "Expected size to be %d but it was actually %r." % (
                int(line[1]),
                len(table)
            )
            assert(len(table) == int(line[1])), error_msg_start + nice_error_msg

        elif line[0] == "capacity":
            nice_error_msg = "Expected capacity to be %d but it was actually %r." % (
                int(line[1]),
                table.capacity()
            )
            assert(table.capacity() == int(line[1])), error_msg_start + nice_error_msg
        
        elif line[0] == "empty":
            nice_error_msg = "Expected table to be %s but it was actually not" % (
                "empty" if bool(int(line[1])) else "not empty"
            )
            assert(table.empty() == bool(int(line[1]))), error_msg_start + nice_error_msg
        
        elif line[0] == "bin":
            actual_box_contents = table.box(int(line[1])).contents
            expected_box_contents =int(line[2])
            nice_error_msg = "Expected contents of bin %d to be %d but it was actually %d." % (
                int(line[1]),
                expected_box_contents,
                actual_box_contents
            )
            assert(expected_box_contents == actual_box_contents), error_msg_start + nice_error_msg
        
        elif line[0] == "member":
            actual_membership = (int(line[1]) in table)
            expected_membership = bool(int(line[2]))
            nice_error_msg = "Expected membership of %d to be %r but it was actually not." % (
                int(line[1]),
                expected_membership
            )
            assert(expected_membership == actual_membership), error_msg_start + nice_error_msg
        
        # Verifiers
        elif line[0] in ("//break", "cout"):
            print(str(table))
            input("Press any key to continue ...")

print("Everything is AWESOME!")


