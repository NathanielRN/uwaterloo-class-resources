#!python3
"""Test Generator for Quadratic_hash_table

Requirements: python3

Description:
    This python file will run randomly combine the instructions
    in the test driver to create test files. It will run these generated
    test files on your code and check if they pass. If they don't pass,
    you can review the test file, modify it and re-run it manually.
    If you use the defaults, the test files are very long, so each 
    run is relatively slow. [3-3.5k lines each & took ~0.2s per file]

    It's suggested you have a good cout function and
    then add the 'cout' command before the failing lines to debug.

    Also you can now use the '--disallowed-commands' option to turn off
    any functionality you haven't implemented yet or to speed up runtime.

Disclaimer:
    Random tests suck because edge cases are very unlikely to happen.
    Think about what these edge cases might be and create custom tests
    for them.
    
    !! Important !!
    Also I'm using my own quadratic hash table implementation. 
    So ... if that's buggy, these tests won't work

Credit: Blatantly stole idea from someone else's P1 C++ version of this

See -h for avaiable options
"""

# Imports
import sys
import random
from subprocess import Popen, PIPE
import re
from argparse import ArgumentParser, RawDescriptionHelpFormatter

GOOD_LINE = re.compile(
    r"^(?:\d+ % Okay)|"                                          # Okay lines are ok
    r"(?:Starting Test Run)|"                                    # Starting test run line is ok
    r"(?:Finishing Test Run)|"                                   # Finishing test run lines are ok
    r"(?:\d+ % Memory allocated minus memory deallocated: 0)$",  # Memory summary test lines are ok
    re.M | re.I                                                  # Multiline + Ignore case flag
)

PRINT_COLORS = {
    "HEADER": '\033[95m',
    "OKBLUE" : '\033[94m',
    "OKGREEN": '\033[92m',
    "WARNING": '\033[93m',
    "FAIL": '\033[91m',
    "ENDC": '\033[0m',
    "BOLD": '\033[1m',
    "UNDERLINE": '\033[4m'
}

class HashItem:
    UNOCCUPIED = 0
    OCCUPIED = 1
    DELETED = 2

    def __init__(self, contents=None, state=UNOCCUPIED):
        self.contents = contents
        self.state = state
    
    def fill(self, new_content):
        self.contents = new_content
        self.state = HashItem.OCCUPIED
    
    def fillable(self):
        return self.state != HashItem.OCCUPIED
    
    def occupied(self):
        return self.state == HashItem.OCCUPIED

    def unoccupied(self):
        return self.state == HashItem.UNOCCUPIED
    
    def erased(self):
        return self.state == HashItem.DELETED

    def erase(self):
        self.contents = None
        self.state = HashItem.DELETED

    def contains(self, value):
        return self.occupied() and self.contents == value
    
    def __contains__(self, val):
        return self.contains(val)

