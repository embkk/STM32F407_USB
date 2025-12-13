#!/usr/bin/python3
#######################################################################
#                   (c) SEGGER Microcontroller GmbH                   #
#                         The Embedded Experts                        #
#                            www.segger.com                           #
#######################################################################
#
#-------------------------- END-OF-HEADER -----------------------------
"""
Purpose : Generate .c and .h files from HTML files
"""
#
# Configurable definitions.
#
#
# Generated file prefix/postfix (index.htm will become <prefix>index<postfix>.c and <prefix>index<postfix>.h)
#
_POSTFIX = ""
_PREFIX  = ""
#
# Folder where the website files are located
#
_HTML_FOLDER = "html"
#
# Folder to output the generated files into
#
_OUTPUT_FOLDER = "Generated"
#
# Should the script gzip certain files?
# When using a read-only file system these files must be added to the list of IP_FS_READ_ONLY_FILE_ENTRY
# with the correct extension, e.g. "events.js" should be added as "events.js.gz"
# Additionally IP_WEBS_ConfigFindGZipFiles(".gz", 0); must be
# called inside the IP_WEBS_X_SampleConfig() to enable gzip support!
#
# Files containing dynamic content (CGI markers or template markers) can not be used with GZIP!
# If you have some HTML files which do not contain CGIs and are certainly static you can enable GZIP for them,
# but you will likely need to change the callback registered via IP_WEBS_SetFileInfoCallback() and make sure it does not
# set the pFileInfo->AllowDynContent to 1 for these HTML files. Otherwise, the webserver code will treat these files as
# dynamic and will not try to deliver them as GZIP.
#
# There is no point in using GZIP for image files (.png, .jpg, .gif, etc.) these already use their own compression.
# SVG is an exception and benefits from compression because it is a text based format.
#
_DO_GZIP = 1
_GZIP_FILE_TYPES = ['.js', '.css', '.svg']


import sys
if sys.version_info < (3,5,1):
  sys.stderr.write("You need python 3.5.1 or later to run this script/n")
  exit(1)

import os
import pathlib
import multiprocessing
import re
import gzip
import shutil

from typing import final

_HEADER: final = (
  "/*********************************************************************\n"
  "*                   (c) SEGGER Microcontroller GmbH                  *\n"
  "*                        The Embedded Experts                        *\n"
  "*                           www.segger.com                           *\n"
  "**********************************************************************\n"
  "\n"
  "-------------------------- END-OF-HEADER -----------------------------\n"

)
_FOOTER: final = (
  "\n"
  "/*************************** End of file ****************************/\n"
)

_COLOR_WARN = '\033[93m'
_COLOR_END = '\033[0m'

def _VerifyEncoding(file: str) -> None:
  """
      Checks file encoding, only UTF8 is allowed

      :return:  Ascii value in hex as string.
  """
  try:
    with open(file, encoding="utf-8") as f:
      for line in f:
        if not line.isascii():
          print(f"{_COLOR_WARN}WARNING: File is not 100% ascii.{_COLOR_END}")
  except UnicodeError as e:
    print(f"{_COLOR_WARN}WARNING: The following exception was raised while trying to verify the encoding: \n{e}{_COLOR_END}")

def _GetByte(bytestream: bytes) -> str:
  """
      Convert any character to hex in the format "0xXX"

      :param bytestream: Single character sting.
      :return:  Ascii value in hex as string.
  """
  if len(bytestream) != 1:
    raise ValueError("Provided string must be a single character.")
  c = hex(ord(bytestream))
  if len(c) != 4:
    c = c[:2] + "0" + c[2:]
  c = c[:2] + c[2:].upper()
  return c

def _isHtml(file: str) -> bool:
  """
      Micro function - checks if a file ends with .htm or .html .

      :param file:    Any filepath.
      :return:        Nothing
  """
  return True if file.endswith(".htm") or file.endswith(".html") else False

def _WriteHead(in_file: pathlib.Path, out_file: pathlib.Path, original_name: str = "") -> None:
  """
      :param out_file: Input file object.
      :param in_file:  Output file object.
      :return:         Nothing.
  """
  header = _HEADER + (
    "\n"
    f"File    : {out_file.name}\n"
    f"Purpose : Automatically created from {(original_name if original_name != "" else in_file.name)} using GenFiles.py\n"
    "*/\n"
  )
  with open(out_file, "w") as F:
    F.write(header)

