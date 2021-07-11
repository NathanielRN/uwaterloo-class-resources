#!/usr/bin/env python3
"""Test Generator for Resizable_deque

Requirements: bash, python3


Description:
    This python file will run randomly combine the instructions
    in the test driver to create test files. It will run these generated
    test files on your code and check if they pass. If they don't pass,
    you can review the test file, modify it and re-run it manually.
    If you use the defaults, the test files are very long
    and takes a while to run. [Approx: 1 run per 2s w/ 30k line file.]

    It suggested you have a good cout function and
    then add the 'cout' command before the failing lines to debug.

Disclaimer:
    Random tests suck because edge cases are very unlikely to happen.
    Think about what these edge cases might be and create custom tests
    for them.

Usage:
    tester.py [options]

Options:
  test_driver=<driver>  Path to compiled tester driver executable [Default: ./tester.out]
  commands=<commands>   Number of commands to put in each run. Some "commands" map to multiple
                        test instructions. Expects positive int. [Default: 1000]
  runs=<runs>           Number of times to run random test cases.
                        Expects positive int. [Default: 1000]
  memcheck=<check>      Which kind of memory check to do. Possible values are 'full' or 'simple'.
                        Don't use 'full' otherwise you will always fail. [Default: simple]
  type=<type>           What type of deque this is.
                        Possible values are 'int' and 'double' [Default: int]

Stole the idea from someone else's P1 C++ version of this.
"""
# Imports
import sys
import random
from subprocess import Popen, PIPE
import re

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