class HashTable:
    def __init__(self, power, always_insert=False):
        self.table = [HashItem() for i in range(2**power)]
        self.power = power
        self.filled = set()
        self.erased = 0
        self.always_insert = always_insert

    def repr(self):
        return str(str(self))

    def __str__(self):
        output = ""
        region_start = None
        for i, cell in enumerate(self.table):
            i+= 1
            contents = cell.contents
            if cell.erased():
                contents = "x"
            elif cell.unoccupied():
                contents = "-"
                if not region_start:
                    region_start = i
            if region_start:
                if contents != "-":
                    if region_start != i:
                        output += "%d->" % region_start
                    output += "%d | [ - ]" % (i - 1) + "\n\n"
                    output += "%d | [ %s ]" %(i, contents) + "\n"
                    region_start = None
            else:
                output += "%d | [ %s ]" %(i, contents) + "\n"   
        if region_start:
            output += "%d | [ - ]" % (i) + "\n"
                
        return output
    
    def capacity(self):
        return 2**self.power
    
    def __len__(self):
        return len(self.filled)
    
    def __bool__(self):
        return not self.empty()

    def load_factor(self):
        return (len(self) + self.erased)/self.capacity()

    def full(self):
        return len(self) == self.capacity()
    
    def empty(self):
        return not len(self)

    def clear(self):
        self.table = [HashItem() for i in range(self.capacity())]
        self.filled = set()
        self.erased = 0

    def insert(self, val):
        if self.always_insert:
            if val in self:
                return
            if self.full():
                raise OverflowError("Attempting to insert into full hash table")
        else:    
            if self.full():
                raise OverflowError("Attempting to insert into full hash table") 
            if val in self:
                return
        
        box_num = self.map_hash(self.calc_hash(val))
        curr_box = self.box(box_num)
        
        for i in range(1, self.capacity() + 1):    
            if curr_box.fillable():
                break
            if val in curr_box:
                return
            box_num = (box_num + i) % self.capacity()
            curr_box = self.box(box_num)
        
        if curr_box.erased():
            self.erased -= 1
                
        curr_box.fill(val)
        self.filled.add(box_num)

            
    def erase(self, val):
        if self.empty():
            return False
        
        box_num = self.map_hash(self.calc_hash(val))
        curr_box = self.box(box_num)
        
        for i in range(1, self.capacity() + 1):
            if curr_box.unoccupied():
                return False
            if val in curr_box:
                curr_box.erase()
                self.filled.remove(box_num)
                self.erased += 1
                return True
            box_num = (box_num + i) % self.capacity()
            curr_box = self.box(box_num)
        
        return False
    
    def box(self, box_num):
        return self.table[box_num]
       
    def contains(self, val):
        if self.empty():
            return False

        box_num = self.map_hash(self.calc_hash(val))
        curr_box = self.box(box_num)
        for i in range(1, self.capacity() + 1):
            if curr_box.unoccupied():
                break
            if val in curr_box:
                return True
            box_num = (box_num + i) % self.capacity()
            curr_box = self.box(box_num)
        return False
        
    def __contains__(self, val):
        return self.contains(val)

    def map_hash(self, hashed):
        return hashed
    
    def calc_hash(self, val):
        return int(val) % self.capacity()

    def duplicate(self):
        duplicated = HashTable(self.power, always_insert=self.always_insert)
        duplicated.table = [HashItem(self.box(i).contents) for i in range(self.capacity())]
        duplicated.filled = self.filled
        return duplicated

    def full_boxes(self):
        return self.filled

