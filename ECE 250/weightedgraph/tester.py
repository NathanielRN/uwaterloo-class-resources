#!/usr/bin/env python3
"""Test Generator for Weighted_graph

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
    Also I'm using my own weighted graph implementation.
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
from pathlib import Path

from wgraph import DistanceGraph

TARGET_PROJECT = "Weighted_graph"
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
    RANDOM_RANGE = 10000

    def __init__(self, file):
        self.file = file
        self.graph = None
        self.params = None
        self.pre = ""

    def set_params(self, params):
        """Define the parameters used by this driver"""
        self.params = params

    def set_graph(self, graph):
        """Define the weighted graph used by this driver"""
        self.graph = graph

    def set_pre(self, pre):
        """Define any text to print before writing commands to the file"""
        self.pre = pre

    @staticmethod
    def rng():
        """Helper method to create random value depending on params"""
        # should give values anwhere from -100.00 to 100.00
        return random.choice(
            [i*0.01 for i in range(-1*TestDriver.RANDOM_RANGE, TestDriver.RANDOM_RANGE)]
        )

    def rng_node(self, niceness="nice"):
        """Helper method to create random node value depending on niceness and graph size"""
        if niceness == "nice":
            # only 3 external values
            return random.randrange(-1, self.graph.size+3)
        # only 10 external values
        return random.randrange(-5, self.graph.size+6)


    def generate(self, num_commands, *disallowed_commands):
        """Generate commands in file. """
        # Verify class is generated properly
        params_defined = self.params is not None
        graph_defined = self.graph is not None
        is_ready = (
            params_defined and
            graph_defined
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

    def _make_degree(self):
        """Create instruction to verify degree of a random node"""
        node = self.rng_node("salty")
        try:
            self.write("degree %d %d" % (node, self.graph.degree(node)))
        except ValueError:
            # TODO: Modify driver to get allow degree to be invalid
            pass # Currently test drivers cannot handle this eventuallity!
            # self.write("!degree %d" % node)

    def _make_edge_count(self):
        """Create instruction to verify edge count"""
        self.write("edge_count %d" % self.graph.edge_count())

    def _make_adjacency(self):
        """Create instruction to verify adjacency of two random nodes"""
        node1, node2 = self.rng_node(), self.rng_node()
        try:
            adjacency = self.graph.adjacent(node1, node2)
            if adjacency != float("inf"):
                self.write("adjacent %d %d %s" % (node1, node2, str(adjacency)))
            else:
                self.write("~adjacent %d %d" % (node1, node2))
        except ValueError:
            self.write("adjacent! %d %d" % (node1, node2))

    def _make_distance(self):
        """Create instruction to verify distance of two random nodes"""
        # When the graph size is close to the number of nodes, this works well
        for _ in range(random.randrange(1, self.params["max_distance_check"])):
            node1, node2 = self.rng_node(), self.rng_node()
            try:
                distance = self.graph.distance(node1, node2)
                if distance != float("inf"):
                    self.write("distance %d %d %s" % (node1, node2, str(distance)))
                else:
                    self.write("~connected %d %d" % (node1, node2))
            except ValueError:
                self.write("distance! %d %d" % (node1, node2))

    def _make_insert(self):
        """Create instruction to insert a random number into the hash table"""
        # Do multiple inserts to increase likelyhood of collisions
        for _ in range(random.randrange(1, self.params["max_insert"])):
            node1, node2 = self.rng_node(), self.rng_node()
            dist = TestDriver.rng()
            try:
                self.graph.insert(node1, node2, dist)
                self.write("insert %d %d %s" % (node1, node2, str(dist)))
            except ValueError:
                self.write("insert! %d %d %s" % (node1, node2, str(dist)))

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
        sub_test_driver.set_graph(self.graph.duplicate()) # copy list
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
        # Added a deadspace so that the dank memes won't run dry
        allowed_sizes = set(range(-2, 10)) | set(range(45, self.params["max_size"]))

        graph_size = random.choice(list(allowed_sizes))
        use_default_constructor = False
        self.graph = DistanceGraph(graph_size)
        if self.graph.size == 50:
            use_default_constructor = bool(random.randrange(2))

        if use_default_constructor:
            self.write("new")
        else:
            self.write("new: %d" % graph_size)

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
        help="Number of commands to put in each run. Some 'commands' map to multiple"
        "test instructions. Expects positive number. [Default: %(default)d]"
    )
    parser.add_argument(
        "--runs",
        "-r",
        type=int,
        metavar="NUM_RUNS",
        help="Number of times to run random test cases."
        " Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--memcheck",
        "-m",
        choices={"simple", "full"},
        help="Which kind of memory check to do. Don't use 'full'"
        " otherwise you will always fail. [Default: %(default)s]"
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
        "--max-size",
        "--max_size",
        "--ms",
        type=int,
        help="Set the upper-bound for the the number of nodes in a graph. "
        "Lower numbers are good for forcing extremely connected graphs. "
        "Expects positive int. [Default: %(default)d]"
    )
    parser.add_argument(
        "--max-distance-check",
        "--max_distance_check",
        "--max-dist-check",
        "--max_dist_check",
        "--mdc",
        type=int,
        help="Set the upper-bound for the number of distance checks"
        " to do in a row. [Default: %(default)d]"
    )

    parser.set_defaults(
        test_driver="./a.out",
        commands=1000,
        runs=1000,
        memcheck="simple",
        disallowed_commands=[],
        max_insert=5,
        max_size=100,
        max_distance_check=20
    )
    #mangle argv
    def add_prefix_char(arg):
        """Mangle the input arguments to allow an older archaic format style"""
        return ("--" + arg) if not arg.startswith("--") and "=" in arg else arg
    mangled_args = [add_prefix_char(arg) for arg in argv]
    params = vars(parser.parse_args(mangled_args))
    return params

def guess_driver(param_driver):
    """"Attempt to guess where the driver is when it isn't the default"""
    if Path(param_driver).exists():
        return param_driver
    if param_driver != "./a.out":
        print_fail(
            "You tried to run this script using "
            "'%s' however, that file doesn't exist" % param_driver
        )
        sys.exit(1)

    guesses = {
        # Top 3 alternatives
        "./a.exe"
        "./" + TARGET_PROJECT +"_driver.exe",
        "./tester.out",
        # Other alternatives
        # project style
        "./" + TARGET_PROJECT +"_driver.out",
        "./" + TARGET_PROJECT +"_driver",
        "./"+TARGET_PROJECT+".exe",
        "./"+TARGET_PROJECT+".out",
        "./"+TARGET_PROJECT,
        # 'a' style
        "./a",
        # 'tester' style
        "./tester.exe",
        "./tester"
    }
    for guess in guesses:
        if Path(guess).exists():
            return guess

    print_fail(
        "Couldn't find compiled test driver file. "
        "Make sure to set the 'test_driver' appropriately"
        "Is currently the default '%s'" % param_driver
    )
    sys.exit(1)

    return param_driver


def main(params):
    """Run tests"""
    failure = False
    cpp_driver = guess_driver(params["test_driver"])

    total_runs = params["runs"]
    total_commands = params["commands"]
    test_filename = 'tmp_test.%s.in.txt' % ("int")
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
                    [str(cpp_driver), "int"],
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

# TODO: Use random normal variation to have more control over likelyhood of commands
#                                   (insert/distance should have high frequencies)
# TODO: Use random normal variation to have more control over likelyhood of valid vs. invalid
