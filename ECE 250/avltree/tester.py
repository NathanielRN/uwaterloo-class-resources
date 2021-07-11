#!/usr/bin/env python3
"""Test Generator for Search_tree

Requirements: python3

Description:
    This python file will run randomly combine the instructions
    in the test driver to create test files. It will run these generated
    test files on your code and check if they pass. If they don't pass,
    you can review the test file, modify it and re-run it manually.
    If you use the defaults, the test files are very long, so each 
    run is relatively slow. [~0.6s per run w/ 1000 commands]

    It's suggested you have a good cout function and
    then add the 'cout' command before the failing lines to debug.

    Also you can now use this while your code isn't down by using the
    '--disallowed-commands' option to turn off any functionality you haven't
    implemented yet.

Disclaimer:
    Random tests suck because edge cases are very unlikely to happen.
    Think about what these edge cases might be and create custom tests
    for them.
    
    !! Important !!
    Also I'm not using an array, but my own py AVL implementation.
    So ... if that's buggy, these tests won't work. You'll see below but
    how you implement AVL can result in different valid results from the same 
    operations! It's uncertain how this will be marked but none the less, I tried 
    to add enough flexibility here to make sure the internal tree matches expectations.

Credit: Blatantly stole idea from someone else's P1 C++ version of this

See -h for avaiable options
"""
# Imports
import sys
import random
from subprocess import Popen, PIPE
import re
from argparse import ArgumentParser, RawDescriptionHelpFormatter
from avl import AVLTree

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

# TODO: Should use inheritance like Harder does.
class IteratorTestDriver:
    # Cache of methods that create test commands
    TEST_COMMANDS = ()

    def __init__(self, file):
        self.file = file
        self.itr = None
        self.pre = ""

    def set_params(self, params):
        """Define the parameters used by this driver"""
        self.params = params

    def set_itr(self, itr):
        """Define the array of this driver"""
        self.itr = itr

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
    
    def gen_summary(self):
        """Create summary command based on params"""
        simple_summary = self.params["memcheck"] != "full"
        if simple_summary:
            self.write("summary")
        else:
            self.write("details")
    
    def gen_exit(self):
        """Exit the test driver"""
        self.write("exit")

    def generate(self, num_commands, *disallowed_commands):
        """Generate commands in file. """
        # Verify class is generated properly
        params_defined = self.params is not None
        itr_defined = self.itr is not None
        is_ready = (
            params_defined and
            itr_defined
        )

        if not is_ready:
            raise ValueError(
                "This class can't generate tests until parameters "
                "and tree iterator are defined"
            )
        while num_commands >= 0:
            self.call_random(*disallowed_commands)
            num_commands -= 1
    
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
        method_list = IteratorTestDriver.TEST_COMMANDS
        if not method_list:
            method_list = tuple(func for func in filter(command_method, dir(self)))
            IteratorTestDriver.TEST_COMMANDS = method_list

        # convert disallowed_methods to a set to improve performance
        disallowed_methods = set(disallowed_methods)
        # filter out diallowed methods and call random one
        method_list = tuple(filter(allowed_method, method_list))
        getattr(self, random.choice(method_list))()
    
    def write(self, text):
        """Helper method to write commands to file"""
        print(self.pre, file=self.file, end="")
        print(text, file=self.file)

    def _make_next(self):
        """Create instruction to verify first element"""
        self.write("next")
        try:
            self.itr.next()
        except StopIteration:
            pass
    
    def _make_previous(self):
        """Create instruction to verify first element"""
        self.write("previous")
        try:
            self.itr.previous()
        except StopIteration:
            pass
    
    def _make_value(self):
        val = self.itr.value() or 0
        self.write("value " + str(val))

class TestDriver:
    """Creates test file"""
    # Cache of methods that create test commands

    TEST_COMMANDS = ()
    MAX_ITER_COMMANDS = 2

    def __init__(self, file):
        self.file = file
        self.tree = None
        self.params = {}
        self.pre = ""

    def set_params(self, params):
        """Define the parameters used by this driver"""
        self.params = params
    
    def set_tree(self, tree):
        """Define the array of this driver"""
        self.tree = tree

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
        tree_defined = self.tree is not None
        is_ready = (
            params_defined and
            tree_defined
        )

        if not is_ready:
            raise ValueError(
                "This class can't generate tests until parameters "
                "and tree structure are defined"
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
        size = len(self.tree)
        self.write("size %d" % size)

    def _make_height(self):
        """Create instruction to verify height"""
        self.write("height %d" % self.tree.height())

    def _make_empty(self):
        """Create instruction to verify emptyness"""
        is_empty = int(not self.tree)
        self.write("empty %d" % is_empty)

    def _make_front(self):
        """Create instruction to verify first element"""
        if not self.tree:
            self.write("front!")
        else:
            self.write("front %s" % str(self.tree.front()))

    def _make_back(self):
        """Create instruction to verify last element"""
        if not self.tree:
            self.write("back!")
        else:
            self.write("back %s" % str(self.tree.back()))

    def _make_begin(self):
        itr = self.tree.begin()
        self.write("begin")
        self.gen_iter_driver(itr, TestDriver.MAX_ITER_COMMANDS)
    
    def _make_end(self):
        itr = self.tree.end()
        self.write("end")
        self.gen_iter_driver(itr, 100)

    def _make_rbegin(self):
        itr = self.tree.rbegin()
        self.write("rbegin")
        self.gen_iter_driver(itr, 100)
    
    def _make_rend(self):
        itr = self.tree.rend()
        self.write("rend")
        self.gen_iter_driver(itr, 100)
    
    def _make_find(self):
        value = self.rng()
        itr = self.tree.find(value)
        self.write("find " + str(value))
        self.gen_iter_driver(itr, 100)

    def _make_insert(self):
        """Create instruction to insert random number"""
        value = self.rng()
        success = int(self.tree.insert(value))
        self.write("insert %s %d" % (str(value), success))

    def _make_erase(self):
        """Create instruction to remove random element"""
        value = self.rng()
        success = int(self.tree.erase(value))
        self.write("erase %s %d" % (str(value), success))

    def _make_clear(self):
        """Create clear instruction"""
        self.tree.clear()
        self.write("clear")

    def gen_iter_driver(self, itr, num_commands):
        """"Helper method to create iterator subtest drivers"""
        no_recurse_commands = []
        sub_test_driver = IteratorTestDriver(self.file)
        sub_test_driver.set_itr(itr)
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
        """Create new command and initialize driver"""
        self.tree = AVLTree(preferred_erase=self.params["preferred_erase"], preferred_dir=self.params["preferred_dir"])
        self.write("new")

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
        "--preferred-erase",
        "--pc",
        help="If given the choice, will erase in this direction. When erasing"
        " with 2 children, will choose this a child from this subtree. Will sometimes"
        " affect expected height. [Default: %(default)s]",
        choices={"right", "left"}
    )
    parser.add_argument(
        "--preferred-dir",
        "--pd",
        help="If given the choice, will rotate in this direction. Will sometimes"
        " affect expected height. [Default: %(default)s]",
        choices={"right", "left"}
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
    parser.set_defaults(
        test_driver="./tester.out",
        commands=1000,
        runs=1000,
        preferred_dir="left",
        preferred_erase="left",
        type="int",
        memcheck="simple",
        disallowed_commands=[]
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
        # except Exception as exception:
        #     print_fail(
        #         "Something complicated went wrong '%s'" % exception
        #     )
        #     sys.exit(1)


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