class TestDriver:
    """Creates test file"""
    # Cache of methods that create test commands
    TEST_COMMANDS = ()
    RANDOM_RANGE = 2**12

    def __init__(self, file):
        self.file = file
        self.hash_table = None
        self.pre = ""

    def set_params(self, params):
        """Define the parameters used by this driver"""
        self.params = params
    
    def set_table(self, table):
        """Define the hash table used by this driver"""
        self.hash_table = table
    
    def set_pre(self, pre):
        """Define any text to print before writing commands to the file"""
        self.pre = pre

    def rng(self):
        """Helper method to create random value depending on type"""
        if self.params["type"] == "int":
            return random.randrange(-1*TestDriver.RANDOM_RANGE , TestDriver.RANDOM_RANGE )
        if self.params["type"] == "double":
            return random.choice([i*0.1 for i in range(-1*TestDriver.RANDOM_RANGE, TestDriver.RANDOM_RANGE)])
        return random.random()

    def generate(self, num_commands, *disallowed_commands):
        """Generate commands in file. """
        # Verify class is generated properly
        params_defined = self.params is not None
        table_defined = self.hash_table is not None
        is_ready = (
            params_defined and
            table_defined
        )

        if not is_ready:
            raise ValueError(
                "This class can't generate tests until parameters, "
                "current array, capacity and initial capacity are defined"
            )
        while num_commands >= 0:
            self.call_random(*disallowed_commands)
            num_commands -= 1
        self.write("delete")

    def call_random(self, *disallowed_methods):
        """Helper function to call random command"""
        def command_method(func):
            """Helper function to get allowed methods"""
            is_callable = callable(getattr(self, func))
            is_command_generator = func.startswith("_make_")
            return is_callable and is_command_generator

        def allowed_method(func):
            """Helper function to filter out allowed methods"""
            return func[len("_make_"):] not in disallowed_methods
        method_list = TestDriver.TEST_COMMANDS
        if not method_list:
            method_list = tuple(func for func in filter(command_method, dir(self)))
            TestDriver.TEST_COMMANDS = method_list

        # convert disallowed_methods to a set to improve performance
        disallowed_methods = set(disallowed_methods)
        # filter out diallowed methods and call random one
        method_list = tuple(filter(allowed_method, method_list))
        getattr(self, random.choice(method_list))()

    def write(self, text):
        """Helper method to write commands to file"""
        print(self.pre, file=self.file, end="")
        print(text, file=self.file)

    def _make_size(self):
        """Create instruction to verify size"""
        size = len(self.hash_table)
        self.write("size %d" % size)

    def _make_capacity(self):
        """Create instruction to verify capacity"""
        self.write("capacity %d" % self.hash_table.capacity())

    def _make_empty(self):
        """Create instruction to verify emptyness"""
        is_empty = int(self.hash_table.empty())
        self.write("empty %d" % is_empty)
    
    def _make_bin(self):
        if self.hash_table.empty():
            return
        to_check = random.choice(list(self.hash_table.full_boxes()))
        self.write("bin %s %s" % (str(to_check), self.hash_table.box(to_check).contents) )

    def _make_member(self):
        value = self.rng()
        exists = int(value in self.hash_table)
        self.write("member %d %d" % (value,exists) )

    def _make_load_factor(self):
        self.write("load_factor %s" % str(self.hash_table.load_factor()))

    def _make_insert(self):
        """Create instruction to insert a random number into the hash table"""
        # Do multiple inserts to increase likelyhood of collisions
        for _ in range(random.randrange(1, self.params["max_insert"])):
            value = self.rng()
            try:
                self.hash_table.insert(value)
                self.write("insert %s" % str(value))
            except OverflowError:
                self.write("insert! %s" % str(value))
    
    def _make_erase(self):
        """Create instruction to pop last element"""
        # Erase slightly less to increase likelyhood of collisions
        for _ in range(random.randrange(1, self.params["max_erase"])):
            value = self.rng()
            erased = self.hash_table.erase(value)
            self.write("erase %s %d" % (str(value), int(erased)) )

    def _make_clear(self):
        """Create clear instruction"""
        self.hash_table.clear()
        self.write("clear")

    # def _make_assign(self):
    #     """Create a assign instruction with random sub-tests to verify success. Will not recurse."""
    #     tests_to_run = random.randrange(100)
    #     self.write("assign")
    #     self.gen_subtest_driver(tests_to_run)

    # def _make_move(self):
    #     """Create a move instruction with random sub-tests to verify success. Will not recurse."""
    #     tests_to_run = random.randrange(100)
    #     self.write("move")
    #     self.gen_subtest_driver(tests_to_run)

    def gen_subtest_driver(self, num_commands):
        """"Helper method to create sub-test drivers"""
        no_recurse_commands = ["move", "assign"]
        sub_test_driver = TestDriver(self.file)
        sub_test_driver.set_table(self.hash_table.duplicate()) # copy list
        sub_test_driver.set_pre(self.pre + "\t")
        sub_test_driver.generate(num_commands, *no_recurse_commands)
        sub_test_driver.gen_exit()
        return sub_test_driver

    def gen_summary(self):
        """Create summary command based on params"""
        simple_summary = self.params["memcheck"] != "full"
        if simple_summary:
            self.write("summary")
        else:
            self.write("details")

    def gen_new(self):
        """Create new command (either normal or random initial capacity) and initialize driver"""
        table_size = random.randrange(0, self.params["max_size"])
        use_default_constructor = False
        self.hash_table = HashTable(table_size, always_insert=self.params["always_insert"])
        if self.hash_table.capacity() == 2**5:
            use_default_constructor = bool(random.randrange(2))
        
        if use_default_constructor:
            self.write("new")
        else:
            self.write("new: %d" % table_size)

    def gen_exit(self):
        """Exit the test driver"""
        self.write("exit")

def print_pass(text, **kwargs):
    """Helper method to colorize passes"""
    print(PRINT_COLORS["OKGREEN"] + text + PRINT_COLORS["ENDC"], **kwargs)

def print_fail(text, **kwargs):
    """Helper method to colorize passes"""
    print(PRINT_COLORS["FAIL"] + text + PRINT_COLORS["ENDC"], **kwargs)

