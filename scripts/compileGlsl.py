"""
scripts/compileGlsl.py

This script compiles all the GLSL shaders in the shaders/ directory into a single header file.

Requires glslang and glslangValidator to be on the system PATH.

Author: Surge
Version: 1.0
Last Updated: 2024-12-26

Usage:
    python compileGlsl.py [-o output] [-d] <input>
"""

import dotenv
import os

FILE_EXT_KNOWN = [".vert", ".frag", ".comp", ".geom", ".tesc", ".tese", ".mesh", ".task", ".glsl"]

def verifyGlslang() -> bool:
    """
    Verifies that glslang and glslangValidator are on the system PATH.

    Returns:
        bool: True if both glslang and glslangValidator are on the system PATH, False otherwise.
    """
    return os.system("glslangValidator --version") == 0 or (os.getenv("GLSLANG_PATH") is not None and os.system(f"{os.getenv('GLSLANG_PATH/glslangValidator')} --version") == 0)

def compileGlsl(input:str, output:str=None) -> bool:
    """
    Compiles all the GLSL shaders in the shaders/ directory into a single header file.

    Args:
        input (str): The name of the input file.
        output (str): The name of the output file. (Optional: if omitted, output = input + ".spv";
        i.e. input file.vert: output file.vert.spv)

    Returns:
        bool: True if the compilation was successful, False otherwise.
    """
    if output is None:
        output = input + ".spv"

    if not os.path.exists(input):
        print(f"Input file '{input}' does not exist.")
        return False

    out = os.system(f"glslangValidator -V {input} -o {output}")
    if out != 0:
        print(f"Failed to compile '{input}'.")
        return False

    print(f"Successfully compiled '{input}' to '{output}'.")
    return True


if __name__=="__main__":
    dotenv.load_dotenv() # Load environment variables from .env file, if it exists.

    if not verifyGlslang():
        print("glslang and glslangValidator must be on the system PATH.")
        exit(1)

    # Parse command line arguments
    import argparse

    parser = argparse.ArgumentParser(description="Compile GLSL shaders into a single header file.")
    parser.add_argument("-o", "--output", help="The name of the output file.")
    parser.add_argument("-d", "--directory", help="If the input is a directory.", action="store_true")
    parser.add_argument("-r", "--recursive", help="If the input is a directory, search recursively.", action="store_true")

    parser.add_argument("input", help="The name of the input file.")

    args = parser.parse_args()

    if args.directory:
        if not args.recursive:
            for file in os.listdir(args.input):
                if os.path.splitext(file)[1] in FILE_EXT_KNOWN:
                    if not compileGlsl(os.path.join(args.input, file)):
                        exit(1)
        else:
            for root, _, files in os.walk(args.input):
                for file in files:
                    if os.path.splitext(file)[1] in FILE_EXT_KNOWN:
                        if not compileGlsl(os.path.join(root, file)):
                            exit(1)
    else:
        if not compileGlsl(args.input, args.output):
            exit(1)