def _WriteToCFile(in_file: pathlib.Path, out_file: pathlib.Path) -> None:
  """
  Write the OutFile with input from InFile.

  :param in_file:  Filepath for input.
  :param out_file: Filepath for output.
  :return:
  """
  length = os.stat(in_file).st_size

  # Open the input file in binary mode and output file in append mode
  with open(in_file, "rb") as f_in, open(out_file, "a") as f_out:
    out_name = os.path.splitext(os.path.basename(out_file))[0]

    # Write the header and array declaration to the output file
    f_out.write(f'\n#include "{out_name}.h"\n')
    f_out.write(f'\nconst unsigned char {out_name.lower()}_file[{length}] = {{\n')

    width_count = 0
    first_byte = True
    for line in f_in:
      for character in line:
        # Format the byte
        byte_str = _GetByte(character.to_bytes(1, byteorder="little"))

        # Write the byte
        if first_byte:
          f_out.write("  " + byte_str)
          first_byte = False
        else:
          # Write a new line if necessary
          if width_count == 0:
            f_out.write(",\n  ")
          else:
            f_out.write(", ")
          f_out.write(byte_str)
        width_count = (width_count + 1) % 16

    # Write the closing bracket and footer
    f_out.write("\n};\n")
    f_out.write(_FOOTER)
  return

def _WriteToHFile(in_file: pathlib.Path, out_file: pathlib.Path):
  """
  Write the OutFile with input from InFile.

  :param in_file:  Filepath for input.
  :param out_file: Filepath for output.
  """
  # Get the file size
  length = os.stat(in_file).st_size

  # Extract filename and remove the file extension
  out_file_name_no_ext = out_file.stem

  # Write the header information to the output file
  _WriteHead(in_file, out_file)
  with open(out_file, "a") as out:
    out.write(f"#ifndef __{out_file_name_no_ext.upper()}_H__\n")
    out.write(f"#define __{out_file_name_no_ext.upper()}_H__\n")
    out.write("\n")
    out.write("#ifdef __cplusplus\n")
    out.write("extern \"C\" {\n")
    out.write("#endif\n")
    out.write(f"#define {out_file_name_no_ext.upper()}_SIZE {length}\n")
    out.write("\n")
    out.write(f"extern const unsigned char {out_file_name_no_ext.lower()}_file[{length}];\n")
    out.write("\n")
    out.write("#ifdef __cplusplus\n")
    out.write("};\n")
    out.write("#endif\n")
    out.write(f"#endif  // __{out_file_name_no_ext.upper()}_H__\n")
    out.write(_FOOTER)
  return

def list_files_in_directory(directory):
  """
  Get all files inside a directory recursively.

  :param directory: Path of directory.
  """
  file_list = []
  for root, dirs, files in os.walk(directory):
    for file in files:
      file_list.append(os.path.join(root, file))
  return file_list

def Bin2C(in_file: pathlib.Path, out_file: pathlib.Path):
  """
  Convert an input file into .c and .h files

  :param in_file: Filepath for input.
  :param out_file: Output file name.
  """
  #
  # Write header file.
  #
  _WriteToHFile(in_file, out_file.parents[0] / f"{out_file.stem}.h")
  #
  # Write the C file.
  #
  _WriteHead(in_file, out_file)
  _WriteToCFile(in_file, out_file)

def escape_file_path(name):
  """
  Escapes any characters which should not be used in filepaths or C variable names.

  :param name: File name for output.
  """

  # Replace invalid characters with an underscore
  escaped_name = re.sub(r'[^a-zA-Z0-9_]', '_', name)

  # Ensure the name does not start with a digit
  if escaped_name[0].isdigit():
    escaped_name = '_' + escaped_name
  return escaped_name

def gzip_file(input_file_path, output_file_path):
  with open(input_file_path, 'rb') as f_in:
    with gzip.open(output_file_path, 'wb') as f_out:
      shutil.copyfileobj(f_in, f_out)

def Generate(file: str):
  """
  Generate a single file.

  :param file: Filepath for input.
  """
  try:
    file_path = pathlib.Path(os.getcwd()) / file
    output_name = pathlib.Path(_OUTPUT_FOLDER) / f"{_PREFIX + escape_file_path(pathlib.Path(file).stem) + _POSTFIX}.c"

    # Check if the file ends with .js or .css and gzip it
    gzip_output_name = ""
    if _DO_GZIP:
      if file_path.suffix in _GZIP_FILE_TYPES:
        gzip_output_name = f"{file_path}.gz"
        gzip_file(file_path, gzip_output_name)
        print(f"Gzipped {file_path.name}\n{_COLOR_WARN}Please read the note regarding GZIP at the top of this script (_DO_GZIP is enabled){_COLOR_END}")
        file_path = pathlib.Path(gzip_output_name)

    print(f"Generating {output_name}")
    Bin2C(file_path, output_name)

    # Delete temporary .gz file if it was created
    if gzip_output_name != "":
      os.remove(gzip_output_name)

  except Exception as e:
    print(f"Unexpected error: {e}")

if __name__ == "__main__":
  # Get list of all files in the _HTML_FOLDER
  files = list_files_in_directory(_HTML_FOLDER)

  # Call "Generate" for each file using a multiprocess pool,
  # this will convert the files simultaneously.
  with multiprocessing.Pool() as _Pool:
    Result = [_Pool.apply_async(Generate, (File,)) for File in files]
    [_Result.wait(15) for _Result in Result]
    _Pool.close()
    _Pool.join()
    for r in Result:
      r.successful()
