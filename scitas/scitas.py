import numpy as np
import argparse
import os
import subprocess

def __str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')

def __generate_file_placeholder(template_file, output_file, placeholders):
    """Generates a file from template_file with replacing all in placehodlers"""
    template_head = open(template_file, 'r')
    input_head = open(output_file, 'w+')

    for line in template_head.readlines():

        # replace all placeholders
        for key, value in placeholders.items():
            line = line.replace('{{{0}}}'.format(key), value)

        input_head.writelines(line)

    template_head.close()
    input_head.close()

def generate_problem(name, number_agents, problem_size, use_reservation = False):
    # generates the shell-file
    my_dir = os.path.dirname(__file__)

    placeholders = {}
    placeholders['problem_name'] = name
    placeholders['number_agents'] = str(number_agents)
    placeholders['problem_size'] = str(problem_size)
    placeholders['reservation'] = '#SBATCH --reservation=phpc2018' if use_reservation else ''

    __generate_file_placeholder(os.path.join(my_dir, 'template.sh'), 
        os.path.join(my_dir, '..', 'run_scripts', '{0}.sh'.format(name)),
        placeholders)

def submit(name):
    # submits the problem
    my_dir = os.path.dirname(__file__)
    subprocess.call("sbatch {0}".format(os.path.join(my_dir, '..', 'run_scripts', '{0}.sh'.format(name))), shell=True)

def execute_problem(name, number_agents, problem_size, use_reservation = False):
    generate_problem(name, number_agents, problem_size, use_reservation)
    submit(name)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-N", help="Size of the problem", type=int)
    parser.add_argument("-np", help="Number of agents", type=int)
    parser.add_argument("-name", help="Name")
    parser.add_argument("-submit", type=__str2bool, nargs='?',
                        const=True, default=False,
                        help="Submit with sbatch")
    args = parser.parse_args()

    if args.name == None or args.np == None or args.N == None:
        raise RuntimeError("check with --help option for the necessary parameters")

    generate_problem(args.name, args.np, args.N)

    if (args.submit):
        submit(args.name)