class TestDriver:
    """Creates test file"""
    # Cache of methods that create test commands
    TEST_COMMANDS = ()

    def __init__(self, file):
        self.file = file
        self.params = None
        self.curr_array = None
        self.capacity = None
        self.initial_capacity = None
        self.pre = ""

    def set_params(self, params):
        """Define the parameters used by this driver"""
        self.params = params

    def set_curr_array(self, curr_array):
        """Define the array of this driver"""
        self.curr_array = curr_array

    def set_capacity(self, capacity):
        """Define the current capacity of this driver"""
        self.capacity = capacity

    def set_initial_capacity(self, initial_capacity):
        """Define the initial capacity of this driver"""
        self.initial_capacity = initial_capacity

    def set_pre(self, pre):
        """Define any text to print before writing commands to the file"""
        self.pre = pre

    def rng(self):
        """Helper method to create random value depending on type"""
        if self.params["type"] == "int":
            return random.randrange(-20, 20)
        if self.params["type"] == "double":
            return random.choice([i*0.1 for i in range(-200, 200)])
        return random.random()

    def generate(self, num_commands, *disallowed_commands):
        """Generate commands in file. """
        # Verify class is generated properly
        params_defined = self.params is not None
        curr_array_defined = self.curr_array is not None
        capacity_defined = self.capacity is not None
        initial_capacity_defined = self.initial_capacity is not None
        is_ready = (
            params_defined and
            curr_array_defined and
            capacity_defined and
            initial_capacity_defined
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
        size = len(self.curr_array)
        self.write("size %d" % size)

    def _make_capacity(self):
        """Create instruction to verify capacity"""
        self.write("capacity %d" % self.capacity)

    def _make_empty(self):
        """Create instruction to verify emptyness"""
        is_empty = int(not self.curr_array)
        self.write("empty %d" % is_empty)

    def _make_front(self):
        """Create instruction to verify first element"""
        if not self.curr_array:
            self.write("front!")
        else:
            self.write("front %s" % str(self.curr_array[0]))

    def _make_back(self):
        """Create instruction to verify last element"""
        if not self.curr_array:
            self.write("back!")
        else:
            self.write("back %s" % str(self.curr_array[-1]))

    def _make_push_front(self):
        """Create instruction to push random number to front"""
        # Do multiple pushes to increase likelyhood of capacity change
        max_push = 10
        for _ in range(random.randrange(1, max_push)):
            value = self.rng()
            self.write("push_front %s" % str(value))
            self.curr_array.insert(0, value)
            if self.capacity < len(self.curr_array):
                self.capacity = self.capacity * 2

    def _make_push_back(self):
        """Create instruction to push random number to end"""
        # Do multiple pushes to increase likelyhood of capacity change
        max_push = 10
        for _ in range(random.randrange(1, max_push)):
            value = self.rng()
            self.write("push_back %s" % str(value))
            self.curr_array.append(value)
            if self.capacity < len(self.curr_array):
                self.capacity = self.capacity * 2

    def _make_pop_front(self):
        """Create instruction to pop first element"""
        # Do multiple pops to increase likelyhood of capacity change
        max_pop = 10
        for _ in range(random.randrange(1, max_pop)):
            if self.curr_array:
                self.curr_array.pop(0)
                self.write("pop_front")
                if self.capacity > self.initial_capacity:
                    if len(self.curr_array) <= self.capacity/4:
                        self.capacity = int(self.capacity / 2)
            else:
                self.write("pop_front!")

    def _make_pop_back(self):
        """Create instruction to pop last element"""
        # Do multiple pops to increase likelyhood of capacity change
        max_pop = 10
        for _ in range(random.randrange(1, max_pop)):
            if self.curr_array:
                self.curr_array.pop()
                self.write("pop_back")
                if self.capacity > self.initial_capacity:
                    if len(self.curr_array) <= self.capacity/4:
                        self.capacity = int(self.capacity / 2)
            else:
                self.write("pop_back!")

    def _make_clear(self):
        """Create clear instruction"""
        del self.curr_array[:]
        self.capacity = self.initial_capacity
        self.write("clear")

    def _make_assign(self):
        """Create a assign instruction with random sub-tests to verify success. Will not recurse."""
        tests_to_run = random.randrange(100)
        self.write("assign")
        self.gen_subtest_driver(tests_to_run)

    def _make_move(self):
        """Create a move instruction with random sub-tests to verify success. Will not recurse."""
        tests_to_run = random.randrange(100)
        self.write("assign")
        self.gen_subtest_driver(tests_to_run)

    def gen_subtest_driver(self, num_commands):
        """"Helper method to create sub-test drivers"""
        no_recurse_commands = ["move", "assign"]
        sub_test_driver = TestDriver(self.file)
        sub_test_driver.set_curr_array(list(self.curr_array)) # copy list
        sub_test_driver.set_capacity(self.capacity)
        sub_test_driver.set_initial_capacity(self.initial_capacity)
        sub_test_driver.set_params(self.params)
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
        self.curr_array = []
        if random.randrange(2):
            self.initial_capacity = 16
            self.capacity = self.initial_capacity
            self.write("new")
        else:
            self.initial_capacity = random.randrange(16, 1025)
            self.capacity = self.initial_capacity
            self.write("new: %d" % self.initial_capacity)

    def gen_exit(self):
        """Exit the test driver"""
        self.write("exit")

def print_pass(text, **kwargs):
    """Helper method to colorize passes"""
    print(PRINT_COLORS["OKGREEN"] + text + PRINT_COLORS["ENDC"], **kwargs)

def print_fail(text, **kwargs):
    """Helper method to colorize passes"""
    print(PRINT_COLORS["FAIL"] + text + PRINT_COLORS["ENDC"], **kwargs)

def parse_args(argv):
    """Parse arguments into dictionary"""
    params = {"other": []}
    for arg in argv:
        if "=" in arg:
            param, arg = arg.split("=", 1)
            if arg.isnumeric():
                params[param] = int(arg)
            else:
                params[param] = arg
        else:
            print_fail(
                "Error got argument '%s'. "
                "Expected arguments of the form `key=value" % arg,
                file=sys.stderr
            )
            # print help string
            print()
            print(__doc__)
            sys.exit(1)
    #default params
    if "test_driver" not in params:
        params["test_driver"] = "./tester.out"
    if "commands" not in params:
        params["commands"] = 1000
    if "runs" not in params:
        params["runs"] = 1000
    if "memcheck" not in params:
        params["memcheck"] = "simple"
    if "type" not in params:
        params["type"] = "int"
    return params

def main(params):
    """Run tests"""
    failure = False
    total_runs = params["runs"]
    total_commands = params["commands"]
    for current_run in range(1, total_runs + 1):
        # Clear and open test file
        test_filename = 'tmp_test.in.%s.txt' % params["type"]
        open(test_filename, 'w').close()
        with open(test_filename, 'w') as test_file:
            test_driver = TestDriver(test_file)
            test_driver.set_params(params)
            test_driver.gen_new()
            test_driver.generate(total_commands)
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
                "Where is that file dude? "
                "Make sure the 'test_driver' parameter is correct. "
                "Is currently '%s'" % params["test_driver"]
            )
            sys.exit(1)
        # except Exception as exception:
        #     print_fail(
        #         "Something complicated went wrong '%s'" % exception
        #     )
        #     sys.exit(1)


        # Exit at the end of parsing so you can see all of them XD
        if failure:
            print_fail(
                "Something went wrong look in tmp_test.in.%s.txt" % params["type"],
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
    main(parse_args(sys.argv[1:]))