def parse_args():
    """Parse arguments into dictionary"""
    argv = sys.argv[1:]
    ## Improve arg parser
    parser = ArgumentParser(
        description=__doc__, 
        formatter_class=RawDescriptionHelpFormatter,
        epilog="Old argument format still works!"
    )
    parser.add_argument(
        "--test-driver",
        "--test_driver", 
        "-t",
        help="Path to compiled tester driver executable [Default: %(default)s]"
    )
    parser.add_argument(
        "--commands", 
        "-c",
        type=int, 
        metavar="NUM_COMMANDS",
        help="Number of commands to put in each run. Some 'commands' map to multiple test instructions."
        " Expects positive number. [Default: %(default)d]"
    )
    parser.add_argument(
        "--runs", 
        "-r",
        type=int, 
        metavar="NUM_RUNS",
        help="Number of times to run random test cases. Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--memcheck", 
        "-m",
        choices={"simple", "full"},
        help="Which kind of memory check to do. Don't use 'full'"
       " otherwise you will always fail. [Default: %(default)s]"
    )
    parser.add_argument(
        "--type", 
        choices={"int", "double"},
        help="What type of seach tree this is. [Default: %(default)s]"
    )
    parser.add_argument(
        "--disallowed-commands",
        "--disallowed_commands",
        "-d",
        nargs="*",
        help="List of commands (space seperated), that you don't want to be tested i.e. "
        " --disallowed_commands begin end rbegin rend erase. Might for testing while"
        " writing code.",
    )

    parser.add_argument(
        "--max-insert",
        "--max_insert",
        "--mi",
        type=int, 
        help="Set the upper-bound for consecutive inserts. Higher numbers are good for "
        "forcing collisions. Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--max-erase",
        "--max_erase",
        "--me",
        type=int, 
        help="Set the upper-bound for consecutive erases. Lower numbers are good for "
        "forcing collisions. Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--max-size",
        "--max_size",
        "--ms",
        type=int, 
        help="Set the upper-bound for the value `m` which will be used to create a hashtable of size 2^m. "
        "Lower numbers are good for forcing collisions. Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--always-insert",
        "--always_insert",
        "--ai",
        action="store_true",
        help="There is an ambiguity in the instructions which states that if the array is full and you "
        "insert an existing element, it is valid to both a) throw an error OR b) do nothing. "
        "Include this flag if you do nothing in the above case. Expects nothing. [Default: OFF]"
    )

    parser.set_defaults(
        test_driver="./tester.out",
        commands=1000,
        runs=1000,
        type="int",
        memcheck="simple",
        disallowed_commands=[],
        max_insert=2**5,
        max_erase=2**4,
        max_size=12
    )
    #mangle argv
    def add_prefix_char(arg):
        return ("--" + arg) if not arg.startswith("--") and "=" in arg else arg
    mangled_args = [add_prefix_char(arg) for arg in argv]
    params = vars(parser.parse_args(mangled_args))
    return params

def main(params):
    """Run tests"""
    failure = False
    total_runs = params["runs"]
    total_commands = params["commands"]
    test_filename = 'tmp_test.%s.in.txt' % params["type"]
    for current_run in range(1, total_runs + 1):
        # Clear and open test file
        open(test_filename, 'w').close()
        with open(test_filename, 'w') as test_file:
            test_driver = TestDriver(test_file)
            test_driver.set_params(params)
            test_driver.gen_new()
            test_driver.generate(total_commands, *params["disallowed_commands"])
            test_driver.gen_summary()
            test_driver.gen_exit()

        # run test file
        try:
            with open(test_filename) as test_file:
                (result, stderr) = Popen(
                    [params["test_driver"], params["type"]],
                    stdin=test_file,
                    stdout=PIPE
                ).communicate()
            # verify test file results
            if not stderr:
                for line in result.decode('utf-8').splitlines():
                    if not GOOD_LINE.match(line):
                        print_fail(line, file=sys.stderr)
                        failure = True
            else:
                failure = True
                print_fail(
                    "Something went wrong with running the test file. "
                    "Make sure the 'test_driver' parameter is correct. "
                    "Is currently '%s'" % params["test_driver"]
                )
                sys.exit(1)
        except FileNotFoundError:
            print_fail(
                "Something went wrong with running the test file. "
                "Make sure the 'test_driver' parameter is correct. "
                "Is currently '%s'" % params["test_driver"]
            )
            sys.exit(1)

        # Exit at the end of parsing so you can see all of them XD
        if failure:
            print_fail(
                "Something went wrong look in %s" % test_filename,
                file=sys.stderr
            )
            sys.exit(1)
            break
        else:
            print_pass("Pass %d \r" % current_run, end="")

    if not failure:
        print_pass("Passed All!")
        sys.exit()

if __name__ == '__main__':
    main(parse_args())
